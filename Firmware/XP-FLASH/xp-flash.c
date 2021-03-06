//*****************************************************************************
//
// Integrity Instruements XP Converter firmware.
//
// For: XP-DIN and XP-DB9 boards.
//
// Target: PIC16F628A
//
// Comments:
// ----------
// Original
//

#if defined(__PCM__)
#include <16f628a.h>
// #device ICD=TRUE  -- Cannot use ICD with PIC16F628A !!!!!

// HS = High Speed osc, PROTECT = code protect, BROWNOUT = brownout protection
// PUT = power up timer, NOLVP = no low voltage program,
// NOWRT = no write protect, NOCPD = no EEPROM protect
#fuses HS, MCLR, NOWDT, PROTECT, BROWNOUT, PUT, NOLVP, NOCPD
#use delay(clock=14745600)
#use rs232(baud=9600, xmit=PIN_B2, rcv=PIN_B1)
#endif

//
// Turn off the compiler's caching (virtual port) auto data direction crap
//
#use fast_io(A)
#use fast_io(B)

//
// Memory (port) defines so we can use ports directly in 'C'
// e.g.   EEADR = value;
//
#byte EEADR=0x9B
#byte EEDATA=0x9A
#byte EECON1=0x9C
#byte EECON2=0x9D
#byte TMR1L=0x0E
#byte TMR1H=0x0F

//
// EEPROM Locations
//
#define EEPROM_MODE              0x00
#define EEPROM_BAUD              0x01
#define EEPROM_SILENT_POST       0x04
#define EEPROM_SN1               0x10
#define EEPROM_SN2               0x11
#define EEPROM_SN3               0x12
#define EEPROM_SN4               0x13

//
// Auto Enable modes
//
#define MODE_BIT_CALCULATED     0x01
#define MODE_MANUAL_BAUD        0x02

//
// Chip select and control lines
//
#define ENABLE_HOLD     PIN_A0
#define ADDR_TXD        PIN_A1
#define ADDR_RXD        PIN_A2

#define RCVBUFFERSIZE   8
#define XMITBUFFERSIZE  25

//
// Global variable declarations
//
char    LastCharRcvd;
char    RcvBuffer[RCVBUFFERSIZE];
int8    RcvBufferIndex;
char    XmitBuffer[XMITBUFFERSIZE];
int8    DontBufferRcv;
int8    ByteRcvd;

int16   OneMsCount;
int16   MinBitTime;
int16   EnableTimeout;

//
// Additional includes
//
#include "string.h"


//-----------------------------------------------------------------------------
// Read EEPROM
// The 16F628A has an internal EEPROM
//
#separate
int8 rd_eeprom(int8 address)
{
    int8    data;

    EEADR = address;    // Address register
    EECON1 = 0x01;      // Access Data EEPROM, RD=1
    data = EEDATA;      // Read the data out of the EEPROM
    return (data);
}


//-----------------------------------------------------------------------------
// Write EEPROM
// The 16F628A has an internal EEPROM
//
#separate
wr_eeprom(int8 address, int8 data)
{
    int8 i;

    EEADR = address;    // Address register
    EEDATA = data;      // Data to write to the EEPROM

    // Do not change a single line in the following #asm block
    // The assembly output must match the Microchip documentation exactly
    // and changing anything will screw it up since the compiler shoves
    // its crap in-line also.
#asm
    bcf     0x0B, 7     ; Disable INT's INTCON GIE
    bsf     EECON1, 2   ; Write enable - WREN
    movlw   0x55        ; Write unlock sequence
    movwf   EECON2        ; EECON2
    movlw   0xAA
    movwf   EECON2        ; EECON2
    bsf     EECON1, 1   ; Initiate write - WR
    bsf     0x0B, 7     ; enable INT's - INTCON GIE
#endasm

    //
    // Wait for WRITE to complete
    do {
        delay_cycles(1);
        i = EECON1;
    } while (i & 0x02);         // Bit 1 = WR (write status)

    return;
}


//-----------------------------------------------------------------------------
// Converts 2 hexadecimal ASCII characters pointed to *ptr to a binary value
//
#separate
int8 hex_to_bin(int8 *ptr)
{
    int8 value, i;
    int8 *tmp_ptr;

    tmp_ptr = ptr;
    value = 0;

    // Upper nibble conversion
    if (isdigit(*tmp_ptr))
    {
        value = (*tmp_ptr - '0') << 4;
    } else {
        if (isupper(*tmp_ptr))
        {
            value = ((*tmp_ptr - 'A') + 0x0A) << 4;
        } else {
            value = ((*tmp_ptr - 'a') + 0x0A) << 4;
        }
    }

    // LSB conversion
    tmp_ptr++;
    if (isdigit(*tmp_ptr))
    {
        i = (*tmp_ptr - '0');
    } else {
        if (isupper(*tmp_ptr))
        {
            i = (*tmp_ptr - 'A') + 0x0A;
        } else {
            i = (*tmp_ptr - 'a') + 0x0A;
        }
    }
    value = value + i;
    return (value);
}


//-----------------------------------------------------------------------------
// Converts binary value to two hexadecimal ASCII characters
//
#separate
void bin_to_hex(int8 value, int8 *ptr)
{
    int8 digit;
    int8 *tmp_ptr;

    tmp_ptr = ptr;

    // MSB first
    digit = ((value & 0xF0) >> 4) + 0x30;
    if (digit > 0x39)
        digit += 0x07;
    *tmp_ptr = digit;

    // LSB next
    tmp_ptr++;
    digit = (value & 0x0F) + 0x30;
    if (digit > 0x39)
        digit += 0x07;
    *tmp_ptr = digit;

    return;
}


//-----------------------------------------------------------------------------
// External interrupt ISR
// PORTB Pin 0
//
#int_EXT
EXT_isr()
{
    // Hold the RS-485 enable
    output_low(ENABLE_HOLD);   // Active = OFF
    return;
}


//-----------------------------------------------------------------------------
// UART Receive character ISR
//
#int_RDA
RDA_isr()
{
//    output_high(ENABLE_HOLD);   // Release RS-485 Enable. Idle = ON

    // Get character out of UART and save in receive buffer
    LastCharRcvd = getc();

    if (DontBufferRcv)
    {
        ByteRcvd++;
    } else {
        // Save character in receive buffer
        RcvBuffer[RcvBufferIndex] = LastCharRcvd;
        RcvBufferIndex++;
        // Check for receive buffer overflow
        if (RcvBufferIndex >= RCVBUFFERSIZE) {
            RcvBufferIndex--;
        }
    }
    return;
}


//-----------------------------------------------------------------------------
// Timer2 1 ms. ISR
//
#int_TIMER2
TIMER2_isr()
{
   OneMsCount++;
   return;
}


//-----------------------------------------------------------------------------
// Reads the Buad Rate out of EEPROM and sets the UART accordingly
//
void print_baud(void)
{
    int8    BaudCode;
    int32   BaudRate;

    // Get Baud rate code from EEPROM
    BaudCode = rd_eeprom(EEPROM_BAUD);

    // Only lower nibble is valid
    switch (BaudCode & 0x0F) {
        case 0:
        case 1:     // 1200
            BaudRate = 1200;
            break;
        case 2:     // 2400
            BaudRate = 2400;
            break;
        case 3:     // 4800
            BaudRate = 4800;
            break;
        case 4:     // 9600
            BaudRate = 9600;
            break;
        case 5:     // 19200
            BaudRate = 19200;
            break;
        case 6:     // 38400
            BaudRate = 38400;
            break;
        case 7:     // 57600
            BaudRate = 57600;
            break;
        case 8:     // 115200
            BaudRate = 115200;
            break;
        case 9:     // 230400
            BaudRate = 230400;
            break;
        default:    // 115200
            BaudRate = 115200;
            break;
    }

    printf(" %lu BPS\r", BaudRate);
    return;
}


void print_mode(void)
{
    int8 i;

    i = rd_eeprom(EEPROM_MODE);

    if (i == MODE_MANUAL_BAUD) {
        printf(" manual fixed baud\r");
    } else {
        printf(" automatic\r");
    }
    return;
}


//-----------------------------------------------------------------------------
// Handles Progamming Mode User Interface
//
void ProgramMode(void)
{
    int8 Quit, i, j;

    Quit = 0;

    // Loop until the user quits
    while (!Quit) {
        // Wait for CR (0x0D) before processing command
        while (LastCharRcvd != 0x0D) {
            delay_cycles(1);
        }

        switch (RcvBuffer[0]) {
            case '?':            // Display current settings
                printf("S/N ");
                i = rd_eeprom(EEPROM_SN1);
                printf("%X", i);
                i = rd_eeprom(EEPROM_SN2);
                printf("%X", i);
                i = rd_eeprom(EEPROM_SN3);
                printf("%X", i);
                i = rd_eeprom(EEPROM_SN4);
                printf("%X\r", i);
                i = rd_eeprom(EEPROM_MODE);
                printf("Mode %X (Mx where x=1 or 2)", i);
                print_mode();
                i = rd_eeprom(EEPROM_BAUD);
                printf("Default Baud %X (Bx where x=1...9)", i);
                print_baud();
                i = rd_eeprom(EEPROM_SILENT_POST);
                if (i)
                {
                    printf("Silent Power Up OFF (Sx where x=0 or 1)\r", i);
                } else {
                    printf("Silent Power Up ON (Sx where x=0 or 1)\r", i);
                }
                break;
            case 'W':            // Write EEPROM
            case 'w':
                if (RcvBufferIndex != 6)            // Check for correct command length
                    goto error_rsp;
                i = hex_to_bin(&RcvBuffer[1]);      // EEPROM Address
                j = hex_to_bin(&RcvBuffer[3]);      // EEPROM Value
                wr_eeprom(i, j);
                printf("W\r");
                break;
            case 'R':            // Read EEPROM
            case 'r':
                if (RcvBufferIndex != 4)            // Check for correct command length
                    goto error_rsp;
                i = hex_to_bin(&RcvBuffer[1]);      // EEPROM Address
                j = rd_eeprom(i);
                printf("R%X\r", j);
                break;
            case 'M':            // Mode change command
            case 'm':
                if (RcvBufferIndex != 3)            // Check for correct command length
                    goto error_rsp;
                // Since there is only 1 hex nibble, overwrite command byte with '0'
                RcvBuffer[0] = '0';
                i = hex_to_bin(&RcvBuffer[0]);      // Mode value
                // Todo: Check range of i
                wr_eeprom(EEPROM_MODE, i);
                printf("M\r");
                break;
            case 'B':            // Baud change command
            case 'b':
                if (RcvBufferIndex != 3)            // Check for correct command length
                    goto error_rsp;
                // Since there is only 1 hex nibble, overwrite command byte with '0'
                RcvBuffer[0] = '0';
                i = hex_to_bin(&RcvBuffer[0]);      // Baud value
                // Todo: Check range of i
                wr_eeprom(EEPROM_BAUD, i);
                printf("B\r");
                break;
            case 'S':            // Silent power up
            case 's':
                if (RcvBufferIndex != 3)            // Check for correct command length
                    goto error_rsp;
                // Since there is only 1 hex nibble, overwrite command byte with '0'
                RcvBuffer[0] = '0';
                i = hex_to_bin(&RcvBuffer[0]);      // Silent POST value
                // Todo: Check range of i
                wr_eeprom(EEPROM_SILENT_POST, i);
                printf("S\r");
                break;
            case 'Q':            // Quit to run mode
            case 'q':
            case 'G':
            case 'g':
                printf("Exit\r");
                // Delay until all characters are sent
                delay_ms(100);
                Quit = 1;
                break;
            default:             // Unknown command
error_rsp:
                printf("Err!\r");
                break;
        } // end of ... switch (RcvBuffer[0]) {
        // Ready receive buffer and pointers for commands
        LastCharRcvd = 0;
        RcvBufFerIndex = 0;
    } // end of ... while (!Quit) {
    return;
}

//-----------------------------------------------------------------------------
// Reads the Buad Rate out of EEPROM and sets the UART accordingly
//
int8 set_baud(void)
{
    int8  BaudCode;
    int16 RtnVal;


    // Get Baud rate code from EEPROM
    BaudCode = rd_eeprom(EEPROM_BAUD);

    // Only lower nibble is valid
    switch (BaudCode & 0x0F) {
        case 0:
        case 1:     // 1200
            set_uart_speed(1200);
            RtnVal = 340;
            break;
        case 2:     // 2400
            set_uart_speed(2400);
            RtnVal = 41;
            break;
        case 3:     // 4800
            set_uart_speed(4800);
            RtnVal = 18;
            break;
        case 4:     // 9600
            set_uart_speed(9600);
            RtnVal = 7;
            break;
        case 5:     // 19200
            set_uart_speed(19200);
            RtnVal = 1;
            break;
        case 6:     // 38400
            set_uart_speed(38400);
            RtnVal = 0;
            break;
        case 7:     // 57600
            set_uart_speed(57600);
            RtnVal = 0;
            break;
        case 8:     // 115200
            set_uart_speed(115200);
            RtnVal = 0;
            break;
        case 9:     // 230400
            set_uart_speed(230400);
            RtnVal = 0;
            break;
        default:    // 115200
            set_uart_speed(115200);
            RtnVal = 0;
            break;
    }
    return (RtnVal);
}

void check_new_min(void)
{
    int16 i;

    i = get_timer1();
    // Clear Timer1 counter - used to calculate next minimum
    set_timer1(0);
    if (i < MinBitTime)
    {
        MinBitTime = i;
        EnableTimeout = MinBitTime * 8;
// BUGBUG: Test code
//        wr_eeprom(0x20, MinBitTime&0x00FF);
//        wr_eeprom(0x21, (MinBitTime&0xFF00)>>8);
//        wr_eeprom(0x22, EnableTimeout&0x00FF);
//        wr_eeprom(0x23, (EnableTimeout&0xFF00)>>8);
    }

    return;
}

int8 check_timeout(void)
{
    int16 i;

    // Check for timeout
    i = get_timer1();
    if (i > EnableTimeout)
    {
        // Timeout -- Release RS-485 enable
        output_high(ENABLE_HOLD);   // Idle = ON
        return (1);
    } else {
        return (0);
    }
}

//-----------------------------------------------------------------------------
// Auto Enable Algorithm - Bit Time calculated
//
void Mode_Bit_Time(void)
{
    int8    done;

    // Kill the UART and Timer2 interrupts
    disable_interrupts(INT_RDA);
    disable_interrupts(INT_TIMER2);
    disable_interrupts(INT_EXT);

    // Set initial RS-485 enable hold
    output_high(ENABLE_HOLD);   // Release RS-485 Enable. Idle = ON

    // Reset minimum time and enable timeout (start with Max values)
    MinBitTime = 0xFFFF;
    EnableTimeout = 0xFFFF;     // 0x0141 = 10 bit times at 115200, 0xFFFF = 17.8 ms timeout worst case

    // We never exit this loop
    while (TRUE) {
    
        // Wait for H-L transition of TXD (start bit) at idle=H
        while ( input(PIN_B0) )
        {
            // NOP
        }
        output_low(ENABLE_HOLD);   // RS-485 enable Hold = L
        // Clear Timer1 counter - used to calculate next minimum
        set_timer1(0);
        MinBitTime = 0xFFFF;

        done = 0;
        while (!done)
        {
            // Wait for L-H transition of TXD
            while ( !input(PIN_B0) )
            {
                // NOP
            }
            
            // Signal now H - check new minimum time
            check_new_min();

            // Wait for H-L transition of TXD at idle=H
            while ( input(PIN_B0) )
            {
                // Check for timeout
                if (check_timeout()) {
                    done = 1;
                    break;
                }
            }

            // Signal now L - check new minimum time
            if (!done) {
                check_new_min();
            }
        }
    }

    return;
}


//-----------------------------------------------------------------------------
// Auto Enable Algorithm - Fixed (Manual) Baud Rate
//
void Mode_Manual()
{
    int16   HoldDelay, DelayCount;

    // Kill the Timer2 interrupts
    disable_interrupts(INT_TIMER2);

    // Don't buffer the UART received data
    DontBufferRcv = 1;
    ByteRcvd = 0;

    // Set initial RS-485 enable hold
    output_high(ENABLE_HOLD);   // Release RS-485 Enable. Idle = ON

    // Set the UART with the current baud rate in EEPROM
    HoldDelay = set_baud();

    // We never exit this loop
    while (TRUE) {

        // Note: Since the RS-485 enable hold is cleared in the UART ISR
        // (which happens before the full stop bit time), we can also delay
        // here after the UART interrupt a small amount of time that
        // adjusts for each baud rate.

        if (ByteRcvd)
        {
            // delay_us(HoldDelay);
            DelayCount = 0;
            while (HoldDelay > DelayCount)
              DelayCount++;

            output_high(ENABLE_HOLD);   // Release RS-485 Enable. Idle = ON
            ByteRcvd = 0;
        }
    }
    return;
}

//-----------------------------------------------------------------------------
// Main program entry point.
main() {

    int8    Mode_Value;

    // Setup the remaining CCS peripherals
//    setup_wdt(WDT_OFF);

    // Configure the timers
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);     // Free running clock on Timer 1, approx. 3,686,400 Mhz (271 ns)
                                                // Minimum Bit times:
                                                // 115200 = 8.7 us, 57600 = 17.4 us, 38400 = 26.0 us
                                                // 19200 = 52 us, 9600 = 104 us, 4800 = 208 us
                                                // 2400 = 416 us, 1200 = 833 us, 600 = 1667 us
    setup_timer_2(T2_DIV_BY_16,231,1);          // Timer 2 = 1 ms.

    setup_comparator(NC_NC_NC_NC);              // No compartor
    setup_vref(FALSE);                          // No A/D so no voltage ref.

    //
    // Setup the TRIS registers and initial port outputs
    //
    set_tris_a(0x24);   // A0-1=O, A2=I A3-4=O, A5=I, A6-7=O
    set_tris_b(0x07);   // B0-2=I, B3-7=O

    // Set the control line outputs on PORT_A
    output_high(ENABLE_HOLD);   // Idle = ON
    output_high(ADDR_TXD);      // Idle = ON

    // Ready receive buffer and pointers for commands
    RcvBufferIndex = 0;
    LastCharRcvd = 0;
    DontBufferRcv = 0;

    // Clear the one millisecond count
    OneMsCount = 0;

    // Initial UART 9600 BPS
    set_uart_speed(9600);

    // Setup external interrupt edge
    ext_int_edge(H_TO_L);

    // Enable the interrupts
    enable_interrupts(INT_RDA);
    enable_interrupts(INT_TIMER2);
    enable_interrupts(INT_EXT);
    enable_interrupts(GLOBAL);

    // Initial logon RS-232 message?
    if (rd_eeprom(EEPROM_SILENT_POST) != 0) {
        printf("\rIntegrity Instruments XP Converter v2.00\r");
    }

    // Check to see if the user wants to go into programming mode
    // Wait 3 seconds
    while (OneMsCount < 3000) {

        if (LastCharRcvd == ' ') {

            // Ready receive buffer and pointers for commands
            RcvBufferIndex = 0;
            LastCharRcvd = 0;

            // Enter programming mode
            printf("Programming mode entered.\r");

            // Enter programming mode
            ProgramMode();

            // Exit out ... while (OneMsCount < 3000) {
            break;
        }
    }

    // Retrieve the auto enable mode from EEPROM
    Mode_Value = rd_eeprom(EEPROM_MODE);

    // Select the auto enable algorithm
    if (Mode_Value == MODE_MANUAL_BAUD) {
        Mode_Manual();
    } else {
        Mode_Bit_Time();
    }

    //
    // Main Loop - never exits. Should never get here. Just spit out crap
    // on the RS-232 port continuously
    //
    while (TRUE)
    {
        printf("Main loop failure!\r");
        delay_ms(1000);
    }

    return;
}
