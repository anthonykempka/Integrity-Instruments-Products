//
// GUID_USBM300.H
// 
// Defines the GUID for the USBM300 I/O Module
//

#ifndef GUIDUSBM300_INC
#define GUIDUSBM300_INC

#include <initguid.h>

// {D3CC0825-BE50-4baa-83A1-6A8303999170}  for USBM300.SYS

DEFINE_GUID(GUID_CLASS_USBM300, 
0xd3cc0825, 0xbe50, 0x4baa, 0x83, 0xa1, 0x6a, 0x83, 0x3, 0x99, 0x91, 0x70);

// {14155DA4-CA6E-4ae7-9CAF-810005960035}  for USBM400.SYS
DEFINE_GUID(GUID_CLASS_USBM400, 
0x14155da4, 0xca6e, 0x4ae7, 0x9c, 0xaf, 0x81, 0x0, 0x5, 0x96, 0x0, 0x35);

#endif // #ifndef GUIDUSBM300_INC
