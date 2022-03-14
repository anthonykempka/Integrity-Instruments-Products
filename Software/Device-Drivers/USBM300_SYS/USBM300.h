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

#ifndef USBM300_INCD
#define USBM300_INCD


#define kMaxNumInstances		999

#define Eval_NAME_MAX			64

//
// Memory allocation tag
//
#define USBM300_TAG '003M'

//Pipe information:
#define kNumPipeAttributes		4
#define kConfigNumIndex			0
#define kInterfaceNumIndex		1
#define kAltInterfaceNumIndex	2
#define kPipeNumIndex			3
#define kMaxTransferSize		128


/*
// This is an unused structure in this driver, but is provided here
// so when you extend the driver to deal with USB pipes, you may wish
// to use this structure as an example or model.
*/

typedef struct _Eval_PIPE {
    ULONG Mode;
    ULONG Option;
    ULONG Param1;
    ULONG Param2;
    PUSBD_PIPE_INFORMATION PipeInfo;
} Eval_PIPE, *PEval_PIPE;

/*
// The interface number on this device that this driver expects to use
// This would be in the bInterfaceNumber field of the Interface Descriptor, hence
// this device driver would need to know this value.
*/
#define EVAL_INTERFACE_NBR  0x00

// This driver supports only interface
#define MAX_INTERFACE       0x01


// used to track information on pipes in use;
//  currently just to flag if opened or closed
typedef struct BULKUSB_PIPEINFO {

    BOOLEAN fPipeOpened;

} BULKUSB_PIPEINFO, *PBULKUSB_PIPEINFO;



//
// A structure representing the instance information associated with
// this particular device.
//
typedef struct _DEVICE_EXTENSION
{
    PDEVICE_OBJECT				PhysicalDeviceObject;					// physical device object
    PDEVICE_OBJECT				StackDeviceObject;						// Device object we call when submitting Urbs/Irps to the USB stack

    USBD_CONFIGURATION_HANDLE	ConfigurationHandle;					// configuration handle for the configuration the device is currently in
    PUSB_DEVICE_DESCRIPTOR		DeviceDescriptor;						// ptr to the USB device descriptor for this device	
    PUSBD_INTERFACE_INFORMATION UsbInterface;						    // we support up to one interface

	WCHAR						DeviceLinkNameBuffer[MAXIMUM_FILENAME_LENGTH];	// MAXIMUM_FILENAME_LENGTH defined in wdm.h

	// default maximum transfer per staged irp size		
	ULONG                       MaximumTransferSize;  

    // ptr to array of structs to track pipeinfo;
    //  in this basic sample it's only used to track if open/closed;
    PBULKUSB_PIPEINFO           PipeInfo;

    BOOLEAN                     Removed;                                // TRUE when the device is removed
	BOOLEAN						NeedCleanup;							// Indicates the device needs to be cleaned up (ie., some configuration has occurred and needs to be torn down).
    BOOLEAN                     DeviceStarted;                          // TRUE when the device is started
    BOOLEAN						Stopped;								// Indicates that we have recieved a STOP message
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;


#endif  // USBM300_INCD
