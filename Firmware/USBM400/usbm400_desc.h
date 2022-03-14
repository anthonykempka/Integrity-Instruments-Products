///////////////////////////////////////////////////////////////////////////
//
//  USBM300_DESC.H
//
//  USB descriptors for the Integrity Instruments, Inc.
//  Model: USBM400 I/O Module
//
//  Revision History
//  --------------------
//  2-9-2008  (Anthony Kempka)
//  * Removed all compiler warnings (mostly void declarations and unused variables)
//  * Added Serial Number (S/N) support where S/N is located in EEPROM
//

#define __USB_DESCRIPTORS__

//
// Note: Endpoint 0 doesnt count here since all devices have an EP0 for control
//
#define USB_NUM_ENDPOINTS      4


#define USB_NUM_CLASSES        0

//
// Standard USB request codes
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

// NOTE: AAK added 2-9-2008
#define STR_INDX_NONE       0x00    // Unused string index (no string)
#define STR_INDX_LANG       0x00    // Language identifier
#define STR_INDX_MFG        0x01    // Manufacturer string index
#define STR_INDX_PROD       0x02    // Product string index
#define STR_INDX_SN         0x03    // Serial number string index

//
// Device descriptor
//
const int8 USB_DEVICE_DESC[USB_DEVICE_DESC_LEN] = {
    USB_DEVICE_DESC_LEN,        // the length of this report
    USB_DEVICE_DESC_KEY,        // constant DEVICE (0x01)
    0x10,0x01,                  // usb version in bcd
    0x00,                       // class code (if 0, then interface defines class.  FF is vendor defined)
    0x00,                       // subclass code
    0x00,                       // protocol code
    8,                          // max packet size for endpoint 0
    0xEF,0x10,                  // VID = 0x10EF   Integrity Instruments, Inc.
    0x00,0x04,                  // PID = 0x0400  (USB400M)
    0x00,0x01,                  // device release number  1.0
    STR_INDX_MFG,               // (String) index of string description of manufacturer. therefore we point to string_1 array (see below)
    STR_INDX_PROD,              // (String) index of string descriptor of the product
    STR_INDX_SN,                // (String) index of string descriptor of serial number
    1                           // number of possible configurations (only one)
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
    STR_INDX_NONE,              // (String) index of string descriptor for this configuration
    0x40,                       // bit 7=0 (reserved USB 1.0), bit 6=1 self powered, bit 5=1 remote wakeup , bits 0-4 reserved
    0x00,                       // bus power required (maximum milliamperes/2)  (0x32 = 100mA)

    // Interface descriptor 0 alt 0
    USB_INTERFACE_DESC_LEN,     // length of descriptor
    USB_INTERFACE_DESC_KEY,     // constant INTERFACE (0x04)
    0x00,                       // number defining this interface (IF we had more than one interface)
    0x00,                       // alternate setting
    USB_NUM_ENDPOINTS,          // number of endpoints, except 0.
    0xFF,                       // class code, FF = vendor defined
    0x00,                       // subclass code
    0x00,                       // protocol code
    STR_INDX_NONE,              // (String) index of string descriptor for interface

    // Endpoint descriptor EP1 - IN (to the Host)
    // WDM = "PIPE0"  response (to command)
    USB_ENDPOINT_DESC_LEN,      // length of descriptor
    USB_ENDPOINT_DESC_KEY,      // constant ENDPOINT (0x05)
    0x81,                       // endpoint number and direction (0x81 = EPC1 IN)
    0x02,                       // transfer type supported (0 is control, 1 is iso, 2 is bulk, 3 is interrupt)
    0x40,0x00,                  // maximum packet size supported (64 bytes)
    0x00,                       // polling interval in ms. (for interrupt transfers ONLY)

    // Endpoint descriptor EP2 - OUT (from the Host)
    // WDM = "PIPE1"  command
    USB_ENDPOINT_DESC_LEN,      // length of descriptor
    USB_ENDPOINT_DESC_KEY,      // constant ENDPOINT (0x05)
    0x02,                       // endpoint number and direction (0x02 = EPC2 OUT)
    0x02,                       // transfer type supported (0 is control, 1 is iso, 2 is bulk, 3 is interrupt)
    0x40,0x00,                  // maximum packet size supported (64 bytes)
    0x00,                       // polling interval in ms. (for interrupt transfers ONLY)

    // Endpoint descriptor EP3 - IN (to the Host)
    // WDM = "PIPE2"  streaming 1
    USB_ENDPOINT_DESC_LEN,      // length of descriptor
    USB_ENDPOINT_DESC_KEY,      // constant ENDPOINT (0x05)
    0x83,                       // endpoint number and direction (0x83 = EPC3 IN)
    0x02,                       // transfer type supported (0 is control, 1 is iso, 2 is bulk, 3 is interrupt)
    0x40,0x00,                  // maximum packet size supported (64 bytes)
    0x00,                       // polling interval in ms. (for interrupt transfers ONLY)

    // Endpoint descriptor EP4 - IN (to the Host)
    // WDM = "PIPE3"  streaming 2
    USB_ENDPOINT_DESC_LEN,      // length of descriptor
    USB_ENDPOINT_DESC_KEY,      // constant ENDPOINT (0x05)
    0x84,                       // endpoint number and direction (0x84 = EPC5 IN)
    0x02,                       // transfer type supported (0 is control, 1 is iso, 2 is bulk, 3 is interrupt)
    0x40,0x00,                  // maximum packet size supported (64 bytes)
    0x00                        // polling interval in ms. (for interrupt transfers ONLY)
}; 


//
// String descriptors and string lengths
//
#define USB_STRING_0_LEN    4
#define USB_STRING_1_LEN    56
#define USB_STRING_2_LEN    50
#define USB_STRING_SN_LEN   18

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
         ',',0,
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
         '4',0,
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


int8 const USB_STRING_SN[USB_STRING_SN_LEN] = {  
         USB_STRING_SN_LEN,   // length of string index
         0x03,                // descriptor type 0x03 (STRING)
         'D',0,
         'E',0,
         'A',0,
         'D',0,
         'B',0,
         'E',0,
         'E',0,
         'F',0
};

// Same as the const (ROM) structure above, but in RAM
int8 USB_STRING_SN_RAM[USB_STRING_SN_LEN];


