//*****************************************************************************
//
// USBM400 Firmware
//
// Target: PIC18F6520
//
// Comments:
// ----------
// Based on the original USBM400 firmware
//

#include <18f6520.h>

// Define for ICD debugging
// #device ICD=TRUE

// Establish the clock
#use delay(clock=40000000)

// BUGBUG Change these fuses in release!
// --> Set for debug
// #fuses H4, NOWDT, NOCPB, NOCPD, NOLVP, BROWNOUT, BORV42, PUT, DEBUG
// --> Set for release
#fuses H4, NOWDT, NOCPB, NOCPD, NOLVP, BROWNOUT, BORV42, PUT


// Turn off the compiler's caching (virtual port) auto data direction crap
#use fast_io(A)
#use fast_io(B)
#use fast_io(C)
#use fast_io(D)
#use fast_io(E) 
#use fast_io(F)
#use fast_io(G)

// Memory (port) defines so we can use ports directly in 'C'
// e.g.   PORT_B = value;
#byte PORT_A=0xF80
#byte PORT_B=0xF81
#byte PORT_C=0xF82
#byte PORT_D=0xF83
#byte PORT_E=0xF84
#byte PORT_F=0xF85
#byte PORT_G=0xF86
#byte SSPSTAT=0xFC7
#byte SSPCON1=0xFC6
#byte SSPCON2=0xFC5
#byte EEADRH=0xFAA
#byte EEADR=0xFA9
#byte EEDATA=0xFA8
#byte EECON1=0xFA6
#byte EECON2=0xFA7
#byte INTCON=0xFF2


// EEPROM Locations
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

#define EEPROM_AD_CONFIG        0x11
#define EEPROM_AD_CONFIG_END    0x21

// Locations contain ASCII serial number, which will be converted to
// UNICODE when it is read in update_eesettings()
#define EEPROM_SN_START         0x30
#define EEPROM_SN_END           0x38

// Chip select and control lines
#define AD_CS           PIN_A0
#define DAC_CS          PIN_A1
#define USB_CS          PIN_A2
#define USB_RD          PIN_B2
#define USB_WR          PIN_B3
#define USB_A0          PIN_B4
#define USB_RESET       PIN_C1

// LED defines
#define LED_RED         PIN_G3
#define LED_GREEN       PIN_G4

// Current SPI mode defines
#define SPI_NOT_SET     0
#define SPI_ADC_FAST    2
#define SPI_DAC         2
#define SPI_ADC_SLOW    3


// Global variable declarations
#define EXPANDER_FLAG   1

int8    EEFlags = 0;            // EXPANDER)FLAG (invert digital), ...

int8    Adc_Control[16];        // EEPROM locations 0x11 to 0x20

int8    SpiMode = SPI_NOT_SET;  // Guarantees that setup_spi_mode() will be called the first time.

int8    StreamFlag = 0;          // Initially not streaming
int8    bWaitingTxFifoEmpty = 0; // Initially not waiting for USB TX FIFO 2 empty

int8    LedGreenFlag = 1;       // Controls the LED operation
int8    LedCount = 0;

int8    Port1_Dir = 0;          // Digital port direction (shadow)
int8    Port2_Dir = 0;
int16   DAC_CH0 = 0;            // DAC output (shadow)
int16   DAC_CH1 = 0;

int32   PulseCount = 0;         // Count the incoming pulses

int16   ADC_Buffer[32];         // A/D buffer for Streaming mode

char    CommandBuffer[8];       // Command and response buffers
char    ResponseBuffer[10];

// Constant table definitions
const int8 BIN_TO_HEX_TBL[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

// Forward function declarations
#separate
void trigger_led_red(void);
void led_on_green(void);

// Additional includes
#include "string.h"


//-----------------------------------------------------------------------------
// setup_spi_fast()
//
// Used to configure the PIC SPI correctly since the built-in
// CCS routines are broken. CLK = Fosc / 16 = 2.5 Mhz
//
void setup_spi_fast(void)
{
//    SSPCON1 = 0x21;     // SSPCON1: SPEN=1, CKP=0 (idle high), Fosc/16
    SSPCON1 = 0x20;     // SSPCON1: SPEN=1, CKP=0 (idle high), Fosc/4
    SSPSTAT = 0xC0;     // SSPSTAT: SMP=1 (input sample at end of data output time)
                        //          CKE=1 (CKP=1 data transmitted on falling edge of SCK)
    return;
}


//-----------------------------------------------------------------------------
// setup_spi_clk64()
//
// Used to configure the PIC SPI correctly since the built-in
// CCS routines are broken. CLK = Fosc / 64 = 625 Khz
//
void setup_spi_clk64(void)
{
    SSPCON1 = 0x22;     // SSPCON1: SPEN=1, CKP=0 (idle high), Fosc/64
    SSPSTAT = 0xC0;     // SSPSTAT: SMP=1 (input sample at end of data output time)
                        //          CKE=1 (CKP=1 data transmitted on falling edge of SCK)
    return;
}

//-----------------------------------------------------------------------------
// setup_spi_mode()
//
// Used to setup the speed of the SPI clock depending on usage
//
void setup_spi_mode(int8 mode)
{
    // SPI_ADC_FAST, SPI_DAC
    if (mode == SPI_ADC_FAST) {
        setup_spi_fast();
        SpiMode = SPI_ADC_FAST;
        return;
    }

    // SPI_DAC
    setup_spi_clk64();
    SpiMode = SPI_ADC_SLOW;
    return;
}

//------------------------------------------------------------------------------
// Include the USB code. Note that the USBN9603 uses the SPI port
#include <usbn9603.c>

//-----------------------------------------------------------------------------
// Read EEPROM
// The PIC18F6520 has an internal EEPROM
//
#separate
int8 rd_eeprom(int8 address)
{
    int8    data;

    EEADRH = 0;         // Upper bits of 10 bit address = 0
    EEADR = address;    // Lower bits of 10 bit address = address
    EECON1 = 0x00;      // Clear EEPGD and CFGS (access EEPROM)
    EECON1 = 0x01;      // Initiate read RD=1
    data = EEDATA;      // Read the data out of the EEPROM
    return (data);
}

//-----------------------------------------------------------------------------
// Write EEPROM
// The PIC18F6520 has an internal EEPROM
//
#separate
void wr_eeprom(int8 address, int8 data)
{
    int8 i;
    
    // Wait for any previous WRITE to complete
    do {
        delay_cycles(5);
        i = EECON1;
    } while (i & 0x02);         // Bit 1 = WR (write status)

    EEADRH = 0;         // Upper bits of 10 bit address = 0
    EEADR = address;    // Lower bits of 10 bit address = address
    EEDATA = data;      // Data to write to the EEPROM
    EECON1 = 0x00;      // Clear EEPGD and CFGS (access EEPROM)
    EECON1 = 0x04;      // Set write enable - WREN

    bit_clear(INTCON, 7); // bcf     0x0B, 7     ; Disable INT's INTCON GIE
    // Do not change a single line in the following #asm block
    // The assembly output must match the Microchip documentation exactly
    // and changing anything will screw it up since the compiler shoves
    // its crap in-line also.
#asm
    movlw   0x55        ; Write unlock sequence
    movwf   EECON2      ; EECON2
    movlw   0xAA
    movwf   EECON2      ; EECON2
    bsf     EECON1, 1   ; Initiate write - WR
#endasm

    bit_set(INTCON, 7);// bsf     0x0B, 7     ; enable INT's - INTCON GIE
    EECON1 = 0x00;      // WREN
    return;
}

//-----------------------------------------------------------------------------
// Output an updated DAC value in LTC1448
//
// DAC output values in DAC_CH0 and DAC_CH1. These 12 bit values need to be put
// together into 3 bytes (24 bits) for the DAC output update sequence.
//
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
// Acquire an A/D value from ADS7871
//
int16 acquire_adc(int8 control)
{
    int16   ad_sample = 0;
    int8    data;

    // Check and configure SPI if needed
    if (SpiMode != SPI_ADC_FAST)
        setup_spi_mode(SPI_ADC_FAST);

    output_low(AD_CS);          // Select the A/D converter

    data = control | 0x80;      // OR in Direct Mode (Start Conversion) in bit 7
    spi_write(data);

    while (PORT_B & 0x02)       // Wait for the conversion to complete (BUSY = 0)
    {
        ad_sample++;
    }
    
    // 8 bit read = (0x40 | Addr)
    spi_write(0x40 | 0x01);
    data = spi_read(0);         // A/D Sample Bits 11-4
    ad_sample = data;           // To get around shitty type conversion
    ad_sample = (ad_sample << 4);
    spi_write(0x40 | 0x00);
    data = spi_read(0);         // A/D Sample Bits 3-0
    // TODO: Check for OVR (Bit 0)
    ad_sample |= (data >> 4);
    output_high(AD_CS);         // De-Select the A/D converter
    return (ad_sample);
}

//-----------------------------------------------------------------------------
// config_adc(void)
//
// Configures the A/D converter for usage
//
void config_adc(void)
{
    int8    data;

    // Check and set SPI mode
    if (SpiMode != SPI_ADC_FAST)
        setup_spi_mode(SPI_ADC_FAST);

    output_low(AD_CS);          // Select the A/D converter

    // 8 bit read = (0x40 | Addr)

    spi_write(0x40 | 31);       // Read the ID register
    data = spi_read(0);

    // Check if the chip has a valid ID
// BUGBUG: Removed since a Digital only unit does not have a ADS7871
//    if (data != 1)
//    {
//        while (TRUE)
//        {
//            restart_wdt();
//            led_on_green();
//        }
//    }

    // 8 bit write = (0x00 | Addr)

    // Configre the Buffer driver (turn on) Reg Addr 7, Bit 2 (ON)
    spi_write(0x00 | 7);
    spi_write(0x04);

    output_high(AD_CS);         // De-Select the A/D converter

    return;
}

//-----------------------------------------------------------------------------
// Turn LED on (RED)
//
void led_on_red(void)
{
    output_high(LED_RED);
    output_low(LED_GREEN);
    return;
}

//-----------------------------------------------------------------------------
// Turn LED on (GREEN)
//
void led_on_green(void)
{
    output_high(LED_GREEN);
    output_low(LED_RED);
    return;
}

//-----------------------------------------------------------------------------
// Turn LED off
//
void led_off(void)
{
    output_low(LED_RED);
    output_low(LED_GREEN);
    return;
}

//-----------------------------------------------------------------------------
// Turn LED on (RED) only if it was previously GREEN
//
#separate
void trigger_led_red(void)
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
// PORT_E
//
#separate
void write_port1(int8 value)
{
    // Expander inversion
    if (EEFlags & EXPANDER_FLAG)
        value = value ^ 0xFF;

    // Write port data out
    PORT_E = value;
    return;
}

//-----------------------------------------------------------------------------
// Read the digital input of PORT1
// PORT_E
//
#separate
int8 read_port1(void)
{
    int8 value;

    // Read port data in
    value = PORT_E;

    // Expander inversion
    if (EEFlags & EXPANDER_FLAG)
        value = value ^ 0xFF;
    return (value);
}

//-----------------------------------------------------------------------------
// Write the digital output to PORT2
// PORT_F
//
void write_port2(int8 value)
{
    // Expander inversion
    if (EEFlags & EXPANDER_FLAG)
        value = value ^ 0xFF;
    // Write port data out
    PORT_F = value;
    return;
}

//-----------------------------------------------------------------------------
// Read the digital input of PORT2
// PORT_F
//
int8 read_port2(void)
{
    int8 value;

    // Read port data in
    value = PORT_F;

    // Expander inversion
    if (EEFlags & EXPANDER_FLAG)
        value = value ^ 0xFF;
    return (value);
}

//-----------------------------------------------------------------------------
// Set the data direction of PORT1
// PORT_E
//
void set_dir_port1(int8 direction)
{
    Port1_Dir = direction;
    // Save to EEPROM
    wr_eeprom(EEPROM_DIR_PORT1, Port1_Dir);
    set_tris_e(direction);
    return;
}

//-----------------------------------------------------------------------------
// Returns the data line direction for Port1.
// Per digital line (bit) 1=IN and 0=OUT
// 
int8 get_dir_port1(void)
{
    return (Port1_Dir);
}

//-----------------------------------------------------------------------------
// Set the data direction of PORT2
// PORT_F
//
void set_dir_port2(int8 direction)
{
    Port2_Dir = direction;
    // Save to EEPROM
    wr_eeprom(EEPROM_DIR_PORT2, Port2_Dir);
    set_tris_f(Port2_Dir);
    return;
}

//-----------------------------------------------------------------------------
// Returns the data line direction for Port2.
// Per digital line (bit) 1=IN and 0=OUT
// 
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
// (40Mhz Clk / 4) = 10 Mhz source to Timer1
// 10ms = 65536-(.01/(1/(10000000/4))) = 0x9E58
    set_timer1(0x9E58);

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
    int8 i, j;

    //
    // Read flags out of EEPROM
    // 1) Expander flag
    //
    i = rd_eeprom(EEPROM_EXPANDER_FLAG);
    if (i) {
        EEFlags = EEFLags | EXPANDER_FLAG;
    } else {
        EEFlags = EEFLags & (~EXPANDER_FLAG);
    }

    // A/D Control nibble. In EEPROM locations 0x11 to 0x18
    for (i=0; i<16; i++) {
        Adc_Control[i] = rd_eeprom(EEPROM_AD_CONFIG+i);
    }
    
    // Get the USB serial number out of the EEPROM
    // First, copy the ROM structure into RAM structure
    for (i=0; i<USB_STRING_SN_LEN; i++) {
      USB_STRING_SN_RAM[i] = USB_STRING_SN[i];
    }
    // First two (2) bytes of a USB string are length and type, 
    // and we've already copied those above. We are going to copy
    // just the ASCII string since USB_STRING_SN[] already contains
    // the UNICODE 0. This means copying every-other byte into USB_STRING_SN_RAM[]
    j = 2;
    for (i=EEPROM_SN_START; i<EEPROM_SN_END; i++) {
      USB_STRING_SN_RAM[j] = rd_eeprom(i);
      // If the EEPROM hasn't been programmed, all values = 0xFF. So default to S/N 0000000
      if (USB_STRING_SN_RAM[j] == 0xFF) 
        USB_STRING_SN_RAM[j] = '0';
      j += 2;
    }
    return;
}

//-----------------------------------------------------------------------------
// Main program entry point.
//
void main(void) {

    int8    i, j, k, x;
    int16   temp;

    // Setup the built-int ADC. The enables all PORT_A to general purpose I/O
    setup_adc_ports(NO_ANALOGS);
    setup_adc(ADC_OFF);

    // Setup the remaining CCS peripherals
    setup_psp(PSP_DISABLED);
    setup_spi(FALSE);
    setup_wdt(WDT_OFF);
    setup_timer_0(RTCC_INTERNAL);
    setup_timer_1(T1_DISABLED);
    setup_timer_2(T2_DISABLED,0,1);
    setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
    setup_timer_4(T4_DISABLED,0,1);
    setup_comparator(NC_NC_NC_NC);
    setup_vref(FALSE);

    //
    // Setup the TRIS registers and initial port outputs
    //
    set_tris_a(0x10);   // A0-3=O, A4=I, A5=O

    // Set the chip select outputs on PORT_A
    output_high(AD_CS);
    output_high(DAC_CS);
    output_high(USB_CS);

    // Set the remaining data direction registers.
    set_tris_b(0xE3);   // B0-1=I, B2-4=O, B5-7=I
    set_tris_c(0xD0);   // C0-3=O, C4=I, C5=0, C6=I, C7=I
    set_tris_d(0xFF);   // D0-7=I (used by USBN9603)

    set_tris_e(0xFF);   // PORT1
    set_tris_f(0xFF);   // PORT2

    set_tris_g(0x00);   // G0-4=O

    // Set the remaining select and control lines
    output_high(USB_RD);
    output_high(USB_WR);
    output_high(USB_RESET);

    // Get EEPROM settings
    update_eesettings();

//    while (TRUE) {
//        temp = acquire_adc(8, 1);
//    }

    //
    // Set Port data direction from EEPROM
    //
    i = rd_eeprom(EEPROM_DIR_PORT1);
    set_dir_port1(i);
    i = rd_eeprom(EEPROM_DIR_PORT2);
    set_dir_port2(i);

    //
    // Output power on default for I/O ports from EEPROM
    //
    i = rd_eeprom(EEPROM_POD_PORT1);
    write_port1(i);
    i = rd_eeprom(EEPROM_POD_PORT2);
    write_port2(i);

    //
    // Output power on default for D/A converter
    //
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

    update_dac();                       // Send the new values to the D/A converter

    // Configure and setup the ADS7871 A/D
    config_adc();

    // Clear the command buffer
    for (i=0; i<8; i++) {
        CommandBuffer[i] = 0;
    }

    // Get the version out of the USB chip
    i = usbn_get_version();

    // USBN9603 and USBN9604 rev = 0x02
    if (i != 0x02) {
        // Sit here with a solid RED LED on
        while (TRUE)
        {
            restart_wdt();
            led_on_red();
        }
    }

    // Initialize the USB chip. Now ready to communicate
    usb_init();

    //
    // Initialize the other interrupts.
    // Note: enable_interrupts(global) is called in usb_init()
    //
    set_timer0(0);
    setup_timer_0(RTCC_EXT_H_TO_L | RTCC_DIV_1);
    enable_interrupts(int_timer0);              // Timer0 RTCC Pulse counter input
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_4);     // setup interrupts
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
        // Wait for an ASCII command character in the USB Command buffer
        if (CommandBuffer[0] == 0) {
        
            // Check stream mode
            if (StreamFlag) {
            
                // We have plenty of RAM, so we read A/D values into memory.
                // When the USB TX FIFO2 is empty, then we copy the A/D values 
                // into the USB FIFO. This sort of acts like a ping-pong-buffer

                // Is TX FIFO2 empty?
                if (!bWaitingTxFifoEmpty) {

                    // A/D data to TX FIFO2 = 64 bytes. 
                    // 2 sets of 16 samples of 16 bite seach (saved BINARY)
                    i = 0;
                    for (k=0; k<2; k++) 
                    {
                        // Initial sample to flush out previous sample in A/D
                        x = Adc_Control[0];
                        temp = acquire_adc(x);
                        
                        for (j=1; j<17; j++)
                        {
                            if (j >= 16) 
                                x = Adc_Control[15];
                            else 
                                x = Adc_Control[j];
                                
                            // turn off interrupts to grant exclusive SPI acess
                            disable_interrupts(global);
                            
                            // A/D Conversion
                            ADC_Buffer[i] = acquire_adc(x);
                            
                            // turn on interrupts
                            enable_interrupts(global);
                            i++;
                        }
                    }
                    // We've copied the A/D values into memory. Now wait until
                    // TX FIFO2 is empty before we copy data and send out USB.
                    bWaitingTxFifoEmpty = 1;
                } else {    // if (!bWaitingTxFifoEmpty) {
                    // bWaitingTxFifoEmpty = 1
                    // When TX FIFO2 is empty, then copy data into buffer
                    if (endpoint_stat[2].FIFO_status == EMPTY) {
                    
                        // Copy the data from ADC_Buffer[] into TX FIFO2
                        // 32 samples of 16 bits each = 64 bytes
                        for (i=0; i<32; i++) {
                            // Little endian order for Intel PC
                            j = (ADC_Buffer[i] & 0x00FF);
                            usbn_write(TXD2, j);
    
                            j = (ADC_Buffer[i] & 0xFF00) >> 8;
                            usbn_write(TXD2, j);                        
                        }
                        
                        // Waiting for ACK state from PC Host
                        endpoint_stat[2].FIFO_status = WAIT_FOR_ACK;
    
                        // Enable TX for EP1 (TX FIFO2) - includes DATAPID
                        i = endpoint_stat[2].toggle_bit << 2;
                        usbn_write(TXC2, TX_EN | TX_LAST | i);
                        bWaitingTxFifoEmpty = 0;
                    } // if (endpoint_stat[2].FIFO_status == EMPTY) {
                } 
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
                strcpy(ResponseBuffer, "V15");
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

            case 'U':
                //
                // Unipolar A/D sample
                //
                if (i != 3)                         // Check for correct command length
                    goto error_rsp;
                i = hex_to_bin(&CommandBuffer[1]);  // Control byte
                temp = acquire_adc(i);              // Flush out the previous conversion
                temp = acquire_adc(i);              // Get the current (new) conversion results
                ResponseBuffer[0] = 'U';
                bin_to_hex(i, &ResponseBuffer[1]);  // Control byte
                j = ((temp & 0xFF00) >> 8);
                bin_to_hex(j, &ResponseBuffer[3]);  // MSB of ADC value
                j = (temp & 0x00FF);
                bin_to_hex(j, &ResponseBuffer[5]);  // LSB of ADC value
                ResponseBuffer[7] = 0;              // Null terminate
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
                wr_eeprom(i, j);

                // Get new EEPROM settings
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
                j = rd_eeprom(i);
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

        // Clear the command buffer for next receive
        for (i=0; i<8; i++) {
            CommandBuffer[i] = 0;
        }

        // Write response string to the TX FIFO1
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
    } // While (TRUE)
    
    // We should never get here
    return;
}
