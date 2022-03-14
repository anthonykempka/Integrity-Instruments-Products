///////////////////////////////////////////////////////////////////////////
//
//  USBM300_DESC.H
//
//  USB descriptors for the Integrity Instruments, Inc.
//  Model: USBM300 I/O Module
//

#define __USB_DESCRIPTORS__

//
// Note: Endpoint 0 doesnt count here since all devices have an EP0 for control
//
#define USB_NUM_ENDPOINTS      4


#define USB_NUM_CLASSES        0

//
// Standard request codes
//
#define GET_STATUS          0
#define CLEAR_FEATURE       1
#define SET_FEATURE         3
#define SET_ADDRESS         5
#define GET_DESCRIPTOR      6
#define SET_DESCRIPTOR      7
#define GET_CONFIGURATION   8
#define SET_CONFIGURATION   9
#define GET_INTERFACE       10
#define SET_INTERFACE       11
#define SYNCH_FRAME         12

//
// Descriptor types
//
#define USB_DEVICE_DESC_KEY      0x01
#define USB_CONFIG_DESC_KEY      0x02
#define USB_STRING_DESC_KEY      0x03
#define USB_INTERFACE_DESC_KEY   0x04
#define USB_ENDPOINT_DESC_KEY    0x05


#define USB_MAX_EP0_PACKET_LENGTH   8

//
// Descriptor lengths. Specific to our configuration
//
#define USB_DEVICE_DESC_LEN         18
#define USB_CONFIG_DESC_LEN         9
#define USB_CONFIG_DESC_TOTAL_LEN   46
#define USB_INTERFACE_DESC_LEN      9
#define USB_ENDPOINT_DESC_LEN       7

//
// Device descriptor
//
const int8 USB_DEVICE_DESC[USB_DEVICE_DESC_LEN] = {
    USB_DEVICE_DESC_LEN,        // the length of this report
    USB_DEVICE_DESC_KEY,        // constant DEVICE (0x01)
    0x10,0x01,                  // usb version in bcd
    0x00,                       // class code (if 0, interface defines class.  FF is vendor defined)
    0x00,                       // subclass code
    0x00,                       // protocol code
    8,                          // max packet size for endpoint 0
    0xEF,0x10,                  // vendor id = 0x10EF   Integrity Instruments, Inc.
    0x03,0x00,                  // product id
    0x00,0x00,                  // device release number
    0x01,                       // index of string description of manufacturer. therefore we point to string_1 array (see below)
    0x02,                       // index of string descriptor of the product
    0x00,                       // NOT USED: index of string descriptor of serial number
    1                           // number of possible configurations
};

//
// Configuration descriptor
//
int8 const USB_CONFIG_DESC[USB_CONFIG_DESC_TOTAL_LEN] = {
    // Config_descriptor for config index 1
    USB_CONFIG_DESC_LEN,        // length of descriptor size
    USB_CONFIG_DESC_KEY,        // constant CONFIGURATION (0x02)
    USB_CONFIG_DESC_TOTAL_LEN,0,// size of all data returned for this config
    0x01,                       // number of interfaces this device supports
    0x01,                       // identifier for this configuration.  (IF we had more than one configurations)
    0x00,                       // index of string descriptor for this configuration
    0x40,                       // bit 6=1 if self powered, bit 5=1 if supports remote wakeup (we don't), bits 0-4 reserved and bit7=1
    0x00,                       // maximum bus power required (maximum milliamperes/2)  (0x32 = 100mA)

    // Interface descriptor 0 alt 0
    USB_INTERFACE_DESC_LEN,     // length of descriptor
    USB_INTERFACE_DESC_KEY,     // constant INTERFACE (0x04)
    0x00,                       // number defining this interface (IF we had more than one interface)
    0x00,                       // alternate setting
    USB_NUM_ENDPOINTS,          // number of endpoints, except 0.
    0xFF,                       // class code, FF = vendor defined
    0xFF,                       // subclass code, FF = vendor
    0xFF,                       // protocol code, FF = vendor
    0x00,                       // index of string descriptor for interface

    // Endpoint descriptor EP1 - IN (to the Host)
    // WDM "PIPE0"
    USB_ENDPOINT_DESC_LEN,      // length of descriptor
    USB_ENDPOINT_DESC_KEY,      // constant ENDPOINT (0x05)
    0x81,                       // endpoint number and direction (0x81 = EPC1 IN)
    0x02,                       // transfer type supported (0 is control, 1 is iso, 2 is bulk, 3 is interrupt)
    0x40,0x00,                  // maximum packet size supported
    0xFF,                       // polling interval in ms. (for interrupt transfers ONLY)

    // Endpoint descriptor EP2 - OUT (from the Host)
    // WDM "PIPE1"
    USB_ENDPOINT_DESC_LEN,      // length of descriptor
    USB_ENDPOINT_DESC_KEY,      // constant ENDPOINT (0x05)
    0x02,                       // endpoint number and direction (0x02 = EPC2 OUT)
    0x02,                       // transfer type supported (0 is control, 1 is iso, 2 is bulk, 3 is interrupt)
    0x40,0x00,                  // maximum packet size supported
    0xFF,                       // polling interval in ms. (for interrupt transfers ONLY)

    // Endpoint descriptor EP3 - IN (to the Host)
    // WDM "PIPE2"
    USB_ENDPOINT_DESC_LEN,      // length of descriptor
    USB_ENDPOINT_DESC_KEY,      // constant ENDPOINT (0x05)
    0x83,                       // endpoint number and direction (0x83 = EPC3 IN)
    0x02,                       // transfer type supported (0 is control, 1 is iso, 2 is bulk, 3 is interrupt)
    0x40,0x00,                  // maximum packet size supported
    0xFF,                       // polling interval in ms. (for interrupt transfers ONLY)

    // Endpoint descriptor EP4 - IN (to the Host)
    // WDM "PIPE3"
    USB_ENDPOINT_DESC_LEN,      // length of descriptor
    USB_ENDPOINT_DESC_KEY,      // constant ENDPOINT (0x05)
    0x84,                       // endpoint number and direction (0x84 = EPC5 IN)
    0x02,                       // transfer type supported (0 is control, 1 is iso, 2 is bulk, 3 is interrupt)
    0x40,0x00,                  // maximum packet size supported
    0xFF                        // polling interval in ms. (for interrupt transfers ONLY)
}; 


//
// String descriptors and string lengths
//
#define USB_STRING_0_LEN    4
#define USB_STRING_1_LEN    54
#define USB_STRING_2_LEN    50
#define USB_STRING_3_LEN    46
#define USB_STRING_4_LEN    46

int8 const USB_STRING_0[USB_STRING_0_LEN] = {  
                                // string index 0 is special
         USB_STRING_0_LEN,      // length of string index
         0x03,                  // descriptor type 0x03 (STRING)
         0x09,0x04              // Microsoft Defined for US-English
};

int8 const USB_STRING_1[USB_STRING_1_LEN] = {  
                                // we defined string index 1 earlier as manuf string
         USB_STRING_1_LEN,      // length of string index
         0x03,                  // descriptor type 0x03 (STRING)
         'I',0,
         'n',0,
         't',0,
         'e',0,
         'g',0,
         'r',0,
         'i',0,
         't',0,
         'y',0,
         ' ',0,
         'I',0,
         'n',0,
         's',0,
         't',0,
         'r',0,
         'u',0,
         'm',0,
         'e',0,
         'n',0,
         't',0,
         's',0,
         ' ',0,
         'I',0,
         'n',0,
         'c',0,
         '.',0
};

int8 const USB_STRING_2[USB_STRING_2_LEN] = {  
                                // we defined string index 2 as description of product
         USB_STRING_2_LEN,      // length of string index
         0x03,                  // descriptor type 0x03 (STRING)
         'U',0,
         'S',0,
         'B',0,
         'M',0,
         '3',0,
         '0',0,
         '0',0,
         ' ',0,
         'D',0,
         'a',0,
         't',0,
         'a',0,
         ' ',0,
         'A',0,
         'c',0,
         'q',0,
         'u',0,
         'i',0,
         's',0,
         'i',0,
         't',0,
         'i',0,
         'o',0,
         'n',0
};

int8 const USB_STRING_3[USB_STRING_3_LEN] = {  
                                // we defined string index 3 as Interface Description
         USB_STRING_3_LEN,      // length of string index
         0x03,                  // descriptor type 0x03 (STRING)
         'S',0,
         't',0,
         'a',0,
         'n',0,
         'd',0,
         'a',0,
         'r',0,
         'd',0,
         ' ',0,
         'C',0,
         'o',0,
         'n',0,
         'f',0,
         'i',0,
         'g',0,
         'u',0,
         'r',0,
         'a',0,
         't',0,
         'i',0,
         'o',0,
         'n',0
};

int8 const USB_STRING_4[USB_STRING_4_LEN] = {  
                                // we defined string index 3 as Interface Description
         USB_STRING_4_LEN,      // length of string index
         0x03,                  // descriptor type 0x03 (STRING)
         'U',0,
         'S',0,
         'B',0,
         'M',0,
         '3',0,
         '0',0,
         '0',0,
         ' ',0,
         'I',0,
         'n',0,
         't',0,
         'e',0,
         'r',0,
         'f',0,
         'a',0,
         'c',0,
         'e',0,
         ' ',0,
         '1',0,
         '.',0,
         '0',0,
         '0',0
};
