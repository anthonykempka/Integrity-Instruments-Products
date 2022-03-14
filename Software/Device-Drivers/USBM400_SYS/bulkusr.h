/*++

Copyright (c) 2000  Microsoft Corporation

Module Name:

    sSUsr.h

Abstract:

Environment:

    Kernel mode

Notes:

    Copyright (c) 2000 Microsoft Corporation.  
    All Rights Reserved.

--*/

#ifndef _BULKUSB_USER_H
#define _BULKUSB_USER_H

#include <initguid.h>

// {6068EB61-98E7-4c98-9E20-1F068295909A}
// DEFINE_GUID(GUID_CLASS_I82930_BULK, 
// 0x873fdf, 0x61a8, 0x11d1, 0xaa, 0x5e, 0x0, 0xc0, 0x4f, 0xb1, 0x72, 0x8b);

// Note: Anthony Kempka   {Date: 2-9-2008}
// GUID obtained from USBM400 v1
//
// USBM400 GUID
// {14155DA4-CA6E-4ae7-9CAF-810005960035}  for USBM400.SYS
DEFINE_GUID(GUID_CLASS_USBM400, 
0x14155da4, 0xca6e, 0x4ae7, 0x9c, 0xaf, 0x81, 0x0, 0x5, 0x96, 0x0, 0x35);


#define BULKUSB_IOCTL_INDEX             0x0000


#define IOCTL_BULKUSB_GET_CONFIG_DESCRIPTOR CTL_CODE(FILE_DEVICE_UNKNOWN,     \
                                                     BULKUSB_IOCTL_INDEX,     \
                                                     METHOD_BUFFERED,         \
                                                     FILE_ANY_ACCESS)
                                                   
#define IOCTL_BULKUSB_RESET_DEVICE          CTL_CODE(FILE_DEVICE_UNKNOWN,     \
                                                     BULKUSB_IOCTL_INDEX + 1, \
                                                     METHOD_BUFFERED,         \
                                                     FILE_ANY_ACCESS)

#define IOCTL_BULKUSB_RESET_PIPE            CTL_CODE(FILE_DEVICE_UNKNOWN,     \
                                                     BULKUSB_IOCTL_INDEX + 2, \
                                                     METHOD_BUFFERED,         \
                                                     FILE_ANY_ACCESS)

#endif

