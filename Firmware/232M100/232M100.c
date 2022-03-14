//
// Integrity Instruments, Inc.
//
// 232M100 Module v4.0
//                v4.1  AAK: 1/17/2010 Fixed LVP bug. 
//                v4.2  AAK: 1/18/2010 Fixed power-up default bug. 
//                v4.3  AAK: 1/24/2010 Changed several things. See
//                      "Code fixes on 17 JAN 2010.txt" 
//
// Revision History
// ----------------------------------------------------------------------------
//  Timothy Gack      6/21/2005
//
//  Original 'C' code version. Based on 232M300 code base by Anthony Kempka.
//  Version 4.0: First Project Release
//

#include <18F4455.h>

//#define _DEBUGGINGMODE_

#ifdef _DEBUGGINGMODE_

#device ICD=TRUE
#device adc=10

#FUSES NOWDT                     //No Watch Dog Timer
#FUSES WDT256                    //Watch Dog Timer uses 1:128 Postscale
#FUSES NOPROTECT                 //Code not protected from reading
#FUSES NOBROWNOUT                //No brownout reset
#FUSES BORV21                    //Brownout reset at 2.1V
#FUSES PUT                       //Power Up Timer
#FUSES NOCPD                     //No EE protection
#FUSES STVREN                    //Stack full/underflow will cause reset
#FUSES DEBUG                     //Debug mode for use with ICD
#FUSES NOWRT                     //Program memory not write protected
#FUSES NOWRTD                    //Data EEPROM not write protected
#FUSES IESO                      //Internal External Switch Over mode enabled
#FUSES FCMEN                     //Fail-safe clock monitor enabled
#FUSES PBADEN                    //PORTB pins are configured as analog input channels on RESET
#FUSES NOWRTC                    //configuration not registers write protected
#FUSES NOWRTB                    //Boot block not write protected
#FUSES NOEBTR                    //Memory not protected from table reads
#FUSES NOEBTRB                   //Boot block not protected from table reads
#FUSES NOCPB                     //No Boot Block code protection
#FUSES LPT1OSC                   //Timer1 configured for low-power operation
#FUSES MCLR                      //Master Clear pin enabled
#FUSES NOXINST                   //Extended set extension and Indexed Addressing mode disabled (Legacy mode)

#FUSES HSPLL                     //High Speed Crystal/Resonator with PLL enabled
#FUSES PLL5                      //Divide By 5(20MHz oscillator input)
#FUSES CPUDIV2                   // Divide by 3 (96MHz PLL source / 3) 32MHz clock

#else

// Release mode
#device adc=10

#FUSES WDT                       //WatchDog Timer
#FUSES WDT256                    //Watch Dog Timer uses 1:128 Postscale
#FUSES NOPROTECT                 //Code not protected from reading
#FUSES BROWNOUT                  //Brownout reset
#FUSES BORV21                    //Brownout reset at 2.1V
#FUSES PUT                       //Power Up Timer
#FUSES NOCPD                     //No EE protection
#FUSES STVREN                    //Stack full/underflow will cause reset
#FUSES NOWRT                     //Program memory not write protected
#FUSES NOWRTD                    //Data EEPROM not write protected
#FUSES IESO                      //Internal External Switch Over mode enabled
#FUSES FCMEN                     //Fail-safe clock monitor enabled
#FUSES PBADEN                    //PORTB pins are configured as analog input channels on RESET
#FUSES NOWRTC                    //configuration not registers write protected
#FUSES NOWRTB                    //Boot block not write protected
#FUSES NOEBTR                    //Memory not protected from table reads
#FUSES NOEBTRB                   //Boot block not protected from table reads
#FUSES NOCPB                     //No Boot Block code protection
#FUSES LPT1OSC                   //Timer1 configured for low-power operation
#FUSES MCLR                      //Master Clear pin enabled
#FUSES NOXINST                   //Extended set extension and Indexed Addressing mode disabled (Legacy mode)

#FUSES HSPLL                     //High Speed Crystal/Resonator with PLL enabled
#FUSES PLL5                      //Divide By 5(20MHz oscillator input)
#FUSES CPUDIV2                   //Divide by 3 (96MHz PLL source / 3) 32MHz clock

#endif

// Note: This include needs to be after the #FUSES and #DEVICE statements
#include "string.h"

#define VER_MAJ '4'
#define VER_MIN '3'


// Setup the clock base time and RS-232 Baud rate
#use delay(clock=32000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

// Turn off the compiler's caching (virtual port) auto data direction crap
#use fast_io(A)
#use fast_io(B)
#use fast_io(C)
#use fast_io(D)
#use fast_io(E)


// Memory register (port) defines so we can use them in 'C'
#byte PORT_A=5
#byte PORT_B=6
#byte PORT_C=7
#byte PORT_D=8
#byte PORT_E=9

#byte EEADR=0xFA9
#byte EEADRH=0x10F
#byte EEDATA=0xFA8
#byte EEDATH=0x10E
#byte EECON1=0xFA6
#byte EECON2=0xFA7

#byte RCSTA = 0xFAB
#define OERR 1
#define FERR 2
#define CREN 4

#byte SSPSTAT=0x94
#byte SSPCON=0x14

#byte INTCON=0xFF2

//
// EEPROM Locations
//
#define EEPROM_MOD_ADDRESS      0x00
// Not used in 232M100 #define EEPROM_DIR_PORT1        0x02
#define EEPROM_DIR_PORT2        0x03
#define EEPROM_ASYNC_MSB        0x04
#define EEPROM_ASYNC_LSB        0x05
// Not used in 232M100 #define EEPROM_POD_PORT1        0x06
#define EEPROM_POD_PORT2        0x07
#define EEPROM_EXPANDER_FLAG    0x08
#define EEPROM_POD_DA_MSB_CH0   0x09
#define EEPROM_POD_DA_LSB_CH0   0x0A
#define EEPROM_POD_DA_MSB_CH1   0x0B
#define EEPROM_POD_DA_LSB_CH1   0x0C
#define EEPROM_AD_SAMPLE_CLK    0x0D
#define EEPROM_STREAM_AD_COUNT  0x10
#define EEPROM_STREAM_AD_START  0x11
// EEPROM locations 0x11 to 0x18 A/D sample configuration
#define EEPROM_STREAM_DIGITAL   0x19
#define EEPROM_STREAM_PULSE     0x1A
#define EEPROM_AD_CALIBRATION  0x1B   // 4 bytes x 8 = 32 bytes - so 0x3B is the next usable EEPROM address

// ADC Channels
#define ADC_CHANNEL_9   9
#define ADC_CHANNEL_11  11

// LED defines -- PORTC
#define LED_RED         PIN_C1
#define LED_GREEN       PIN_C0

// EEPROM flags used to configure run-time settings
#define EXPANDER_FLAG   0x01
#define SLOW_ADC_CLOCK  0x02

#define GIE 7               // Bit 7
int8    EEFlags = 0;

int8    CurrentDip;         // Current DIP switch setting

// Stream/Async mode variables
#define CONTINUOUS_STREAM       0x01
#define DIGITAL_PULSE_CHANGE    0x02
#define ASYNC_TIMED             0x04
#define STREAM_DIGITAL          0x08
#define STREAM_PULSE            0x10
#define DIGITAL_CHANGE          0x20
#define PULSE_CHANGE            0x40

int8    AsyncStreamFlags = 0;   // See above flag defines for Stream/Async modes
int8    AsyncStreamUpdate;
int16   AsyncCount = 0;         // Used for ASYNC_TIMED mode
int16   AsyncTimeout = 0;
int8    StreamADCount = 0;      // Configures stream/async A/D sampling
int8    ADSampleConfig[8];      // EEPROM locations 0x11 to 0x18
// Not used in 232M100: int8    Port1Cur;               // Current Port 1 input - ASYNC_CHANGE
int8    Port2Cur;               // Current Port 2 input - ASYNC_CHANGE
int32   PulseCountCur;          // Current Pulse Count - ASYNC_CHANGE

// Led flash control variables
int8    LedGreenFlag = 1;
int16   LedCount = 0;

// Port direction and D/A shadow variables
// Not used in 232M100: int8    Port1_Dir = 0;
int8    Port2_Dir = 0;

// 32-bit pulse counter
int32   PulseCount = 0;

//
// RS-232 command (receive) and response (transmit) buffers and sizes
#define CMDBUFFERSIZE   16
char    LastCharRcvd;
int8    CommandBufferIndex;
char    CommandBuffer[CMDBUFFERSIZE];
char    ResponseBuffer[32];

//
// RS-232 Communication Error Counter
int8    RxCommErr;

//
// Array of values to multiply with ADC reading - read from EEPROM at reset
float AD_Calibration[8];
float AD_Value;
char  AD_Value_Str[32];

//
// Forward function declarations
//
#inline
void trigger_led_red(void);
void led_on_green(void);

//-----------------------------------------------------------------------------
// int16 acquire_adc(int8 control)
//
// Acquire and A/D value from Internal ADC
//
#separate
int16 acquire_adc(int8 control)
{
    int16   ad_sample;
    int8    ad_control;

    switch(control)
    {
    case 0:
    case 1:
    case 4:
    case 5:
    case 6:
    case 7:
        ad_control = control;
        break;
    case 2:
        ad_control = ADC_CHANNEL_9;
        break;
    case 3:
        ad_control = ADC_CHANNEL_11;
        break;
    }

    ad_sample = 0;

    // Configure ADC for 8uS conversion time
    // All Analog Inputs
    // Vref+ : Vref- Reference
    // setup_adc(ADC_CLOCK_DIV_64 | ADC_TAD_MUL_16 );
    setup_adc(ADC_CLOCK_DIV_64 | ADC_TAD_MUL_20);
    setup_adc_ports(AN0_TO_AN11 | VREF_VREF);

    // Set the multiplexer
    set_adc_channel(ad_control);
    ad_sample = read_adc();
    setup_adc(ADC_OFF);
    setup_adc_ports(NO_ANALOGS);
    
    AD_Value = ad_sample * AD_Calibration[control];
    ad_sample = AD_Value;
    AD_Value = AD_Value * (10.0 / 1024.0);
    sprintf (AD_Value_Str, "%6f\r", AD_Value);

    return (ad_sample);
}


//-----------------------------------------------------------------------------
// int8 rd_eeprom(int8 address)
//
// Read EEPROM
// The 18F4455 has an internal EEPROM
//
#separate
int8 rd_eeprom(int8 address)
{
    int8    data;

    EEADR = address;    // Address register
    EECON1 = 0x00;      // Configured for EEPROM (EEPGD:0, CFGS:0)
    EECON1 = 0x01;      // Access Data EEPROM, RD=1
    data = EEDATA;      // Read the data out of the EEPROM
    
// NOTE: [AAK] the nuilt-in CCS routine for EEPROM is shitty
//    data = read_EEPROM(address);    // Read a byte from EEPROM
    return (data);
}


//-----------------------------------------------------------------------------
// wr_eeprom(int8 address, int8 data)
//
// Write EEPROM
// The 16F847A has an internal EEPROM
//
#separate
void wr_eeprom(int8 address, int8 data)
{
    int8 i;

//    EEADRH = 0;         // Upper bits of 10 bit address = 0
    EEADR = address;    // Address register
    EEDATA = data;      // Data to write to the EEPROM

    // Do not change a single line in the following #asm block
    // The assembly output must match the Microchip documentation exactly
    // and changing anything will screw it up since the compiler shoves
    // its crap in-line also.
#asm
    bcf     INTCON, GIE     ; Disable INT's INTCON GIE
    bsf     EECON1, 2   ; Write enable - WREN
    movlw   0x55        ; Write unlock sequence
    movwf   EECON2        ; EECON2
    movlw   0xAA
    movwf   EECON2        ; EECON2
    bsf     EECON1, 1   ; Initiate write - WR
    bsf     INTCON, GIE     ; enable INT's - INTCON GIE
#endasm

    // Wait for WRITE to complete
    do {
        delay_cycles(1);
        i = EECON1;
    } while (i & 0x02);         // Bit 1 = WR (write status)

    return;
}


//-----------------------------------------------------------------------------
// led_on_red(void)
//
// Turn LED on (RED)
//
void led_on_red(void)
{
    output_high(LED_RED);
    output_low(LED_GREEN);
    return;
}


//-----------------------------------------------------------------------------
// led_on_green(void)
//
// Turn LED on (GREEN)
//
void led_on_green(void)
{
    output_high(LED_GREEN);
    output_low(LED_RED);
    return;
}


//-----------------------------------------------------------------------------
// led_off(void)
//
// Turn LED off
//
void led_off(void)
{
    output_low(LED_RED);
    output_low(LED_GREEN);
    return;
}


//-----------------------------------------------------------------------------
// trigger_led_red(void)
//
// Triggers the RED LED sequence
//
#inline
void trigger_led_red(void)
{
    // Only start the Red sequence if it was Green
    if (LedGreenFlag)
    {
        LedGreenFlag = 0;
        LedCount = 900;         // To make a faster Red LED blink, put a value here (0 ... 100)
                                // Full blink gets a 0
        led_on_red();
    }
}

// BUGBUG: AAK. There is no Port1 (PORTB) in the 232M100
// void write_port1(int8 value)
// int8 read_port1(void)

//-----------------------------------------------------------------------------
// write_port2(int8 value)
//
// Write the digital output to PORT2
// BIT 0-7 = PORTD 0-7
//
#inline
void write_port2(int8 value)
{
    // Expander inversion
    if (EEFlags & EXPANDER_FLAG)
        value = value ^ 0xFF;
    OUTPUT_D(value);
    return;
}

//-----------------------------------------------------------------------------
// int8 read_port2(void)
//
// Reads the digital input of PORT2
// BIT 0-7 = PORTB 0-7
//
#inline
int8 read_port2(void)
{
    int8 value;

    value = INPUT_D();             // PORT2 bits 0-7 = PORT_D bits 0-7

    // Expander inversion
    if (EEFlags & EXPANDER_FLAG)
        value = value ^ 0xFF;
    return (value);
}


// BUGBUG: AAK. There is no Port1 (PORTB) in the 232M100
// void write_port1(int8 value)
// int8 read_port1(void)
// int8 get_dir_port1(void)


//-----------------------------------------------------------------------------
// set_dir_port2(int8 direction)
//
// Set the data direction of PORT2
// BIT 0-7 = PORTB 0-7
//
void set_dir_port2(int8 direction)
{
    Port2_Dir = direction;
    wr_eeprom(EEPROM_DIR_PORT2, Port2_Dir);
    set_tris_d(Port2_Dir);
    return;
}


//-----------------------------------------------------------------------------
// get_dir_port2()
//
// Returns the current data direction (input/output) of Port2
//
int8 get_dir_port2(void)
{
    return (Port2_Dir);
}


//-----------------------------------------------------------------------------
// hex_to_bin()
//
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
// bin_to_hex()
//
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
// timer0_isr()
//
// RTCC/Timer0 interrupt - pulse counter is redirected to Timer0
//
#int_timer0
void timer0_isr(void)
{
    // Timer0 ISR is triggered on 8 bit counter overflow (256 = 0x100)
    PulseCount = PulseCount + 0x100;
    return;
}


//-----------------------------------------------------------------------------
// timer1_isr()
//
// Timer1 interrupt - 1 millisecond
//
#int_timer1
void timer1_isr(void)
{
//    set_timer1(0x7000);                     // 10ms = 65536-(.01/(4/14745600)) = 0x7000
//    set_timer1(0xF199);                     // 1ms = 65536-(.001/(4/14745600)) = 0xF199
// 1ms = 65536-(.001/(4/(32000000/2))) = 0xF199
    set_timer1(0xF060);                     // 1ms = 65536-(.001/(4/16,000,000)) = 0xF060

    // Kick the dog
    restart_wdt();

    AsyncCount++;                           // One ms Async timer count
    LedCount++;
    if (LedCount < 1000)                    // One second count (LED on)
    {
        if (LedCount & 1)
        {
            if (LedGreenFlag)               // LED on (Red or Green)
                led_on_green();
            else
                led_on_red();
        } else {
            led_off();                      // Toggle off every other cycle (1000 Hz)
        }
    } else {
        led_off();
        if (LedGreenFlag)
        {
            // Green LED timing
            if (LedCount > 2000)            // Two second count (LED off)
            {
                LedCount = 0;
                LedGreenFlag = 1;
            }
        } else {
            // Red LED timing
            if (LedCount > 1100)            // short off phase (250 ms.)
            {
                LedCount = 0;
                LedGreenFlag = 1;
            }
        }
    }
    return;
}


//-----------------------------------------------------------------------------
// RDA_isr()
//
// UART Receive character ISR
//
#int_RDA
void RDA_isr(void)
{
    led_on_green();
    
    // We need to check errors on UART input in order to clear them
    // Overrun error is cleared by disabling, then re-enabling UART
    if ( bit_test(RCSTA, OERR) ) {
        bit_clear(RCSTA, CREN);
        bit_set(RCSTA, CREN);
        RxCommErr++;
        return;
    }
    
    // Framing error is cleared by reading RCV register.
    if ( bit_test(RCSTA, FERR) ) {
        RxCommErr++;
    }
    
    // Get character out of UART and save in receive buffer
    LastCharRcvd = getc();

    if(LastCharRcvd != '\n')        // Strip off newline chars
    {
        // Save character in receive buffer
        CommandBuffer[CommandBufferIndex] = LastCharRcvd;
        CommandBufferIndex++;
        // Check for receive buffer overflow
        if (CommandBufferIndex >= CMDBUFFERSIZE)
            CommandBufferIndex--;
    }
    return;
}


//-----------------------------------------------------------------------------
// get_eesettings()
//
// Reads EEPROM settings and updates the EEPROM based flags and variables
//
#separate
void get_eesettings(int8 PowerUpMode)
{
    int8    i, j;
    int8    *pPtr;    

    // Expander flag
    i = rd_eeprom(EEPROM_EXPANDER_FLAG);
    if (i)
    {
        EEFlags = EEFLags | EXPANDER_FLAG;
    } else {
        EEFlags = EEFLags & (~EXPANDER_FLAG);
    }
    
    // Slow A/D clock flag
    i = rd_eeprom(EEPROM_AD_SAMPLE_CLK);
    if (i)
    {
        EEFlags = EEFLags | SLOW_ADC_CLOCK;
    } else {
        EEFlags = EEFLags & (~SLOW_ADC_CLOCK);
    }
    
    //
    // Get values from EEPROM to multiply with ADC readings for each channel
    i = EEPROM_AD_CALIBRATION;
    for(j=0; j<8; j++) {
    
/* BUGBUG: Originally broken (AAK). The pointer needs to be typecast
        *(&(AD_Calibration[j])) = rd_eeprom(i++);
        *(&(AD_Calibration[j]) + 1) = rd_eeprom(i++);
        *(&(AD_Calibration[j]) + 2) = rd_eeprom(i++);
        *(&(AD_Calibration[j]) + 3) = rd_eeprom(i++);
*/
        pPtr = &(AD_Calibration[j]);
        *pPtr++ = rd_eeprom(i++);
        *pPtr++ = rd_eeprom(i++);
        *pPtr++ = rd_eeprom(i++);
        *pPtr = rd_eeprom(i++);
    }

/* Un-comment for testing and development
   AD_Calibration[0] = 1.0;
   AD_Calibration[1] = 1.0;
   AD_Calibration[2] = 1.0;
   AD_Calibration[3] = 1.0;
   AD_Calibration[4] = 1.0;
   AD_Calibration[5] = 1.0;
   AD_Calibration[6] = 1.0;
   AD_Calibration[7] = 1.0;
*/ 

   if (PowerUpMode) {
        // Get Port data direction from EEPROM
        i = rd_eeprom(EEPROM_DIR_PORT2);
        set_dir_port2(i);
    
        // Get power on defaults for I/O ports from EEPROM
        i = rd_eeprom(EEPROM_POD_PORT2);
        write_port2(i);
   }
   
    return;
}


//-----------------------------------------------------------------------------
// get_async_config()
//
// Reads EEPROM settings and updates ASYNC stream mode configuration
//
#separate
void get_async_config(void)
{
    int8    i;
    int16   j;

    // Async configuration
    j = rd_eeprom(EEPROM_ASYNC_MSB);
    j = j << 8;
    j = j + rd_eeprom(EEPROM_ASYNC_LSB);
    if (j == 0)
    {
        // Clear Async modes
        AsyncStreamFlags = AsyncStreamFlags & (~DIGITAL_PULSE_CHANGE);
        AsyncStreamFlags = AsyncStreamFlags & (~ASYNC_TIMED);
        // No timeout
        AsyncTimeout = 0;
    } else {
        if (j == 1)
        {
            // Digital Change mode
            AsyncStreamFlags = AsyncStreamFlags | DIGITAL_PULSE_CHANGE;
        } else {
            // Timed Async mode
            AsyncStreamFlags = AsyncStreamFlags | ASYNC_TIMED;
            // Reset count to zero
            AsyncCount = 0;
            // Save off new timeout
            AsyncTimeout = j;
        }
    }

    // Stream digital flag
    i = rd_eeprom(EEPROM_STREAM_DIGITAL);
    if (i)
    {
        AsyncStreamFlags = AsyncStreamFlags | STREAM_DIGITAL;
    } else {
        AsyncStreamFlags = AsyncStreamFlags & (~STREAM_DIGITAL);
    }

    // Stream pulse counter flag
    i = rd_eeprom(EEPROM_STREAM_PULSE);
    if (i)
    {
        AsyncStreamFlags = AsyncStreamFlags | STREAM_PULSE;
    } else {
        AsyncStreamFlags = AsyncStreamFlags & (~STREAM_PULSE);
    }
    // A/D Stream count -- max 8 for 232M100!
    StreamADCount = rd_eeprom(EEPROM_STREAM_AD_COUNT);
    if (StreamADCount > 8)
    {
        StreamADCount = 8;
    }
    // A/D Control nibbles. In EEPROM locations 0x11 to 0x18
    for (i=0; i<8; i++)
    {
        ADSampleConfig[i] = rd_eeprom(EEPROM_STREAM_AD_START+i);
    }

    return;
}


//-----------------------------------------------------------------------------
// get_dip_set_baud()
//
// Get the DIP switch settings and update Baudrate
// DIP switch settings:
// ---------------------
// Pos 1    Pos 2   Baud
//   0        0     9600  (DEFAULT)
//   1        0     19200
//   0        1     57600
//   1        1     115200
//
void get_dip_set_baud(void)
{
    int8 NewDip;

    NewDip = INPUT_B() & 0x07;
    NewDip = NewDip >> 1;

    // Only set UART baud if a change is detected
    if (NewDip != CurrentDip)
    {
        // Debounce switch with a little time
        delay_ms(100);
        CurrentDip = NewDip;
        switch (CurrentDip) {
            case 0:             // 9600
            default:
                set_uart_speed(9600);
                break;
            case 1:             // 19200
                set_uart_speed(19200);
                break;
            case 2:             // 57600
                set_uart_speed(57600);
                break;
            case 3:             // 115200
                set_uart_speed(115200);
                break;
        }
    }
    return;
}


//-----------------------------------------------------------------------------
// xmit_response()
//
// Transmits the contents of ResponseBuffer[] out the UART
// Expects ResponseBuffer[] to be NULL terminated
//
#separate
void xmit_response(VOID)
{
    char *tmp_ptr;

    tmp_ptr = ResponseBuffer;
    while (*tmp_ptr)
    {
        // Out the UART it goes
        putc(*tmp_ptr);
        tmp_ptr++;
    }

    // blinky blinky feedback
    trigger_led_red();

    return;
}


//-----------------------------------------------------------------------------
// check_async_stream()
//
// Checks the current Async and Stream mode configuration and
// executes the configured Async/Stream operations.
//
void check_async_stream(void)
{
    int8    i, j, x;
    int16   temp;
    int32   i32;

    // Take at look at the Async Stream flags
    if (AsyncStreamFlags)
    {
        // Start with NO async update flag set
        AsyncStreamUpdate = 0;

        // Check Continuous Stream mode
        if (AsyncStreamFlags & CONTINUOUS_STREAM)
        {
            AsyncStreamUpdate |= CONTINUOUS_STREAM;
        }

        // Check Async Timed mode
        if (AsyncStreamFlags & ASYNC_TIMED)
        {
            // Check for timeout
            if (AsyncCount > AsyncTimeout)
            {
                // TIMEOUT!
                // restart timeout count
                AsyncCount = 0;
                AsyncStreamUpdate |= ASYNC_TIMED;
            }
        }

        // Check Digital Change mode
        if (AsyncStreamFlags & DIGITAL_PULSE_CHANGE)
        {
            AsyncStreamUpdate |= DIGITAL_PULSE_CHANGE;
            // Check for Digital input or Pulse counter change
            i = read_port2();
            if (Port2Cur != i)
            {
                Port2Cur = i;
                AsyncStreamUpdate |= DIGITAL_CHANGE;
            }
            // Timer0 keeps LSB of pulse count
            i32 = PulseCount + get_timer0();
            if (PulseCountCur != i32)
            {
                PulseCountCur = i32;
                AsyncStreamUpdate |= PULSE_CHANGE;
            }
        }

        // Do we do Continuous Stream or Digital Change or Async Timed update?
        if (AsyncStreamUpdate)
        {
            // Do CONTINUOUS_STREAM or ASYNC_TIMED
            //    Analog samples first, then check if DIGITAL or PULSE outputs
            if ((AsyncStreamUpdate & CONTINUOUS_STREAM) ||
                (AsyncStreamUpdate & ASYNC_TIMED))
            {
                // Analog sample.  The 232M100 only has total of 8 channels
                for (x=0; x<StreamADCount; x++)
                {
                    // BUGBUG: Was    i = ADSampleConfig[x] & 0x0F;
                    // but i cannot be greater than 7
                    i = ADSampleConfig[x] & 0x07;
                    temp = acquire_adc(i);
                    ResponseBuffer[0] = 'U';
                    j = ((temp & 0xFF00) >> 8);
                    j = j | (i << 4);                   // Control nibble plus MSB byte of high 12 bits
                    bin_to_hex(j, &ResponseBuffer[1]);
                    j = (temp & 0x00FF);                // LSB of 12 bits
                    bin_to_hex(j, &ResponseBuffer[3]);
                    ResponseBuffer[5] = '\r';
                    ResponseBuffer[6] = 0;              // Null terminate
                    // Send response out UART
                    xmit_response();
                }
                // Digital and Pulse outputs after Analog
                if (AsyncStreamFlags & STREAM_DIGITAL)
                {
                    Port2Cur = read_port2();
                    AsyncStreamUpdate |= DIGITAL_CHANGE;
                }
                // Pulse count output
                if (AsyncStreamFlags & STREAM_PULSE)
                {
                    PulseCountCur = PulseCount + get_timer0();
                    AsyncStreamUpdate |= PULSE_CHANGE;
                }
            }
            //
            // Digital and Pulse outputs after Analog
            // Note: Using Port1Cur and Port2Cur since count could have changed
            //        between comparison above and when it is sent here.

            if (AsyncStreamUpdate & DIGITAL_CHANGE)
            {
                ResponseBuffer[0] = 'I';
                // No Port1 in 232M100
                bin_to_hex(0x00, &ResponseBuffer[1]);
                j = Port2Cur;
                bin_to_hex(j, &ResponseBuffer[3]);
                ResponseBuffer[5] = '\r';
                ResponseBuffer[6] = 0;              // Null terminate
                // Send response out UART
                xmit_response();
            }
            // Pulse count output
            // Note: Using PulseCountCur since count could have changed between
            //       comparison above and when it is sent here.
            if (AsyncStreamUpdate & PULSE_CHANGE)
            {
                ResponseBuffer[0] = 'N';
                i = ((PulseCountCur & 0xFF000000) >> 24);
                bin_to_hex(i, &ResponseBuffer[1]);
                i = ((PulseCountCur & 0xFF0000) >> 16);
                bin_to_hex(i, &ResponseBuffer[3]);
                i = ((PulseCountCur & 0xFF00) >> 8);
                bin_to_hex(i, &ResponseBuffer[5]);
                i = PulseCountCur & 0xFF;
                bin_to_hex(i, &ResponseBuffer[7]);
                ResponseBuffer[9] = '\r';
                ResponseBuffer[10] = 0;             // Null terminate
                // Send response out UART
                xmit_response();
            }
        } // if (AsyncStreamUpdate) {
    } // if (AsyncStreamFlags) {
    return;
}


//-----------------------------------------------------------------------------
// post_message()
//
// Display the POST message
//
void post_message(void)
{
    // Welcome message
    printf("Integrity Instruments, Inc. Model 232M100 v%c.%c\r", VER_MAJ, VER_MIN);
    printf("Built on: %s   %s\r", __DATE__, __TIME__);
    return;
}


//-----------------------------------------------------------------------------
// commands()
//
// Display the supported commands
//
void commands(void)
{
    // Dump the supported commands
    printf("\rASCII commands.  Input paramaters in HEX as: xxyy  Unused parameters as: **\r\r");
    printf("I {get digital input}, O**xx {set digital output}\r");
    printf("G {get digital direction}, T**xx {set digital direction}\r");
    printf("N {get pulse counter}, M {clear pulse counter}\r");
    printf("Ux {A-to-D sample in HEX}, Q {A-to-D sample in Decimal}\r");
    printf("Wyyxx {Write EEPROM yy with xx}, Ryy {Read EEPROM address yy}\r");
    printf("Pxxyyy {PWM xx freq yyy duty}, Z {Reset CPU}\r");
    printf("S {start streaming mode}, H {halt streaming mode}\r");
    printf("K {get comms error count}, J {clear comms error count}\r");
    printf("V {get firmware version}, ? {command list}\r\r");
    return;
}


//-----------------------------------------------------------------------------
// Main program entry point.
//
void main()
{
    int8    i, j;
    int16   temp;

    // Turn off comparator and comparator reference
    setup_comparator(NC_NC_NC_NC);
    setup_vref(FALSE);

    // Configure or shut off remaining PIC/CCS peripherals
    setup_psp(PSP_DISABLED);
    setup_spi(FALSE);

    //
    // Setup the TRIS registers and initial port outputs
    // Disable all SPI device chip selects
    //
//    set_tris_a(0x10);   // A0-3=O, A4=I, A5=O
    set_tris_a(0xFF);   // Port A all inputs
    set_tris_b(0xFF);   // B0-7=I
    set_tris_d(0xFF);   // D0-7=I
    set_tris_c(0xD0);   // C0-3=O C5=O, C4=I, C6=I, C7=I
    set_tris_e(0x07);   // E0-2=I

    trigger_led_red();  // Initial Led RED

    // Get settings from EEPROM
    get_eesettings(TRUE);

    // Clear the command buffer, index and last character rcvd
    CommandBufferIndex = 0;
    LastCharRcvd = 0;
    for (i=0; i<CMDBUFFERSIZE; i++) {
        CommandBuffer[i] = 0;
    }

    // Set initial current DIP switch setting so the UART baud is set
    // by the first call to get_dip_set_baud();
    CurrentDip = 0xFF;

    //
    // Initialize timers and interrupts.
    //
    set_timer0(0);
    setup_timer_0(RTCC_8_BIT | RTCC_EXT_L_TO_H | RTCC_DIV_1);
    enable_interrupts(int_timer0);              // Timer0 RTCC Pulse counter input
    //setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);     // setup interrupts
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_2);     // setup interrupts
    enable_interrupts(int_timer1);              // Timer1 Periodic 1 ms. timer
    enable_interrupts(INT_RDA);

    enable_interrupts(global);                  // All Interrupts on

    // Setup PWM inital settings
    setup_timer_2 (T2_DIV_BY_1, 0, 1);          // Mode=div/1 Period=0 Postscale=1 (not used)
    setup_ccp1 (CCP_PWM);                       // Configure CCP1 for PWM
    set_pwm1_duty (0);                          // Initially off

    // Configure watchdog -- 18ms implies not scale through Timer 0
//    setup_wdt(WDT_36MS);

    // Check DIP switch settings for initial baud rate
    get_dip_set_baud();
    
    // Get ASYNC continuous stream mode configuration
    get_async_config();

    // Initial ASYNC digital states
    Port2Cur = read_port2();
    PulseCountCur = 0;
    RxCommErr = 0;

    // Display POST message
    post_message();
    printf("> ");
    
    //
    // Main Loop - never exits
    //
    while (TRUE)
    {

        // Kick the dog
        restart_wdt();

        // Wait for a CR character to be received
        if (LastCharRcvd != 0x0D)
        {
            // Check DIP switch settings for new baud rate
            get_dip_set_baud();

            // Check continuous stream mode
            check_async_stream();

            continue;
        } // if (CommandBuffer[0] == 0) {

        // Convert CommandBuffer[0] to UPPERCASE
        if (islower(CommandBuffer[0]))
            CommandBuffer[0] = CommandBuffer[0] & (~0x20);

        switch (CommandBuffer[0]) {
            case 'I':                               // Digital Input
                if (CommandBufferIndex != 2)        // Check for correct command length
                    goto error_rsp;
                ResponseBuffer[0] = 'I';
                bin_to_hex(0x00, &ResponseBuffer[1]);
                j = read_port2();
                bin_to_hex(j, &ResponseBuffer[3]);
                ResponseBuffer[5] = '\r';
                ResponseBuffer[6] = 0;              // Null terminate
                break;

            case 'O':                               // Digital Output
                if (CommandBufferIndex != 6)        // Check for correct command length
                    goto error_rsp;
// Port1 not used in 232M100
//                i = hex_to_bin(&CommandBuffer[1]);  // PORT1
//                write_port1(i);
                j = hex_to_bin(&CommandBuffer[3]);  // PORT2
                write_port2(j);
                ResponseBuffer[0] = 'O';
                ResponseBuffer[1] = '\r';
                ResponseBuffer[2] = 0;              // Null terminate
                break;

            case 'U':                               // Unipolar A/D
                if (CommandBufferIndex != 3)        // Check for correct command length
                    goto error_rsp;
                CommandBuffer[0] = '0';             // Add leading 0 since hex_to_bin() converts two bytes
                i = hex_to_bin(&CommandBuffer[0]);  // Control nibble
                if (i > 7)                          // Bounds checking on input command
                    goto error_rsp;
                temp = acquire_adc(i);
                ResponseBuffer[0] = 'U';
                j = ((temp & 0xFF00) >> 8);
                j = j | (i << 4);                   // Control nibble plus MSB byte of high 12 bits
                bin_to_hex(j, &ResponseBuffer[1]);
                j = (temp & 0x00FF);                // LSB of 12 bits
                bin_to_hex(j, &ResponseBuffer[3]);
                ResponseBuffer[5] = '\r';
                ResponseBuffer[6] = 0;              // Null terminate
                break;

            case 'Q':                               // Unipolar A/D (ASCII floating point output)
                if (CommandBufferIndex != 3)        // Check for correct command length
                    goto error_rsp;
                CommandBuffer[0] = '0';             // Add leading 0 since hex_to_bin() converts two bytes
                i = hex_to_bin(&CommandBuffer[0]);  // Control nibble
                if (i > 7)                          // Bounds checking on input command
                    goto error_rsp;
                temp = acquire_adc(i);
                ResponseBuffer[0] = 'Q';
                for (i=0; i< 10; i++) {
                     ResponseBuffer[i+1] = AD_Value_Str[i];
                }
                ResponseBuffer[i+1] = 0;              // Null terminate
                break;

            case 'T':                               // Set Digital Direction
                if (CommandBufferIndex != 6)        // Check for correct command length
                    goto error_rsp;
// Port1 not used in 232M100
//                i = hex_to_bin(&CommandBuffer[1]);  // PORT1
//                set_dir_port1(i);
                j = hex_to_bin(&CommandBuffer[3]);  // PORT2
                set_dir_port2(j);
                ResponseBuffer[0] = 'T';
                ResponseBuffer[1] = '\r';
                ResponseBuffer[2] = 0;              // Null terminate
                break;

            case 'G':                               // Get Digital Direction
                if (CommandBufferIndex != 2)        // Check for correct command length
                    goto error_rsp;
                ResponseBuffer[0] = 'G';
// Port1 not used in 232M100                
//                i = get_dir_port1();
                bin_to_hex(0, &ResponseBuffer[1]);
                j = get_dir_port2();
                bin_to_hex(j, &ResponseBuffer[3]);
                ResponseBuffer[5] = '\r';
                ResponseBuffer[6] = 0;              // Null terminate
                break;

            case 'N':                               // Get Pulse Counter
                if (CommandBufferIndex != 2)        // Check for correct command length
                    goto error_rsp;
                ResponseBuffer[0] = 'N';

                i = ((PulseCount & 0xFF000000) >> 24);
                bin_to_hex(i, &ResponseBuffer[1]);

                i = ((PulseCount & 0xFF0000) >> 16);
                bin_to_hex(i, &ResponseBuffer[3]);

                i = ((PulseCount & 0xFF00) >> 8);
                bin_to_hex(i, &ResponseBuffer[5]);

                j = PulseCount & 0xFF;              // PulseCount is updated +0x100 in Timer0 ISR
                i = get_timer0();                   // Get last 8 bits out of Timer0
                i = i + j;
                bin_to_hex(i, &ResponseBuffer[7]);

                ResponseBuffer[9] = '\r';
                ResponseBuffer[10] = 0;             // Null terminate
                break;

            case 'P':                               // PWM
                if (CommandBufferIndex != 7)        // Check for correct command length
                    goto error_rsp;
                i = hex_to_bin(&CommandBuffer[1]);  // Frequency (PWM period)
                setup_timer_2 (T2_DIV_BY_1, i, 1);  // Mode=div/1 Period=i Postscale=1 (not used)

                j = hex_to_bin(&CommandBuffer[3]);  // Duty - MSB
                temp = j;
                temp = temp << 4;
                CommandBuffer[4] = '0';             // 3 nibbles, insert 0
                j = hex_to_bin(&CommandBuffer[4]);  // Duty - LSB
                temp = temp + j;
                set_pwm1_duty (temp);               // Set 10 bits duty

                ResponseBuffer[0] = 'P';
                ResponseBuffer[1] = '\r';
                ResponseBuffer[2] = 0;              // Null terminate
                break;

            case 'M':                               // Clear Pulse Counter
                if (CommandBufferIndex != 2)        // Check for correct command length
                    goto error_rsp;
                set_timer0(0);                      // Clear Timer0 and the count
                PulseCount = 0;
                ResponseBuffer[0] = 'M';
                ResponseBuffer[1] = '\r';
                ResponseBuffer[2] = 0;              // Null terminate
                break;

            case 'W':                               // Write EEPROM
                if (CommandBufferIndex != 6)        // Check for correct command length
                    goto error_rsp;
                i = hex_to_bin(&CommandBuffer[1]);  // EEPROM Address
                j = hex_to_bin(&CommandBuffer[3]);  // EEPROM Value
                wr_eeprom(i, j);

                // Get new EEPROM settings
                get_eesettings(FALSE);

                ResponseBuffer[0] = 'W';
                ResponseBuffer[1] = '\r';
                ResponseBuffer[2] = 0;              // Null terminate
                break;

            case 'R':                               // Read EEPROM
                if (CommandBufferIndex != 4)        // Check for correct command length
                    goto error_rsp;
                i = hex_to_bin(&CommandBuffer[1]);  // EEPROM Address
                j = rd_eeprom(i);
                ResponseBuffer[0] = 'R';
                bin_to_hex(j, &ResponseBuffer[1]);
                ResponseBuffer[3] = '\r';
                ResponseBuffer[4] = 0;              // Null terminate
                break;

            case 'J':                               // Clear Comm Error Counter
                if(CommandBufferIndex != 2)         // Check command length
                    goto error_rsp;                 // ... oops exit command

                RxCommErr = 0;                      // Clear Comm Error
                ResponseBuffer[0] = 'J';
                ResponseBuffer[1] = '\r';
                ResponseBuffer[2] = 0;
                break;


            case 'K':                               // Comm Error Counter
                if(CommandBufferIndex != 2)         // Check command length
                    goto error_rsp;                 // ... oops exit command
                ResponseBuffer[0] = 'K';
                bin_to_hex(RxCommErr, &ResponseBuffer[1]);
                ResponseBuffer[3] = '\r';
                ResponseBuffer[4] = 0;              // Null terminate
                break;

            case 'S':                               // Stream Start
                if (CommandBufferIndex != 2)        // Check for correct command length
                    goto error_rsp;
                // Set continuous stream mode flag
                AsyncStreamFlags = AsyncStreamFlags | CONTINUOUS_STREAM;
                get_async_config();
                ResponseBuffer[0] = 'S';
                ResponseBuffer[1] = '\r';
                ResponseBuffer[2] = 0;              // Null terminate
                break;

            case 'H':                               // Stream Halt
                if (CommandBufferIndex != 2)        // Check for correct command length
                    goto error_rsp;
                // Clear continuous stream mode flag
                AsyncStreamFlags = AsyncStreamFlags & (~CONTINUOUS_STREAM);
                ResponseBuffer[0] = 'H';
                ResponseBuffer[1] = '\r';
                ResponseBuffer[2] = 0;              // Null terminate
                break;


            case 'Z':                               // Reset
                ResponseBuffer[0] = 'Z';
                ResponseBuffer[1] = '\r';
                ResponseBuffer[2] = 0;              // Null terminate
                // Write response string out UART
                xmit_response();

#ifndef _DEBUGGINGMODE_
                while (1) {                         // Force CPU reset by causing WDT
                    disable_interrupts(GLOBAL);
                }
//                reset_cpu();                        // Force CPU reset
#endif
                break;

            case 'V':                               // Version
                if (CommandBufferIndex != 2)        // Check for correct command length
                    goto error_rsp;
                ResponseBuffer[0] = 'V';
                ResponseBuffer[1] = VER_MAJ;
                ResponseBuffer[2] = VER_MIN;
                ResponseBuffer[3] = '\r';
                ResponseBuffer[4] = 0;              // Null terminate
                break;

            case '?':                               // Display POST message
                if (CommandBufferIndex != 2)        // Check for correct command length
                    goto error_rsp;
                post_message();
                commands();
                ResponseBuffer[0] = 0;              // Null terminate
                break;

            default:                                // Unknown Command
error_rsp:      // ERROR response
                ResponseBuffer[0] = 'X';
                ResponseBuffer[1] = '\r';
                ResponseBuffer[2] = 0;              // Null terminate
                break;

        } // switch (CommandChar)

        // Clear the command buffer index and last character rcvd for next command
        CommandBufferIndex = 0;
        LastCharRcvd = 0;

        // Write response string out UART
        xmit_response();
        printf("> ");
    } // while (TRUE)  -- never exit
}
