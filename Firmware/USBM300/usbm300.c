//
// Put in file header comments
//

#if defined(__PCM__)

#include <16c67.h>

// BUGBUG Change these fuses in release!
#fuses HS,NOWDT,PROTECT,PUT,BROWNOUT

//
// Setup the clock base time and RS-232 Baud rate
//
#use delay(clock=20000000)
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

//
// Memory (port) defines so we can use them in 'C'
#byte PORT_B=6
#byte PORT_C=7
#byte PORT_D=8
#byte PORT_E=9
#byte SSPSTAT=0x14
#byte SSPCON=0x14

//
// EEPROM access routine defines
//
#define EEPROM_SELECT PIN_B0
#define EEPROM_DI     PIN_C5
#define EEPROM_DO     PIN_C4
#define EEPROM_CLK    PIN_C3

#define EEPROM_ADDRESS byte
#define EEPROM_SIZE    256

//
// EEPROM Locations
//
#define EEPROM_MOD_ADDRESS      0x00
#define EEPROM_DIR_PORT1        0x02
#define EEPROM_DIR_PORT2        0x03
#define EEPROM_POD_PORT1        0x06
#define EEPROM_POD_PORT2        0x07
#define EEPROM_EXPANDER_FLAG    0x08
#define EEPROM_POD_DA_MSB_CH0   0x09
#define EEPROM_POD_DA_LSB_CH0   0x0A
#define EEPROM_POD_DA_MSB_CH1   0x0B
#define EEPROM_POD_DA_LSB_CH1   0x0C
#define EEPROM_AD_SAMPLE_CLK    0x0D

//
// SPI device chip select lines
// PORTA
//
#define AD_CS           PIN_A0
#define EEPROM_CS       PIN_A1
#define USB_CS          PIN_A2
#define RTC_CS          PIN_A3
#define DAC_CS          PIN_A5

//
// LED defines
// PORTC
//
#define LED_RED         PIN_C1
#define LED_GREEN       PIN_C0

//
// Global variable declarations
//
#define EXPANDER_FLAG   1
#define SLOW_ADC_CLOCK  2

int8    EEFlags = 0;

int8    ADCNibble[8];       // EEPROM locations 0x11 to 0x18

#define SPI_USB         1
#define SPI_ADC_FAST    2
#define SPI_EEPROM      2
#define SPI_DAC         2
#define SPI_ADC_SLOW    3

int8    SpiMode = 0;        // 0 Guarantees that setup_spi_mode() will be called the first time

int8    StreamFlag = 0;

int8    LedGreenFlag = 1;
int8    LedCount = 0;

int8    Port1_Dir = 0;
int8    Port2_Dir = 0;
int16   DAC_CH0 = 0;
int16   DAC_CH1 = 0;

int32   PulseCount = 0;

int8    CommandBuffer[8];
int8    ResponseBuffer[10];

//
// Constant table definitions
//
const int8 BIN_TO_HEX_TBL[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

//
// Forward function declarations
//
#separate
trigger_led_red(void);
led_on_green(void);

#include "string.h"


setup_spi_adc_fast()
{
    #asm
    movlw  0x04
    movwf  0x12
    movlw  0x02
    bsf    0x03,5
    movwf  0x12
    bcf    0x03,5

    bcf    0x14,5
    movlw  0x33
    movwf  0x14
    movlw  0xC0
    bsf    0x03,5
    movwf  0x14
    bcf    0x03,5
    #endasm
    return;
}

setup_spi_clk16()
{
    #asm
    bcf    0x14,5
    movlw  0x31
    movwf  0x14
    movlw  0xC0
    bsf    0x03,5
    movwf  0x14
    bcf    0x03,5
    #endasm
    return;
}

setup_spi_clk64()
{
    #asm
    bcf    0x14,5
    movlw  0x32
    movwf  0x14
    movlw  0xC0
    bsf    0x03,5
    movwf  0x14
    bcf    0x03,5
    #endasm
    return;
}

#separate
setup_spi_mode(int8 mode)
{
    if (mode == SPI_USB) {
        setup_spi(SPI_MASTER | SPI_L_TO_H | SPI_CLK_DIV_16);
        SpiMode = SPI_USB;
        return;
    }

    // SPI_EEPROM, SPI_ADC_FAST, SPI_DAC
    if (mode == SPI_EEPROM) {
        setup_spi_adc_fast();
        SpiMode = SPI_EEPROM;
        return;
    }

    // SPI_ADC_SLOW
    setup_spi_clk64();
    SpiMode = SPI_ADC_SLOW;
    return;
}



//
// Include the USB code
//
#include <usbn9603.c>

//-----------------------------------------------------------------------------
// 25C040 read EEPROM
//
#separate
int8 read_eeprom(int8 address)
{
    int8    data;

    // Check and set SPI mode
    if (SpiMode != SPI_EEPROM)
        setup_spi_mode(SPI_EEPROM);

    output_low(EEPROM_CS);  // Select the EEPROM
    spi_write(0x03);        // READ command byte, A8=0
    spi_write(address);     // ADDRESS byte
    data = spi_read(0);     // read DATA byte
    output_high(EEPROM_CS); // De-select the EEPROM
    return (data);
}

//-----------------------------------------------------------------------------
// 25C040 write EEPROM
//
#separate
write_eeprom(int8 address, int8 data)
{
    int8 i;

    // Check and set SPI mode
    if (SpiMode != SPI_EEPROM)
        setup_spi_mode(SPI_EEPROM);

    output_low(EEPROM_CS);  // Select the EEPROM
    spi_write(0x06);        // WRITE ENABLE command byte
    output_high(EEPROM_CS); // De-select the EEPROM to set WRITE ENABLE
    // The 25C40 needs a little delay (500 ns)
#asm
    nop
    nop
    nop
#endasm
    output_low(EEPROM_CS);  // Select the EEPROM
    spi_write(0x02);        // WRITE  command byte, A8=0
    spi_write(address);     // ADDRESS byte
    spi_write(data);        // DATA byte
    output_high(EEPROM_CS); // De-select the EEPROM

    //
    // Wait for WRITE to complete
    do {
        // The 25C40 needs a little delay (500 ns) between CS
#asm
        nop
        nop
        nop
#endasm
        output_low(EEPROM_CS);  // Select the EEPROM
        spi_write(0x05);    // READ STATUS REGISTER
        i = spi_read(0);    // WIP (Write In Progess) = bit 0
        output_high(EEPROM_CS); // De-select the EEPROM
    } while (i & 0x01);

    return;
}

//-----------------------------------------------------------------------------
// Output an updated DAC value in LTC1448
//
// DAC output values in DAC_CH0 and DAC_CH1
//
// These need to be put together into 3 bytes (24 bits) for the DAC
//
#separate
update_dac()
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
// Acquire and A/D value from LTC1296
//
#separate
int16 acquire_adc(int8 control, int8 unipolar)
{
    int16   ad_sample;
    int8    data;

    // Check and configure SPI if needed
    if (EEFLags & SLOW_ADC_CLOCK) {
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

    if (unipolar) {
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
// Turn LED on (RED)
//
led_on_red(void)
{
    output_high(LED_RED);
    output_low(LED_GREEN);
    return;
}

//-----------------------------------------------------------------------------
// Turn LED on (GREEN)
//
led_on_green(void)
{
    output_high(LED_GREEN);
    output_low(LED_RED);
    return;
}

//-----------------------------------------------------------------------------
// Turn LED off
//
led_off(void)
{
    output_low(LED_RED);
    output_low(LED_GREEN);
    return;
}

#separate
trigger_led_red(void)
{
    // Only start the Red sequence if it was Green
    if (LedGreenFlag) {
        LedGreenFlag = 0;
        LedCount = 90;          // To make a faster Red LED blink, put a value here (0 ... 100)
                                // Full blink gets a 0
        led_on_red();
    }
}

//-----------------------------------------------------------------------------
// Write the digital output to PORT1
// BIT 0 = PORTE 0
// BIT 1-7 = PORTB 1-7
//
#separate
write_port1(int8 value)
{
    // Expander inversion
    if (EEFlags & EXPANDER_FLAG)
        value = value ^ 0xFF;

    if (value & 0x01)           // bit 0 = PORTe bit 0
        output_high(PIN_E0);
    else
        output_low(PIN_E0);

    value = value & 0xFE;       // mask off bit 0
    PORT_B = value;             // bits 1-7 = PORTB 1-7
    return;
}

//-----------------------------------------------------------------------------
// Read the digital input of PORT1
// BIT 0 = PORTE 0
// BIT 1-7 = PORTB 1-7
//
#separate
int8 read_port1(void)
{
    int8 value, mask;

    value = PORT_B;             // PORT1 bits 1-7 are PORTB bits 1-7
    value = PORT_B & 0xFE;      // mask off bit 0
    mask  = PORT_E;             // PORT1 bit 0 = PORTE bit 0
    mask  = mask * 0x01;
    value = value | mask;

    // Expander inversion
    if (EEFlags & EXPANDER_FLAG)
        value = value ^ 0xFF;
    return (value);
}

//-----------------------------------------------------------------------------
// Write the digital output to PORT2
// BIT 0-7 = PORTD 0-7
//
write_port2(int8 value)
{
    // Expander inversion
    if (EEFlags & EXPANDER_FLAG)
        value = value ^ 0xFF;
    PORT_D = value;
    return;
}

//-----------------------------------------------------------------------------
// Read the digital input of PORT2
// BIT 0-7 = PORTB 0-7
//
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
// Set the data direction of PORT1
// BIT 0 = PORTE 0
// BIT 1-7 = PORTB 1-7
//
set_dir_port1(int8 direction)
{
    int8 i;
    Port1_Dir = direction;
    write_eeprom(EEPROM_DIR_PORT1, Port1_Dir);
    i = Port1_Dir | 0x01;
    set_tris_b(i);          // bit 0 = input, bits 1-7 set by Port1_Dir
    i = Port1_Dir & 0x01;
    i = i | 0x06;
    set_tris_e(i);          // bits 1-2 = input, bit 0 set by Port1_Dir
    return;
}

int8 get_dir_port1(void)
{
    return (Port1_Dir);
}

//-----------------------------------------------------------------------------
// Set the data direction of PORT2
// BIT 0-7 = PORTB 0-7
//
set_dir_port2(int8 direction)
{
    Port2_Dir = direction;
    write_eeprom(EEPROM_DIR_PORT2, Port2_Dir);
    set_tris_d(Port2_Dir);
    return;
}

int8 get_dir_port2(void)
{
    return (Port2_Dir);
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

    // MSB conversion
    for (i=0; i<16; i++) {
        if (BIN_TO_HEX_TBL[i] == *tmp_ptr) {
            value = i * 16;
            break;
        }
    }

    // LSB conversion
    tmp_ptr++;
    for (i=0; i<16; i++) {
        if (BIN_TO_HEX_TBL[i] == *tmp_ptr) {
            value = value + i;
            break;
        }
    }

    return (value);
}

//-----------------------------------------------------------------------------
// Converts binary value to two hexadecimal ASCII characters
//
#separate
void bin_to_hex(int8 value, int8 *ptr)
{
    int8 tbl_index;
    int8 *tmp_ptr;

    tmp_ptr = ptr;

    // MSB first
    tbl_index = value;
    tbl_index = tbl_index & 0xF0;
    tbl_index = tbl_index >> 4;
    *tmp_ptr = BIN_TO_HEX_TBL[tbl_index];
    // LSB last
    tbl_index = value;
    tbl_index = tbl_index & 0x0F;
    tmp_ptr++;
    *tmp_ptr = BIN_TO_HEX_TBL[tbl_index];
    return;
}

//-----------------------------------------------------------------------------
// RTCC/Timer0 interrupt - pulse counter is redirected to Timer0
//
#int_timer0
void timer0_isr(void)
{
    PulseCount = PulseCount + 0x100;
    return;
}

//-----------------------------------------------------------------------------
// Timer1 interrupt - 10 milliseconds
//
#int_timer1
void timer1_isr(void)
{
//    set_timer1(0xEC78);                       // 1ms = 65536-(.001/(4/20000000)) = 60536 (0xEC78)
    set_timer1(0x3CB0);                       // 10ms = 65536-(.01/(4/20000000)) = 0x3CB0

    LedCount++;
    if (LedCount < 100) {                   // One second count (LED on)
        if (LedCount & 1) {
            if (LedGreenFlag)               // LED on (Red or Green)
                led_on_green();
            else
                led_on_red();
        } else {
            led_off();                      // Toggle off every other cycle (100 Hz)
        }
    } else {
        led_off();
        if (LedGreenFlag) {
            // Green LED timing
            if (LedCount > 200) {               // Two second count (LED off)
                LedCount = 0;
                LedGreenFlag = 1;
            }
        } else {
            // Red LED timing
            if (LedCount > 110) {               // short off phase (250 ms.)
                LedCount = 0;
                LedGreenFlag = 1;
            }
        }
    }
    return;
}


//-----------------------------------------------------------------------------
// update_eesettings - updates the EEPROM based flags and vriables
//
#separate
void update_eesettings(void)
{
    int8 i;

    //
    // Expander flag and slow A/D flag
    //
    i = read_eeprom(EEPROM_EXPANDER_FLAG);
    if (i) {
        EEFlags = EEFLags | EXPANDER_FLAG;
    } else {
        EEFlags = EEFLags & (~EXPANDER_FLAG);
    }
    i = read_eeprom(EEPROM_AD_SAMPLE_CLK);
    if (i) {
        EEFlags = EEFLags | SLOW_ADC_CLOCK;
    } else {
        EEFlags = EEFLags & (~SLOW_ADC_CLOCK);
    }

    // A/D Control nibble. In EEPROM locations 0x11 to 0x18
    for (i=0; i<8; i++) {
        AdcNibble[i] = read_eeprom(0x11+i);
    }

    return;
}

//-----------------------------------------------------------------------------
// Main program entry point.
//
main() {

    int8    i, j, k, x;
    int16   temp;

    //
    // Setup the TRIS registers and initial port outputs
    // Disable all SPI device chip selects
    //
    set_tris_a(0x10);   // A0-3=O, A4=I, A5=O
    output_high(AD_CS);
    output_high(EEPROM_CS);
    output_high(USB_CS);
    output_high(RTC_CS);
    output_high(DAC_CS);

    set_tris_b(0xFF);   // B0-7=I
    set_tris_d(0xFF);   // D0-7=I
    set_tris_c(0xD0);   // C0-3=O, C4=I, C6=I, C7=I
    set_tris_e(0x07);   // E0-3=I

    //
    // Get EEPROM settings
    //
    update_eesettings();

//    while (TRUE) {
//        temp = acquire_adc(8, 1);
//    }

    //
    // Set Port data direction from EEPROM
    //
    i = read_eeprom(EEPROM_DIR_PORT1);
    set_dir_port1(i);
    i = read_eeprom(EEPROM_DIR_PORT2);
    set_dir_port2(i);

    //
    // Output power on default for I/O ports from EEPROM
    //
    i = read_eeprom(EEPROM_POD_PORT1);
    write_port1(i);
    i = read_eeprom(EEPROM_POD_PORT2);
    write_port2(i);

    //
    // Output power on default for D/A converter
    //
    i = read_eeprom(EEPROM_POD_DA_MSB_CH0);
    temp = i;
    temp = temp << 8;
    i = read_eeprom(EEPROM_POD_DA_LSB_CH0);
    temp = temp | i;
    DAC_CH0 = temp;
    i = read_eeprom(EEPROM_POD_DA_MSB_CH1);
    temp = i;
    temp = temp << 8;
    i = read_eeprom(EEPROM_POD_DA_LSB_CH1);
    temp = temp | i;
    DAC_CH1 = temp;

    update_dac();                       // Send the new values to the D/A converter

    //
    // Clear the command buffer
    //
    for (i=0; i<8; i++) {
        CommandBuffer[i] = 0;
    }

    //
    // Get the version out of the USB chip
    //
    i = usbn_get_version();

    if (i != 0x02) {  // USBN9603 and USBN9604 rev = 0x02
        //
        // This should reset the CPU
        //
        while (TRUE) {
            led_on_red();
        }
    }

    //
    // Initialize the USB chip. Now ready to communicate
    //
    usb_init();

    //
    // Initialize the other interrupts.
    // Note: enable_interrupts(global) is called in usb_init()
    //
    set_timer0(0);
    setup_timer_0(RTCC_EXT_H_TO_L | RTCC_DIV_1);
    enable_interrupts(int_timer0);              // Timer0 RTCC Pulse counter input
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);     // setup interrupts
    enable_interrupts(int_timer1);              // Timer1 Periodic 1 ms. timer
    enable_interrupts(global);

// Output a string to the RS-232 port
//   printf("\r\n\Running...ABCDEFGHIJKLMNOPQRSTUVWXYZ1\r\n");

//    CommandBuffer[0] = 0;

    //
    // Main Loop - never exits
    //
    while (TRUE)
    {
        //
        // Wait for a character in the USB Command buffer
        //
        if (CommandBuffer[0] == 0) {
            //
            // Check stream mode
            //
            if (StreamFlag) {
                //
                // Is TX FIFO2 empty?
                //
                if (endpoint_stat[2].FIFO_status == EMPTY) {
                    //
                    // A/D data to TX FIFO2.
                    // 24 samples = 3 sets of 8 samples
                    //
                    for (k=0; k<3; k++) {
                        for (j=0; j<8; j++) {
                            // turn off interrupts to grant exclusive SPI acess
                            disable_interrupts(global);
                            x = ADCNibble[j];
                            if (x & 0x80) {
                                // Unipolar A/D
                                x = x & 0x0F;
                                temp = acquire_adc(x, 1);
                            } else {
                                // Bipolar A/D
                                x = x & 0x0F;
                                temp = acquire_adc(x, 0);
                            }

                            // Little endian order for Intel PC
                            i = (temp & 0x00FF);
                            usbn_write(TXD2, i);

                            i = (temp & 0xFF00) >> 8;
                            usbn_write(TXD2, i);

                            // turn on interrupts
                            enable_interrupts(global);
                        }
                    }

                    //
                    // Digital port data tp TX FIFO2
                    // Little endian order for Intel PC
                    //
                    i = read_port2();
                    usbn_write(TXD2, i);

                    i = read_port1();
                    usbn_write(TXD2, i);

                    //
                    // Pulse Counter data to TX FIFO2
                    // Little endian order for Intel PC
                    //
                    j = PulseCount & 0x000000FF;
                    i = get_timer0();
                    i = i + j;
                    usbn_write(TXD2, i);

                    i = ((PulseCount & 0x0000FF00) >> 8);
                    usbn_write(TXD2, i);

                    i = ((PulseCount & 0x00FF0000) >> 16);
                    usbn_write(TXD2, i);

                    i = ((PulseCount & 0xFF000000) >> 24);
                    usbn_write(TXD2, i);

                    // Waiting for ACK state from PC Host
                    endpoint_stat[2].FIFO_status = WAIT_FOR_ACK;

                    //
                    // Enable TX for EP1 (TX FIFO2) - includes DATAPID
                    i = endpoint_stat[2].toggle_bit << 2;
                    usbn_write(TXC2, TX_EN | TX_LAST | i);
                    continue;
                } // if (endpoint_stat[2].FIFO_status == EMPTY) {
            } // if (StreamFlag) {
            
            continue;
        } // if (CommandBuffer[0] == 0) {

        // Flush TX FIFO1
        usbn_write(TXC1, FLUSH);

        i = strlen(CommandBuffer);                  // Used to check correct command length

        switch (CommandBuffer[0]) {
            case 'V':
                //
                // Version string
                //
                if (i != 1)                         // Check for correct command length
                    goto error_rsp;
                strcpy(ResponseBuffer, "V14");
                break;

            case 'I':
                //
                // Input digital port
                //
                if (i != 1)                         // Check for correct command length
                    goto error_rsp;
                ResponseBuffer[0] = 'I';
                i = read_port1();
                bin_to_hex(i, &ResponseBuffer[1]);
                j = read_port2();
                bin_to_hex(j, &ResponseBuffer[3]);
                ResponseBuffer[5] = 0;              // Null terminate
                break;

            case 'O':
                //
                // Output digital port
                //
                if (i != 5)                         // Check for correct command length
                    goto error_rsp;
                i = hex_to_bin(&CommandBuffer[1]);  // PORT1
                write_port1(i);
                j = hex_to_bin(&CommandBuffer[3]);  // PORT2
                write_port2(j);
                strcpy(ResponseBuffer, "O");
                break;

            case 'T':
                //
                // Set digital port direction
                //
                if (i != 5)                         // Check for correct command length
                    goto error_rsp;
                i = hex_to_bin(&CommandBuffer[1]);  // PORT1
                set_dir_port1(i);
                j = hex_to_bin(&CommandBuffer[3]);  // PORT2
                set_dir_port2(j);
                strcpy(ResponseBuffer, "T");
                break;

            case 'G':
                //
                // Get digital port direction
                //
                if (i != 1)                         // Check for correct command length
                    goto error_rsp;
                ResponseBuffer[0] = 'G';
                i = get_dir_port1();
                bin_to_hex(i, &ResponseBuffer[1]);
                j = get_dir_port2();
                bin_to_hex(j, &ResponseBuffer[3]);
                ResponseBuffer[5] = 0;              // Null terminate
                break;

            case 'N':
                //
                // Get pulse counter
                //
                if (i != 1)                         // Check for correct command length
                    goto error_rsp;
                ResponseBuffer[0] = 'N';

                i = ((PulseCount & 0xFF000000) >> 24);
                bin_to_hex(i, &ResponseBuffer[1]);

                i = ((PulseCount & 0xFF0000) >> 16);
                bin_to_hex(i, &ResponseBuffer[3]);

                i = ((PulseCount & 0xFF00) >> 8);
                bin_to_hex(i, &ResponseBuffer[5]);

                j = PulseCount & 0xFF;
                i = get_timer0();
                i = i + j;
                bin_to_hex(i, &ResponseBuffer[7]);

                ResponseBuffer[9] = 0;              // Null terminate
                break;

            case 'M':
                //
                // Clear pulse counter
                //
                if (i != 1)                         // Check for correct command length
                    goto error_rsp;
                set_timer0(0);                      // Clear Timer0 and the count
                PulseCount = 0;
                strcpy(ResponseBuffer, "M");
                break;

            case 'Q':
                //
                // Bipolar A/D sample
                //
                if (i != 2)                         // Check for correct command length
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
                ResponseBuffer[5] = 0;              // Null terminate
                break;

            case 'U':
                //
                // Unipolar A/D sample
                //
                if (i != 2)                         // Check for correct command length
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
                ResponseBuffer[5] = 0;              // Null terminate
                break;

            case 'L':
                //
                // Output D/A value
                //
                if (i != 5)                         // Check for correct command length
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
                strcpy(ResponseBuffer, "L");
                break;

            case 'W':
                //
                // Write EEPROM
                //
                if (i != 5)                         // Check for correct command length
                    goto error_rsp;
                i = hex_to_bin(&CommandBuffer[1]);  // EEPROM Address
                j = hex_to_bin(&CommandBuffer[3]);  // EEPROM Value
                write_eeprom(i, j);

                //
                // Get new EEPROM settings
                //
                update_eesettings();

                strcpy(ResponseBuffer, "W");
                break;

            case 'R':
                //
                // Read EEPROM
                //
                if (i != 3)                         // Check for correct command length
                    goto error_rsp;
                i = hex_to_bin(&CommandBuffer[1]);  // EEPROM Address
                j = read_eeprom(i);
                strcpy(ResponseBuffer, "R");
                bin_to_hex(j, &ResponseBuffer[1]);
                ResponseBuffer[3] = 0;              // Null terminate
                break;

            case 'S':
                //
                // Start stream mode
                //
                if (i != 1)                         // Check for correct command length
                    goto error_rsp;
                StreamFlag = 1;
                strcpy(ResponseBuffer, "S");
                break;

            case 'H':
                //
                // Halt stream mode
                //
                if (i != 1)                         // Check for correct command length
                    goto error_rsp;
                StreamFlag = 0;
                strcpy(ResponseBuffer, "H");
                break;

            default:
                //
                // Unknown command
                //
error_rsp:      // ERROR response
                strcpy(ResponseBuffer, "X");
                break;

        } // switch (CommandChar)

        //
        // Clear the command buffer for next receive
        //
        for (i=0; i<8; i++) {
            CommandBuffer[i] = 0;
        }

        //
        // Write string to the TX FIFO1
        //
        j = strlen(ResponseBuffer);
        for (i=0; i<j; i++) {
            usbn_write(TXD1, ResponseBuffer[i]);
        }

        // Enable TX for EP1 (TX FIFO1) - includes DATAPID
        i = endpoint_stat[1].toggle_bit << 2;
        usbn_write(TXC1, TX_EN | TX_LAST | i);

        // Re-enable RX
        usbn_write(RXC1,FLUSH);     // Flush  RX1
        usbn_write(RXC1,RX_EN);     // Enable command receive (EP2)
    }
}
