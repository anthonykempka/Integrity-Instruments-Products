//***************************************************************************
//
//
// CMDS.H
//
//                      INTEGRITY DESIGNS, LLC
//                      ---------------------------
//                      209 Grand Avenue, Suite 403
//                      Laramie, WY  82070
//                      Tel: (307)721-4881
//                      Fax: (307)742-2090
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


#ifndef _CMDINC
#define _CMDINC

//
// Macro definitions
//


#define READ_INPUTS         0x10            
#define SET_OUTPUTS         0x11
#define READ_ANALOG         0x12
#define READ_OUTPUTS        0x13

#define INPUT_STATUS        0x90
#define OUTPUTS_SET         0x91
#define ANALOG_STATUS       0x92
#define OUTPUT_STATUS       0x93

#define READ_SW_VER         0x50
#define WRITE_EEPROM        0x51
#define READ_EEPROM         0x52
#define RESET_MODULE        0x5F

#define SW_VERSION          0xD0
#define EEPROM_WRITTEN      0xD1
#define EEPROM_READ         0xD2
#define MODULE_RESET        0xDF


#define ERROR               0xF0
#define ACKNOWLEDGE         0xF1


#endif // _CMDINC


