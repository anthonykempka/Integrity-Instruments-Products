//
// Integrity Instruments, Inc.
//
// 232M300 Module v4.5
//
// Revision History
// ----------------------------------------------------------------------------
//  Anthony Kempka      3/25/2004
//
//  Original 'C' code version. Based on USBM300 code base.
//
//
//  AAK                 5/6/2004
//  Fixed the Async Change mode to only output either Digital
//  or Pulse Counter outputs (not both on a change of either)
//
//  TGACK               5/2/2005        4.5
//  Fixed startup when pulse counter in is high.
//
//  TGACK               6/24/2005       4.6
//  J and K commands are missing in the Version 4.x series
//  Added J and K Commands
//
//  TGACK               7/11/2005       4.7
//  LF characters cause invalid command responses
//  Stripped incomming LF (0x0A) characters from stream

#if defined(__PCM__)

#include <16F874A.h>

// BUGBUG: These two lines are enabled for debugging
//#device ICD=TRUE
//#fuses HS, NOWDT, NOPROTECT, BROWNOUT, PUT, NOLVP, DEBUG

//
// HS = High Speed osc, PROTECT = code protect, BROWNOUT = brownout protection
// PUT = power up timer, NOLVP = no low voltage program,
// NOWRT = no write protect, NOCPD = no EEPROM protect
#fuses HS, WDT, PROTECT, BROWNOUT, PUT, NOLVP, NOWRT, NOCPD

// Setup the clock base time and RS-232 Baud rate
#use delay(clock=14745600)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)
#endif

//
// Turn off the compiler's caching (virtual port) auto data direction crap
//
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

#byte EEADR=0x10D
#byte EEADRH=0x10F
#byte EEDATA=0x10C
#byte EEDATH=0x10E
#byte EECON1=0x18C
#byte EECON2=0x18D

#byte SSPSTAT=0x94
#byte SSPCON=0x14

//
// EEPROM Locations
//
#define EEPROM_MOD_ADDRESS      0x00
#define EEPROM_DIR_PORT1        0x02
#define EEPROM_DIR_PORT2        0x03
#define EEPROM_ASYNC_MSB        0x04
#define EEPROM_ASYNC_LSB        0x05
#define EEPROM_POD_PORT1        0x06
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

// SPI device chip select lines -- PORTA
#define AD_CS           PIN_A0
#define DAC_CS          PIN_A5

// LED defines -- PORTC
#define LED_RED         PIN_C1
#define LED_GREEN       PIN_C0

// EEPROM flags used to configure run-time settings
#define EXPANDER_FLAG   0x01
#define SLOW_ADC_CLOCK  0x02
int8    EEFlags = 0;

int8    CurrentDip;         // Current DIP switch setting

// SPI settings for various speeds
#define SPI_ADC_FAST    2
#define SPI_DAC         2
#define SPI_ADC_SLOW    3
int8    SpiMode = 0;        // 0 Guarantees that setup_spi_mode() will be called the first time

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
int8    Port1Cur;               // Current Port 1 input - ASYNC_CHANGE
int8    Port2Cur;               // Current Port 2 input - ASYNC_CHANGE
int32   PulseCountCur;          // Current Pulse Count - ASYNC_CHANGE

// Led flash control variables
int8    LedGreenFlag = 1;
int16   LedCount = 0;

// Port direction and D/A shadow variables
int8    Port1_Dir = 0;
int8    Port2_Dir = 0;
int16   DAC_CH0 = 0;
int16   DAC_CH1 = 0;

// 32-bit pulse counter
int32   PulseCount = 0;

//
// RS-232 command (receive) and response (transmit) buffers and sizes
#define CMDBUFFERSIZE   16
char    LastCharRcvd;
int8    CommandBufferIndex;
char    CommandBuffer[CMDBUFFERSIZE];
char    ResponseBuffer[16];


//
// RS-232 Communication Error Counter
int8    RxCommErr;


//
// Forward function declarations
//
#inline
void trigger_led_red(void);
void led_on_green(void);

#include "string.h"


//-----------------------------------------------------------------------------
// setup_spi_fast()
//
// Used to configure the PIC SPI correctly since the built-in
// CCS routines are broken. CLK = Fosc / 16
//
void setup_spi_fast(void)
{
    SSPCON = 0x21;      // SSPCON: SPEN=1, CKP=0 (idle high), Fosc/16
    SSPSTAT = 0xC0;     // SSPSTAT: SMP=1 (input sample at end of data output time)
                        //          CKE=1 (CKP=1 data transmitted on falling edge of SCK)
    return;
}


//-----------------------------------------------------------------------------
// setup_spi_clk64()
//
// Used to configure the PIC SPI correctly since the built-in
// CCS routines are broken. CLK = Fosc / 64
//
void setup_spi_clk64(void)
{
    SSPCON = 0x22;      // SSPCON: SPEN=1, CKP=0 (idle high), Fosc/64
    SSPSTAT = 0xC0;     // SSPSTAT: SMP=1 (input sample at end of data output time)
                        //          CKE=1 (CKP=1 data transmitted on falling edge of SCK)
    return;
}


//-----------------------------------------------------------------------------
// setup_spi_clk64()
//
// SPI clock/64 Used to configure the PIC SPI correctly since the built-in
// CCS routines are broken.
//
void setup_spi_clk64_org(void)
{
    #asm
    bcf    0x14,5   ; Disable SPI (SSPCON: SPEN=0)
    movlw  0x32     ; SSPCON: CKP=1 (idle high), SPEN=1, SPI Master, Fosc/64
    movwf  0x14
    movlw  0xC0     ; SSPSTAT: SMP=1 (input sample at end of data output time)
    bsf    0x03,5   ;          CKE=1 (CKP=1 data transmitted on falling edge of SCK)
    movwf  0x14
    bcf    0x03,5
    #endasm
    return;
}


//-----------------------------------------------------------------------------
// setup_spi_mode(int8 mode)
//
// Used to configure the PIC SPI correctly since the built-in
// CCS routines are broken.
//
#separate
void setup_spi_mode(int8 mode)
{
    // SPI_ADC_FAST, SPI_DAC
    if (mode == SPI_ADC_FAST) {
        setup_spi_fast();
        SpiMode = SPI_ADC_FAST;
    } else {
        // SPI_ADC_SLOW
        setup_spi_clk64();
        SpiMode = SPI_ADC_SLOW;
    }
    return;
}


//-----------------------------------------------------------------------------
// update_dac()
//
// Output an updated DAC value in LTC1448
// DAC output values in DAC_CH0 and DAC_CH1
// These need to be put together into 3 bytes (24 bits) for the DAC
//
#separate
void update_dac(void)
{
    int8    data;

    // Check and set SPI mode
    if (SpiMode != SPI_DAC)
        setup_spi_mode(SPI_DAC);

    output_low(DAC_CS);     // Select the D/A converter

    data = ((DAC_CH0 & 0x0FF0) >> 4);
    spi_write(data);

    data = ((DAC_CH0 & 0x000F) << 4);
    data = data | ((DAC_CH1 & 0x0F00) >> 8);
    spi_write(data);

    data = (DAC_CH1 & 0x00FF);
    spi_write(data);

    output_high(DAC_CS);    // De-Select the D/A converter
    return;
}

//-----------------------------------------------------------------------------
// int16 acquire_adc(int8 control, int8 unipolar)
//
// Acquire and A/D value from LTC1296
//
#separate
int16 acquire_adc(int8 control, int8 unipolar)
{
    int16   ad_sample;
    int8    data;

    // Check and configure SPI if needed
    if (EEFLags & SLOW_ADC_CLOCK)
    {
        if (SpiMode != SPI_ADC_SLOW)
            setup_spi_mode(SPI_ADC_SLOW);
    } else {
        if (SpiMode != SPI_ADC_FAST)
            setup_spi_mode(SPI_ADC_FAST);
    }

    output_low(AD_CS);              // Select the D/A converter

    data = control & 0x0F;          // Mask off lower nibble
    data = data | 0x10;             // OR in the START bit
    spi_write(data);

    if (unipolar)                   // UNIPOLAR or BIPOLAR sampling
    {
        data = spi_read(0xE0);      // UNI=1, MSBF=1, PS=1
    } else {
        data = spi_read(0x60);      // UNI=0, MSBF=1, PS=1
    }

    ad_sample = data & 0x0F;        // Upper 4 bits of data conversion
    ad_sample = ad_sample << 8;

    data = spi_read(0);             // Get lower 8 bits of data comnversion
    ad_sample = ad_sample | data;

    output_high(AD_CS);     // De-Select the D/A converter
    return (ad_sample);
}


//-----------------------------------------------------------------------------
// int8 rd_eeprom(int8 address)
//
// Read EEPROM
// The 16F847A has an internal EEPROM
//
#separate
int8 rd_eeprom(int8 address)
{
    int8    data;

    EEADRH = 0;         // Upper bits of 10 bit address = 0
    EEADR = address;    // Address register
    EECON1 = 0x01;      // Access Data EEPROM, RD=1
    data = EEDATA;      // Read the data out of the EEPROM
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

    EEADRH = 0;         // Upper bits of 10 bit address = 0
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

//-----------------------------------------------------------------------------
// write_port1(int8 value)
//
// Write the digital output to PORT1
// BIT 0-7 = PORTB 0-7
//
#inline
void write_port1(int8 value)
{
    // Expander inversion
    if (EEFlags & EXPANDER_FLAG)
        value = value ^ 0xFF;

    PORT_B = value;             // bits 0-7 = PORTB 0-7
    return;
}

//-----------------------------------------------------------------------------
// int8 read_port1(void)
//
// Reads the digital input of PORT1
// BIT 0-7 = PORTB 0-7
//
#inline
int8 read_port1(void)
{
    int8 value;

    value = PORT_B;             // PORT1 bits 1-7 are PORTB bits 1-7

    // Expander inversion
    if (EEFlags & EXPANDER_FLAG)
        value = value ^ 0xFF;
    return (value);
}

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
    PORT_D = value;
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

    value = PORT_D;             // PORT2 bits 0-7 = PORT_D bits 0-7

    // Expander inversion
    if (EEFlags & EXPANDER_FLAG)
        value = value ^ 0xFF;
    return (value);
}


//-----------------------------------------------------------------------------
// set_dir_port1(int8 direction)
//
// Set the data direction of PORT1
// BIT 0-7 = PORTB 0-7
//
void set_dir_port1(int8 direction)
{
    Port1_Dir = direction;
    wr_eeprom(EEPROM_DIR_PORT1, Port1_Dir);
    set_tris_b(Port1_Dir);
    return;
}


//-----------------------------------------------------------------------------
// get_dir_port1()
//
// Returns the current data direction (input/output) of Port1
//
int8 get_dir_port1(void)
{
    return (Port1_Dir);
}

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
    // Timer0 ISR is triggered on 88 bit overflow (256)
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
    set_timer1(0xF199);                     // 1ms = 65536-(.001/(4/14745600)) = 0xF199

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
RDA_isr()
{
    led_on_green();
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
void get_eesettings(void)
{
    int8    i;

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
    // A/D Stream count -- max 8!
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

    NewDip = PORT_E & 0x7;
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
            i = read_port1();
            if (Port1Cur != i)
            {
                Port1Cur = i;
                AsyncStreamUpdate |= DIGITAL_CHANGE;
            }
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
                //
                // Analog sample
                for (x=0; x<StreamADCount; x++)
                {
                    if (ADSampleConfig[x] & 0x80)
                    {
                        //
                        // Unipolar sample
                        i = ADSampleConfig[x] & 0x0F;
                        temp = acquire_adc(i, 1);
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
                    } else {
                        //
                        // Bipolar sample
                        i = ADSampleConfig[x] & 0x0F;
                        temp = acquire_adc(i, 0);
                        ResponseBuffer[0] = 'Q';
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
                }
                // Digital and Pulse outputs after Analog
                if (AsyncStreamFlags & STREAM_DIGITAL)
                {
                    Port1Cur = read_port1();
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
                i = Port1Cur;
                bin_to_hex(i, &ResponseBuffer[1]);
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
    printf("\rIntegrity Instruments, Inc. Model 232M300 v4.6\r");
    return;
}


//-----------------------------------------------------------------------------
// Main program entry point.
//
main()
{
    int8    i, j, k, x;
    int16   temp;

    // Shut off built-int ADC. The enables all PORT_A to general purpose I/O
    setup_adc_ports(NO_ANALOGS);
    setup_adc(ADC_OFF);
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
    set_tris_a(0x10);   // A0-3=O, A4=I, A5=O
    output_high(AD_CS);
    output_high(DAC_CS);

    set_tris_b(0xFF);   // B0-7=I
    set_tris_d(0xFF);   // D0-7=I
    set_tris_c(0xD0);   // C0-3=O C5=O, C4=I, C6=I, C7=I
    set_tris_e(0x07);   // E0-2=I

    trigger_led_red();  // Initial Led RED

    // Get EEPROM settings
    get_eesettings();

    //
    // Get Port data direction from EEPROM
    i = rd_eeprom(EEPROM_DIR_PORT1);
    set_dir_port1(i);
    i = rd_eeprom(EEPROM_DIR_PORT2);
    set_dir_port2(i);

    //
    // Get power on defaults for I/O ports from EEPROM
    i = rd_eeprom(EEPROM_POD_PORT1);
    write_port1(i);
    i = rd_eeprom(EEPROM_POD_PORT2);
    write_port2(i);

    //
    // Get power on defaults for D/A converter
    i = rd_eeprom(EEPROM_POD_DA_MSB_CH0);
    temp = i;
    temp = temp << 8;
    i = rd_eeprom(EEPROM_POD_DA_LSB_CH0);
    temp = temp | i;
    DAC_CH0 = temp;
    i = rd_eeprom(EEPROM_POD_DA_MSB_CH1);
    temp = i;
    temp = temp << 8;
    i = rd_eeprom(EEPROM_POD_DA_LSB_CH1);
    temp = temp | i;
    DAC_CH1 = temp;

    // Send the power on default values to the D/A converter
    update_dac();

    //
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
    setup_timer_0(RTCC_EXT_H_TO_L | RTCC_DIV_1);
    enable_interrupts(int_timer0);              // Timer0 RTCC Pulse counter input
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);     // setup interrupts
    enable_interrupts(int_timer1);              // Timer1 Periodic 1 ms. timer
    enable_interrupts(INT_RDA);

    enable_interrupts(global);                  // All Interrupts on

    // Setup PWM inital settings
    setup_timer_2 (T2_DIV_BY_1, 0, 1);          // Mode=div/1 Period=0 Postscale=1 (not used)
    setup_ccp1 (CCP_PWM);                       // Configure CCP1 for PWM
    set_pwm1_duty (0);                          // Initially off

    // Configure watchdog -- 18ms implies not scale through Timer 0
    setup_wdt(WDT_36MS);

    // Check DIP switch settings for initial baud rate
    get_dip_set_baud();

    // Display POST message
    post_message();

    // Get ASYNC continuous stream mode configuration
    get_async_config();
    // Initial ASYNC digital states
    Port1Cur = read_port1();
    Port2Cur = read_port2();
    PulseCountCur = 0;
    RxCommErr = 0;


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
                i = read_port1();
                bin_to_hex(i, &ResponseBuffer[1]);
                j = read_port2();
                bin_to_hex(j, &ResponseBuffer[3]);
                ResponseBuffer[5] = '\r';
                ResponseBuffer[6] = 0;              // Null terminate
                break;

            case 'O':                               // Digital Output
                if (CommandBufferIndex != 6)        // Check for correct command length
                    goto error_rsp;
                i = hex_to_bin(&CommandBuffer[1]);  // PORT1
                write_port1(i);
                j = hex_to_bin(&CommandBuffer[3]);  // PORT2
                write_port2(j);
                ResponseBuffer[0] = 'O';
                ResponseBuffer[1] = '\r';
                ResponseBuffer[2] = 0;              // Null terminate
                break;

            case 'Q':                               // Bipolar A/D
                if (CommandBufferIndex != 3)        // Check for correct command length
                    goto error_rsp;
                CommandBuffer[0] = '0';             // Add leading 0 since hex_to_bin() converts two bytes
                i = hex_to_bin(&CommandBuffer[0]);  // Control nibble
                temp = acquire_adc(i, 0);
                ResponseBuffer[0] = 'Q';
                j = ((temp & 0xFF00) >> 8);
                j = j | (i << 4);                   // Control nibble plus MSB byte of high 12 bits
                bin_to_hex(j, &ResponseBuffer[1]);
                j = (temp & 0x00FF);                // LSB of 12 bits
                bin_to_hex(j, &ResponseBuffer[3]);
                ResponseBuffer[5] = '\r';
                ResponseBuffer[6] = 0;              // Null terminate
                break;

            case 'U':                               // Unipolar A/D
                if (CommandBufferIndex != 3)        // Check for correct command length
                    goto error_rsp;
                CommandBuffer[0] = '0';             // Add leading 0 since hex_to_bin() converts two bytes
                i = hex_to_bin(&CommandBuffer[0]);  // Control nibble
                temp = acquire_adc(i, 1);
                ResponseBuffer[0] = 'U';
                j = ((temp & 0xFF00) >> 8);
                j = j | (i << 4);                   // Control nibble plus MSB byte of high 12 bits
                bin_to_hex(j, &ResponseBuffer[1]);
                j = (temp & 0x00FF);                // LSB of 12 bits
                bin_to_hex(j, &ResponseBuffer[3]);
                ResponseBuffer[5] = '\r';
                ResponseBuffer[6] = 0;              // Null terminate
                break;

            case 'L':                               // Output D/A
                if (CommandBufferIndex != 6)        // Check for correct command length
                    goto error_rsp;
                CommandBuffer[0] = '0';             // Add leading 0 since hex_to_bin() converts two bytes
                i = hex_to_bin(&CommandBuffer[0]);  // Control nibble (channel number 0 or 1)
                CommandBuffer[1] = '0';             // Zero the control nibble since hex_to_bin() converts two bytes
                j = hex_to_bin(&CommandBuffer[1]);  // MSB (nibble) of the D/A value
                temp = j;
                temp = temp << 8;
                j = hex_to_bin(&CommandBuffer[3]);  // LSB (byte) of the D/A value
                temp = temp | j;
                if (i == 0) {                       // Channel 0
                    DAC_CH0 = temp;
                } else {                            // Channel 1
                    DAC_CH1 = temp;
                }
                update_dac();                       // Send the new values to the D/A converter
                ResponseBuffer[0] = 'L';
                ResponseBuffer[1] = '\r';
                ResponseBuffer[2] = 0;              // Null terminate
                break;

            case 'T':                               // Set Digital Direction
                if (CommandBufferIndex != 6)        // Check for correct command length
                    goto error_rsp;
                i = hex_to_bin(&CommandBuffer[1]);  // PORT1
                set_dir_port1(i);
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
                i = get_dir_port1();
                bin_to_hex(i, &ResponseBuffer[1]);
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

                //
                // Get new EEPROM settings
                //
                get_eesettings();

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

                while (1) {                         // Force CPU reset by causing WDT
                    disable_interrupts(GLOBAL);
                }

//                reset_cpu();                        // Force CPU reset
                break;

            case 'V':                               // Version
                if (CommandBufferIndex != 2)        // Check for correct command length
                    goto error_rsp;
                ResponseBuffer[0] = 'V';
                ResponseBuffer[1] = '4';
                ResponseBuffer[2] = '7';
                ResponseBuffer[3] = '\r';
                ResponseBuffer[4] = 0;              // Null terminate
                break;

            case '?':                               // Display POST message
                if (CommandBufferIndex != 2)        // Check for correct command length
                    goto error_rsp;
                post_message();
                ResponseBuffer[0] = 0;              // Null terminate
                break;

            default:                                // Unknown Command
error_rsp:      // ERROR response
                ResponseBuffer[0] = 'X';
                ResponseBuffer[1] = '\r';
                ResponseBuffer[2] = 0;              // Null terminate
                RxCommErr++;
                break;

        } // switch (CommandChar)

        // Clear the command buffer index and last character rcvd for next command
        CommandBufferIndex = 0;
        LastCharRcvd = 0;

        // Write response string out UART
        xmit_response();

    } // while (TRUE)  -- never exit
}
