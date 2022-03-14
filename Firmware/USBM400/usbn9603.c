//*****************************************************************************
//
// USBN9603 USB Controller firmware
//
// Target: Designed for USBM400 board
//
// Comments:
// ----------
// Based on the original USBM300 firmware
//
//

//
// USB Device desriptor defines
//
#ifndef __USB_DESCRIPTORS__
   #include "USBM400_desc.h"
#endif


#define USB_MAX_ENDPOINTS 5     // number of bidrectional endpoints.  960x has 1 control port and 6 individual endpoints = 4 bidrectional endpoints

//
// USBN9603 register defines
//
#include "usbn9603_reg.h"

//
// Forward function declarations
//
void usb_init(void);
int8 usbn_get_version(void);

//
// ISR prototypes and associated handlers
//
void usb_isr(void);
void handle_warn(void);
void handle_alt(void);
void handle_txev(void);
void handle_frame(void);
void handle_uld(void);
void handle_rxev(void);

//
// Functions that change with SPI vs. Memory Mapped interfaces
//
void usbn_write(int8 address, int8 data);
int8 usbn_read(int8 address);

//
// USBN9603 endpoints           EP Addressing
// -----------------------------------------------
// EP0  TX/RX   8 byte FIFO
// EP1  TX      64 byte FIFO    bulk 1          WDM "PIPE0"
// EP2  RX      64 byte FIFO    bulk 2          WDM "PIPE1"
// EP3  TX      64 byte FIFO    not used
// EP4  RX      64 byte FIFO    not used
// EP5  TX      64 byte FIFO    not used
// EP6  RX      64 byte FIFO    not used

// BUGBUG: AAK these variables are not used (2-9-2008)
//DONT CHANGE THE FOLLOWING 3 CONSTANT ARRAYS
// const char EPCx[]=    {EPC0, EPC1, EPC2, EPC3, EPC4, EPC5, EPC6};   // epc_address[x] finds the constant EPCx
// const char TX_FIFOx[]={TXD0, TXD1, 0,    TXD2, 0,    TXD3, 0};      // ecp0,ecp1,ecp3,ecp5 have tx_fifos
// const char RX_FIFOx[]={RXD0, 0,    RXD1, 0,    RXD2, 0,    RXD3};   // ecp0,ecp2,ecp4,ecp6 have rx_fifos


/// BEGIN User Functions

int8 ep0_rx_buffer[USB_MAX_EP0_PACKET_LENGTH];
int8 ep0_tx_buffer[USB_MAX_EP0_PACKET_LENGTH];


//
// Types of Xfer's supported in control EP0
//
#define     NO_XFER         0
#define     DEVICE_DESC     1
#define     CONFIG_DESC     2
#define     STRING_DESC     3
#define     ZERO_LEN_XFER   4


struct {
    int8    nXferType;
    int8    nCurPosition;
    int8    nLength;
    int8    nStrIndex;
} XferEP0;


struct {
    int8    toggle_bit;
    int8    FIFO_status;
} endpoint_stat[USB_MAX_ENDPOINTS];


typedef enum {
      EMPTY,
      FULL,
      WAIT_FOR_ACK
} FIFO_status_t;


//
//
int8 alt_mask;
int8 setaddress = 0;

// Bits 0-6 correspond to each endpoint. Bit set = stalled
// Written by Set/Clear Feature, Read by Get Status
int8 stalld;

// Current USB configuration (only 1 here)
int8 usb_cfg;

void endpoint_status_init(void)
{
   int8 i;

   for(i=0; i<USB_MAX_ENDPOINTS; i++)
    {
        endpoint_stat[i].FIFO_status = EMPTY;
        if (i == 0)
        {
            //
            // control endpoint - EP0
            //
            endpoint_stat[i].toggle_bit = 0x01;
        } else {
            //
            // tx & rx data endpoints
            //
            endpoint_stat[i].toggle_bit = 0x00;
        }
   }
    return;
}


void reset_usb(void)
{
   usbn_write(EPC0, 0x00);                 // Default addres 0x00 for EP0
    usbn_write(FAR, 0x00);

//   usbn_write(FAR, AD_EN | 0x00); // Default device addres in FAR
//   usbn_write(FAR, usbn_read(FAR) | AD_EN);// Enable FAR addressing

    usbn_write(TXC0, FLUSH);         // flush fifos
    usbn_write(TXC1, FLUSH);
    usbn_write(TXC2, FLUSH);
    usbn_write(TXC3, FLUSH);
    usbn_write(RXC0, FLUSH);
    usbn_write(RXC1, FLUSH);
    usbn_write(RXC2, FLUSH);
    usbn_write(RXC3, FLUSH);

//    usbn_write(EPC0, 0x40);        //  Default address on EP0

//    memset(ep0_rx_buffer, 0, USB_MAX_EP0_PACKET_LENGTH);
//    memset(ep0_tx_buffer, 0, USB_MAX_EP0_PACKET_LENGTH);
   endpoint_status_init();

   // Enable the receiver - EP0
   usbn_write(RXC0, RX_EN);

    return;
}


/*******************************************************************************
/* usb_init()
/*
/* Summary: Resets and initalizes USB code and USBN device.  You must call this
/*          first before using code.
/*
/*          NOTE: this enables interrupts.
/*
/********************************************************************************/
void usb_init(void)
{
    usb_cfg = 0;

    usbn_write(MCNTRL, SRST);               // Force the chip to RESET
    while (usbn_read(MCNTRL) & SRST);       // and wait for reset to complete

    usbn_write(CCONF, 0x80);                // Disable clock output

    usbn_write(MCNTRL, INT_L_P | VGE);      // Active low interrupts + VGE

    usbn_write(MAMSK, 0x00);                // Mask all interrupt events
    usbn_write(NAKMSK, NAK_OUT0);               // NAK mask
    usbn_write(TXMSK, TX_FIFO0 | TX_FIFO1 | TX_FIFO2 | TX_FIFO3);     // TX mask  | TX_FIFO2 | TX_FIFO3
    usbn_write(RXMSK, RX_FIFO0 | RX_FIFO1);     // RX mask  | RX_FIFO2 | RX_FIFO3
    alt_mask = ALT_SD3 | ALT_RESET;
    usbn_write(ALTMSK, alt_mask);               // ALT mask

    usbn_write(MAMSK, INTR_E | RX_EV | NAK | TX_EV | ALT);  // Enable node interrupts

    usbn_write(WKUP,0x0C);

    usbn_write(0x1F,0x40);                          // 3.3V regulator work around

//    usbn_write(FWMSK, FWMSK_ENABLES);

    reset_usb();                                    // Reset USB interface

    ext_int_edge(H_TO_L);                           // Enable the PIC interrupts
    enable_interrupts(int_ext);
    enable_interrupts(global);

    delay_ms(5);                                    // Wait for the 3.3 V regulator to stabilize
    usbn_write(FAR, AD_EN);
    usbn_write(EPC0, 0);
    usbn_write(NFSR, OPR_ST);                       // Go operational
    usbn_write(RXC0, RX_EN);
    usbn_write(MCNTRL, usbn_read(MCNTRL) | NAT);    // Attach node
    return;
}




/**********************************************************************/
/* Transmit on USB endpoint 0                                         */
/*   Must setup structure XP0 before calling this function            */
/**********************************************************************/
void XmitOnUSB0(void) {
   int8    i = 0;
   int8    j;

    //
    //
    j = XferEP0.nCurPosition;
    if (j > 0x80) {
      i = 9;
    }

//    memset(ep0_tx_buffer, 0, 8);

   //switch on descriptor type in location 3 (high order of wValue)
   switch (XferEP0.nXferType){
        case NO_XFER:                   // No xfer - ERROR!
            i = 99;
            break;
      case DEVICE_DESC:               // DEVICE descriptor
           // Load the buffer
            for (i=0; i<8; i++) {
                if (j > USB_DEVICE_DESC_LEN)
                   i = 9;

                ep0_tx_buffer[i] = USB_DEVICE_DESC[j];
                j++;
                XferEP0.nLength--;
                if(XferEP0.nLength == 0) {
                    XferEP0.nXferType = ZERO_LEN_XFER;  // Finish up with a zero length Xfer
                    i++;
                    break;
                }
            }
         break;
      case CONFIG_DESC:               // CONFIGURATION descriptor includes INTERFACE and ENDPOINT descriptors
            // Load the buffer
            for (i=0; i<8; i++) {
                if (j > USB_CONFIG_DESC_TOTAL_LEN)
                    i = 9;

                ep0_tx_buffer[i] = USB_CONFIG_DESC[j];
                j++;
                XferEP0.nLength--;
                if(XferEP0.nLength == 0) {
                    XferEP0.nXferType = ZERO_LEN_XFER;  // Finish up with a zero length Xfer
                    i++;
                    break;
                }
            }
         break;
        case STRING_DESC:               // STRING descriptor
            //
            // NOTE: AAK  - Strings were hard-coded, so I changed them to defined values.
            //              These need to match the index values found in the USB descriptor definitions.
            switch (XferEP0.nStrIndex ) {
                case STR_INDX_LANG:         // Language ID string
                    // Load the buffer
                    for (i=0; i<8; i++)  {
                        ep0_tx_buffer[i] = USB_STRING_0[j];
                        j++;
                        XferEP0.nLength--;
                        if(XferEP0.nLength == 0) {
                            XferEP0.nXferType = ZERO_LEN_XFER;  // Finish up with a zero length Xfer
                            i++;
                            break;
                        }
                    }
                    break;
                case STR_INDX_MFG:          // Manufacturer string
                    // Load the buffer
                    for (i=0; i<8; i++) {
                        ep0_tx_buffer[i] = USB_STRING_1[j];
                        j++;
                        XferEP0.nLength--;
                        if(XferEP0.nLength == 0) {
                            XferEP0.nXferType = ZERO_LEN_XFER;  // Finish up with a zero length Xfer
                            i++;
                            break;
                        }
                    }
                    break;
                case STR_INDX_PROD:             // Product Description string
                    // Load the buffer
                    for (i=0; i<8; i++) {
                        ep0_tx_buffer[i] = USB_STRING_2[j];
                        j++;
                        XferEP0.nLength--;
                        if(XferEP0.nLength == 0) {
                            XferEP0.nXferType = ZERO_LEN_XFER;  // Finish up with a zero length Xfer
                            i++;
                            break;
                        }
                    }
                    break;
                case STR_INDX_SN:               // Serial Number string
                    // Load the buffer
                    for (i=0; i<8; i++) {
                        // BUGBUG: AAK - get S/N from EEPROM
                        ep0_tx_buffer[i] = USB_STRING_SN_RAM[j];
                        j++;
                        XferEP0.nLength--;
                        if(XferEP0.nLength == 0) {
                            XferEP0.nXferType = ZERO_LEN_XFER;  // Finish up with a zero length Xfer
                            i++;
                            break;
                        }
                    }
                    break;
                    
                default:                // Error - unknown string
                    break;
            }
            break;
        case ZERO_LEN_XFER:             // Zero length xfer - Sent after other descriptors are sent
            i = j = 0;
            break;
      default:
         break;
   }

    // Adjust current position for next write
    XferEP0.nCurPosition += i;
    for (j=0; j<i; j++) {
       usbn_write(TXD0, ep0_tx_buffer[j]);
    }

    return;
}


/**********************************************************************/
/* The GET_DESCRIPTOR request is done here                            */
/**********************************************************************/
void getdescriptor(int8 *buf) {

    // Set the information for sending this descriptor over multiple packets
    XferEP0.nCurPosition = 0;
    XferEP0.nXferType = buf[3];
    XferEP0.nStrIndex = buf[2];

   //switch on descriptor type in location 3 (high order of wValue)
   switch (XferEP0.nXferType){
      case DEVICE_DESC:               // DEVICE descriptor
         XferEP0.nLength = USB_DEVICE_DESC_LEN;
         break;
      case CONFIG_DESC:               // CONFIGURATION descriptor includes INTERFACE and ENDPOINT descriptors
         XferEP0.nLength = USB_CONFIG_DESC_TOTAL_LEN;
         break;
        case STRING_DESC:               // STRING descriptor
                                        // index in location 2 (low order of wValue)
            switch (XferEP0.nStrIndex) {
                case STR_INDX_LANG:         // Language ID string
                  XferEP0.nLength = USB_STRING_0_LEN;
                    break;
                case STR_INDX_MFG:          // Manufacturer string
                    XferEP0.nLength = USB_STRING_1_LEN;
                    break;
                case STR_INDX_PROD:         // Product Description string
                    XferEP0.nLength = USB_STRING_2_LEN;
                    break;
                case STR_INDX_SN:           // Serial Number 
                    XferEP0.nLength = USB_STRING_SN_LEN;
                    break;

                default:                // Error - unknown string
                    return;
            }
            break;
      default:
         return;
   }

    // Limit the number of bytes to the request length
    if(XferEP0.nLength > buf[6])
        XferEP0.nLength = buf[6];

   // Start to transmit
   XmitOnUSB0();
}

void clrfeature(void)
{
    switch (ep0_rx_buffer[0]&0x03)      // Find the request target
    {
        case 0:                         // DEVICE
            break;

        case 1:                         // INTERFACE
            break;

        case 2:                         // ENDPOINT
            //
            // Find the endpoint. Strip off direction bit
            //
            switch (ep0_rx_buffer[4]&0x0F)
            {
                case 0:
                    usbn_write(EPC0, usbn_read(EPC0) & ~STALL);     // CLEAR_STALL_EP0
                    bit_clear(stalld,0);
                    break;
                case 1:
                    usbn_write(EPC1, usbn_read(EPC1) & ~STALL);     // CLEAR_STALL_EP1
                    bit_clear(stalld,1);
                    break;
                case 2:
                    usbn_write(EPC2, usbn_read(EPC2) & ~STALL);     // CLEAR_STALL_EP2
                    bit_clear(stalld,2);
                    break;
                case 3:
                    usbn_write(EPC3, usbn_read(EPC3) & ~STALL);     // CLEAR_STALL_EP3
                    bit_clear(stalld,3);
                    break;
                case 4:
                    usbn_write(EPC4, usbn_read(EPC4) & ~STALL);     // CLEAR_STALL_EP4
                    bit_clear(stalld,4);
                    break;
                case 5:
                    usbn_write(EPC5, usbn_read(EPC5) & ~STALL);     // CLEAR_STALL_EP5
                    bit_clear(stalld,5);
                    break;
                case 6:
                    usbn_write(EPC6, usbn_read(EPC6) & ~STALL);     // CLEAR_STALL_EP6
                    bit_clear(stalld,6);
                    break;
                default:
                    break;
            }
            break;

        default:                        // UNDEFINED
            break;
    }
    return;
}

void setconfiguration(void)
{
    int8 i;

    usb_cfg = ep0_rx_buffer[2];         // Save off the configuration #

    if (ep0_rx_buffer[2] != 0)          // Non-zero = set the configuration
    {
        // First PID is DATA0
        // Don't muck with EP0
       for(i=1; i<USB_MAX_ENDPOINTS; i++)
        {
            endpoint_stat[i].toggle_bit = 0x00;
            endpoint_stat[i].FIFO_status = EMPTY;
        }

        stalld = 0;                     // Nothing stalled yet

        // Enable Pipe 0, EPC1, address 1
        usbn_write(TXC1,FLUSH);         // Flush  TX1 and disable
        usbn_write(EPC1,EP_EN+01);      // Enable EP1 at adr 1

        // Enable Pipe 1, EPC2, address 2
        usbn_write(RXC1,FLUSH);         // Flush  RX1 and disable
        usbn_write(EPC2,EP_EN+02);      // Enable EP2 at adr 2
        usbn_write(RXC1,RX_EN);         // Enable RX1

        // Enable Pipe 2, EPC3, address 3
        usbn_write(TXC2,FLUSH);         // Flush  TX1 and disable
        usbn_write(EPC3,EP_EN+03);      // Enable EP1 at adr 3

        // Enable Pipe 3, EPC5, address 4
        usbn_write(TXC3,FLUSH);         // Flush  TX1 and disable
        usbn_write(EPC5,EP_EN+04);      // Enable EP1 at adr 4

    } else {                            // Zero = unconfigure the device

        usbn_write(EPC1,0);             // Disable EP1
        usbn_write(EPC2,0);             // Disable EP2
        usbn_write(EPC3,0);             // Disable EP3
        usbn_write(EPC4,0);             // Disable EP4
        usbn_write(EPC5,0);             // Disable EP5
        usbn_write(EPC6,0);             // Disable EP6
    }
    return;
}


void setfeature(void)
{
    switch (ep0_rx_buffer[0]&0x03)      // Find the request target
    {
        case 0:                         // DEVICE
            break;

        case 1:                         // INTERFACE
            break;

        case 2:                         // ENDPOINT
            //
            // Find the endpoint. Strip off direction bit
            //
            switch (ep0_rx_buffer[4]&0x0F)
            {
                case 0:
                    // Control pipes must accept setup requests even if
                    // stalled. So we log the stall here, but don't actually do it
                    // usbn_write(EPC0, usbn_read(EPC0) | STALL);      // STALL EP0
                    bit_set(stalld,0);
                    break;
                case 1:
                    usbn_write(EPC1, usbn_read(EPC1) | STALL);      // STALL EP1
                    bit_set(stalld,1);
                    break;
                case 2:
                    usbn_write(EPC2, usbn_read(EPC2) | STALL);      // STALL EP2
                    bit_set(stalld,2);
                    break;
                case 3:
                    usbn_write(EPC3, usbn_read(EPC3) | STALL);      // STALL EP3
                    bit_set(stalld,3);
                    break;
                case 4:
                    usbn_write(EPC4, usbn_read(EPC4) | STALL);      // STALL EP4
                    bit_set(stalld,4);
                    break;
                case 5:
                    usbn_write(EPC5, usbn_read(EPC5) | STALL);      // STALL EP5
                    bit_set(stalld,5);
                    break;
                case 6:
                    usbn_write(EPC6, usbn_read(EPC6) | STALL);      // STALL EP6
                    bit_set(stalld,6);
                    break;
                default:
                    break;
            }
            break;

        default:                        // UNDEFINED
            break;
    }
    return;
}

void getstatus(void)
{
    switch (ep0_rx_buffer[0]&0x03)      // Find the request target
    {
        case 0:                         // DEVICE
            usbn_write(TXD0,0);         // First byte is reserved
            break;

        case 1:                         // INTERFACE
            usbn_write(TXD0,0);         // First byte is reserved
            break;

        case 2:                         // ENDPOINT
            //
            // Find the endpoint. Strip off direction bit
            //
            switch (ep0_rx_buffer[4]&0x0F)
            {
                case 0:
                    if (bit_test(stalld, 0))    // EP0
                        usbn_write(TXD0,1);
                    else
                        usbn_write(TXD0,0);
                    break;
                case 1:
                    if (bit_test(stalld, 1))    // EP1
                        usbn_write(TXD0,1);
                    else
                        usbn_write(TXD0,0);
                    break;
                case 2:
                    if (bit_test(stalld, 2))    // EP2
                        usbn_write(TXD0,1);
                    else
                        usbn_write(TXD0,0);
                    break;
                case 3:
                    if (bit_test(stalld, 3))    // EP3
                        usbn_write(TXD0,1);
                    else
                        usbn_write(TXD0,0);
                    break;
                case 4:
                    if (bit_test(stalld, 4))    // EP4
                        usbn_write(TXD0,1);
                    else
                        usbn_write(TXD0,0);
                    break;
                case 5:
                    if (bit_test(stalld, 5))    // EP5
                        usbn_write(TXD0,1);
                    else
                        usbn_write(TXD0,0);
                    break;
                case 6:
                    if (bit_test(stalld, 6))    // EP6
                        usbn_write(TXD0,1);
                    else
                        usbn_write(TXD0,0);
                    break;
                default:
                    break;
            }
            break;

        default:                        // UNDEFINED
            usbn_write(TXD0,0);         // First byte is reserved
            break;
    }
    usbn_write(TXD0,0);                 // Second byte is reserved
    return;
}

//
// #separate means not #inline
//
#separate
void handle_setup()
{
    int8 i;

    if ((ep0_rx_buffer[0] & 0x60) == 0x00)      // if a standard request
    {
        switch (ep0_rx_buffer[1]) {             // find request target

            case CLEAR_FEATURE:                 // CLEAR Feature (device = remote wakeip, endpoint = stall)
                i = 1;
                clrfeature();
                break;

            case GET_CONFIGURATION:
                i = 2;
                usbn_write(TXD0, usb_cfg);      // Only have one configuration - return value from setconfiguration()
                break;

            case GET_DESCRIPTOR:
                getdescriptor(ep0_rx_buffer);   // Process the descriptor request
                break;

            case GET_STATUS:                    // GET status (endpoint = stall)
                getstatus();
                break;

            case SET_ADDRESS:
                //
                // Save off the new address, but do not set until the TX handshake completes
                //
                setaddress = ep0_rx_buffer[2] | AD_EN;
                break;

            case SET_CONFIGURATION:
                i = 3;
                setconfiguration();
                break;

            case SET_FEATURE:                   // SET Feature (device = remote wakeip, endpoint = stall)
                i = 4;
                setfeature();
                break;

            default:
                i = 5;
                //unsupported standard req
                break;
           } // end switch
    } else {
        i =9;
    }

    // Start with TOGGLE = 1
    endpoint_stat[0].toggle_bit = 0x01;
    i = endpoint_stat[0].toggle_bit << 2;

    // the following is done for all setup packets.  Note that if
    // no data was stuffed into the FIFO, the result of the fol-
    // lowing will be a zero-length response.

    usbn_write(TXC0, TX_EN | i);   // enable the TX (DATA1) and toggle PID

    endpoint_stat[0].toggle_bit ^= 0x01;
    return;
}


void tx_1(void)
{
    int8    txstat;

    txstat = usbn_read(TXS1);       // Get transmitter status
    usbn_write(TXC1, FLUSH);        // Flush the FIFO
    //
    // If a transmission has completed successfully, update the data and toggle the PID
    //
    if ((txstat & ACK_STAT) && (txstat & TX_DONE)) {
        // This is where we know there's been a successful transmission to Host.
        // So we can re-enable RX1 for reception. Allowing new commands
        //
        endpoint_stat[1].toggle_bit ^= 0x01;    // Flip the PID bit on EP1
        endpoint_stat[1].FIFO_status = EMPTY;
        // usbn_write(RXC1,RX_EN);                 // Enable command receive (EP2)
    } else {
        //
        // TODO: Resend data
        endpoint_stat[1].FIFO_status = EMPTY;
        // usbn_write(RXC1,RX_EN);                 // Enable command receive (EP2)
    }
    return;
}

void tx_2(void)
{
    int8    txstat;

    txstat = usbn_read(TXS2);       // Get transmitter status
    usbn_write(TXC2, FLUSH);        // Flush the FIFO
    //
    // If a transmission has completed successfully, update the data and toggle the PID
    //
    if ((txstat & ACK_STAT) && (txstat & TX_DONE)) {
        // This is where we know there's been a successful transmission to Host.
        endpoint_stat[2].toggle_bit ^= 0x01;    // Flip the PID bit on EP2
        endpoint_stat[2].FIFO_status = EMPTY;
    } else {
        endpoint_stat[2].FIFO_status = EMPTY;
    }
    return;
}

void tx_3(void)
{
    int8    txstat;

    txstat = usbn_read(TXS3);       // Get transmitter status
    usbn_write(TXC3, FLUSH);        // Flush the FIFO
    //
    // If a transmission has completed successfully, update the data and toggle the PID
    //
    if ((txstat & ACK_STAT) && (txstat & TX_DONE)) {
        // This is where we know there's been a successful transmission to Host.
        // So we can re-enable RX1 for reception. Allowing new commands and/or data
        // to be received
        endpoint_stat[3].toggle_bit ^= 0x01;    // Flip the PID bit on EP3
        endpoint_stat[3].FIFO_status = EMPTY;
    } else {
        endpoint_stat[3].FIFO_status = EMPTY;
    }
    return;
}

void rx_1(void)
{
    int8    i, len, rxstatus, toggle;

    rxstatus = usbn_read(RXS1);     // Get the receiver status

//    if(rxstatus & SETUP_R) {        // We don't do setup packets on RX1
//        usbn_write(RXC1, FLUSH);    // Flush  RX1 and disable
//        return;
//    }

    if (rxstatus & RX_ERR) {        // If Error, flush fifo and re-enable, i.e. simply ignore it
        usbn_write(RXC1, FLUSH);    // Flush  RX1 and disable
        usbn_write(RXC1, RX_EN);    // Re-Enable receiving
        return;
    }

    len = rxstatus & 0x0F;      // Get data length

    if ((rxstatus & RX_LAST) && (len != 0)) {


        toggle = endpoint_stat[4].toggle_bit << 5;
        if ((rxstatus & RX_TOGL) == toggle) {

            endpoint_stat[4].toggle_bit ^= 0x01;
        } else {
            usbn_write(RXC1, FLUSH);    // Flush  RX1 and disable
            usbn_write(RXC1, RX_EN);    // Re-Enable receiving
            return;
        }
        //
        // Read the data out of the FIFO
        //
        for (i=0; i<len; i++)
        {
            if (len >= 8) {         // Cap at 8 (shovel the rest into the bit bucket)
                rxstatus = usbn_read(RXD1);
            } else {                // Stuff the data into the command buffer
                CommandBuffer[i] = usbn_read(RXD1);
            }
        }

        usbn_write(RXC1,FLUSH);     // Flush  RX1 and disable
        //
        // NOTE: Reciever will be re-enable after a response is sent
        //
    } else {
        //
        // Zero length packet.
        //
        usbn_write(RXC1, FLUSH);    // Flush  RX1 and disable
        usbn_write(RXC1, RX_EN);    // Re-Enable receiving
    }
    return;
}


/*******************************************************************************
/* usbn_get_version()
/*
/* Summary: Stictly for debugging, this function gets the revision number of the
/*          USBN960x you are connected to.  Good to see if you're connection
/*          to the USBN960x is good.
/*
/********************************************************************************/
int8 usbn_get_version(void) {
   return(usbn_read(RID));
}


/*******************************************************************************
/* usb_isr()
/*
/* Summary: Checks the Main Event interrupt register, and acts upon USB interrupts.
/*          ALT interrupt happens for the misc interrupts (reset, idle, wakeup, etc.)
/*          NAK interrupt happens when the USBN device generated a NAK to an IN or OUT/SETUP token
/*          TX interrupt happens when an IN token has happend (PIC -> HOST)
/*          RX interrupt happens when an OUT/SETUP token has happened. (HOST -> PIC).
/*          Majority of USB token handling is done through the RX event.
/*
/*          If you choose to use a polling method instead of by interrupt then call
/*          this function on any free time.  Use great care if you choose to poll:
/*          some USB requests require a response within a few milli-seconds.
/*
/********************************************************************************/
#int_ext NOCLEAR
void usb_isr(void)
{
    int8 usbn_event, i;

    // Clear the interrupt manually and immediately so another can come in
    // BUGBUG: Fix this
    bit_clear(*0xFF2, 1);

    //
    // Loop until all ISR events are handled
    //
    do {
        usbn_event = usbn_read(MAEV);

        if (usbn_event & RX_EV)          // RX_EV - OUT and SETUP tokens (data from HOST)
        {
            trigger_led_red();
            handle_rxev();
            continue;
        }

        if (usbn_event & TX_EV)        // TX_EV - IN token (DEVICE sending data)
        {
            trigger_led_red();
            handle_txev();
            continue;
        }

        if (usbn_event & ALT)           // ALT - Resets, Idles, Suspends, etc.
        {
            led_on_green();
         handle_alt();
            continue;
        }

      if (usbn_event & NAK)           // NAK - PC tried to send us data, but 960x nak'd it
      {
            // NAK on EP0
         if (usbn_read(NAKEV) & 0x10)
         {
                //
                // IF we're in the middle of a multipacket transfer, then the
                // host has NAL'd us and wants us to stop transmitting.
                //
               if ( XferEP0.nXferType != NO_XFER) {
                    // clear any output transmission
                    XferEP0.nXferType = NO_XFER;
                    // Flush xmitter, disable transmitting, enable reception
                    usbn_write(TXC0, FLUSH);
                    usbn_write(RXC0, RX_EN);
                }
         } else {
              i = 9;
           }
            continue;
      }
        //
        // TODO: This events should not  happen
        //
        if (usbn_event & WARN)          // WARN - FIFO warn event (FWEV)
            handle_warn();

        if (usbn_event & FRAME)         // FRAME - frame counter updated
            handle_frame();

        if (usbn_event & ULD)           // ULD - Unlock/Lock deteced
            handle_uld();

    } while (usbn_event);

    return;
}

/*******************************************************************************
/* handle_alt()
/*
/* Summary:  This interrupt happens when an RESET, SUSPEND or RESUME event has happened.
/*            RESET - USBN device is reset, USB token handling code is put into initial state
/*            SUSPEND - Resume interrupt is enabled, USBN device put into suspended mode
/*            RESUME - Suspend interrupt is enabled, USBN device put into operational state, EP0 reception enabled
/*
/********************************************************************************/
void handle_alt(void) {
    int8 alt_event, i;

    alt_event = usbn_read(ALTEV);
    alt_event &= alt_mask;

    //
    // Loop through and handle all events one at a time
    //
    if (alt_event & ALT_RESET)                  // RESET
    {
        usbn_write(EPC0, usbn_read(EPC0) & ~STALL);     // CLEAR_STALL_EP0
        usbn_write(NFSR, RST_ST);                       // Enter RESET state
        delay_ms(1);                                    // Must stay in RESET for at least 100 us)
        usbn_write(FAR, AD_EN+0);                       // set default address 0x00
        usbn_write(EPC0, 0x00);                         // enable EP0 only

       reset_usb();
       // TODO: Set device state
        // usb_device_reset();

        XferEP0.nXferType = NO_XFER;

       usbn_write(NFSR, OPR_ST);                       // Enter OPERATIONAL state

        usbn_write(FAR, AD_EN+0);                       // set default address 0x00
        usbn_write(EPC0, 0x00);                         // default address
        usbn_write(RXC0, RX_EN);                        // enable reception

    } else if (alt_event & ALT_SD3) {
        //
        // Enable the RESUME interrupt
        alt_mask = ALT_RESET | ALT_RESUME;
        usbn_write(ALTMSK, alt_mask);
        //
        // Enter SUSPENDED state
      usbn_write(NFSR, SUS_ST);

    } else if (alt_event & ALT_RESUME) {
        //
        // Disable the RESUME interrupt
        alt_mask = ALT_SD3 | ALT_RESET;
        usbn_write(ALTMSK, alt_mask);
        //
        // Enter operation state
        usbn_write(NFSR, OPR_ST);
        //
        // Enable receiving
        usbn_write(RXC0, RX_EN);
    } else {
        //
        // Spurious ALT event
        //
        i = 99;
    }
    return;
}

/*******************************************************************************
/* handle_txev()
/*
/* The TX interrupt happens whenever an IN token has been processed.  (PIC -> HOST).
/*
/********************************************************************************/
void handle_txev(void) {
   int8 txs;
   int8 tx_event, i;
   int8 event_mask = 1;

    tx_event = usbn_read(TXEV);

    //
    // Loop through and handle all events one at a time
    //
    while(tx_event)
    {
        switch(tx_event & event_mask)
        {
            case TX_FIFO0:              // EP0

                txs=usbn_read(TXS0);    // Read TX status for EP0

                if (txs & TX_DONE)
                {
                    // FLUSH - disable TX
                    usbn_write(TXC0, FLUSH);

                    if (txs & ACK_STAT)     // ACK status
                    {
                        //
                        if (XferEP0.nXferType != NO_XFER) {    // if we are processing a report then send the next part
                            // Continue transmitting descriptor
                           XmitOnUSB0();
                            i = endpoint_stat[0].toggle_bit << 2;
                            endpoint_stat[0].toggle_bit ^= 0x01;
                            usbn_write(TXC0, TX_EN | i);   // enable the TX (DATA1) and toggle PID

                        } else {

                            if (setaddress) {
                                usbn_write(FAR, setaddress);
                                setaddress = 0;
                            }

                            usbn_write(RXC0, RX_EN);  // enable reception
                        }
                    } else {

                        XferEP0.nXferType = NO_XFER;
                   /*-------------------
                   * there is no ACK
                   * Re-enable receiving
                   *--------------------*/
                        usbn_write(RXC0, RX_EN);
                    }
                } else {
                    i =9;
                }
                break;

            case TX_FIFO1:              // TX event - FIFO1
                tx_1();
                break;
            case TX_FIFO2:              // TX event - FIFO2
                tx_2();
                break;
            case TX_FIFO3:              // TX event - FIFO3
                tx_3();
                break;
            case TX_UDRN0:
            case TX_UDRN1:
            case TX_UDRN2:
            case TX_UDRN3:
            default:

#if 0
                txs=usbn_read(TXS1);
                if (txs & ACK_STAT)     // ACK status
                {
                    // Done transmitting data
                } else {
                    // Retransmit data
                }
#endif
                break;

      }
        tx_event &= ~event_mask;
        event_mask = event_mask << 1;
    }
    return;
}

/*******************************************************************************
/* handle_rxev()
/*
/* Summary: Handles the RX interrupt.  This happens when an OUT/SETUP token has happened (HOST -> PIC)
/*          and there is data in the RX buffers.
/*
/*          If there is data in the EP0 buffer and it is a SETUP token, it is processed by the
/*          SETUP token handler.  The SETUP token handler is the bulk of this code.
/*
/********************************************************************************/
void handle_rxev(void) {
    int8    rxstatus, len, i;
    int8    rx_event, rx_event2;
    int8    event_mask = 1;

    //
    // Get the RX event status
    //

    rx_event = usbn_read(RXEV);
    rx_event2= rx_event;

    while(rx_event)
    {
        switch(rx_event & event_mask)
        {
            case RX_FIFO0:              // RX0, EP0 - Main control and Setup endpoint
                memset(ep0_rx_buffer, 0, USB_MAX_EP0_PACKET_LENGTH);
                rxstatus = usbn_read(RXS0);     // Get Receiver 0 status
                len = rxstatus & 0x0F;          // Get the length

             if (rxstatus & SETUP_R)         // SETUP packet
             {

                    //
                    // Ignore zero length packets
                    if ((rxstatus & RX_LAST) && (len == 0)) {
                        i = 1;
                        break;
                    }

//                    usbn_write(EPC0, usbn_read(EPC0) & ~STALL); // clear STALL
                    endpoint_stat[0].toggle_bit = 0x01;         // Initial TOGGLE status

                    //
                    // Establish maximum length
                    if (len > USB_MAX_EP0_PACKET_LENGTH)
                        len = USB_MAX_EP0_PACKET_LENGTH;
                    // Read EP0 FIFO data
                    for (i=0; i<len; i++)
                    {
                        ep0_rx_buffer[i] = usbn_read(RXD0);
                    }

                    usbn_write(TXC0, FLUSH);    // Flush TX and RX fifo's
                    usbn_write(RXC0, FLUSH);
                    handle_setup();             // Process setup packet
                } else {
                    //
                    // OUT packet of Status stage in control read/write sequence
                    // This signals the normal completion of a CONTROL IN
                    // such as Descriptor data sent to host.
                    //
                    if (XferEP0.nXferType != NO_XFER) {
                        XferEP0.nXferType = NO_XFER;
                        usbn_write(TXC0, FLUSH);    // Flush TX and RX fifo's
                    }
                    usbn_write(RXC0, RX_EN);        // Enable receiving on EP0
                }
                break;

            case RX_FIFO1:
                i = 8;
                rx_1();
                break;

            case RX_FIFO2:
            case RX_FIFO3:
            case RX_OVRN0:
            case RX_OVRN1:
            case RX_OVRN2:
            default:
              i = 9;
                break;
        }
        rx_event &= ~event_mask;
        event_mask = event_mask << 1;
    }

    return;
}



/*******************************************************************************
/* handle_warn()
/*
/* Summary: The WARN event happens when a USBN buffer reaches a desired state.
/*          This requires the user to setup the WARN setting for each buffer.
/*
/********************************************************************************/
void handle_warn(void) {

   // TODO: add code to handle when FIFOs get near full / empty

}

/*******************************************************************************
/* handle_frame()
/*
/* Summary: Handles a FRAME event interrupt.
/*          Only relevant to users using isochronous.
/*
/********************************************************************************/
void handle_frame(void) {

   // TODO: add code if needed
}

/*******************************************************************************
/* handle_uld()
/*
/* Summary:  Handles ULD interrupt.
/*
/********************************************************************************/
void handle_uld(void) {

   // TODO: add code if needed
}


/////////////////////////////////////////////////////////////////////////////////////////
// usbn_write(address, data)
//
// Input: address - address of USBN960x register to write to
//        data - data to write to USBN960x
//
// Summary: Writes a byte of memory to specified address on USBN960x
//
void usbn_write(int8 address, int8 data) {

    // Mask off bits address 7 and 6
    address &= 0x3F;

    // PORT D to output
    set_tris_d(0x00);
    // Write the register address
    PORT_D = address;
    // A0 = 1 (register access)
    output_high(USB_A0);
    // Chip select enable / Write enable
    output_low(USB_CS);
    output_low(USB_WR);
    // Wait about 1 clock for the data to be latched into the USBN9603
#asm
    nop
#endasm
    // Chip select disable
    output_high(USB_CS);
    // Write disable
    output_high(USB_WR);
    // Wait at least 3 clocks before next access to USBN9603
#asm
    nop
    nop
    nop
#endasm

    // A0 = 0 (data access)
    output_low(USB_A0);
    // Write the data
    PORT_D = data;
    // Chip select enable / Write enable
    output_low(USB_CS);
    output_low(USB_WR);
    // Wait about 1 clock for the data to be valid out of the USBN9603
#asm
    nop
#endasm
    // Chip select, Write, Read disable
    output_high(USB_CS);
    output_high(USB_RD);
    output_high(USB_WR);
    return;
}

////////////////////////////////////////////////////////////////////////////////////////
// usbn_read(address)
//
// Input:   address - address of USBN960x register to read
// Output:  data (byte) in specified address.
//
// Summary: Reads a byte of memory from specified address on USBN960x
//
int8 usbn_read(int8 address) {
    int8 data;

    // Mask off bits address 7 and 6
    address &= 0x3F;

    // PORT D to output
    set_tris_d(0x00);
    // Write the register address
    PORT_D = address;
    // A0 = 1 (register access)
    output_high(USB_A0);
    // Chip select enable / Write enable
    output_low(USB_CS);
    output_low(USB_WR);
    // Wait about 1 clock for the data to be latched into the USBN9603
#asm
    nop
#endasm
    // Chip select disable
    output_high(USB_CS);
    // Write disable
    output_high(USB_WR);
    // Wait at least 3 clocks before next access to USBN9603
#asm
    nop
    nop
    nop
#endasm

    // Now we can issue the data command
    // PORT D to input
    set_tris_d(0xFF);
    // A0 = 0 (data access)
    output_low(USB_A0);
    // Chip select enable / Read enable
    output_low(USB_CS);
    output_low(USB_RD);
    // Wait about 1 clock for the data to be valid out of the USBN9603
#asm
    nop
#endasm
    // Read the data in
    data = PORT_D;

    // Chip select, Write, Read disable
    output_high(USB_CS);
    output_high(USB_RD);
    output_high(USB_WR);

    // Wait at least 3 clock cycles before next USBN9603 access
    // This is implied here.
    return(data);
}

