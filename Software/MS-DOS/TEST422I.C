//***************************************************************************
//
//
// TEST422I.C
//
//          Integrity Instruments 
//          -----------------------------------------------------------
//          P.O. Box 451
//          Pine River, MN  56474
//          Tel: 800-450-2001
//          Tel: 218-587-3175
//          Fax: 218-587-3414
//          Web: www.integrityusa.com
//
//
// Description:
//  Tests the 422-25I data converter in LOOP BACK mode.
//
// 
//
// Last Modified:       
//      A. A. Kempka    01/29/97        Original.
//  
//
//***************************************************************************
//
//
//  Compiling:  This program was compiled under Borland 'C' 4.0 using
//              the following command line:
//
//                  BCC -3 -ml TEST422I.C
//
//***************************************************************************
//
//
//
//
//
//
//
//


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <bios.h>
#include <string.h>
#include <time.h>

//---------------------------------------------------------------------------
//
// Local includes.
//

#include "pic.h"
#include "com.h"


//---------------------------------------------------------------------------
//
// TRUE/FALSE defines
//

#define FALSE   0
#define TRUE    (!FALSE)


//---------------------------------------------------------------------------
//
// Generic type definitions for portability.
//

typedef unsigned short USHORT;
typedef unsigned char  UCHAR;
typedef unsigned long  ULONG;
typedef signed long    LONG;
typedef signed short   SHORT;
typedef char           CHAR;
typedef double         REAL;


//---------------------------------------------------------------------------
//
// Input/Output defines used to replace (inp and outp)
//

#define IN_BYTE  inportb
#define OUT_BYTE outportb


//---------------------------------------------------------------------------
//
// Error defines:
//

#define NO_ERROR                0x0000
#define ERR_INVALID_ADDRESS     0x0001
#define ERR_INVALID_PARAMETER   0x0002
#define ERR_NO_RESPONSE         0x0003
#define ERR_INVALID_RESPONSE    0x0004


//---------------------------------------------------------------------------
//
// Module address defines
//

#define MAX_MODULE      0xFE
#define HOST_MODULE     0x00
#define PROGRAM_ADDRESS 0xFF

//---------------------------------------------------------------------------
//
// Channel defines
//

#define MAX_CHANNEL     0x03


//---------------------------------------------------------------------------
//
// RTC (Clock Calendar) defines and port addresses.
//

#define RTC_ADDRESS      0x70
#define RTC_DATA         0x71


//---------------------------------------------------------------------------
//
// Rtc control variables.
//

void (_interrupt _far *OldRtcIsr)();
ULONG RtcCount = 0;
UCHAR RtcStatus;


//---------------------------------------------------------------------------
//
// Communications variables, Port defines, data structures, etc.
//

ULONG  ComIdleTime = 0;

COMPORT ComPort;

void (_interrupt _far *OldComIsr)();

SHORT   ExpectingAResponse = FALSE;
SHORT   ReceivedAResponse = FALSE;
SHORT   TransmittingAck = FALSE;

UCHAR OutSequenceNum[MAX_MODULE] = {0};
UCHAR InSequenceNum[MAX_MODULE] = {0};

ULONG ChksumBadCount = 0;
ULONG AsyncPacketsRcvd = 0;
ULONG SyncPacketsRcvd = 0;

#define RECEIVE_BUFFER_SIZE     48

UCHAR RcvBuffer [RECEIVE_BUFFER_SIZE];
UCHAR *RcvBufferPosition;
UCHAR *RcvBufferEnd;
ULONG RcvBufferCount;

#define XMIT_BUFFER_SIZE        48

UCHAR XmitBuffer [XMIT_BUFFER_SIZE];
UCHAR *XmitBufferPosition;
UCHAR *XmitBufferEnd;
ULONG XmitBufferCount;

#define PACKET_BUFFER_SIZE      32768

UCHAR PacketBuffer [PACKET_BUFFER_SIZE];
UCHAR *PacketBufferHead;
UCHAR *PacketBufferTail;
UCHAR *PacketBufferEnd;
ULONG PacketCount = 0;


//---------------------------------------------------------------------------
//
// Communications timeouts - re-zero time in milliseconds.
//

#define COM_IDLE_TIMEOUT                3
#define COM_RESPONSE_TIMEOUT            3
#define COM_PACKET_RETRIES              64


//---------------------------------------------------------------------------
//
//  Adjust the PacketUpdateTimeout parameter for Faster clearing of
//  the packet buffer (an subsequent updates of Async data)
//
//  This Timeout parameter is in milliseconds.
// 

LONG PacketUpdateTime = 0;
LONG PacketUpdateTimeout = 250;


//---------------------------------------------------------------------------
//
// The following structure is filled with Async data received
// from modules.
//

USHORT Module_Value [MAX_MODULE+1][MAX_CHANNEL+1] = {0xFFFF};


//---------------------------------------------------------------------------
//
// Internal Prototypes.
//


UCHAR Gen_Chksum (UCHAR *Buffer, SHORT Len);



//---------------------------------------------------------------------------
//
// Copyright info. This information MUST REMAIN INTACT.
//
//---------------------------------------------------------------------------

const CHAR Copyright [] = "Copyright (c) 1997, Integrity Instruments";
const CHAR Address1 []  = "718 S. 2nd Street";
const CHAR Address2 []  = "Laramie, WY  307-721-4881";


//***************************************************************************
//
// Functions follow.
//



//---------------------------------------------------------------------------
//
// Disables the UART and restores the interrupt vector.
//
void RestoreUART (void)
{
    int i;

    //
    // Deactivate all of the MCR interrupt sources.
    //

    OUT_BYTE (ComPort.MCR, MCR_DEACTIVATE_ALL);

    //
    // Disable interrupts at the PIC
    //

    i = ((IN_BYTE (PIC1_IMR)) | ComPort.SPDM);
    OUT_BYTE (PIC1_IMR, i);

    //
    // Restore the original ISR vector
    //

    _dos_setvect (ComPort.InterruptVector, OldComIsr);
    return;
}


//---------------------------------------------------------------------------
//
// Disables the RTC (Clock Calendar) and restores the interrupt vector.
//
void RestoreRTC (void)
{
    int i;

    //
    // Disable interrupts at the PIC
    //

    i = ((IN_BYTE (PIC2_IMR)) | IMR_DISABLE_IRQ8);
    OUT_BYTE (PIC2_IMR, i);

    //
    // Restore the original ISR vector
    //

    _dos_setvect (IRQ8_ISR_VECTOR, OldRtcIsr);
    return;
}


//---------------------------------------------------------------------------
//
// Delays for x, milliseconds. Accurate! (If using the RTC ISR)
//
void our_delay (ULONG ms)
{
    ULONG TerminateCount;

    _disable ();                        // Guard access
    TerminateCount = RtcCount + ms;
    _enable ();

    while (RtcCount < TerminateCount) { // Wait until time.
    }
    return;
}


//
// Error beep!
//
void beep (void)
{
    sound (2000);
    our_delay (100);
    nosound();
    return;
}




//---------------------------------------------------------------------------
//
// Real Time Clock/Calendar ISR - 1ms.
//
void _interrupt _far _loadds RtcIsr ()
{
    USHORT Mod_Address;
    USHORT Buffer_Len;
    UCHAR  Buffer [32];

    //
    // Bump miscellaneous timers.
    //

    RtcCount++;
    ComIdleTime++;                      // Incr the com idle clock.

    //
    // Clear the interrupt source.
    //

    OUT_BYTE (RTC_ADDRESS, 0x0C);           // Read the status register.
    RtcStatus = IN_BYTE (RTC_DATA);         // to clear the interrupt.

    OUT_BYTE (PIC2_ICR, PIC_GENERAL_EOI);   // Clear the PIC
    OUT_BYTE (PIC1_ICR, PIC_GENERAL_EOI);
}


//---------------------------------------------------------------------------
//
// Initialize the RTC timer interrupt.
//
SHORT InitializeRtcIsr (void)
{
    UCHAR ch;
    SHORT Done = FALSE;
    SHORT i;
    SHORT j = 0;

    OldRtcIsr = _dos_getvect (0x70);    // Save the OLD isr address.

    while (!Done) {                     // Loop until done.

        _disable ();                    // Shut down the interrupts.

        OUT_BYTE (RTC_ADDRESS, 0x0C);   // Clear the status first.
        RtcStatus = IN_BYTE (RTC_DATA); // a NECESSARY step.
    
        OUT_BYTE (RTC_ADDRESS, 0x0A);   // Set up the timing - 976.83 us.
        OUT_BYTE (RTC_DATA, 0x26);

        OUT_BYTE (RTC_ADDRESS, 0x0B);   // Enable periodic output
        OUT_BYTE (RTC_DATA, 0x4A);      // and interrupt.

        ch = ((IN_BYTE (PIC2_IMR)) & IMR_ENABLE_IRQ8);
        OUT_BYTE (PIC2_IMR, ch);

        //
        // Install the new ISR.
        //

        _dos_setvect (IRQ8_ISR_VECTOR, RtcIsr);

        _enable ();                     // Enable the interrupts.

        for (i=0; i< 20000; i++) {      // Dummy time eating loop. 
            ch = i;
        }

        if (RtcStatus == 0xC0) {        // RTC is running if status = C0.
            Done = TRUE;
        } else {                        // Incr j count
            j++;
            if (j > 100) {              // Do we quit?
                Done = TRUE;
            }
        }
    }

    if (j >= 100) {                     // TOO many errors.
        printf ("\nERROR - Can't initialize RTC periodic interrupt.\n");
        return TRUE;
    } else {                            // init was successfull.
        return FALSE;
    }
} // InitializeTimers


//---------------------------------------------------------------------------
//
// Returns TRUE if there is enough room in the PacketBufffer
//
SHORT EnoughRoomInPacketBuffer(SHORT Len)
{
    UCHAR *TempHead;
    USHORT i;
    SHORT EnoughRoom = TRUE;

    TempHead = PacketBufferHead;
    
    for (i=0; i<Len-2; i++) {
        TempHead++;
        if (TempHead > PacketBufferEnd) {
            TempHead = PacketBuffer;
        }

        //
        // Check if head catches up with the tail.
        //

        if (TempHead == PacketBufferTail) {
            EnoughRoom = FALSE;
        }
    }

    return EnoughRoom;
}


//---------------------------------------------------------------------------
//
// Communications ISR.
//
void _interrupt _far _loadds ComIsr()
{
    UCHAR ch, Chksum, SeqNum;
    USHORT i, Module_Address;
    SHORT Len;

    //
    // Clear any pending UART interrupts
    //

    ch = IN_BYTE (ComPort.IIR);

    while ((ch & IIR_INTERRUPT_MASK) != IIR_NO_INTERRUPT_PENDING) {
        switch (ch & IIR_INTERRUPT_MASK) {
            case IIR_RX_ERROR_IRQ_PENDING: // 1st priority interrupt
                ch = IN_BYTE (ComPort.LSR);
                break;

            case IIR_RX_DATA_READY_IRQ_PENDING: // 2nd priority interrupt

                ch = IN_BYTE (ComPort.RBR);

                break;

            case IIR_TX_HBE_IRQ_PENDING: // 3rd priority interrupt

                break;

            case IIR_MODEM_STATUS_IRQ_PENDING: // 4th priority interrupt
                ch = IN_BYTE (ComPort.MSR);
                break;

            default:
                break;
        }
        ch = IN_BYTE (ComPort.IIR);
    }

    //
    // Clear the PIC with a non-specific EOI.
    //

    OUT_BYTE (PIC1_ICR, PIC_GENERAL_EOI);
}


//---------------------------------------------------------------------------
//
// Initiializes the UART and communications buffers.
//
SHORT InitializeCom (SHORT ComPortNumber, SHORT BaudRate, UCHAR Lcr)
{
    SHORT BaseAddress; // Serial port base address.
    SHORT i = 0;
    UCHAR ch;

    //
    // Select the serial port (COM1, COM2)
    //

    switch (ComPortNumber) {
        case COM1:
            BaseAddress = COM1_BASE_ADDRESS;
            break;

        case COM2:
            BaseAddress = COM2_BASE_ADDRESS;
            break;
        default:                        // Invalid comport.
            printf ("Invalid COMPORT=%i\n",ComPortNumber);
            return TRUE;
    }

    printf ("Comport base address = 0x%04X\n",BaseAddress);

    //
    // Initialize all of the 8250 registers.
    //

    ComPort.RBR = BaseAddress + RX_REGISTER_8250;
    ComPort.TBR = BaseAddress + TX_REGISTER_8250;
    ComPort.IER = BaseAddress + IER_8250;
    ComPort.IIR = BaseAddress + IIR_8250;
    ComPort.LCR = BaseAddress + LCR_8250;
    ComPort.MCR = BaseAddress + MCR_8250;
    ComPort.LSR = BaseAddress + LSR_8250;
    ComPort.MSR = BaseAddress + MSR_8250;
    ComPort.BAUD = BaseAddress + DIVISOR_REGISTER_8250;

    switch (ComPortNumber) {
        case COM1:                      // IRQ4
            ComPort.SPEM = IMR_ENABLE_IRQ4;
            ComPort.SPDM = IMR_DISABLE_IRQ4;
            ComPort.InterruptVector = IRQ4_ISR_VECTOR;

            //
            // Disable interrupts through the PIC.
            //

            ch = ((IN_BYTE (PIC1_IMR)) | (ComPort.SPDM));
            OUT_BYTE (PIC1_IMR, ch);
            
            //
            // Install the new ISR.
            //

            OldComIsr = _dos_getvect (ComPort.InterruptVector);
            _dos_setvect (ComPort.InterruptVector, ComIsr);
            break;

        case COM2:                      // IRQ3
            ComPort.SPEM = IMR_ENABLE_IRQ3;
            ComPort.SPDM = IMR_DISABLE_IRQ3;
            ComPort.InterruptVector = IRQ3_ISR_VECTOR;

            //
            // Disable interrupts through the PIC.
            //

            ch = ((IN_BYTE (PIC1_IMR)) | (ComPort.SPDM));
            OUT_BYTE (PIC1_IMR, ch);

            //
            // Install the new ISR.
            //

            OldComIsr = _dos_getvect (ComPort.InterruptVector);
            _dos_setvect (ComPort.InterruptVector, ComIsr);
            break;
    }

    //
    // Set the baud rate to the divisor value.
    //

    ch = ((IN_BYTE (ComPort.LCR)) | LCR_ENABLE_DIVISOR_LATCH);
    OUT_BYTE (ComPort.LCR, ch);
    ch = IN_BYTE (ComPort.LCR);
    OUT_BYTE (ComPort.BAUD, BaudRate);
    ch = IN_BYTE (ComPort.BAUD);

    if (ch != BaudRate) {
        printf ("Couldn't set BaudRate port %i\n", ComPortNumber);
        return TRUE;
    }

    ch = ((IN_BYTE (ComPort.LCR)) & LCR_DISABLE_DIVISOR_LATCH);
    OUT_BYTE (ComPort.LCR, ch);

    //
    // Enable Interrupts
    //

//    ch = (IER_ENABLE_RX_DATA_READY_IRQ | IER_ENABLE_TX_BE_IRQ |
//          IER_ENABLE_RX_ERROR_IRQ);
//    OUT_BYTE (ComPort.IER, ch);

    //
    // The data format = 1 start bit, 8 data bits, 1 stop bit, no parity.
    //

    ch = Lcr;
    OUT_BYTE (ComPort.LCR, ch);
    if ((IN_BYTE (ComPort.LCR)) != ch) {
        switch (ComPortNumber) {
            case COM1:
                printf ("Couldn't set LCR port %i\n", ComPortNumber);
                return TRUE;

            case COM2:
                printf ("Couldn't set LCR port %i\n", ComPortNumber);
                return TRUE;
        }
    }

    //
    // Clear any pending UART interrupts
    //
    i = 0;
    ch = IN_BYTE (ComPort.IIR);
    while (((ch & IIR_INTERRUPT_MASK) != IIR_NO_INTERRUPT_PENDING) &&
           (i++ < 1000)) {
        switch (ch & IIR_INTERRUPT_MASK) {
            case IIR_MODEM_STATUS_IRQ_PENDING:
                ch = IN_BYTE (ComPort.MSR);
                break;
            case IIR_TX_HBE_IRQ_PENDING:
                ch = IN_BYTE (ComPort.LSR);
                break;
            case IIR_RX_DATA_READY_IRQ_PENDING:
                ch = IN_BYTE (ComPort.RBR);
                break;
            case IIR_RX_ERROR_IRQ_PENDING:
                ch = IN_BYTE (ComPort.LSR);
                break;
            default:
                break;
        }
        ch = IN_BYTE (ComPort.IIR);
    }

    //
    // Check for stuck Interrupts
    //

    if (i >= 1000) {
        return TRUE;
    }

    //
    // Enable all UART interrupts on the IBM PC by asserting the GP02 general
    // purpose output.  Clear all other MCR bits. Activate DTR for RS485 use.
    //

    ch = MCR_ACTIVATE_GP02 | MCR_ACTIVATE_DTR;
    OUT_BYTE (ComPort.MCR, ch);

    //
    // Enable all Com interrupts through the PIC.
    //

//    ch = ((IN_BYTE (PIC1_IMR)) & ComPort.SPEM);
//    OUT_BYTE (PIC1_IMR, ch);

    printf ("Comport %i is initialized.\n", ComPortNumber);

    return FALSE;                       // Error FREE
}


//---------------------------------------------------------------------------
//***************************************************************************
//
// Main program loop - Put here for G.P.
//
int main (int argc, char *argv[])
{
    SHORT done = FALSE;
    SHORT i;

    SHORT DataLen;
    UCHAR DataBuffer[128];

    UCHAR Lcr;

    USHORT Val = 0;

    UCHAR Cur_Char = 0;
    UCHAR Rcv_Char;

    ULONG Test_Ok = 0;

   
    printf ("422-25I Testing Program\n");
    printf ("--------------------------------------------\n");
    printf ("%s\n", Copyright);
    printf ("%s\n", Address1);
    printf ("%s\n", Address2);
    printf ("--------------------------------------------\n");

    //
    // Do the RTC initialization first.
    //

    InitializeRtcIsr();

    //
    // Initialize the COM1 serial communication port.
    //

    Lcr = (LCR_EIGHT_BITS_PER_WORD | LCR_ONE_STOP_BIT | LCR_NO_PARITY);
    InitializeCom (COM2, BAUD_RATE_DIVISOR_57600, Lcr);

    printf ("--------------------------------------------\n");
    printf ("Press the ESC key to exit.\n");

    //
    // The following is a sample of how to use the functions.
    //

    while (!done) {

        if (kbhit()) {                  // Check for a keyboard event
            i = getch();

            if (i==0) {                 // check for extended keys.
                getch();
            } else {

                if (i == 27) {          // got an ESC key
                    done = TRUE;
                }
            }
        }                               // if (kbhit())


        //
        // Wait for Transmit buffer empty
        //

        while ((ComPort.LSR & LSR_TX_BOTH_EMPTY) == 0) {
            printf ("Waiting..\n");
            if (kbhit())
                break;
        }

        //
        // Output a byte
        //

        OUT_BYTE (ComPort.TBR, Cur_Char);

        //
        // delay 10 ms.
        //

        our_delay (1);

        //
        // Receive a byte.
        //

        Rcv_Char = IN_BYTE (ComPort.RBR);

        if (Rcv_Char == Cur_Char) {
          Cur_Char++;
        } else {
          Cur_Char = 0;
        }

        if (Cur_Char == 255) {
          beep();
          printf ("422_25 test O.K. #%li\n", Test_Ok++);
        }


    }                                   // while (!done)

    //
    // Restore the UART and RTC and deactivate ISR sources.
    //

    RestoreUART ();
    RestoreRTC ();

    printf ("Program exited normally.\n");

    return (1);
}


