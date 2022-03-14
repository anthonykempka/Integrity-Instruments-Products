///////////////////////////////////////////////////////////////////////////////////////////////////
//
// USBM300.cpp: implementation of the USBM300 class.
//
// Copyright (c) 2003, Integrity Instruments, Inc.
//
// 1-800-450-2001
//
// http://www.integrityusa.com
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//
// Version 1.4
// Moved code to library form
//
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

#include "USBM300.h"

//
// Setup API stuff. 
//
// NOTE: You must have the SDK include and library paths defined in Visual Studio
//
#include <setupapi.h>

//
// USBM300 IOCTL and GUID includes
//
#include "devioctl.h"
#include "guid_usbm300.h"
#include "ioctl_usbm300.h"

// 
// Setup API library. Inform linker to use "SetupAPI.lib" library
//
#pragma comment(lib,"SetupAPI.lib") 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CHAR sCopyrightNotification[] = "Copyright (c) 2003-2004, Integrity Instruments, Inc.";



///////////////////////////////////////////////////////////////////////////////////////////////////
// DbgOut
//
// Prints a variable argument debug string to the debugger 
// Use like printf().. For example:  DbgOut("Output string index = %i\n", i);
//
// Parameters
// ----------
// FormatString     Format string like printf
// ...              Variable argument list
//
VOID DbgOut(LPSTR FormatString, ...)
{
    TCHAR   szString[1024];
    va_list ap;
    va_start(ap, FormatString);

    _vsnprintf(szString, 1024, FormatString, ap);
    OutputDebugString(szString);

    va_end(ap);
    return;
} 


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

USBM300::USBM300()
{
    DWORD   i;

    // Dump out class version
    GetClassVersion();

    // Initialize device structures
    for (i=0; i<MAX_USBM300_DEVICES; i++) {

        // NULL device name and handle
        m_sDeviceName[i] = NULL;
        m_hDevice[i] = INVALID_HANDLE_VALUE;

        // Zero out thread handle and ID
        m_hStreamThread[i] = INVALID_HANDLE_VALUE;
        m_dwStreamThreadId[i] = 0;

        //
        // Initial thread control settings
        //
        m_UsbStreamCtrl[i].dwDeviceIndex = 0;
        m_UsbStreamCtrl[i].hUsbDevice = INVALID_HANDLE_VALUE;
        m_UsbStreamCtrl[i].dwUsbStreamingState = USB_THREAD_NOT_STARTED;
        m_UsbStreamCtrl[i].dwStreamThreadIoError = 0;
        m_UsbStreamCtrl[i].dwQueueBlocksToAllocate = 0;
        m_UsbStreamCtrl[i].hDataReadyEvent = NULL;
        m_UsbStreamCtrl[i].lQueueCount = 0;
        m_UsbStreamCtrl[i].pQueue = NULL;
        m_UsbStreamCtrl[i].pHead = NULL;
        m_UsbStreamCtrl[i].pTail = NULL;
    }

    m_dwUSBM300DeviceCount = 0;

    // Perform the initial USBM300 device enumeration
    Enum_USBM300_Devices();
    return;
}


USBM300::~USBM300()
{
    DbgOut("USBM300 destructor called. Bye!\n");
    // Free all memory and handles
    FreeMemoryAndHandles();
    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Returns version number of the class
//
// Arguments:
// -----------
// none
//
// Return Value:
// --------------
// 0xAAAABBBB       Class version vAAAA.BBBB
//
ULONG USBM300::GetClassVersion()
{
    // Version 1.4
    // Major version is upper 16 bits
    // Minor version is lower 16 bits
    //
    ULONG   ulVersion = 0x00010004;

    ULONG   ulMajorVersion, ulMinorVersion;

    ulMajorVersion = ((ulVersion & 0xFFFF0000) >> 16);
    ulMinorVersion =  (ulVersion & 0x0000FFFF);

    DbgOut("USBM300 Class version %i.%i\n", ulMajorVersion, ulMinorVersion);
    DbgOut("%s\n", sCopyrightNotification);

    return (ulVersion);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Free memory and handles used. 
//
// NOTE: All streaming threads are stopped as a result of this function
//
void USBM300::FreeMemoryAndHandles()
{
    DWORD   i;

    for (i=0; i<MAX_USBM300_DEVICES; i++) {

        // TODO: Stop all stream threads

        // Close all thread handles
        if (m_hStreamThread[i] != INVALID_HANDLE_VALUE) {
            CloseHandle(m_hStreamThread[i]);
            m_hStreamThread[i] = INVALID_HANDLE_VALUE;
        }

        // Free all device names
        if (m_sDeviceName[i] != NULL) {
            free(m_sDeviceName[i]);
            m_sDeviceName[i] = NULL;
        }

        // Close all device handles
        if (m_hDevice[i] != INVALID_HANDLE_VALUE) {
            CloseHandle(m_hDevice[i]);
            m_hDevice[i] = INVALID_HANDLE_VALUE;
        }
    }
    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Enumerates all USBM300 devices currently connected to the system.
//
// NOTE: All streaming threads are stopped as a result of this function
//
void USBM300::Enum_USBM300_Devices()
{
    HDEVINFO                    hDevInfo;
    SP_INTERFACE_DEVICE_DATA    DeviceInterfaceData;
    ULONG                       i=0;

    PSP_INTERFACE_DEVICE_DETAIL_DATA    DeviceInterfaceDetailData = NULL;
    ULONG                               predictedLength = 0;
    ULONG                               requiredLength = 0;
    ULONG                               DeviceIndex = 0;
    ULONG                               DeviceNameLength;

    // Free all strings, close all handles, etc.
    FreeMemoryAndHandles();

    //
    // Get the USBM300 device list
    //
    hDevInfo = SetupDiGetClassDevs ((LPGUID)&GUID_CLASS_USBM400,
                                    NULL,                       // Define no enumerator (global)
                                    NULL,                       // Define no
                                    (DIGCF_PRESENT |            // Only Devices present
                                    DIGCF_INTERFACEDEVICE));    // Function class devices.

    if (hDevInfo == INVALID_HANDLE_VALUE) {
        return;
    }
  
    //
    // Loop through each device in the enumeration list
    //
    DeviceInterfaceData.cbSize = sizeof(DeviceInterfaceData);

    for(i=0; SetupDiEnumDeviceInterfaces (hDevInfo,
                                 0, // No care about specific PDOs
                                 (LPGUID)&GUID_CLASS_USBM400,
                                 i, //
                                 &DeviceInterfaceData); i++ ) {
   
        //
        // Establish the required structure length for the DeviceInterfaceData
        //
        SetupDiGetInterfaceDeviceDetail (
                hDevInfo,
                &DeviceInterfaceData,
                NULL, // probing so no output buffer yet
                0, // probing so output buffer length of zero
                &requiredLength,
                NULL); // not interested in the specific dev-node

        predictedLength = requiredLength;

        DeviceInterfaceDetailData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(predictedLength);
        DeviceInterfaceDetailData->cbSize = sizeof (SP_INTERFACE_DEVICE_DETAIL_DATA);

        //
        // Retrieve the information from Plug and Play.
        //
        if (SetupDiGetInterfaceDeviceDetail (
                   hDevInfo,
                   &DeviceInterfaceData,
                   DeviceInterfaceDetailData,
                   predictedLength,
                   &requiredLength,
                   NULL)) {

            //
            // Save off the device path - the name used in CreateFile()
            // 
            DeviceNameLength = strlen(DeviceInterfaceDetailData->DevicePath) + 2;
            m_sDeviceName[DeviceIndex] = (LPSTR)malloc(DeviceNameLength);
            ZeroMemory(m_sDeviceName[DeviceIndex], DeviceNameLength);
            strcpy(m_sDeviceName[DeviceIndex], DeviceInterfaceDetailData->DevicePath);
            // Open the device
            OpenDevice(DeviceIndex);
            DeviceIndex++;
        }
		free(DeviceInterfaceDetailData);
    }

    // Save off new device count
    m_dwUSBM300DeviceCount = DeviceIndex;

    DbgOut("%i USBM300 devices found.\n", m_dwUSBM300DeviceCount);

    // Free setup memory
    SetupDiDestroyDeviceInfoList (hDevInfo);
    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Opens a handle to the USBM300 device
//
// Arguments:
// -----------
// dwDeviceIndex    Zero based USBM300 device index
//
// Return Value:
// --------------
// TRUE             Success
// FAIL             Error
//
BOOLEAN USBM300::OpenDevice(DWORD dwDeviceIndex)
{
    // Device index range check
    if (dwDeviceIndex > MAX_USBM300_DEVICES) {
        DbgOut("Error opening USBM300 device. Index %i out of range.\n", dwDeviceIndex);
        return (FALSE);
    }

    // Is there a device name associated with this index
    if (m_sDeviceName[dwDeviceIndex] == NULL) {
        DbgOut("Error opening USBM300 device. Index %i has NULL device name.\n", dwDeviceIndex);
        return (FALSE);
    }

    // Close the device handle, if already opened
    if (m_hDevice[dwDeviceIndex] != INVALID_HANDLE_VALUE) {
        CloseHandle(m_hDevice[dwDeviceIndex]);
        m_hDevice[dwDeviceIndex] = INVALID_HANDLE_VALUE;
    }

    m_hDevice[dwDeviceIndex] = CreateFile (
                  m_sDeviceName[dwDeviceIndex],
                  GENERIC_READ | GENERIC_WRITE,
                  0,                                            // FIX: v1.2 No share -- was FILE_SHARE_READ | FILE_SHARE_WRITE,
                  NULL,                                         // no SECURITY_ATTRIBUTES structure
                  OPEN_EXISTING,                                // No special create flags
                  FILE_ATTRIBUTE_NORMAL,                        // No special attributes
                  NULL);                                        // No template file

    if (m_hDevice[dwDeviceIndex] == INVALID_HANDLE_VALUE) {
        DbgOut("Error opening USBM300 device index %i name %s\n", dwDeviceIndex, m_sDeviceName[dwDeviceIndex]);
        return (FALSE);
    } else {
        DbgOut("Opened USBM300 device index %i with handle 0x%08X\n", dwDeviceIndex, m_hDevice[dwDeviceIndex]);
    }

    return (TRUE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// 'C' Interface - needed by UsbThread 
//
// Sends a command and returns the response.
//
// Arguments:
// -----------
// hDevice          Handle to opened USBM300 device
// sCmd             Command string to send to module
// sResp            Response received from module
//
// Return Value:
// --------------
// TRUE             Success
// FAIL             Error
//
BOOLEAN C_SendCommand(HANDLE hDevice, LPSTR sCmd, LPSTR sResp)
{
    PVOID   pDataBuffer = NULL;
    PCHAR   pcCommand;
    PCHAR   pOutBuffer;
    PULONG  pulPipeNumber;
    DWORD   dwBytesReturned;
    DWORD   dwLength;
    BOOLEAN bStatus;
    DWORD   dwError;

    // Check for valid device handle
    if (hDevice == INVALID_HANDLE_VALUE) {
        DbgOut("Error sending USBM300 command. Handle %i is invalid.\n", hDevice);
        return (FALSE);
    }

    dwLength = strlen(sCmd);

    // Check for zero length command
    if (dwLength == 0) {
        strncpy(sResp, "no data", MAX_CMD_RESP);
        return (TRUE);
    }

    // FIX: v1.2 Check command length too long
    if (dwLength > 7) {
        DbgOut("Error sending USBM300 command. Command too long %i\n", dwLength);
        strncpy(sResp, "no data", MAX_CMD_RESP);
        return (FALSE);
    }

    // Allocate device I/O buffers
    pDataBuffer = malloc(1024);
    pOutBuffer = (PCHAR)malloc (1024);
    ZeroMemory(pDataBuffer, 1024);
    ZeroMemory(pOutBuffer, 1024);

    //
    // Copy the command string into the data buffer and send to USB device
    //
    pulPipeNumber = (PULONG)pDataBuffer;
    *pulPipeNumber = 1;             // PIPE1: Host PC->PIC
    pcCommand = (PCHAR)pDataBuffer+4;
    strncpy(pcCommand, sCmd, MAX_CMD_RESP);
    dwLength = 4 + strlen(sCmd);

    bStatus = DeviceIoControl(hDevice,
                              IOCTL_Eval_BULK_WRITE,
                              pDataBuffer,
                              dwLength,
                              NULL,
                              0,
                              &dwBytesReturned,
                              NULL);
    if (!bStatus) {
        // Error
        dwError = GetLastError();
        DbgOut("Error sending USBM300 command. Handle %i DeviceIoControl error code %i.\n", hDevice, dwError);
        free(pDataBuffer);
        free(pOutBuffer);
        return (FALSE);
    }

    //
    // Receive any data 
    //
    ZeroMemory(pDataBuffer, 1024);
    ZeroMemory(pOutBuffer, 1024);
    pulPipeNumber = (PULONG)pDataBuffer;
    *pulPipeNumber = 0;             // PIPE0: PIC->Host PC

    bStatus = DeviceIoControl(hDevice,
                              IOCTL_Eval_BULK_OR_INTERRUPT_READ,
                              pDataBuffer,
                              4,
                              pOutBuffer,
                              20,
                              &dwBytesReturned,
                              NULL);
    if (!bStatus) {
        // Error
        dwError = GetLastError();
        DbgOut("Error receiving USBM300 response. Handle %i DeviceIoControl error code %i.\n", hDevice, dwError);
        free(pDataBuffer);
        free(pOutBuffer);
        return (FALSE);
    }

    // Save response off
    pOutBuffer[dwBytesReturned] = 0;            // Null terminate
    strncpy(sResp, pOutBuffer, MAX_CMD_RESP);

    // Cleanup
    free(pDataBuffer);
    free(pOutBuffer);

    return (TRUE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Sends a command and returns the response.
//
// Arguments:
// -----------
// dwDeviceIndex    Zero based USBM300 device index
// sCmd             Command string to send to module
// sResp            Response received from module
//
// Return Value:
// --------------
// TRUE             Success
// FAIL             Error
//
BOOLEAN USBM300::SendCommand(DWORD dwDeviceIndex, LPSTR sCmd, LPSTR sResp)
{
    BOOLEAN bStatus;

    // Device index range check
    if (dwDeviceIndex > MAX_USBM300_DEVICES) {
        DbgOut("Error sending USBM300 command. Index %i out of range.\n", dwDeviceIndex);
        return (FALSE);
    }

    // Check for valid device handle
    if (m_hDevice[dwDeviceIndex] == INVALID_HANDLE_VALUE) {
        DbgOut("Error sending USBM300 command. Index %i handle is invalid.\n", dwDeviceIndex);
        return (FALSE);
    }

    // Send the command
    bStatus = ::C_SendCommand(m_hDevice[dwDeviceIndex], sCmd, sResp);

    return (bStatus);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Obtains the module address for a given device by reading EEPROM address 0x00
//
// Arguments:
// -----------
// dwDeviceIndex    Zero based USBM300 device index
//
// Return Value:
// --------------
// -1               No module address found
// 0...255          Module address
//
LONG USBM300::GetModuleAddress(DWORD dwDeviceIndex)
{
    CHAR    sCmd[MAX_CMD_RESP];
    CHAR    sResp[MAX_CMD_RESP];
//    UCHAR   ucModuleAddress;
    LONG    lModuleAddress = 0;

    // Device index range check
    if (dwDeviceIndex > MAX_USBM300_DEVICES) {
        DbgOut("GetModuleAddress failed. Index %i out of range.\n", dwDeviceIndex);
        return (-1);
    }

    // Check for valid device handle
    if (m_hDevice[dwDeviceIndex] == INVALID_HANDLE_VALUE) {
        DbgOut("GetModuleAddress failed. Index %i handle is invalid.\n", dwDeviceIndex);
        return (-1);
    }

    // Zero out the command and response buffers
    ZeroMemory(sCmd, MAX_CMD_RESP);
    ZeroMemory(sResp, MAX_CMD_RESP);

    // Send the Read EEPROM address 0 command
    strcpy(sCmd, "R00");
    if (SendCommand(dwDeviceIndex, sCmd, sResp)) {
        //if (sscanf(&sResp[1], "%x", &ucModuleAddress) == 1) {
		if (sscanf(&sResp[1], "%x", &lModuleAddress) == 1) {
            //lModuleAddress = ucModuleAddress;
            return (lModuleAddress);
        } else {
            // sscanf(0 error
            DbgOut("GetModuleAddress failed. sscanf failure\n");
            return (-1);
        }
    } else {
        // SendCommand error
        DbgOut("GetModuleAddress failed. SendCommand failure.\n");
        return (-1);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Determines if a duplicate module address is present among the USBM300 devices currently 
// attached to the system.
//
// Arguments:
// -----------
// none
//
// Return Value:
// --------------
// TRUE             Duplicate module address found in modules currently attached
// FAIL             Not duplicate module address found
//
BOOLEAN USBM300::DuplicateAddressPresent()
{
    ULONG       i, j;
    PLONG       pAddresses;
    BOOLEAN     bDuplicateFound = FALSE;

    // Alocate a buffer for the module addresses
    pAddresses = (PLONG)malloc(sizeof(LONG)*m_dwUSBM300DeviceCount);

    // Get the module addresses
    for (i=0; i<m_dwUSBM300DeviceCount; i++) {
        pAddresses[i] = GetModuleAddress(i);
    }

    // Check for a duplicate
    for (i=0; i<m_dwUSBM300DeviceCount; i++) {
        for (j=0; j<m_dwUSBM300DeviceCount; j++) {
            // Look for mathing address in locations other than the current
            if (i != j) {
                // Match found
                if (pAddresses[i] == pAddresses[j]) {
                    bDuplicateFound = TRUE;
                }
            }
        }
    }

    // Cleanup
    free(pAddresses);
    return(bDuplicateFound);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// USBM300 stream mode thread
//
//
DWORD WINAPI UsbStreamThread(LPVOID lpParameter)
{
    PVOID   pDataBuffer = NULL;
    PUSHORT pOutBuffer;
    PULONG  pulPipeNumber;
    DWORD   dwBytesReturned;
    BOOLEAN bStatus;
    DWORD   dwError = 0;
    DWORD   dwCount = 0;
    DWORD   dwStartTime, dwEndTime, i;
    CHAR    sCmd[MAX_CMD_RESP];
    CHAR    sResp[MAX_CMD_RESP];

    // Parameter block
    PUSB_STREAM_CONTROL  pUsbCtrl = (PUSB_STREAM_CONTROL)lpParameter;

    DbgOut ("UsbStreamThread device index %i started\n", pUsbCtrl->dwDeviceIndex);

    // Clear the command and response buffers
    ZeroMemory(sCmd, MAX_CMD_RESP);
    ZeroMemory(sResp, MAX_CMD_RESP);

    // Allocate the queue buffer 
    pUsbCtrl->pQueue = (PUSBM300_STREAM_BLOCK_QUEUE)malloc(pUsbCtrl->dwQueueBlocksToAllocate * sizeof(USBM300_STREAM_BLOCK_QUEUE));

    // Setup the queue element pointers
    for (i=0; i < (pUsbCtrl->dwQueueBlocksToAllocate-1); i++) {
        pUsbCtrl->pQueue[i].NextElement = &pUsbCtrl->pQueue[i+1];
    }
    pUsbCtrl->pQueue[i].NextElement = &pUsbCtrl->pQueue[0];

    // Head and tail to start of queue
    pUsbCtrl->pHead = &pUsbCtrl->pQueue[0];
    pUsbCtrl->pTail = &pUsbCtrl->pQueue[0];

    // Data buffers for DeviceIoControl() to USBM300 driver
    pDataBuffer = malloc(256);
    pOutBuffer = (PUSHORT)malloc (256);

    // Data ready event
    pUsbCtrl->hDataReadyEvent = CreateEvent(NULL,
                                            FALSE,      // Automatic event reset
                                            FALSE,      // Initial state not signaled
                                            NULL);      // not named

    // Send the start streaming command
    strcpy(sCmd, "S");
    if (C_SendCommand(pUsbCtrl->hUsbDevice, sCmd, sResp)) {
        // Nothing to do on success
    } else {
        // SendCommand error
        DbgOut("UsbStreamThread index %i Start stream SendCommand failure.\n", pUsbCtrl->dwDeviceIndex);
        pUsbCtrl->dwUsbStreamingState |= USB_THREAD_TERMINATE;
        return (1);
    }

    // Indicate the thread is running
    pUsbCtrl->dwUsbStreamingState |= USB_THREAD_RUNNING;

    // Performance calculations
    dwStartTime = GetTickCount();
    pUsbCtrl->dwTotalBytes = 0;

    // Check state variable for USB_THREAD_TERMINATE
    while (!(pUsbCtrl->dwUsbStreamingState & USB_THREAD_TERMINATE)) {

//        DbgOut ("\nUsbStreamThread getting packet %i\n", dwCount++);

        //
        // Receive any data from PIPE2
        //
        ZeroMemory(pDataBuffer, 256);
        ZeroMemory(pOutBuffer, 256);
        pulPipeNumber = (PULONG)pDataBuffer;
        *pulPipeNumber = 2;             // PIPE2: PIC->Host PC

        bStatus = DeviceIoControl(pUsbCtrl->hUsbDevice,
                                  IOCTL_Eval_BULK_OR_INTERRUPT_READ,
                                  pDataBuffer,
                                  4,
                                  pOutBuffer,
                                  64,
                                  &dwBytesReturned,
                                  NULL);
        if (!bStatus) {
            // Error
            dwError = GetLastError();
            DbgOut("UsbStreamThread %i Error receiving stream packet from USBM300. Error code = 0x%08X\n", pUsbCtrl->dwDeviceIndex, dwError);
            DbgOut("UsbStreamThread %i Error handle = 0x%08X\n", pUsbCtrl->dwDeviceIndex, pUsbCtrl->hUsbDevice);
            // Out of the loop
            // break;
            continue;
        } else {

//            DbgOut ("\nUsbStreamThread getting packet %i, bytes returned = %i\n", dwCount++, dwBytesReturned);
            pUsbCtrl->dwTotalBytes += dwBytesReturned;

            // Check queue full status
            if (pUsbCtrl->pHead->NextElement == pUsbCtrl->pTail) {
                DbgOut("UsbStreamThread %i queue full!\n", pUsbCtrl->dwDeviceIndex);
                continue;
            }

            // USBM300 Data is sent in the following format
            // (24) 16 bit A/D sample
            // (1)  16 bit digital input
            // (1)  32 bit Pulse counter

            // USBM400 Data is sent in the following format
            // (32) 16 bit A/D samples

            // Get A/D data from pOutBuffer and put into queue
            for (i=0; i<ADC_SAMPLE_COUNT; i++) {
                pUsbCtrl->pHead->Data.usAdcSamples[i] = pOutBuffer[i];
            }

            // Increment queue count and move queue head pointer
            InterlockedIncrement(&pUsbCtrl->lQueueCount);
            pUsbCtrl->pHead = pUsbCtrl->pHead->NextElement;

            // Signal event to GetStreamingData()
            SetEvent(pUsbCtrl->hDataReadyEvent);
        }
    }

    // Performance calculations
    dwEndTime = GetTickCount();
    pUsbCtrl->dwTotalMs = dwEndTime - dwStartTime;

    // Send the halt streaming command
    strcpy(sCmd, "H");
    if (C_SendCommand(pUsbCtrl->hUsbDevice, sCmd, sResp)) {
        //
        // Receive any REMAINING data from PIPE2
        //
        ZeroMemory(pDataBuffer, 256);
        ZeroMemory(pOutBuffer, 256);
        pulPipeNumber = (PULONG)pDataBuffer;
        *pulPipeNumber = 2;             // PIPE2: PIC->Host PC

        bStatus = DeviceIoControl(pUsbCtrl->hUsbDevice,
                                  IOCTL_Eval_BULK_OR_INTERRUPT_READ,
                                  pDataBuffer,
                                  4,
                                  pOutBuffer,
                                  64,
                                  &dwBytesReturned,
                                  NULL);
        if (!bStatus) {
            // Error
            dwError = GetLastError();
            DbgOut("UsbStreamThread %i Error receiving last stream packet from USBM300. Error code = 0x%08X\n", pUsbCtrl->dwDeviceIndex, dwError);
            DbgOut("UsbStreamThread %i Error handle = 0x%08X\n", pUsbCtrl->dwDeviceIndex, pUsbCtrl->hUsbDevice);
        }
    } else {
        // SendCommand error
        DbgOut("UsbStreamThread index %i Halt stream SendCommand failure.\n", pUsbCtrl->dwDeviceIndex);
    }

    // Free the event
    if (pUsbCtrl->hDataReadyEvent != NULL) {
        CloseHandle(pUsbCtrl->hDataReadyEvent);
        pUsbCtrl->hDataReadyEvent = NULL;
    }

    // Free DeviceIoControl() buffers
    free(pDataBuffer);
    free(pOutBuffer);

    // Free queue buffer
    if (pUsbCtrl->pQueue != NULL) {
        free(pUsbCtrl->pQueue);
        pUsbCtrl->pQueue = NULL;
    }

    // Return any error code
    pUsbCtrl->dwStreamThreadIoError = dwError;

    // Clear streaming state flags
    pUsbCtrl->dwUsbStreamingState &= (~USB_THREAD_RUNNING);
    pUsbCtrl->dwUsbStreamingState &= (~USB_THREAD_TERMINATE);

    DbgOut ("UsbStreamThread %i EXIT. dwError = %i\n\n", pUsbCtrl->dwDeviceIndex, dwError);

    return (0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Starts or stops the streaming thread
//
// Arguments:
// -----------
// dwDeviceIndex            Zero based USBM300 device index
// bStart                   TRUE to start stream thread, FALSE to stop
// dwQueueBlocksToAllocate  Number of stream blocks to allocate for the queue FIFO. 
//                          See USBM300_STREAM_BLOCK definition in USBM300.H
//
// Return Value:
// --------------
// TRUE                     Success
// FAIL                     Error
//
BOOLEAN USBM300::ControlStreaming(DWORD dwDeviceIndex, BOOLEAN bStart, DWORD dwQueueBlocksToAllocate)
{
    DWORD   dwError, dwWait, i;
    double  fBytes, fSeconds, fBytesPerSecond;

    DbgOut("ControlStreaming enter\n");

    // Device index range check
    if (dwDeviceIndex > MAX_USBM300_DEVICES) {
        DbgOut("ControlStreaming failed. Index %i out of range.\n", dwDeviceIndex);
        return (FALSE);
    }

    // Check for valid device handle
    if (m_hDevice[dwDeviceIndex] == INVALID_HANDLE_VALUE) {
        DbgOut("ControlStreaming failed. Index %i handle is invalid.\n", dwDeviceIndex);
        return (FALSE);
    }

    if (bStart) {
        // Is the thread already running?
        if (m_UsbStreamCtrl[dwDeviceIndex].dwUsbStreamingState & USB_THREAD_RUNNING) {
            DbgOut("ControlStreaming index %i thread already started.\n", dwDeviceIndex);
            return (TRUE);
        }

        // Setup parameter variables
        m_UsbStreamCtrl[dwDeviceIndex].dwDeviceIndex = dwDeviceIndex;
        m_UsbStreamCtrl[dwDeviceIndex].dwQueueBlocksToAllocate = dwQueueBlocksToAllocate;
        m_UsbStreamCtrl[dwDeviceIndex].dwUsbStreamingState = USB_THREAD_NOT_STARTED;
        m_UsbStreamCtrl[dwDeviceIndex].hUsbDevice = m_hDevice[dwDeviceIndex];
        m_UsbStreamCtrl[dwDeviceIndex].lQueueCount = 0;

        // Create the thread
        m_hStreamThread[dwDeviceIndex] = CreateThread(NULL, 
                                                      0, 
                                                      UsbStreamThread, 
                                                      &m_UsbStreamCtrl[dwDeviceIndex], 
                                                      0, 
                                                      &m_dwStreamThreadId[dwDeviceIndex]);

        // Check for thread creation error
        if (m_hStreamThread[dwDeviceIndex] == NULL) {
            dwError = GetLastError();
            DbgOut("ControlStreaming failed creating thread. Index %i error status = %i\n", dwDeviceIndex, dwError);
            return (FALSE);
        } else {
            // Wait for thread to start before returning
            i = 0;
            while (!(m_UsbStreamCtrl[dwDeviceIndex].dwUsbStreamingState & USB_THREAD_RUNNING)) {
                DbgOut("ControlStreaming waiting for thread to start count %i\n", i);
                Sleep(100);

                if (m_UsbStreamCtrl[dwDeviceIndex].dwUsbStreamingState & USB_THREAD_TERMINATE) {
                    // The thread killed itself
                    DbgOut("ControlStreaming index %i thread failed to start\n", dwDeviceIndex);
                    break;
                }

                i++;
                if (i > 50) {
                    DbgOut("ControlStreaming index %i bail waiting for thread to start\n", dwDeviceIndex);
                    break;
                }
            }
        }
    } else {
        // Stop the streaming thread

        // Is the thread already stopped?
        if (m_UsbStreamCtrl[dwDeviceIndex].dwUsbStreamingState & USB_THREAD_RUNNING) {

            // Flag the thread to terminate
            m_UsbStreamCtrl[dwDeviceIndex].dwUsbStreamingState |= USB_THREAD_TERMINATE;

            // Cancel any I/O in progress
            CancelIo(m_UsbStreamCtrl[dwDeviceIndex].hUsbDevice);

            // Wait for thread to exit. Timeout is 5 seconds (5,000 milliseconds)
            dwWait = WaitForSingleObject(m_hStreamThread[dwDeviceIndex], 5000);

            if (dwWait == WAIT_TIMEOUT) {
                DbgOut("ControlStreaming thread terminate wait timeout. Index %i \n", dwDeviceIndex);
                return (FALSE);
            } else {

                // Performance data calculations
                fBytes =  m_UsbStreamCtrl[dwDeviceIndex].dwTotalBytes;
                fSeconds =  m_UsbStreamCtrl[dwDeviceIndex].dwTotalMs;
                fSeconds = fSeconds / 1000.0;
                fBytesPerSecond = fBytes / fSeconds;
                // Display results
                DbgOut("Thread performance for index %i:\n", dwDeviceIndex);
                DbgOut("-> %i total bytes\n-> %i milliseconds\n", m_UsbStreamCtrl[dwDeviceIndex].dwTotalBytes, m_UsbStreamCtrl[dwDeviceIndex].dwTotalMs);
                DbgOut("-> %f bytes per second\n", fBytesPerSecond);

                // Close thread handle
                CloseHandle(m_hStreamThread[dwDeviceIndex]);
                m_hStreamThread[dwDeviceIndex] = INVALID_HANDLE_VALUE;

                // Reset thread state
                m_UsbStreamCtrl[dwDeviceIndex].dwUsbStreamingState = USB_THREAD_NOT_STARTED;
            }
        } else {
            // Thread is already stopped
            DbgOut("ControlStreaming thread already stopped. Index %i \n", dwDeviceIndex);
        }
    }

    return (TRUE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Returns the streaming thread status and blocks ready to be read from FIFO queue
//
// Arguments:
// -----------
// dwDeviceIndex            Zero based USBM300 device index
// *dwQueueBlocksReady      Number of stream data blocks ready in the queue FIFO
// *dwUsbStreamingState     Current stream thread state. See USB_THREAD_RUNNING, USB_THREAD_BUFFER_FULL
//
// Return Value:
// --------------
// TRUE                     Success
// FAIL                     Error
//
BOOLEAN USBM300::GetStreamingStatus(DWORD dwDeviceIndex, DWORD *dwQueueBlocksReady, DWORD *dwUsbStreamingState)
{
    // Device index range check
    if (dwDeviceIndex > MAX_USBM300_DEVICES) {
        DbgOut("GetStreamingStatus failed. Index %i out of range.\n", dwDeviceIndex);
        return (FALSE);
    }

    // Check for valid device handle
    if (m_hDevice[dwDeviceIndex] == INVALID_HANDLE_VALUE) {
        DbgOut("GetStreamingStatus failed. Index %i handle is invalid.\n", dwDeviceIndex);
        return (FALSE);
    }

    // Blocks ready to be copied out
    *dwQueueBlocksReady = m_UsbStreamCtrl[dwDeviceIndex].lQueueCount;

    // Thread status
    *dwUsbStreamingState = m_UsbStreamCtrl[dwDeviceIndex].dwUsbStreamingState;

    return (TRUE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Retrieves data from the Stream Data FIFO queue
//
// Arguments:
// -----------
// dwDeviceIndex            Zero based USBM300 device index
// bWait                    TRUE thread waits for dwBlocksRequested, FALSE returns data currently available
// pData                    Array of USBM300_STREAM_BLOCK structures to be filled
//                          Caller must allocate enough 
// dwBlocksRequested        Number of block requested. Must match pData array length
// *dwBlocksReturned        Number of blocks copied into pData
//
// Return Value:
// --------------
// TRUE                     Success
// FAIL                     Error
//
BOOLEAN USBM300::GetStreamingData(DWORD dwDeviceIndex, BOOLEAN bWait, PUSBM300_STREAM_BLOCK pData, DWORD dwBlocksRequested, DWORD *dwBlocksReturned)
{
    DWORD   i, j, k, dwWait;
    DWORD   dwBlocksCopied = 0;

    // Device index range check
    if (dwDeviceIndex > MAX_USBM300_DEVICES) {
        DbgOut("GetStreamingStatus failed. Index %i out of range.\n", dwDeviceIndex);
        return (FALSE);
    }

    // Check for valid device handle
    if (m_hDevice[dwDeviceIndex] == INVALID_HANDLE_VALUE) {
        DbgOut("GetStreamingStatus failed. Index %i handle is invalid.\n", dwDeviceIndex);
        return (FALSE);
    }

    if (dwBlocksRequested < 1) {        // Nothing to do. Exit success
        return (TRUE);
    }

    if (bWait) {
        // Wait for enough data to arrive
        while (dwBlocksRequested > (DWORD)m_UsbStreamCtrl[dwDeviceIndex].lQueueCount) {
            // Wait for 30 seconds. Fail otherwise.
            dwWait = WaitForSingleObject(m_UsbStreamCtrl[dwDeviceIndex].hDataReadyEvent, 30000);

            if (dwWait == WAIT_TIMEOUT) {
                DbgOut("GetStreamingData thread data ready event timeout. Index %i \n", dwDeviceIndex);
                *dwBlocksReturned = 0;
                return (FALSE);
            }

        }
        j = dwBlocksRequested;
     
    } else {
        // Return what data is currently available
        if (dwBlocksRequested > (DWORD)m_UsbStreamCtrl[dwDeviceIndex].lQueueCount) {
            j = m_UsbStreamCtrl[dwDeviceIndex].lQueueCount;
        } else {
            j = dwBlocksRequested;
        }
    }

    // Dequeue data and copy into user's pData buffer
    for (i=0; i<j; i++) {
        // Check queue empty (head == tail)
        if (m_UsbStreamCtrl[dwDeviceIndex].pTail == m_UsbStreamCtrl[dwDeviceIndex].pHead) {
            DbgOut("GetStreamingData unexpected queue empty.\n", dwDeviceIndex);
            break;
        }

        // Data copy
        for (k=0; k<ADC_SAMPLE_COUNT; k++) {
            pData[i].usAdcSamples[k] = m_UsbStreamCtrl[dwDeviceIndex].pTail->Data.usAdcSamples[k];
        }

        // Queue tail move
        m_UsbStreamCtrl[dwDeviceIndex].pTail = m_UsbStreamCtrl[dwDeviceIndex].pTail->NextElement;
        // Decrement queue count
        InterlockedDecrement(&m_UsbStreamCtrl[dwDeviceIndex].lQueueCount);
        // Another block copied
        dwBlocksCopied++;
    }

    *dwBlocksReturned = dwBlocksCopied;

    return (TRUE);
}
