//***************************************************************************
//
//
// TEST_ASY.C
//
//          Integrity Instruments (a division of Cogito Software, Inc.)
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
//  Tests the ASYNCHRONOUS function of a 485-ADC/485-POW/485-DIG board.
//  Make sure ONLY ONE 485-xxx board is connected!
// 
// 
//
// Last Modified:       
//      A. A. Kempka    11/10/95        Original.
//      A. A. Kempka    04/08/96        Added support for v1.2 firmware
//  
//
//***************************************************************************
//
//
//  Compiling:  This program was compiled under Borland 'C' 4.0 using
//              the following command line:
//
//                  BCC -3 -ml TEST_ASY.C
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
#include "cmds.h"


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
void (_interrupt _far *OldTimer0Isr)();

ULONG Timer0Count = 0;

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

const CHAR Copyright [] = "Copyright (c) 1996, Integrity Designs LLC";
const CHAR Address1 []  = "209 Grand Ave., Suite 403";
const CHAR Address2 []  = "Laramie, WY  307-721-4881";


//***************************************************************************
//
// Functions follow.
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
// Delays for x, milliseconds. Accurate!
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


//---------------------------------------------------------------------------
//
// Returns TRUE if a packet was yanked out of the PacketBuffer
//
SHORT ReceivedPacket (USHORT *Module_Address, UCHAR *DataBuffer, 
                      USHORT *DataBufferLen)
{
    USHORT Len, i;

    if (PacketCount) {                  // Retrieve packet from buffer

        //
        // Skip past the Host Address first.
        //

        PacketBufferTail++;
        if (PacketBufferTail > PacketBufferEnd) {
           PacketBufferTail = PacketBuffer;
        }

        //
        // Retrieve the Module Address.
        //

        PacketBufferTail++;
        if (PacketBufferTail > PacketBufferEnd) {
           PacketBufferTail = PacketBuffer;
        }

        *Module_Address = *PacketBufferTail;

        //
        // Next, get the control byte. Use the Length, discard  the
        // sequence number since it was checked in the ISR.
        //

        PacketBufferTail++;
        if (PacketBufferTail > PacketBufferEnd) {
           PacketBufferTail = PacketBuffer;
        }

        Len = (*PacketBufferTail) & 0x1F;

        //
        // Copy the Packet data into the DataBuffer.
        //

        for (i=0; i<Len; i++) {
            PacketBufferTail++;
            if (PacketBufferTail > PacketBufferEnd) {
               PacketBufferTail = PacketBuffer;
            }
            DataBuffer[i] = *PacketBufferTail;
        }

        //
        // Skip past the Chksum.
        //

        PacketBufferTail++;
        if (PacketBufferTail > PacketBufferEnd) {
           PacketBufferTail = PacketBuffer;
        }

        //
        // Data buffer length is packet length.
        //

        *DataBufferLen = Len;

        PacketCount--;                  

        return (TRUE);
    } else {
        return (FALSE);                 // No packet in buffer
    }
}


//---------------------------------------------------------------------------
//
//  May be used in lieu of RTC ISR - makes a .85ms timer
//
void _interrupt _far _loadds Timer0Isr ()
{
    int i;
    USHORT Mod_Address;
    USHORT Buffer_Len;
    UCHAR  Buffer [32];

    //
    // Bump miscellaneous timers.
    //

    RtcCount++;
    ComIdleTime++;                      // Incr the com idle clock.

    //
    // Check if we're to update internal async tables.
    //

    if (PacketUpdateTime >= PacketUpdateTimeout) {
        PacketUpdateTime = 0;

        //
        // Retrieve all packets from the buffer and update the 
        // data structure.
        //

        while (ReceivedPacket (&Mod_Address, Buffer, &Buffer_Len)) {

            //
            // Determine the Command (in Buffer [0]) and process
            //

            switch (Buffer [0]) {
                case INPUT_STATUS:
                    Module_Value [Mod_Address] [0] = (Buffer [1] << 8) |
                                                      Buffer [2];
                    break;

                case ANALOG_STATUS:
                    Module_Value [Mod_Address] [0] = (Buffer [1] << 8) |
                                                      Buffer [2];
                    Module_Value [Mod_Address] [1] = (Buffer [3] << 8) |
                                                      Buffer [4];
                    Module_Value [Mod_Address] [2] = (Buffer [5] << 8) |
                                                      Buffer [6];
                    Module_Value [Mod_Address] [3] = (Buffer [7] << 8) |
                                                      Buffer [8];
                    break;
            }
        }

    } else {
        PacketUpdateTime++;
    }

    Timer0Count++;                      // .85 ms (850 us) counter

    if (Timer0Count % 64) {             // hit every time except the 64th

        //
        // Clear the interrupt source.
        //

        i = IN_BYTE (0x61);

        OUT_BYTE (0x61, i | 0x80);
        OUT_BYTE (0x61, i);

        OUT_BYTE (PIC1_ICR, PIC_GENERAL_EOI);
    } else {                            // Do previous (old) ISR every 64th
        OldTimer0Isr();
    }
}

//---------------------------------------------------------------------------
//
// May be used in lieu of RTC timer - makes a .85 ms timer
//
void InitializeTimer0Isr (void)
{
    _disable();                         // Kill interrupts

    OldTimer0Isr = _dos_getvect (0x08); // get the current Timer 0 ISR
    _dos_setvect (0x08, Timer0Isr);

    OUT_BYTE (0x43, 0x34);
    OUT_BYTE (0x40, 0x00);
    OUT_BYTE (0x40, 0x04);

    _enable ();                     // Enable the interrupts.
    return;
}

//---------------------------------------------------------------------------
//
// Restores the original state of the 8254 Timer0
//
void RestoreTimer0Isr (void)
{
    _disable();                         // Reset the TIMER0 operation

    OUT_BYTE (0x43, 0x34);              // Back to 0xFFFF count
    OUT_BYTE (0x40, 0xFF);
    OUT_BYTE (0x40, 0xFF);
    _dos_setvect (0x08, OldTimer0Isr);

    _enable();
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
    // Check if we're to update internal async tables.
    //

    if (PacketUpdateTime >= PacketUpdateTimeout) {
        PacketUpdateTime = 0;

        //
        // Retrieve all packets from the buffer and update the 
        // data structure.
        //

        while (ReceivedPacket (&Mod_Address, Buffer, &Buffer_Len)) {

            //
            // Determine the Command (in Buffer [0]) and process
            //

            switch (Buffer [0]) {
                case INPUT_STATUS:
                    Module_Value [Mod_Address] [0] = (Buffer [1] << 8) |
                                                      Buffer [2];
                    break;

                case ANALOG_STATUS:
                    Module_Value [Mod_Address] [0] = (Buffer [1] << 8) |
                                                      Buffer [2];
                    Module_Value [Mod_Address] [1] = (Buffer [3] << 8) |
                                                      Buffer [4];
                    Module_Value [Mod_Address] [2] = (Buffer [5] << 8) |
                                                      Buffer [6];
                    Module_Value [Mod_Address] [3] = (Buffer [7] << 8) |
                                                      Buffer [8];
                    break;
            }
        }

    } else {
        PacketUpdateTime++;
    }

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
                if (ComIdleTime > COM_IDLE_TIMEOUT) {
                    
                    //
                    // Clear the counts and start the rcv buffer 
                    // pointer at the start of RcvBuffer if enough
                    // time has elapsed.
                    //
                    // Note: These values should also be cleared
                    //       immediately after sending a packet.
                    //

                    RcvBufferPosition = RcvBuffer;
                    RcvBufferCount = 0;

                } // if (ComIdleTime > COM_TIMEOUT)

                ComIdleTime = 0;        // Clear the timeout.

                ch = IN_BYTE (ComPort.RBR);

                //
                // Get the received CHARacter and place into the 
                // receive buffer. Also clears receive interrupt.
                //

                *RcvBufferPosition = ch;

                //
                // Use pointer arithmetic to get to the next position and
                // check buffer overrun.
                //

                RcvBufferPosition++;

                if (RcvBufferPosition > RcvBufferEnd) {
                    RcvBufferPosition--;
                }

                RcvBufferCount++;       // Count another byte.

                //
                // check if we have a complete packet only after the
                // addresses and control byte have been received along
                // with the Chksum byte
                //


                if (RcvBufferCount > 4) {

                    // 
                    // Make a check for end of packet via the length.
                    //

                    Len = (RcvBuffer[2] & 0x1F) + 4;

                    if ( (RcvBufferCount >= Len) &&
                         (Len < RECEIVE_BUFFER_SIZE)) {

                        //
                        // Clear up the counts.
                        //

                        RcvBufferPosition = RcvBuffer;
                        RcvBufferCount = 0;

                        //
                        // Verify the destination address
                        //

                        if (RcvBuffer [0] != HOST_MODULE) {
                            break;
                        }

                        //
                        // Verify the Chksum
                        //

                        Chksum = Gen_Chksum(RcvBuffer, Len);

                        if (Chksum) {
                            ChksumBadCount++;
                            break;
                        }

                        //
                        // If we're not expecting a response, this packet
                        // goes into the PacketBuffer and was not a response
                        // to a send packet.
                        //

                        if (!ExpectingAResponse) {

                            AsyncPacketsRcvd++;

                            //
                            // Check sequence number. Always allow
                            // a zero (initial) sequence number.
                            //

                            SeqNum = RcvBuffer[2] >> 5;
                            Module_Address = RcvBuffer [1];

                            if ((SeqNum == InSequenceNum[Module_Address]) &&
                                (SeqNum != 0)) {
                               break; 
                            }

                            //
                            //
                            // Save the new incoming sequence number.
                            //

                            InSequenceNum[Module_Address] = SeqNum;

                            //
                            // Check for room in the buffer and copy in.
                            //

                            if (EnoughRoomInPacketBuffer(Len)) {
                                for (i=0; i<Len; i++) {
                                    PacketBufferHead++;
                                    if (PacketBufferHead > PacketBufferEnd) {
                                        PacketBufferHead = PacketBuffer;
                                    }
                                    *PacketBufferHead = RcvBuffer [i];
                                }
                            }

                            PacketCount++;

                            //
                            // Construct a response packet and send out.
                            //

                            XmitBufferCount = 5;
                            XmitBuffer [0] =  Module_Address;
                            XmitBuffer [1] =  HOST_MODULE;
                            XmitBuffer [2] =  (SeqNum << 5) + 1;
                            XmitBuffer [3] = ACKNOWLEDGE;

                            Chksum = Gen_Chksum (XmitBuffer, XmitBufferCount - 1);

                            //
                            // Store Chksum where the buffer copy left off.
                            //

                            XmitBuffer [4]   = Chksum;

                            //
                            // Set the initial position, adjust the count, 
                            //  adjust the Position.
                            //

                            XmitBufferPosition = XmitBuffer;
                            XmitBufferCount--;
                            XmitBufferPosition++;

                            //
                            // Setup for data transmission by asserting RTS.
                            //

                            ch = ((IN_BYTE (ComPort.MCR)) | MCR_ACTIVATE_RTS);
                            OUT_BYTE (ComPort.MCR, ch);

                            //
                            // Kick-Start the UART by jamming a byte into 
                            // the Xmit register.
                            //

                            OUT_BYTE (ComPort.TBR, XmitBuffer[0]);

                            TransmittingAck = TRUE;

                        } else {

                            SyncPacketsRcvd++;

                            //
                            // Leave in RcvBuffer and signal SendPacket
                            // function that we've received a response packet.
                            //

                            ReceivedAResponse = TRUE;
                        }

                    } // if (Len >= RcvBufferCounter)

                } // if (RcvBufferCounter > 2)


                break;

            case IIR_TX_HBE_IRQ_PENDING: // 3rd priority interrupt

                ComIdleTime = 0;        // No longer idle.

                if (XmitBufferCount) {  // See if we're at 0.

                    XmitBufferCount--;  // Decr count.

                    //
                    // Jam another byte into the xmit register.
                    //

                    OUT_BYTE (ComPort.TBR, *XmitBufferPosition);
                    XmitBufferPosition++;

                } else {                // This is the last byte to send.


                    //
                    // wait for shift register to empty
                    //

                    ch = IN_BYTE (ComPort.LSR);

                    while ( (ch & LSR_TX_BOTH_EMPTY) != LSR_TX_BOTH_EMPTY) {
                            ch = IN_BYTE (ComPort.LSR);
                    }

                    //
                    // Turn off drive enable - RTS
                    //

                    ch = ((IN_BYTE (ComPort.MCR)) & MCR_DEACTIVATE_RTS);
                    OUT_BYTE (ComPort.MCR, ch);

                    //
                    // Prepare Receiver Pointers and Counts because 
                    // a reply is emminent.
                    //

                    RcvBufferPosition = RcvBuffer;
                    RcvBufferCount = 0;
                    TransmittingAck = FALSE;
                }
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

    ch = (IER_ENABLE_RX_DATA_READY_IRQ | IER_ENABLE_TX_BE_IRQ |
          IER_ENABLE_RX_ERROR_IRQ);
    OUT_BYTE (ComPort.IER, ch);

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
    // Set up the Receive and Transmit buffers, pointers, and counts.
    //

    RcvBufferPosition = RcvBuffer;
    RcvBufferEnd = &RcvBuffer[RECEIVE_BUFFER_SIZE - 1];
    RcvBufferCount = 0;

    XmitBufferPosition = XmitBuffer;
    XmitBufferEnd = &XmitBuffer[XMIT_BUFFER_SIZE - 1];
    XmitBufferCount = 0;

    //
    // Set up the packet buffer pointers.
    //

    PacketBufferHead = PacketBuffer;
    PacketBufferTail = PacketBuffer;
    PacketBufferEnd  = &PacketBuffer[PACKET_BUFFER_SIZE - 1];
    PacketCount = 0;

    //
    // Enable all UART interrupts on the IBM PC by asserting the GP02 general
    // purpose output.  Clear all other MCR bits. Activate DTR for RS485 use.
    //

    ch = MCR_ACTIVATE_GP02 | MCR_ACTIVATE_DTR;
    OUT_BYTE (ComPort.MCR, ch);

    //
    // Enable all Com interrupts through the PIC.
    //

    ch = ((IN_BYTE (PIC1_IMR)) & ComPort.SPEM);
    OUT_BYTE (PIC1_IMR, ch);

    printf ("Comport %i is initialized.\n", ComPortNumber);

    return FALSE;                       // Error FREE
}


//---------------------------------------------------------------------------
//
// Generates a CHKSUM for the specified buffer and length
//
UCHAR Gen_Chksum (UCHAR *Buffer, SHORT Len)
{
    UCHAR x = 0;
    SHORT i;

    for (i=0; i<Len; i++) {
        x += Buffer [i];
    }

    return (0x100 - x);
}


//---------------------------------------------------------------------------
//
// Packet SEND function that waits for a response.
//
SHORT SendCommandPacket (SHORT Module_Address, UCHAR *DataBuffer, 
                         SHORT *DataBufferLen)
{
    UCHAR Chksum, ch;
    USHORT i, Len;
    USHORT Retries = 0;
    SHORT done = FALSE;
    SHORT packet_sent = FALSE;

    while (!done) {                     // Try until done.

        //
        // Wait for our quiescent time
        //

        while ((ComIdleTime < COM_IDLE_TIMEOUT) ||
               (TransmittingAck)) {

            //
            // For for at least 3 milliseconds of bus quiscence.
            // Also, wait if we're transmitting an Async acknowledge.
            //

        }

        _disable();                     // Guard access

        //
        // Set the length (adding wrapper bytes) and install in buffer.
        //

        XmitBufferCount = *DataBufferLen + 4;

        XmitBuffer [0] =  Module_Address;
        XmitBuffer [1] =  HOST_MODULE;
        XmitBuffer [2] =  (OutSequenceNum [Module_Address] << 5) + 
                           *DataBufferLen;

        //
        // Copy data into Transmit buffer.
        //

        for (i=0; i<*DataBufferLen; i++) {
            XmitBuffer [3+i] = DataBuffer [i];
        }

        Chksum = Gen_Chksum (XmitBuffer, XmitBufferCount - 1);

        //
        // Store Chksum where the buffer copy left off.
        //

        XmitBuffer [3+i]   = Chksum;

        //
        // We're expecting a response.
        //

        ExpectingAResponse = TRUE;

        //
        // Set the initial position, adjust the count, adjust the Position.
        //

        XmitBufferPosition = XmitBuffer;
        XmitBufferCount--;
        XmitBufferPosition++;

        //
        // We haven't received a response yet.
        //

        ReceivedAResponse = FALSE;

        ComIdleTime = 0;                // Clear out the idle time before use

        _enable();

        //
        // Setup for data transmission by asserting RTS.
        //

        ch = ((IN_BYTE (ComPort.MCR)) | MCR_ACTIVATE_RTS);
        OUT_BYTE (ComPort.MCR, ch);

        //
        // Kick-Start the UART by jamming a byte into the Xmit register.
        //

        OUT_BYTE (ComPort.TBR, XmitBuffer[0]);

        //
        // Wait for a response or a timeout.
        //

        ComIdleTime = 0;                // Clear out the idle time before use

        while ((!ReceivedAResponse) && 
               (ComIdleTime < COM_RESPONSE_TIMEOUT)) {

            //
            // A smart programmer would calculate pi are something
            // else extremely useful of a humanitarian nature.
            //
               
        }

        //
        // Set expecting to FALSE so received packets go into PacketBuffer.
        //

        ExpectingAResponse = FALSE;

        //
        // Check if it was a timeout or a packet was received.
        //

        if (ReceivedAResponse) {

            //
            // Copy receive buffer.
            //

            Len = RcvBuffer[2] & 0x1F;

            //
            // Make sure there's at least one byte.
            //

            if (Len+4 >= 4) {

                //
                // Copy the data buffer.
                //

                for (i=0; i<Len; i++) {
                    DataBuffer [i] = RcvBuffer[i+3];
                }

                packet_sent = TRUE;
                done = TRUE;
            }

        } else {

            our_delay (5);              // wait for some time

            Retries++;                  // Incr retry count.

            //
            // Check for excessive retries.
            //

            if (Retries >= COM_PACKET_RETRIES) {
                done = TRUE;
            }
        }
    } // while(!done)

    //
    // Handle the sequence number.
    //

    if (OutSequenceNum[Module_Address] >= 0x07) {
        OutSequenceNum[Module_Address] = 0x01;
    } else {                            // Standard increment.
        OutSequenceNum[Module_Address]++;
    }

    return (!packet_sent);              // Error if packet NOT sent.
}


//***************************************************************************
//
// These functions rely on the Async Update occurring. This happens
// after a input state change or time has elapsed.
//
//
//---------------------------------------------------------------------------
//
// Analog data is sent ONLY on an Asyn Update time base.
//
//
SHORT Get_Analog (USHORT Module_Address, USHORT *Val1, USHORT *Val2, 
                                         USHORT *Val3, USHORT *Val4)
{
    if ((Module_Address < 0x01) || (Module_Address > 0xFF)) {
        return (ERR_INVALID_ADDRESS);
    }

    *Val1 = Module_Value [Module_Address] [0];
    *Val2 = Module_Value [Module_Address] [1];
    *Val3 = Module_Value [Module_Address] [2];
    *Val4 = Module_Value [Module_Address] [3];

    return (NO_ERROR);
}


//---------------------------------------------------------------------------
//
// Any change in input state (bit changing from 0 to 1  or 1 to 0  will
// generate an Asyn input status update. Also includes Async Update Time.
//
// *Value is a bit vector of inputs. The following is an example:
//
//  0x0001 = Input0 = ON, all others OFF
//  0x0003 = Input0 = ON Input1 = ON, all others OFF
//  0x0000 = all OFF
//
SHORT Get_Inputs (USHORT Module_Address, USHORT *Value)
{
    if ((Module_Address < 0x01) || (Module_Address > 0xFF)) {
        return (ERR_INVALID_ADDRESS);
    }

    *Value = Module_Value [Module_Address] [0];

    return (NO_ERROR);
}


//***************************************************************************
//
// Utility functions -  These functions send commands in an essentially
//                      synchronous fashion whereby a response packet is
//                      sent immediately by the Module.
//
//
//


//---------------------------------------------------------------------------
//
//  Resets the module! No response is expected
//
SHORT Reset_Module (USHORT Module_Address)
{
    SHORT DataLen;
    UCHAR DataBuffer[128];

    //
    // Construct the command and data.
    //

    DataBuffer [0] = RESET_MODULE;
    DataLen = 1;

    //
    // Send the packet and response. Returns TRUE on send ERROR
    //

    if (SendCommandPacket (Module_Address, DataBuffer, &DataLen)) {
        return (ERR_NO_RESPONSE);
    }

    if (DataBuffer[0] != MODULE_RESET) {
        return (ERR_INVALID_RESPONSE);
    }

    our_delay (50);                     // Make sure we wait for CPU to RESET

    return (NO_ERROR);
}


//---------------------------------------------------------------------------
//
// Returns the Software version of the addressed module.
// The upper byte of *Value contains the major number
// while the lower byte of *Value contains the minor number.
//
// i.e. *Value = 0x0102 = v1.2
//
SHORT Querry_SW_Version (USHORT Module_Address, USHORT *Value)
{
    SHORT DataLen;
    UCHAR DataBuffer[128];

    if ((Module_Address < 0x01) || (Module_Address > 0xFF)) {
        return (ERR_INVALID_ADDRESS);
    }

    //
    // Construct the command and data.
    //

    DataBuffer [0] = READ_SW_VER;
    DataLen = 1;

    //
    // Send the packet and response. Returns TRUE on send ERROR
    //

    if (SendCommandPacket (Module_Address, DataBuffer, &DataLen)) {
        return (ERR_NO_RESPONSE);
    }

    if (DataBuffer[0] != SW_VERSION) {
        return (ERR_INVALID_RESPONSE);
    }

    *Value = (DataBuffer [1] << 8) | DataBuffer [2];

    return (NO_ERROR);
}


//---------------------------------------------------------------------------
//
// WARNING: Do not use locations 0..6 these are used by the MODULE!
//          0 - Module Address & Async Packet Destination Address
//          1 - Module Type & I/O Configuration
//          2 - Async Update Time 
//          3 ... 15 - Reserved
//          16 ... 63 are available for the user
//
// *EEPROM Address must be a number 0..63
//
SHORT Write_EEPROM (USHORT Module_Address, USHORT Address, USHORT *Value)
{
    SHORT DataLen;
    UCHAR DataBuffer[128];

    if ((Module_Address < 0x01) || (Module_Address > 0xFF)) {
        return (ERR_INVALID_ADDRESS);
    }

    if (Address > 63) {
        return (ERR_INVALID_PARAMETER);
    }

    //
    // Construct the command and data.
    //

    DataBuffer [0] = WRITE_EEPROM;
    DataBuffer [1] = Address;
    DataBuffer [2] = (*Value & 0xFF00) >> 8;
    DataBuffer [3] = *Value & 0x00FF;

    DataLen = 4;

    //
    // Send the packet and response. Returns TRUE on send ERROR
    //

    if (SendCommandPacket (Module_Address, DataBuffer, &DataLen)) {
        return (ERR_NO_RESPONSE);
    }

    if (DataBuffer[0] != EEPROM_WRITTEN) {
        return (ERR_INVALID_RESPONSE);
    }

    our_delay (20);                     // Wait for the EEPROM to program.

    return (NO_ERROR);
}


//---------------------------------------------------------------------------
//
// *EEPROM Address must be a number 0...63
//
SHORT Read_EEPROM (USHORT Module_Address, USHORT Address, USHORT *Value)
{
    SHORT DataLen;
    UCHAR DataBuffer[128];

    if ((Module_Address < 0x01) || (Module_Address > 0xFF)) {
        return (ERR_INVALID_ADDRESS);
    }

    if (Address > 63) {
        return (ERR_INVALID_PARAMETER);
    }

    //
    // Construct the command and data.
    //

    DataBuffer [0] = READ_EEPROM;
    DataBuffer [1] = Address;

    DataLen = 2;

    //
    // Send the packet and response. Returns TRUE on send ERROR
    //

    if (SendCommandPacket (Module_Address, DataBuffer, &DataLen)) {
        return (ERR_NO_RESPONSE);
    }

    if (DataBuffer[0] != EEPROM_READ) {
        return (ERR_INVALID_RESPONSE);
    }

    *Value = (DataBuffer [1] << 8) | DataBuffer [2];

    return (NO_ERROR);
}


//---------------------------------------------------------------------------
//
// *Value is the configuration value (using the LSB)
//
// This command is legal only for the ID485-ADC and the ID485-DIG modules.
//---------------------------------------------------------------------------
//
// ID485-ADC
// ---------
// (SE = single ended, DF = differential, UN = unipolar, BI = bipolar
//
// *Value = 0x00 CH0=CH1=SE    CH2=CH3=SE
// *Value = 0x01 CH0=CH1=DF    CH2=CH3=SE
// *Value = 0x02 CH0=CH1=SE    CH2=CH3=DF
// *Value = 0x03 CH0=CH1=DF    CH2=CH3=DF
//
// *Value = 0x0_ CH0=BI CH1=BI CH2=BI CH3=BI
// *Value = 0x1_ CH0=UN CH1=BI CH2=BI CH3=BI
// *Value = 0x2_ CH0=BI CH1=UN CH2=BI CH3=BI
// *Value = 0x3_ CH0=UN CH1=UN CH2=BI CH3=BI
// *Value = 0x4_ CH0=BI CH1=BI CH2=UN CH3=BI
// *Value = 0x5_ CH0=UN CH1=BI CH2=UN CH3=BI
// *Value = 0x6_ CH0=BI CH1=UN CH2=UN CH3=BI
// *Value = 0x7_ CH0=UN CH1=UN CH2=UN CH3=BI
// *Value = 0x8_ CH0=BI CH1=BI CH2=BI CH3=UN
// *Value = 0x9_ CH0=UN CH1=BI CH2=BI CH3=UN
// *Value = 0xA_ CH0=BI CH1=UN CH2=BI CH3=UN
// *Value = 0xB_ CH0=UN CH1=UN CH2=BI CH3=UN
// *Value = 0xC_ CH0=BI CH1=BI CH2=UN CH3=UN
// *Value = 0xD_ CH0=UN CH1=BI CH2=UN CH3=UN
// *Value = 0xE_ CH0=BI CH1=UN CH2=UN CH3=UN
// *Value = 0xF_ CH0=UN CH1=UN CH2=UN CH3=UN
//
// ID485-DIG
// ---------
// *Value = 0x00 Digital PORT0=INPUTS   PORT1=INPUTS
// *Value = 0x01 Digital PORT0=OUTPUTS  PORT1=INPUTS
// *Value = 0x02 Digital PORT0=INPUTS   PORT1=OUTPUTS
// *Value = 0x03 Digital PORT0=OUTPUTS  PORT1=OUTPUTS
// *Value = all others invalid
//
//
// ID485-POW/ID485-OUT
// -------------------
// No affect
//
SHORT Configure_IO (USHORT Module_Address, USHORT *Value)
{
    SHORT DataLen;
    UCHAR DataBuffer[128];
    USHORT Val;
    SHORT  FuncRtn;

    if ((Module_Address < 0x01) || (Module_Address > 0xFF)) {
        return (ERR_INVALID_ADDRESS);
    }

    //
    // First read the current Module Type
    //

    FuncRtn = Read_EEPROM (Module_Address, 0x01, &Val);

    if (FuncRtn != NO_ERROR) {
        return (FuncRtn);
    }

    Val &= 0xFF00;                      // Mask off current configuration
    Val |= (*Value & 0x00FF);           // Make new configuration

    //
    // Write the new configuration to EEPROM
    //

    FuncRtn = Write_EEPROM (Module_Address, 0x01, &Val);

    if (FuncRtn != NO_ERROR) {
        return (FuncRtn);
    }

    //
    // Reset the module so changes take effect
    //

    Reset_Module (Module_Address);

    return (NO_ERROR);
}

//---------------------------------------------------------------------------
//
// *Value is the Async update time in milliseconds.
//
// *Value = 0x0000 disables *ALL* Async Updates.
// *Value = 0x0001 disables *TIMED* Async Updates
// *Value = 0x0002 ... 0x0032 not suggested 
// *Value = all others valid
//
SHORT Set_Update_Time (USHORT Module_Address, USHORT *Value)
{
    SHORT DataLen;
    UCHAR DataBuffer[128];
    USHORT Val;
    SHORT  FuncRtn;

    if ((Module_Address < 0x01) || (Module_Address > 0xFF)) {
        return (ERR_INVALID_ADDRESS);
    }

    // --------------------------------------------------------------
    // Not a good idea to be less than 100 milliseconds if not 0x0000
    // --------------------------------------------------------------

    if ((*Value != 0x0000) && (*Value != 0x001) && (*Value < 100)) {
        return (ERR_INVALID_PARAMETER);
    }

    Val = *Value;                       // Store update time.

    //
    // Write the new configuration to EEPROM
    //

    FuncRtn = Write_EEPROM (Module_Address, 0x02, &Val);

    if (FuncRtn != NO_ERROR) {
        return (FuncRtn);
    }

    //
    // Reset the module so changes take effect
    //

    Reset_Module (Module_Address);


    return (NO_ERROR);
}


//***************************************************************************
//
// Polled synchronous functions to get information to/from modules.
//
// These functions return status as it exists immediately at a module.
//
//***************************************************************************


//---------------------------------------------------------------------------
//
// This command is legal ONLY for ID485_ADC modules.
//
SHORT Querry_Analog (USHORT Module_Address, USHORT *Val1, USHORT *Val2, 
                                            USHORT *Val3, USHORT *Val4)
{
    SHORT DataLen;
    UCHAR DataBuffer[128];

    if ((Module_Address < 0x01) || (Module_Address > 0xFF)) {
        return (ERR_INVALID_ADDRESS);
    }

    //
    // Construct the command and data.
    //

    DataBuffer [0] = READ_ANALOG;
    DataLen = 1;

    //
    // Send the packet and response. Returns TRUE on send ERROR
    //

    if (SendCommandPacket (Module_Address, DataBuffer, &DataLen)) {
        return (ERR_NO_RESPONSE);
    }

    if (DataBuffer[0] != ANALOG_STATUS) {
        return (ERR_INVALID_RESPONSE);
    }

    //
    // Return all channel values
    //

    *Val1 = (DataBuffer[1] << 8) + DataBuffer[2];
    *Val2 = (DataBuffer[3] << 8) + DataBuffer[4];
    *Val3 = (DataBuffer[5] << 8) + DataBuffer[6];
    *Val4 = (DataBuffer[7] << 8) + DataBuffer[8];

    our_delay (10);                     // Give a small delay for A/D conversion

    return (NO_ERROR);
}


//---------------------------------------------------------------------------
//
// This command is legal ONLY for ID485_POW and ID485_DIG modules.
//
// *Value is a bit vector of inputs. 
//
//  The following is an example:
//
//  0x0001 = Input0 Port0 = ON, all others OFF
//  0x0003 = Input0 Port0 = ON Input1 Port0 = ON, all others OFF
//  0x0100 = Input0 Port1 = ON, all others OFF
//  0x0300 = Input0 Port1 = ON Input1 Port1 = ON, all others OFF
//
//  0x0000 = all OFF
//
SHORT Querry_Inputs (USHORT Module_Address, USHORT *Value)
{
    SHORT DataLen;
    UCHAR DataBuffer[128];

    if ((Module_Address < 0x01) || (Module_Address > 0xFF)) {
        return (ERR_INVALID_ADDRESS);
    }

    //
    // Construct the command and data.
    //

    DataBuffer [0] = READ_INPUTS;
    DataLen = 1;

    //
    // Send the packet and response. Returns TRUE on send ERROR
    //

    if (SendCommandPacket (Module_Address, DataBuffer, &DataLen)) {
        return (ERR_NO_RESPONSE);
    }

    if (DataBuffer[0] != INPUT_STATUS) {
        return (ERR_INVALID_RESPONSE);
    }

    *Value = (DataBuffer [1] << 8) | DataBuffer [2];

    return (NO_ERROR);
}

//---------------------------------------------------------------------------
//
// Sets the outputs of the addressed module IMMEDIATELY
// This command is legal ONLY for ID485_POW and ID485_DIG modules.
//
// *Value is a bit vector of outputs. 
//
//  The following is an example:
//
//  0x0001 = Input0 Port0 = ON, all others OFF
//  0x0003 = Input0 Port0 = ON Input1 Port0 = ON, all others OFF
//  0x0100 = Input0 Port1 = ON, all others OFF
//  0x0300 = Input0 Port1 = ON Input1 Port1 = ON, all others OFF
//
//  0x0000 = all OFF
//
SHORT Set_Outputs (USHORT Module_Address, USHORT *Value)
{
    SHORT DataLen;
    UCHAR DataBuffer[128];

    if ((Module_Address < 0x01) || (Module_Address > 0xFF)) {
        return (ERR_INVALID_ADDRESS);
    }

    //
    // Construct the command and data.
    //

    DataBuffer [0] = SET_OUTPUTS;
    DataBuffer [1] = (*Value & 0xFF00) >> 8;
    DataBuffer [2] = *Value & 0xFF;

    DataLen = 3;

    //
    // Send the packet and response. Returns TRUE on send ERROR
    //

    if (SendCommandPacket (Module_Address, DataBuffer, &DataLen)) {
        return (ERR_NO_RESPONSE);
    }

    if (DataBuffer[0] != OUTPUTS_SET) {
        return (ERR_INVALID_RESPONSE);
    }


    return (NO_ERROR);
}


//---------------------------------------------------------------------------
//
//  Reads the current output port(s)
//  This command is legal ONLY for ID485_POW and ID485_DIG modules.
//
// *Value is a bit vector of the current outputs. 
//
//  The following is an example:
//
//  0x0001 = Input0 Port0 = ON, all others OFF
//  0x0003 = Input0 Port0 = ON Input1 Port0 = ON, all others OFF
//  0x0100 = Input0 Port1 = ON, all others OFF
//  0x0300 = Input0 Port1 = ON Input1 Port1 = ON, all others OFF
//
//  0x0000 = all OFF
//
SHORT Querry_Outputs (USHORT Module_Address, USHORT *Value)
{
    SHORT DataLen;
    UCHAR DataBuffer[128];

    if ((Module_Address < 0x01) || (Module_Address > 0xFF)) {
        return (ERR_INVALID_ADDRESS);
    }

    //
    // Construct the command and data.
    //

    DataBuffer [0] = READ_OUTPUTS;
    DataLen = 1;

    //
    // Send the packet and response. Returns TRUE on send ERROR
    //

    if (SendCommandPacket (Module_Address, DataBuffer, &DataLen)) {
        return (ERR_NO_RESPONSE);
    }

    if (DataBuffer[0] != OUTPUT_STATUS) {
        return (ERR_INVALID_RESPONSE);
    }

    *Value = (DataBuffer [1] << 8) | DataBuffer [2];

    return (NO_ERROR);
}


//---------------------------------------------------------------------------
//
//  Defines and routines to convert data from the ID485-ADC
//  analog input module.
//

#define     V_PER_CNT_UNI   (5.0/4096.0)
#define     V_PER_CNT_BI    (10.0/4096.0)

//---------------------------------------------------------------------------
//
//  Converts A/D counts (integer) to a real number voltage.
//
//  For UNIPOLAR conversions!
//
REAL Cnts_to_Volts_Uni (USHORT Val) {
    return (Val * V_PER_CNT_UNI);
}

//---------------------------------------------------------------------------
//
//  Converts A/D counts (integer) to a real number voltage.
//
//  For BIPOLAR conversions!
//
REAL Cnts_to_Volts_Bi (USHORT Val) {
    SHORT x;

    //
    // Check for sign and extend it to 16 bits from 12 bits
    //

    if (Val & 0x0800) {
        x = (SHORT)(Val | 0xF000);
    } else {
        x = Val;
    }

    return (x * V_PER_CNT_BI);
}



//---------------------------------------------------------------------------
//***************************************************************************
//
// Main program loop 
//
int main (int argc, char *argv[])
{
    SHORT done = FALSE;
    SHORT ch, i;

    SHORT DataLen;
    UCHAR DataBuffer[128];

    UCHAR strn[128];

    UCHAR Lcr;

    USHORT Val1, Val2, Val3, Val4, Val5;

    printf ("ID485 ASYNC COMMUNICATIONS TEST PROGRAM v1.2\n");

    printf ("%s\n", Copyright);

    //
    // Do the hardware initialization sequence.
    //

    InitializeRtcIsr();

    //
    // Initialize the COM1 serial communication port.
    //

    Lcr = (LCR_EIGHT_BITS_PER_WORD | LCR_ONE_STOP_BIT | LCR_NO_PARITY);

    if (argc == 2) {
        strupr (argv [1]);

        if (!strcmp (argv[1], "-2")) {
            InitializeCom (COM2, BAUD_RATE_DIVISOR_19200, Lcr);
        } else {
            printf ("ERROR! Illegal command line option: %s\n", argv[1]);
            RestoreRTC ();
            exit (-1);
        }
    } else {

        if (argc == 1) {
            InitializeCom (COM1, BAUD_RATE_DIVISOR_19200, Lcr);
        } else {
            printf ("ERROR! Illegal command line option: %s\n", argv[1]);
            RestoreRTC ();
            exit (-1);
        }
    }

    printf ("---------------------------------------------\n");
    printf ("MAKE SURE THE MODULE CONNECTED IS ADDRESS 1!\n");
    printf ("---------------------------------------------\n");
    printf ("Enter 'ESC' anytime to Quit.\n");

    if (!Read_EEPROM(0x01, 0x00, &Val1)) {
        printf ("Address & Dest=%04X\n", Val1);
    } else {
        printf ("Can't read EEPROM!\n");
    }

    if (!Read_EEPROM(0x01, 0x01, &Val1)) {
        printf ("Type & Config=%04X\n", Val1);
    } else {
        printf ("Can't read EEPROM!\n");
    }

    while (!done) {
        if (kbhit ()) {
            ch = getch();

            if (ch == 27) {
                done = TRUE;
            }

            if (ch == 0) {              // extended key codes
                getch();
            }
        }

        Get_Analog(0x01, &Val1, &Val2, &Val3, &Val4);

        Get_Inputs(0x01, &Val5);

        printf ("CH0=%04X CH1=%04X CH2=%04X CH3=%04X DIG_PORT=%04x\r",
                 Val1, Val2, Val3, Val4, Val5);
    }                                   // while (!done)

    //
    // Restore the UART and RTC and deactivate ISR sources.
    //

    RestoreUART ();
    RestoreRTC ();

    return (1);
}
