///////////////////////////////////////////////////////////////////////////////
//
// USBM300.C
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

#define DRIVER

#include "wdm.h"
#include "usbdi.h"
#include "usbdlib.h"
#include "stdio.h"

#include "debug.h"
#include "USBM300.h"
#include "GUID_USBM300.H"
#include "IOCTL_USBM300.H"


ULONG   gulNameCount = 0;
ULONG   gulNumDevices = 0;

//
// Forward function declarations
//
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, 
                     IN PUNICODE_STRING RegistryPath);

NTSTATUS Create_USBM300(IN PDEVICE_OBJECT DeviceObject, 
                        IN PIRP Irp);

NTSTATUS Close_USBM300(IN PDEVICE_OBJECT DeviceObject, 
                       IN PIRP Irp);

NTSTATUS Dispatch_IOCTL(IN PDEVICE_OBJECT DeviceObject, 
                        IN PIRP Irp);

NTSTATUS Eval_Read_Write(IN PDEVICE_OBJECT DeviceObject, 
                         IN PIRP Irp, 
                         IN BOOLEAN Read);

NTSTATUS Eval_CallUSBD(IN PDEVICE_OBJECT DeviceObject, 
                       IN PURB Urb);

NTSTATUS Dispatch_PNP(IN PDEVICE_OBJECT DeviceObject, 
                         IN PIRP Irp);

NTSTATUS Eval_StartDevice(IN PDEVICE_OBJECT DeviceObject);

VOID Eval_RemoveDevice(IN PDEVICE_OBJECT DeviceObject);

NTSTATUS Eval_StopDevice(IN PDEVICE_OBJECT DeviceObject);

NTSTATUS Eval_ConfigureDevice(IN PDEVICE_OBJECT DeviceObject);

NTSTATUS Eval_SelectInterface(IN PDEVICE_OBJECT DeviceObject, 
                              IN PUSB_CONFIGURATION_DESCRIPTOR	ConfigurationDescriptor);

NTSTATUS AddDevice_USBM300(IN PDRIVER_OBJECT DriverObject, 
                           IN PDEVICE_OBJECT PhysicalDeviceObject);

NTSTATUS CreateDO(IN PDRIVER_OBJECT DriverObject, 
                  IN PDEVICE_OBJECT PhysicalDeviceObject,
                  IN PDEVICE_OBJECT *DeviceObject);

VOID Unload_USBM300(IN PDRIVER_OBJECT DriverObject);

VOID Eval_Cleanup(PDEVICE_OBJECT DeviceObject);

NTSTATUS Dispatch_Power(IN PDEVICE_OBJECT DeviceObject, 
                        IN PIRP Irp);

NTSTATUS Dispatch_System(IN PDEVICE_OBJECT DeviceObject, 
                         IN PIRP Irp);

///////////////////////////////////////////////////////////////////////////////
//
//
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, 
                     IN PUNICODE_STRING RegistryPath)
{
    OurDebugPrint((DBG_INFO, ">> DriverEntry\n"));

	//
    // Standard IRP's
    //
	DriverObject->MajorFunction[ IRP_MJ_CREATE ]            = Create_USBM300;
	DriverObject->MajorFunction[ IRP_MJ_CLOSE ]             = Close_USBM300;
	DriverObject->MajorFunction[ IRP_MJ_DEVICE_CONTROL ]    = Dispatch_IOCTL;
	DriverObject->DriverUnload                              = Unload_USBM300;

    //
    // PnP and Power IRP's
    //
	DriverObject->MajorFunction[ IRP_MJ_PNP ]           = Dispatch_PNP;
    DriverObject->MajorFunction[IRP_MJ_POWER]           = Dispatch_Power;
    DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL]  = Dispatch_System;
	DriverObject->DriverExtension->AddDevice            = AddDevice_USBM300;


    OurDebugPrint((DBG_INFO, "<< DriverEntry Exit\n"));
	return STATUS_SUCCESS;
}


NTSTATUS Create_USBM300(IN PDEVICE_OBJECT DeviceObject, 
                        IN PIRP Irp )
{
	NTSTATUS ntStatus;
    OurDebugPrint((DBG_INFO, ">> Create_USBM300\n"));

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	ntStatus = Irp->IoStatus.Status;
	IoCompleteRequest( Irp, IO_NO_INCREMENT );

    //
    // Don't get cute and try to access the IRP structure after the IoCompleteRequest call
    // since the IRP may be freed upon return. Bad pointer error.
    //

    OurDebugPrint((DBG_INFO, "<< Create_USBM300 Exit Status= 0x%08X\n", ntStatus));
	return ntStatus;
}


NTSTATUS Close_USBM300(IN PDEVICE_OBJECT DeviceObject, 
                       IN PIRP Irp)
{
	NTSTATUS ntStatus;
    OurDebugPrint((DBG_INFO, "Close_USBM300\n"));

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	ntStatus = Irp->IoStatus.Status;
	IoCompleteRequest( Irp, IO_NO_INCREMENT );

    //
    // Don't get cute and try to access the IRP structure after the IoCompleteRequest call
    // since the IRP may be freed upon return. Bad pointer error.
    //
	return ntStatus;
}


VOID Unload_USBM300(IN PDRIVER_OBJECT DriverObject)
{
    OurDebugPrint((DBG_INFO, "Unload_USBM300\n"));
    return;
}


NTSTATUS AddDevice_USBM300(IN PDRIVER_OBJECT DriverObject, 
                           IN PDEVICE_OBJECT PhysicalDeviceObject)
{
	NTSTATUS			ntStatus = STATUS_SUCCESS;
	PDEVICE_OBJECT		deviceObject = NULL;
	PDEVICE_EXTENSION	deviceExtension;

    OurDebugPrint((DBG_INFO, "AddDevice_USBM300  gulNameCount=%li  gulNumDevices=%li\n", gulNameCount, gulNumDevices));

	ASSERT( gulNameCount < kMaxNumInstances && gulNumDevices < kMaxNumInstances );

	ntStatus = CreateDO(DriverObject, PhysicalDeviceObject, &deviceObject);

	if( NT_SUCCESS( ntStatus ) )
	{
        //
        // This prevents a race condition if AddDevice gets called re-entrantly
        //
	    InterlockedIncrement(&gulNameCount);
	    InterlockedIncrement(&gulNumDevices);

		deviceExtension = deviceObject->DeviceExtension;

        //
        // Setting this flag causes the driver to not receive a IRP_MN_STOP_DEVICE
        // during suspend and also not get an IRP_MN_START_DEVICE during resume.
        // This is neccesary because during the start device call,
        // the GetDescriptors() call  will be failed by the USB stack.
        //
        deviceObject->Flags |= DO_POWER_PAGABLE;

        //
        // we support buffered io for read/write
        //
        deviceObject->Flags |= DO_BUFFERED_IO;

        //
        // Handle the Started and Removed flags
        //
        deviceExtension->DeviceStarted = FALSE;
        deviceExtension->Removed = FALSE;

		deviceExtension->PhysicalDeviceObject = PhysicalDeviceObject;
		deviceExtension->StackDeviceObject = IoAttachDeviceToDeviceStack(deviceObject, PhysicalDeviceObject);

        //
        // Done initializing
        //
		deviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

		ASSERT( deviceExtension->StackDeviceObject != NULL );
	}

	return ntStatus;
}


NTSTATUS CreateDO(IN PDRIVER_OBJECT DriverObject, 
                  IN PDEVICE_OBJECT PhysicalDeviceObject,
                  IN PDEVICE_OBJECT *DeviceObject)
{
	NTSTATUS			ntStatus;
	UNICODE_STRING		deviceLinkUnicodeString;
	PDEVICE_EXTENSION	deviceExtension;

    //
    //  Create the symbolic link
    //     
    // IoRegisterDeviceInterface registers device functionality (a device interface) 
    // that a driver will enable for use by applications or other system components.
    //
    ntStatus = IoRegisterDeviceInterface(
                PhysicalDeviceObject,
                (LPGUID)&GUID_CLASS_USBM300,
                NULL,
                &deviceLinkUnicodeString);

    if (NT_SUCCESS(ntStatus)) {
        //
        // IoSetDeviceInterfaceState enables or disables a previously 
        // registered device interface. Applications and other system components 
        // can open only interfaces that are enabled.
        //
        ntStatus = IoSetDeviceInterfaceState(&deviceLinkUnicodeString, TRUE);

    } else {
        return (ntStatus);
    }

	ntStatus = IoCreateDevice(DriverObject, 
                              sizeof(DEVICE_EXTENSION), 
                              NULL,                             // &deviceNameUnicodeString,
		                      FILE_DEVICE_UNKNOWN,              // Unkown device type
                              FILE_AUTOGENERATED_DEVICE_NAME,   // 0, 
                              FALSE,                            // 
                              DeviceObject);

	if( NT_SUCCESS( ntStatus ) )
	{
		deviceExtension = (PDEVICE_EXTENSION)( (*DeviceObject)->DeviceExtension );
        RtlZeroMemory(deviceExtension, sizeof (DEVICE_EXTENSION));


        // Name buffer for our named Functional device object link
        // The name is generated based on the driver's class GUID
        RtlCopyMemory(deviceExtension->DeviceLinkNameBuffer,
                      deviceLinkUnicodeString.Buffer,
                      deviceLinkUnicodeString.Length);

		deviceExtension->ConfigurationHandle = NULL;
		deviceExtension->DeviceDescriptor = NULL;
		deviceExtension->NeedCleanup = FALSE;
		deviceExtension->UsbInterface = NULL;
        deviceExtension->PipeInfo = NULL;

        deviceExtension->MaximumTransferSize = 256;

		//free buffer from unicode string we used to init interface
		RtlFreeUnicodeString(&deviceLinkUnicodeString );
	}

	return ntStatus;
}


VOID Eval_Cleanup(PDEVICE_OBJECT DeviceObject)
{
    PDEVICE_EXTENSION	deviceExtension;
    UNICODE_STRING		deviceLinkUnicodeString;
	NTSTATUS			ntStatus;

    deviceExtension = DeviceObject->DeviceExtension;

    if( deviceExtension->NeedCleanup )
	{
		deviceExtension->NeedCleanup = FALSE;

        RtlInitUnicodeString (&deviceLinkUnicodeString,
                              deviceExtension->DeviceLinkNameBuffer);
        //
        // remove the GUID-based symbolic link
        //
        ntStatus = IoSetDeviceInterfaceState(&deviceLinkUnicodeString, FALSE);

        ASSERT(NT_SUCCESS(ntStatus));
    }
}


NTSTATUS Dispatch_IOCTL(IN PDEVICE_OBJECT DeviceObject, 
                        IN PIRP Irp)
{
	PIO_STACK_LOCATION irpStack;
	PVOID ioBuffer;
	ULONG inputBufferLength;
	ULONG outputBufferLength;
	ULONG ioControlCode;
	NTSTATUS ntStatus;

	PDEVICE_EXTENSION deviceExtension = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;

    OurDebugPrint((DBG_INFO, "Dispatch_IOCTL\n"));

	irpStack = IoGetCurrentIrpStackLocation( Irp );
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	ioBuffer = Irp->AssociatedIrp.SystemBuffer;
	inputBufferLength = irpStack->Parameters.DeviceIoControl.InputBufferLength;
	outputBufferLength = irpStack->Parameters.DeviceIoControl.OutputBufferLength;
	ioControlCode = irpStack->Parameters.DeviceIoControl.IoControlCode;

    OurDebugPrint((DBG_INFO, "Dispatch_IOCTL code= 0x%08X\n", ioControlCode));

	switch( ioControlCode )
	{
		case IOCTL_Eval_BULK_WRITE:
            OurDebugPrint((DBG_INFO, "IOCTL_Eval_BULK_WRITE\n"));
            if  ((ioBuffer != NULL) && (inputBufferLength > 4)) {
				Eval_Read_Write(DeviceObject, Irp, FALSE);
            } else {
                OurDebugPrint((DBG_ERROR, "IOCTL_Eval_BULK_WRITE bad parameter ioBuffer=%p  inputBufferLength=%i\n", ioBuffer, inputBufferLength));
                Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            }
			break;

		case IOCTL_Eval_BULK_OR_INTERRUPT_READ:
            OurDebugPrint((DBG_INFO, "IOCTL_Eval_BULK_OR_INTERRUPT_READ\n"));
            if  ((ioBuffer != NULL) && (inputBufferLength >= 4) && (outputBufferLength > 0)) {
				Eval_Read_Write(DeviceObject, Irp, TRUE);
            } else {
                OurDebugPrint((DBG_ERROR, "IOCTL_Eval_BULK_OR_INTERRUPT_READ bad parameter ioBuffer=%p  inputBufferLength=%i outputBufferLength=%i\n", ioBuffer, inputBufferLength, outputBufferLength));
                Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
            }
			break;

		case IOCTL_Eval_GET_NUM_DEVICES:
            OurDebugPrint((DBG_INFO, "IOCTL_Eval_GET_NUM_DEVICES\n"));
			ioBuffer = Irp->AssociatedIrp.SystemBuffer;
			if( irpStack->Parameters.DeviceIoControl.OutputBufferLength >= sizeof( USHORT ) )
			{
				*(USHORT*)ioBuffer = (USHORT)gulNumDevices;
				Irp->IoStatus.Information = sizeof( USHORT );
			}
			break;

		default:
			Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
			break;
	}

	ntStatus = Irp->IoStatus.Status;
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
    //
    // Don't get cute and try to access the IRP structure after the IoCompleteRequest call
    // since the IRP may be freed upon return. Bad pointer error.
    //

	return ntStatus;
}


NTSTATUS Eval_Read_Write(IN PDEVICE_OBJECT DeviceObject, 
                         IN PIRP Irp, 
                         IN BOOLEAN Read)
{
	USBD_INTERFACE_INFORMATION* pInterfaceInfo;
	USBD_PIPE_INFORMATION*      pPipeInfo;
	PIO_STACK_LOCATION          irpStack;
	PDEVICE_EXTENSION           deviceExtension;
	NTSTATUS            ntStatus;
	PVOID               ioBuffer;
	PCHAR               pcTempBuffer;
	ULONG               length;
	PULONG              pPipeNum;
	ULONG               inputBufferLength;
	ULONG               outputBufferLength;
	ULONG               siz, ulTransferFlags;
	PURB                urb;

    // TODO: Lock exclusive access to prevent multiple threads from calling concurrently.
    //       The lower USB device stack is not multi-thread safe

	irpStack = IoGetCurrentIrpStackLocation( Irp );
	deviceExtension = DeviceObject->DeviceExtension;
	ioBuffer = Irp->AssociatedIrp.SystemBuffer;
	inputBufferLength = irpStack->Parameters.DeviceIoControl.InputBufferLength;
	outputBufferLength = irpStack->Parameters.DeviceIoControl.OutputBufferLength;
	pInterfaceInfo = deviceExtension->UsbInterface;
	pPipeNum = (PULONG)ioBuffer;
	pPipeInfo = &( pInterfaceInfo->Pipes[ *pPipeNum ] );

	siz = sizeof( struct _URB_BULK_OR_INTERRUPT_TRANSFER );
	urb = ExAllocatePoolWithTag(NonPagedPool, siz, USBM300_TAG);

    OurDebugPrint((DBG_INFO, "Eval_Read_Write PIPE=%i\n", *pPipeNum));

    OurDebugPrint((DBG_INFO, "-> Pipe %u\n", *pPipeNum));
    OurDebugPrint((DBG_INFO, "   PipeType 0x%x\n", pPipeInfo->PipeType));
    OurDebugPrint((DBG_INFO, "   EndpointAddress 0x%x\n", pPipeInfo->EndpointAddress));
    OurDebugPrint((DBG_INFO, "   MaxPacketSize 0x%x\n", pPipeInfo->MaximumPacketSize));
    OurDebugPrint((DBG_INFO, "   Interval 0x%x\n", pPipeInfo->Interval));
    OurDebugPrint((DBG_INFO, "   Handle 0x%x\n", pPipeInfo->PipeHandle));
    OurDebugPrint((DBG_INFO, "   MaximumTransferSize 0x%x\n", pPipeInfo->MaximumTransferSize));

	// In a read operation, data is passed back to the application via the output buffer. In
	// a write application, input buffer has data that must be sent to the device. The first
	// four bytes of the buffer contain the pipe number.

    if (Read) {
        //
        // Read 
        //
        pcTempBuffer = (char*)ioBuffer;
		length = outputBufferLength;
        ulTransferFlags = USBD_SHORT_TRANSFER_OK | USBD_TRANSFER_DIRECTION_IN;
    } else {
        //
        // Write - move xmit pointer past ULONG PIPE number
        //
        pcTempBuffer = (char*)ioBuffer + sizeof(ULONG);
		length = inputBufferLength - sizeof( ULONG );
        ulTransferFlags = 0;
    }

    OurDebugPrint((DBG_INFO, "Eval_Read_Write length=%i ulTransferFlags=0x%08X\n", length, ulTransferFlags));

	UsbBuildInterruptOrBulkTransferRequest( urb, (USHORT)siz, pPipeInfo->PipeHandle,
		pcTempBuffer, NULL, length, ulTransferFlags, NULL );
	
	ntStatus = Eval_CallUSBD(DeviceObject, urb);

    OurDebugPrint((DBG_INFO, "Eval_Read_Write Eval_CallUSBD ntStatus = 0x%08X\n", ntStatus));

	Irp->IoStatus.Status = ntStatus;

    OurDebugPrint((DBG_INFO, "TransferBufferLength = 0x%08X\n", urb->UrbBulkOrInterruptTransfer.TransferBufferLength));

    if  ( NT_SUCCESS( ntStatus ) ) {
        if (Read) {
		    Irp->IoStatus.Information = urb->UrbBulkOrInterruptTransfer.TransferBufferLength;
        } else {
            Irp->IoStatus.Information = 0;
        }
    } else {
        OurDebugPrint((DBG_ERROR, "Read_Write(Read=%i) error = 0x%08X\n", Read, ntStatus));
		Irp->IoStatus.Information = 0;
    }

	ExFreePool(urb);
	return( ntStatus );
}


NTSTATUS Eval_CallUSBD(IN PDEVICE_OBJECT DeviceObject, 
                       IN PURB Urb)
{
    NTSTATUS            ntStatus;
    PDEVICE_EXTENSION   deviceExtension;
    PIRP                irp;
    IO_STATUS_BLOCK     ioStatus;
    PIO_STACK_LOCATION  nextStack;
    LONG                timeoutLong = -20000000;	// = 2 seconds. Negative means that the timeout is relative.
	LARGE_INTEGER       timeout;
    KEVENT              event;

    deviceExtension = DeviceObject->DeviceExtension;

	KeInitializeEvent(&event, NotificationEvent, FALSE);

	irp = IoBuildDeviceIoControlRequest(IOCTL_INTERNAL_USB_SUBMIT_URB, 
                                        deviceExtension->StackDeviceObject,
			                            NULL, 
                                        0, 
                                        NULL, 
                                        0, 
                                        TRUE, 
                                        &event, 
                                        &ioStatus);

	nextStack = IoGetNextIrpStackLocation(irp);
	ASSERT( nextStack != NULL );

	nextStack->Parameters.Others.Argument1 = Urb;

	ntStatus = IoCallDriver(deviceExtension->StackDeviceObject, irp);

	if (ntStatus == STATUS_PENDING) { 
        OurDebugPrint((DBG_INFO, "Eval_CallUSBD pending wait\n"));
		timeout = RtlConvertLongToLargeInteger(timeoutLong);
		ntStatus = KeWaitForSingleObject(&event, 
                                         Executive, 
                                         KernelMode, 
                                         FALSE, 
                                         &timeout);     // &timeout

        if (ntStatus == STATUS_TIMEOUT) {

            OurDebugPrint((DBG_WARN, "Eval_CallUSBD timeout. Cancelling I/O and waiting for completion\n"));

            ntStatus = STATUS_IO_TIMEOUT;
            // Cancel the Irp we just sent.
            //
            IoCancelIrp(irp);

            // And wait until the cancel completes
            //
            KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
            OurDebugPrint((DBG_WARN, "Eval_CallUSBD timeout. Cancelling I/O complete\n"));
        }
        else {
            ntStatus = irp->IoStatus.Status;
        }
	}

    return ntStatus;
}


//
// TODO: Add comments
//
NTSTATUS EvalCompletionRoutine(IN PDEVICE_OBJECT DeviceObject,
                               IN PIRP Irp,IN PVOID Context)
{
    PKEVENT             event;
    event = (PKEVENT) Context;

    UNREFERENCED_PARAMETER(DeviceObject);

    if (Irp->PendingReturned) {
        IoMarkIrpPending(Irp);
    }

    //
    // We could switch on the major and minor functions of the IRP to perform
    // different functions, but we know that Context is an event that needs
    // to be set.
    //
    KeSetEvent(event, 0, FALSE);

    //
    // Allows the caller to reuse the IRP
    //
    return STATUS_MORE_PROCESSING_REQUIRED;
}


NTSTATUS Dispatch_PNP(IN PDEVICE_OBJECT DeviceObject, 
                      IN PIRP Irp)
{
	PIO_STACK_LOCATION	irpStack, nextStack;
	PDEVICE_EXTENSION	deviceExtension;
	NTSTATUS			ntStatus;
    KEVENT              event;

	deviceExtension = DeviceObject->DeviceExtension;
	irpStack = IoGetCurrentIrpStackLocation( Irp );

//	Irp->IoStatus.Status = STATUS_SUCCESS;
//	Irp->IoStatus.Information = 0;

    OurDebugPrint((DBG_INFO, "Dispatch_PNP  %s\n", PnPMinorFunctionString(irpStack->MinorFunction)));

	switch( irpStack->MinorFunction )
	{
		case IRP_MN_START_DEVICE:
            //
            // The device is starting.
            //
            // We cannot touch the device (send it any non pnp irps) until a
            // start device has been passed down to the lower drivers.
            //
            IoCopyCurrentIrpStackLocationToNext(Irp);
            KeInitializeEvent(&event, NotificationEvent, FALSE);

            IoSetCompletionRoutine(Irp,
                                   (PIO_COMPLETION_ROUTINE) EvalCompletionRoutine, 
                                   &event,
                                   TRUE,
                                   TRUE,
                                   TRUE);
                               
            ntStatus = IoCallDriver(deviceExtension->StackDeviceObject, Irp);

            if (ntStatus == STATUS_PENDING) {
                KeWaitForSingleObject(
                   &event,
                   Suspended,   // Waiting for reason of a driver
                   KernelMode,  // Must be kernelmode if event memory is in stack
                   FALSE,       // No allert
                   NULL);       // No timeout

                ntStatus = Irp->IoStatus.Status;
            }

            if (NT_SUCCESS(ntStatus)) {
			    ntStatus = Eval_StartDevice( DeviceObject );

                if(NT_SUCCESS(ntStatus))
                {
                    //
                    // As we are successfully now back from our start device
                    // we can do work.
                    //
                    deviceExtension->DeviceStarted = TRUE;
                    deviceExtension->Removed = FALSE;
                }
            }

            //
            // We must now complete the IRP, since we stopped it in the
            // completion routine with STATUS_MORE_PROCESSING_REQUIRED.
            //
            Irp->IoStatus.Status = ntStatus;
            IoCompleteRequest(Irp, IO_NO_INCREMENT);
            //
            // Don't get cute and try to access the IRP structure after the IoCompleteRequest call
            // since the IRP may be freed upon return. Bad pointer error.
            //

			break;

            //
            // TODO: Implement IRP_MN_QUERY_STOP_DEVICE
            //

		case IRP_MN_STOP_DEVICE:

            // The PnP Manager sends this IRP to stop a device so it can reconfigure
            // its hardware resources. The PnP Manager only sends this IRP if a prior
            // IRP_MN_QUERY_STOP_DEVICE completed successfully.

			Eval_Cleanup( DeviceObject );
			ntStatus = Eval_StopDevice( DeviceObject );
			break;

        case IRP_MN_SURPRISE_REMOVAL:

            //
            // The device has been unexpectedly removed from the machine 
            // and is no longer available for I/O. Stop all access to the device.
            // Release any resources associated with the device, but leave the 
            // device object attached to the device stack until the PnP Manager 
            // sends a subsequent IRP_MN_REMOVE_DEVICE request. 
            // You should fail any outstanding I/O to the device. You will
            // not get a remove until all the handles open to the device
            // have been closed.
            //

            deviceExtension->Removed = TRUE;
            deviceExtension->DeviceStarted = FALSE;

            //
            // TODO: Cancel any outstanding I/O and Abort all pipes
            //
            // TODO:
            // Cancel any pending io requests; we may not have gotten a query first!
            // Usb_CancelPendingIo( DeviceObject );
            //
            // TODO:
            // If any pipes are still open, call USBD with URB_FUNCTION_ABORT_PIPE
            // This call will also close the pipes; if any user close calls get through,
            // they will be noops
            // Usb_AbortPipes( DeviceObject );

            //
            // Mark this handled
            //
            Irp->IoStatus.Status = STATUS_SUCCESS;

            // We don't explicitly wait for the below driver to complete, but just make
            // the call and go on, finishing cleanup
            IoCopyCurrentIrpStackLocationToNext(Irp);
            ntStatus = IoCallDriver(deviceExtension->StackDeviceObject, Irp);
            return ntStatus;

		case IRP_MN_REMOVE_DEVICE:

            //
            // Relinquish all resources here.
            // Detach and delete the device object so that
            // your driver can be unloaded. You get remove
            // either after query_remove or surprise_remove.
            //
            if (!deviceExtension->Removed)
            {
                deviceExtension->Removed = TRUE;
                deviceExtension->DeviceStarted = FALSE;
            }

            //
            // TODO: Cancel any outstanding I/O and Abort all pipes
            //
            // TODO:
            // Cancel any pending io requests; we may not have gotten a query first!
            // Usb_CancelPendingIo( DeviceObject );
            //
            // TODO:
            // If any pipes are still open, call USBD with URB_FUNCTION_ABORT_PIPE
            // This call will also close the pipes; if any user close calls get through,
            // they will be noops
            // Usb_AbortPipes( DeviceObject );

            OurDebugPrint((DBG_INFO, "Fowarding REMOVE to next driver\n"));

            // IoSkipCurrentIrpStackLocation(Irp);
            IoCopyCurrentIrpStackLocationToNext(Irp);
            ntStatus = IoCallDriver(deviceExtension->StackDeviceObject , Irp);


			//Check the USBSample in the new beta to see if this changed.
			Eval_Cleanup( DeviceObject );
			Eval_RemoveDevice( DeviceObject );

			IoDetachDevice( deviceExtension->StackDeviceObject );
			IoDeleteDevice( DeviceObject );

			InterlockedDecrement(&gulNumDevices);
			break;

        default:
            //
            // Pass all IRP's not implemented.
            //
            IoCopyCurrentIrpStackLocationToNext(Irp);
            ntStatus = IoCallDriver(deviceExtension->StackDeviceObject, Irp);
            break;
	}

	return ntStatus;
}


void Eval_RemoveDevice(IN PDEVICE_OBJECT DeviceObject)
{
	PDEVICE_EXTENSION	deviceExtension;

	deviceExtension = DeviceObject->DeviceExtension;

    if (deviceExtension->DeviceDescriptor != NULL) {
        OurDebugPrint((DBG_INFO, "Free DeviceDescriptor = %p\n", deviceExtension->DeviceDescriptor));
		ExFreePool(deviceExtension->DeviceDescriptor);
        deviceExtension->DeviceDescriptor = NULL;
    }

    if (deviceExtension->UsbInterface != NULL) {
        OurDebugPrint((DBG_INFO, "Free UsbInterface = %p\n", deviceExtension->UsbInterface));
		ExFreePool(deviceExtension->UsbInterface);
        deviceExtension->UsbInterface = NULL;
    }

    if (deviceExtension->PipeInfo != NULL) {
        OurDebugPrint((DBG_INFO, "Free PipeInfo = %p\n", deviceExtension->PipeInfo));
		ExFreePool(deviceExtension->PipeInfo);
        deviceExtension->PipeInfo = NULL;
    }

    return;
}


NTSTATUS Eval_StopDevice(IN PDEVICE_OBJECT DeviceObject)
{
	PDEVICE_EXTENSION deviceExtension;
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PURB urb;
	ULONG siz;

	deviceExtension = DeviceObject->DeviceExtension;
	siz = sizeof( struct _URB_SELECT_CONFIGURATION );
	urb = ExAllocatePoolWithTag(NonPagedPool, siz, USBM300_TAG);

	if (urb) {
        //
        // Send a NULL configuration to stop
        //
		UsbBuildSelectConfigurationRequest(urb, (USHORT)siz, NULL );

		ntStatus = Eval_CallUSBD( DeviceObject, urb );
		ExFreePool( urb );
    } else {
		ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    }

    if (NT_SUCCESS(ntStatus)) {
        deviceExtension->DeviceStarted = FALSE;
    }

	return ntStatus;
}



NTSTATUS Eval_StartDevice(IN PDEVICE_OBJECT DeviceObject)
{
	PDEVICE_EXTENSION deviceExtension;
	NTSTATUS ntStatus;
	PUSB_DEVICE_DESCRIPTOR deviceDescriptor = NULL;
	PURB urb;
	ULONG siz;

	deviceExtension = DeviceObject->DeviceExtension;
	deviceExtension->NeedCleanup = TRUE;

    //
    // Allocate an URB for the DEVICE descriptor request.
    //
	urb = ExAllocatePoolWithTag(NonPagedPool, sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST) ,USBM300_TAG);

	if (urb) {

        OurDebugPrint((DBG_INFO, "Get DEVICE descriptor URB = %p\n", urb));
        //
        // Allocate the DEVICE descriptor
        //
		siz = sizeof( USB_DEVICE_DESCRIPTOR );
		deviceDescriptor = ExAllocatePoolWithTag(NonPagedPool, siz, USBM300_TAG);

		if (deviceDescriptor) {

            OurDebugPrint((DBG_INFO, "Get DEVICE descriptor = %p\n", deviceDescriptor));

            //
            // Build the DEVICE descriptor request
            //
			UsbBuildGetDescriptorRequest(urb, 
                                         (USHORT)sizeof( struct _URB_CONTROL_DESCRIPTOR_REQUEST ),
				                         USB_DEVICE_DESCRIPTOR_TYPE, 
                                         0, 
                                         0, 
                                         deviceDescriptor, 
                                         NULL, 
                                         siz, 
                                         NULL );
  			//
            // Send the URB to the USB device
            //
			ntStatus = Eval_CallUSBD(DeviceObject, urb);
        } else {
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
        }

		if (NT_SUCCESS(ntStatus)) {
            //
            // Save off the device descriptor
            //
			deviceExtension->DeviceDescriptor = deviceDescriptor;
			deviceExtension->Stopped = FALSE;
        } else {
            //
            // Failed to get a DEVICE descriptor back from the device.
            // Free the memory
            //
            if (deviceDescriptor != NULL) {
                OurDebugPrint((DBG_INFO, "Free StartDevice deviceDescriptor = %p\n", deviceDescriptor));
			    ExFreePool(deviceDescriptor);
			    deviceExtension->DeviceDescriptor = NULL;
            }
		}
		
		ExFreePool(urb);
    } else {
		ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    }

    //
    // Configure the USB device
    //
    if (NT_SUCCESS(ntStatus)) {
        OurDebugPrint((DBG_INFO, "Get DEVICE descriptor success\n"));
		ntStatus = Eval_ConfigureDevice( DeviceObject );
    } else {
        OurDebugPrint((DBG_INFO, "Get DEVICE descriptor failed with status = 0x%08X\n", ntStatus));
    }

    return ntStatus;
}


NTSTATUS Eval_ConfigureDevice(IN PDEVICE_OBJECT DeviceObject)
{
	PDEVICE_EXTENSION deviceExtension;
	NTSTATUS ntStatus;
	PURB urb = NULL;
	ULONG siz;
	PUSB_CONFIGURATION_DESCRIPTOR configurationDescriptor = NULL;

    //
    // Allocate an URB for the CONFIGURATION descriptor request
    //
	deviceExtension = DeviceObject->DeviceExtension;
	urb = ExAllocatePoolWithTag(NonPagedPool, sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST), USBM300_TAG);
	
	if (urb) {

        OurDebugPrint((DBG_INFO, "Get CONFIGURATION descriptor URB = %p\n", urb));

        //
        // Allocate the CONFIGURATION descriptor. Allow extra space for the decriptor
        // otherwise we'll have to do it twice.
        //
        siz = sizeof( USB_CONFIGURATION_DESCRIPTOR ) + 512;
        configurationDescriptor = ExAllocatePoolWithTag(NonPagedPool, siz, USBM300_TAG);

		if (configurationDescriptor) {

            OurDebugPrint((DBG_INFO, "Get CONFIGURATION descriptor = %p\n", configurationDescriptor));
            //
            // Build and get the CONFIGURATION descriptor 
            //
			UsbBuildGetDescriptorRequest(urb,
                                         (USHORT)sizeof( struct _URB_CONTROL_DESCRIPTOR_REQUEST ),
                                         USB_CONFIGURATION_DESCRIPTOR_TYPE, 
                                         0, 
                                         0, 
                                         configurationDescriptor,
                                         NULL, 
                                         siz, 
                                         NULL );
            //
            // Send the URB to the USB device
            //
			ntStatus = Eval_CallUSBD(DeviceObject, urb);  

			if (NT_SUCCESS(ntStatus)) {
                //
                // Build and get the complete CONFIGURATION descriptor 
                //
				siz = configurationDescriptor->wTotalLength + 16;
				ExFreePool(configurationDescriptor);
				configurationDescriptor = NULL;
				configurationDescriptor = ExAllocatePoolWithTag(NonPagedPool, siz, USBM300_TAG);

				if (configurationDescriptor) {

                    OurDebugPrint((DBG_INFO, "Get CONFIGURATION descriptor (2) = %p\n", configurationDescriptor));
                    //
					// We ask for wTotalLength + 16 bytes of data to fill the buffer --
					// might just want to get wTotalLength while still padding the internal buffer.
                    //
					UsbBuildGetDescriptorRequest(urb,
						                         (USHORT)sizeof( struct _URB_CONTROL_DESCRIPTOR_REQUEST ),
						                         USB_CONFIGURATION_DESCRIPTOR_TYPE, 
                                                 0, 
                                                 0, 
                                                 configurationDescriptor,
                                                 NULL, 
                                                 siz, 
                                                 NULL);
                    //
                    // Send the URB to the USB device
                    //
					ntStatus = Eval_CallUSBD(DeviceObject, urb);

                    //
                    // Select which interface to use
                    //
                    if (NT_SUCCESS(ntStatus)) {
                        OurDebugPrint((DBG_INFO, "Get CONFIGURATION descriptor success\n"));
				        ntStatus = Eval_SelectInterface(DeviceObject, configurationDescriptor);
                    } else {
                        OurDebugPrint((DBG_INFO, "Get CONFIGURATION descriptor (2) failed with status = 0x%08X\n", ntStatus));
                    }
                } else {
					ntStatus = STATUS_INSUFFICIENT_RESOURCES;
                }
            } else {
                OurDebugPrint((DBG_INFO, "Get CONFIGURATION descriptor (1) failed with status = 0x%08X\n", ntStatus));
            }
        } else {
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
        }
    } else {
		ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    }

    if (urb) {
		ExFreePool(urb);
    }

    if (configurationDescriptor) {
		ExFreePool(configurationDescriptor);
    }

	return ntStatus;
}


NTSTATUS Eval_SelectInterface(IN PDEVICE_OBJECT DeviceObject, 
                              IN PUSB_CONFIGURATION_DESCRIPTOR	ConfigurationDescriptor)
{
    PDEVICE_EXTENSION deviceExtension;
    NTSTATUS ntStatus;
    PURB urb = NULL;
    ULONG i;
    PUSB_INTERFACE_DESCRIPTOR interfaceDescriptor = NULL;
	PUSBD_INTERFACE_INFORMATION Interface = NULL;
    USHORT siz;
    PUCHAR pInf;

    OurDebugPrint((DBG_INFO, ">> Eval_SelectInterface\n"));

    deviceExtension = DeviceObject->DeviceExtension;

    //
    // BulkUsb driver only supports one interface, we must parse
    // the configuration descriptor for the interface 
    // and remember the pipes.
    //
    urb = USBD_CreateConfigurationRequest(ConfigurationDescriptor, &siz);

    if (urb) {

		//
		// USBD_ParseConfigurationDescriptorEx searches a given configuration
		// descriptor and returns a pointer to an interface that matches the 
		//  given search criteria. We only support one interface on this device
		//
        interfaceDescriptor =
            USBD_ParseConfigurationDescriptorEx(ConfigurationDescriptor,
								  ConfigurationDescriptor, //search from start of config  descriptro
								  -1,	// interface number not a criteria; we only support one interface
								  -1,   // not interested in alternate setting here either
								  -1,   // interface class not a criteria
								  -1,   // interface subclass not a criteria
								  -1    // interface protocol not a criteria
								  );

		if (!interfaceDescriptor) {

			OurDebugPrint((DBG_INFO, "BulkUsb_SelectInterface() ParseConfigurationDescriptorEx() failed\n  returning STATUS_INSUFFICIENT_RESOURCES\n"));
			ExFreePool(urb);
			return STATUS_INSUFFICIENT_RESOURCES;
		}

        Interface = &urb->UrbSelectConfiguration.Interface;


        // allocate space for an array of pipe information structs;
        //  in this basic sample, just used to track if opened/closed
        deviceExtension->PipeInfo = ExAllocatePoolWithTag(NonPagedPool, 
                                                          Interface->NumberOfPipes * sizeof (BULKUSB_PIPEINFO),
                                                          USBM300_TAG);
        if ( !deviceExtension->PipeInfo ) {
		    return STATUS_INSUFFICIENT_RESOURCES;
        }

        RtlZeroMemory(deviceExtension->PipeInfo,
            Interface->NumberOfPipes * sizeof ( BULKUSB_PIPEINFO ) );

        pInf = (PUCHAR ) deviceExtension->PipeInfo;

        for (i=0; i< Interface->NumberOfPipes; i++) {
            //
            // Perform any pipe initialization here;
			// We set the max transfer size and any Pipe flags we use; USBD sets the rest
			// of the Interface struct members
            //
            Interface->Pipes[i].MaximumTransferSize = deviceExtension->MaximumTransferSize;
            ( (PBULKUSB_PIPEINFO) pInf)->fPipeOpened = FALSE;
            pInf += sizeof ( BULKUSB_PIPEINFO );
        }

        //
        // Select this configuration (there is only one supported
        //
        UsbBuildSelectConfigurationRequest(urb, (USHORT) siz, ConfigurationDescriptor);

        ntStatus = Eval_CallUSBD(DeviceObject, urb);

        deviceExtension->ConfigurationHandle = urb->UrbSelectConfiguration.ConfigurationHandle;

    } else {
        OurDebugPrint((DBG_INFO, "Eval_SelectInterface USBD_CreateConfigurationRequest() failed\n  returning STATUS_INSUFFICIENT_RESOURCES\n"));
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
    }


    if (NT_SUCCESS(ntStatus)) {

        //
        // Save the configuration handle for this device
        //
        deviceExtension->ConfigurationHandle = urb->UrbSelectConfiguration.ConfigurationHandle;

        deviceExtension->UsbInterface = ExAllocatePoolWithTag(NonPagedPool,
                                                              Interface->Length,
                                                              USBM300_TAG);

        if (deviceExtension->UsbInterface) {
            ULONG j;

            //
            // save a copy of the interface information returned
            //
            RtlCopyMemory(deviceExtension->UsbInterface, Interface, Interface->Length);

            //
            // Dump the interface to the debugger
            //
            OurDebugPrint((DBG_INFO, "---------\n"));
            OurDebugPrint((DBG_INFO, "NumberOfPipes 0x%x\n", deviceExtension->UsbInterface->NumberOfPipes));
            OurDebugPrint((DBG_INFO, "Length 0x%x\n", deviceExtension->UsbInterface->Length));
            OurDebugPrint((DBG_INFO, "Alt Setting 0x%x\n", deviceExtension->UsbInterface->AlternateSetting));
            OurDebugPrint((DBG_INFO, "Interface Number 0x%x\n", deviceExtension->UsbInterface->InterfaceNumber));
            OurDebugPrint((DBG_INFO, "Class, subclass, protocol 0x%x 0x%x 0x%x\n",
                deviceExtension->UsbInterface->Class,
                deviceExtension->UsbInterface->SubClass,
                deviceExtension->UsbInterface->Protocol));

            // Dump the pipe info

            for (j=0; j<Interface->NumberOfPipes; j++) {
                PUSBD_PIPE_INFORMATION pipeInformation;

                pipeInformation = &deviceExtension->UsbInterface->Pipes[j];

                OurDebugPrint((DBG_INFO, "-> Pipe %u\n", j));
                OurDebugPrint((DBG_INFO, "   PipeType 0x%x\n", pipeInformation->PipeType));
                OurDebugPrint((DBG_INFO, "   EndpointAddress 0x%x\n", pipeInformation->EndpointAddress));
                OurDebugPrint((DBG_INFO, "   MaxPacketSize 0x%x\n", pipeInformation->MaximumPacketSize));
                OurDebugPrint((DBG_INFO, "   Interval 0x%x\n", pipeInformation->Interval));
                OurDebugPrint((DBG_INFO, "   Handle 0x%x\n", pipeInformation->PipeHandle));
                OurDebugPrint((DBG_INFO, "   MaximumTransferSize 0x%x\n", pipeInformation->MaximumTransferSize));
            }

            OurDebugPrint((DBG_INFO, "---------\n"));
        }
    }

    if (urb) {
        ExFreePool(urb);
    }
    OurDebugPrint((DBG_INFO, ">> Eval_SelectInterface (%x)\n", ntStatus));

	return ntStatus;
}



//
// Handles Power related IRP's
//
NTSTATUS Dispatch_Power(IN PDEVICE_OBJECT DeviceObject, 
                        IN PIRP Irp)
{
    PDEVICE_EXTENSION   deviceExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

    OurDebugPrint((DBG_INFO, "Dispatch_Power\n"));

    //
    // If the device has been removed, the driver should not pass 
    // the IRP down to the next lower driver.
    //
    if (deviceExtension->Removed) {
        
        PoStartNextPowerIrp(Irp);
        Irp->IoStatus.Status =  STATUS_DELETE_PENDING;
        IoCompleteRequest(Irp, IO_NO_INCREMENT );
        return STATUS_DELETE_PENDING;
    }

    IoCopyCurrentIrpStackLocationToNext(Irp);    
    PoStartNextPowerIrp(Irp);
    return PoCallDriver(deviceExtension->StackDeviceObject, Irp);
}


//
// Handles System Control IRP's (WMI)
//
NTSTATUS Dispatch_System(IN PDEVICE_OBJECT DeviceObject, 
                         IN PIRP Irp)
{
    PDEVICE_EXTENSION   deviceExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    OurDebugPrint((DBG_INFO, "Dispatch_System\n"));

    IoCopyCurrentIrpStackLocationToNext(Irp);
    return IoCallDriver(deviceExtension->StackDeviceObject, Irp);
}


