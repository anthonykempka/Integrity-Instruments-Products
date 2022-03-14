///////////////////////////////////////////////////////////////////////////////
//
// USBM300.H
//
// USBM300 USB Data Acquisition Driver
//
// Copyright (c), 2002-2003 Integrity Instruments, Inc.
// Tel: 1-800-450-2001
//
// By: Anthony A. Kempka
//
//
//

#ifndef USBM300_IOCTL
#define USBM300_IOCTL


/*
///////////////////////////////////////////////////////
//
//              IOCTL Definitions
//
// User mode applications wishing to send IOCTLs to a kernel mode driver
// must use this file to set up the correct type of IOCTL code permissions.
//
// Note: this file depends on the file DEVIOCTL.H which contains the macro
// definition for "CTL_CODE" below.  Include that file before  you include
// this one in your source code.
//
///////////////////////////////////////////////////////
*/

/*
// Set the base of the IOCTL control codes.  This is somewhat of an
// arbitrary base number, so you can change this if you want unique
// IOCTL codes.  You should consult the Windows NT DDK for valid ranges
// of IOCTL index codes before you choose a base index number.
*/

#define Eval_IOCTL_INDEX  0x0800


#define IOCTL_Eval_GET_PIPE_INFO				CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   Eval_IOCTL_INDEX+0,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_Eval_GET_DEVICE_DESCRIPTOR		CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   Eval_IOCTL_INDEX+1,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_Eval_GET_NUM_DEVICES				CTL_CODE(FILE_DEVICE_UNKNOWN,  \
													Eval_IOCTL_INDEX+2,\
													METHOD_BUFFERED,  \
													FILE_ANY_ACCESS)

#define IOCTL_Eval_BULK_WRITE					CTL_CODE(FILE_DEVICE_UNKNOWN,  \
													Eval_IOCTL_INDEX+3,\
													METHOD_BUFFERED,  \
													FILE_ANY_ACCESS)

#define IOCTL_Eval_BULK_OR_INTERRUPT_READ		CTL_CODE(FILE_DEVICE_UNKNOWN,  \
													Eval_IOCTL_INDEX+4,\
													METHOD_BUFFERED,  \
													FILE_ANY_ACCESS)

#endif  // USBM300_IOCTL
