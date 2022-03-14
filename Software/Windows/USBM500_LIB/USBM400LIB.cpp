///////////////////////////////////////////////////////////////////////////////////////////////////
//
// USBM400.cpp: implementation of the USBM400 class.
//
// Copyright (c) 2003-2008, Integrity Instruments, Inc.
//
// 1-800-450-2001
//
// http://www.integrityusa.com
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//
// Version 1.5
// Moved code to library form building with WDK.
//
#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include <strsafe.h>
// For all of the _t stuff (to allow compiling for both Unicode/Ansi)
#include <tchar.h>

// Local includes
#include "USBM400LIB.h"

// Setup API stuff. 
#include <setupapi.h>

// USBM400 IOCTL and GUID includes
#include "devioctl.h"

// This file is found in the "..\usbm400_sys_v2" directory. Path is defined in the SOURCES file
#include "bulkusr.h"

// Force library includes since the SOURCES file doesn't seem to do it
#pragma comment(lib,"SetupAPI.lib") 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Copyright notification
TCHAR sCopyrightNotification[] = _T("Copyright (c) 2003-2008, Integrity Instruments, Inc.");
CHAR  sAsciiCopyrightNotification[] = "Copyright (c) 2003-2008, Integrity Instruments, Inc.";



///////////////////////////////////////////////////////////////////////////////////////////////////
// DbgOut
//
// Prints a variable argument debug string to the debugger 
// Use like printf().. For example:  DbgOut(_T("Output string index = %i\n", i);
//
// Parameters
// ----------
// FormatString     Format string like printf
// ...              Variable argument list
//
VOID DbgOut(TCHAR *FormatString, ...)
{
    _TCHAR  szString[1024];
    va_list ap;
    va_start(ap, FormatString);

    OutputDebugString(_T("USBM400_LIB: "));
    // Old style variable argument string formatting
    // _vsntprintf(szString, 1024, FormatString, ap);
    StringCchVPrintf (szString, 1024, FormatString, ap);
    OutputDebugString(szString);

    va_end(ap);
    return;
} 


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

USBM400::USBM400()
{
    DWORD   i, j;

    // Dump out class version
    GetClassVersion();

    // Initialize device structures
    for (i=0; i<MAX_USBM400_DEVICES; i++) {

        // NULL device name and handle
        m_sDeviceName[i] = NULL;
        
        // Initialize the handles to the USB driver
        for (j=0; j<H_MAX; j++) {
            m_hDev[i][j] = INVALID_HANDLE_VALUE;
        }

        // Zero out thread handle and ID
        m_hStreamThread[i] = INVALID_HANDLE_VALUE;
        m_dwStreamThreadId[i] = 0;

        // Initial thread control settings
        m_UsbStreamCtrl[i].dwDeviceIndex = 0;
        m_UsbStreamCtrl[i].hCmd = INVALID_HANDLE_VALUE;
        m_UsbStreamCtrl[i].hResp = INVALID_HANDLE_VALUE;
        m_UsbStreamCtrl[i].hStream = INVALID_HANDLE_VALUE;
        m_UsbStreamCtrl[i].dwUsbStreamingState = USB_THREAD_NOT_STARTED;
        m_UsbStreamCtrl[i].dwStreamThreadIoError = 0;
        m_UsbStreamCtrl[i].dwQueueBlocksToAllocate = 0;
        m_UsbStreamCtrl[i].hDataReadyEvent = CreateEvent(NULL,
                                            FALSE,      // Automatic event reset
                                            FALSE,      // Initial state not signaled
                                            NULL);      // not named
        m_UsbStreamCtrl[i].lQueueCount = 0;
        m_UsbStreamCtrl[i].pQueue = NULL;
        m_UsbStreamCtrl[i].pHead = NULL;
        m_UsbStreamCtrl[i].pTail = NULL;
    }

    m_dwDeviceCount = 0;

    // Perform the initial USBM400 device enumeration
    EnumDevices();
    return;
}


USBM400::~USBM400()
{
    DbgOut(_T("USBM400 destructor called. Bye!\n"));
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
ULONG USBM400::GetClassVersion()
{
    // Version 1.5
    // Major version is upper 16 bits
    // Minor version is lower 16 bits
    //
    ULONG   ulVersion = 0x00010005;

    ULONG   ulMajorVersion, ulMinorVersion;

    ulMajorVersion = ((ulVersion & 0xFFFF0000) >> 16);
    ulMinorVersion =  (ulVersion & 0x0000FFFF);

    DbgOut(_T("USBM400 Class version %i.%i\n"), ulMajorVersion, ulMinorVersion);
    DbgOut(_T("%s\n"), sCopyrightNotification);

    return (ulVersion);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Free memory and handles used. 
//
// NOTE: All streaming threads are stopped as a result of this function
//
void USBM400::FreeMemoryAndHandles()
{
    DWORD   i, j;

    for (i=0; i<MAX_USBM400_DEVICES; i++) {

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
        for (j=0; j<H_MAX; j++) {
            if (m_hDev[i][j] != INVALID_HANDLE_VALUE) {
                CloseHandle(m_hDev[i][j]);
                m_hDev[i][j] = INVALID_HANDLE_VALUE;
            }
        }
        
        // Free the events
        if (m_UsbStreamCtrl[i].hDataReadyEvent != NULL) {
            CloseHandle(m_UsbStreamCtrl[i].hDataReadyEvent);
            m_UsbStreamCtrl[i].hDataReadyEvent = NULL;
        }
    }
    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Enumerates all USBM400 devices currently connected to the system.
//
// NOTE: All streaming threads are stopped as a result of this function
//
void USBM400::EnumDevices()
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

    // Get the USBM400 device list
    hDevInfo = SetupDiGetClassDevs ((LPGUID)&GUID_CLASS_USBM400,
                                    NULL,                       // Define no enumerator (global)
                                    NULL,                       // Define no
                                    (DIGCF_PRESENT |            // Only Devices present
                                    DIGCF_INTERFACEDEVICE));    // Function class devices.

    if (hDevInfo == INVALID_HANDLE_VALUE) {
        return;
    }
  
    // Loop through each device in the enumeration list
    DeviceInterfaceData.cbSize = sizeof(DeviceInterfaceData);

    for(i=0; SetupDiEnumDeviceInterfaces (hDevInfo,
                                 0, // No care about specific PDOs
                                 (LPGUID)&GUID_CLASS_USBM400,
                                 i, //
                                 &DeviceInterfaceData); i++ ) {
   
        // Establish the required structure length for the DeviceInterfaceData
        SetupDiGetInterfaceDeviceDetail (
                hDevInfo,
                &DeviceInterfaceData,
                NULL,   // probing so no output buffer yet
                0,      // probing so output buffer length of zero
                &requiredLength,
                NULL);  // not interested in the specific dev-node

        predictedLength = requiredLength;

        DeviceInterfaceDetailData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(predictedLength);
        DeviceInterfaceDetailData->cbSize = sizeof (SP_INTERFACE_DEVICE_DETAIL_DATA);

        // Retrieve the information from Plug and Play.
        if (SetupDiGetInterfaceDeviceDetail (
                   hDevInfo,
                   &DeviceInterfaceData,
                   DeviceInterfaceDetailData,
                   predictedLength,
                   &requiredLength,
                   NULL)) {

            // Save off the device path - the name used in CreateFile()
            DeviceNameLength = (MAX_LENGTH * sizeof(TCHAR));
            m_sDeviceName[DeviceIndex] = (TCHAR *)malloc(DeviceNameLength);
            ZeroMemory(m_sDeviceName[DeviceIndex], DeviceNameLength);
            StringCchCopy(m_sDeviceName[DeviceIndex], MAX_LENGTH, DeviceInterfaceDetailData->DevicePath);
            
            // Dumpt the name to the debugger
            DbgOut(_T("USBM400: %i,  name: %s\n"), DeviceIndex, m_sDeviceName[DeviceIndex]);
            
            // Open the device
            OpenDevice(DeviceIndex);
            DeviceIndex++;
        }
        free(DeviceInterfaceDetailData);
    }

    // Save off new device count
    m_dwDeviceCount = DeviceIndex;

    DbgOut(_T("%i USBM400 devices found.\n"), m_dwDeviceCount);

    // Free setup memory
    SetupDiDestroyDeviceInfoList (hDevInfo);
    return;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Opens a handle to the USBM400 device
//
// Arguments:
// -----------
// dwDeviceIndex    Zero based USBM400 device index
//
// Return Value:
// --------------
// TRUE             Success
// FAIL             Error
//
BOOLEAN USBM400::OpenDevice(DWORD dwDeviceIndex)
{
    TCHAR   *sCmdPipe = NULL;
    TCHAR   *sRespPipe = NULL;
    TCHAR   *sStreamPipe = NULL;
    BOOLEAN bRtn = TRUE;
    
    // Device index range check
    if (dwDeviceIndex > MAX_USBM400_DEVICES) {
        DbgOut(_T("Error opening USBM400 device. Index %i out of range.\n"), dwDeviceIndex);
        return (FALSE);
    }

    // Is there a device name associated with this index
    if (m_sDeviceName[dwDeviceIndex] == NULL) {
        DbgOut(_T("Error opening USBM400 device. Index %i has NULL device name.\n"), dwDeviceIndex);
        return (FALSE);
    }
    
    // Create the pipe names from the base device name: m_sDeviceName[DeviceIndex]
    // Command = PIPE1: Host PC->PIC
    // Response = PIPE0: PIC->Host PC
    // Stream = PIPE2: PIC->Host PC
    sCmdPipe = (TCHAR *)malloc(MAX_LENGTH*sizeof(TCHAR));
    ZeroMemory(sCmdPipe, MAX_LENGTH*sizeof(TCHAR));
    StringCchCopy(sCmdPipe, MAX_LENGTH, m_sDeviceName[dwDeviceIndex]);
    StringCchCat(sCmdPipe, MAX_LENGTH, _T("\\PIPE1"));

    sRespPipe = (TCHAR *)malloc(MAX_LENGTH*sizeof(TCHAR));
    ZeroMemory(sRespPipe, MAX_LENGTH*sizeof(TCHAR));
    StringCchCopy(sRespPipe, MAX_LENGTH, m_sDeviceName[dwDeviceIndex]);
    StringCchCat(sRespPipe, MAX_LENGTH, _T("\\PIPE0"));

    sStreamPipe = (TCHAR *)malloc(MAX_LENGTH*sizeof(TCHAR));
    ZeroMemory(sStreamPipe, MAX_LENGTH*sizeof(TCHAR));
    StringCchCopy(sStreamPipe, MAX_LENGTH, m_sDeviceName[dwDeviceIndex]);
    StringCchCat(sStreamPipe, MAX_LENGTH, _T("\\PIPE2"));

    // Close the device handle, if already opened
    //if (m_hDev[dwDeviceIndex][H_BASE] != INVALID_HANDLE_VALUE) {
//        CloseHandle(m_hDev[dwDeviceIndex][H_BASE]);
//        m_hDev[dwDeviceIndex][H_BASE] = INVALID_HANDLE_VALUE;
//    }

    // Open up the base device
    m_hDev[dwDeviceIndex][H_BASE] = CreateFile (
                  m_sDeviceName[dwDeviceIndex],
                  GENERIC_READ | GENERIC_WRITE,
                  0,                                            // FIX: v1.2 No share -- was FILE_SHARE_READ | FILE_SHARE_WRITE,
                  NULL,                                         // no SECURITY_ATTRIBUTES structure
                  OPEN_EXISTING,                                // No special create flags
                  FILE_ATTRIBUTE_NORMAL,                        // No special attributes
                  NULL);                                        // No template file

    if (m_hDev[dwDeviceIndex][H_BASE] == INVALID_HANDLE_VALUE) {
        DbgOut(_T("Error opening base USBM400 device index %i name %s\n"), dwDeviceIndex, m_sDeviceName[dwDeviceIndex]);
        return (FALSE);
    } else {
        DbgOut(_T("Opened USBM400 device index %i with handle 0x%08X\n"), dwDeviceIndex, m_hDev[dwDeviceIndex][H_BASE]);
        DbgOut(_T("Opened USBM400: %s\n"), m_sDeviceName[dwDeviceIndex]);
    }
    
    // Open up the command pipe device
    m_hDev[dwDeviceIndex][H_CMD] = CreateFile (
                  sCmdPipe,
                  GENERIC_READ | GENERIC_WRITE,
                  0,                                            // FIX: v1.2 No share -- was FILE_SHARE_READ | FILE_SHARE_WRITE,
                  NULL,                                         // no SECURITY_ATTRIBUTES structure
                  OPEN_EXISTING,                                // No special create flags
                  FILE_FLAG_OVERLAPPED,                         // Setup for overlapped (async) I/O (no attributes = FILE_ATTRIBUTE_NORMAL)
                  NULL);                                        // No template file

    if (m_hDev[dwDeviceIndex][H_CMD] == INVALID_HANDLE_VALUE) {
        DbgOut(_T("Error opening USBM400 device index %i name %s\n"), dwDeviceIndex, sCmdPipe);
        bRtn = FALSE;
    } else {
        DbgOut(_T("Opened USBM400 device index %i with handle 0x%08X\n"), dwDeviceIndex, m_hDev[dwDeviceIndex][H_CMD]);
        DbgOut(_T("Opened USBM400: %s\n"), sCmdPipe);
    }
    
    // Open up the response pipe device
    m_hDev[dwDeviceIndex][H_RESP] = CreateFile (
                  sRespPipe,
                  GENERIC_READ | GENERIC_WRITE,
                  0,                                            // FIX: v1.2 No share -- was FILE_SHARE_READ | FILE_SHARE_WRITE,
                  NULL,                                         // no SECURITY_ATTRIBUTES structure
                  OPEN_EXISTING,                                // No special create flags
                  FILE_FLAG_OVERLAPPED,                         /// Setup for overlapped (async) I/O (no attributes = FILE_ATTRIBUTE_NORMAL)
                  NULL);                                        // No template file

    if (m_hDev[dwDeviceIndex][H_RESP] == INVALID_HANDLE_VALUE) {
        DbgOut(_T("Error opening USBM400 device index %i name %s\n"), dwDeviceIndex, sRespPipe);
        bRtn = FALSE;
    } else {
        DbgOut(_T("Opened USBM400 device index %i with handle 0x%08X\n"), dwDeviceIndex, m_hDev[dwDeviceIndex][H_RESP]);
        DbgOut(_T("Opened USBM400: %s\n"), sRespPipe);
    }
    
    // Open up the response pipe device
    m_hDev[dwDeviceIndex][H_STREAM] = CreateFile (
                  sStreamPipe,
                  GENERIC_READ | GENERIC_WRITE,
                  0,                                            // FIX: v1.2 No share -- was FILE_SHARE_READ | FILE_SHARE_WRITE,
                  NULL,                                         // no SECURITY_ATTRIBUTES structure
                  OPEN_EXISTING,                                // No special create flags
                  FILE_FLAG_OVERLAPPED,                         // Setup for overlapped (async) I/O (no attributes = FILE_ATTRIBUTE_NORMAL)
                  NULL);                                        // No template file

    if (m_hDev[dwDeviceIndex][H_STREAM] == INVALID_HANDLE_VALUE) {
        DbgOut(_T("Error opening USBM400 device index %i name %s\n"), dwDeviceIndex, sStreamPipe);
        bRtn = FALSE;
    } else {
        DbgOut(_T("Opened USBM400 device index %i with handle 0x%08X\n"), dwDeviceIndex, m_hDev[dwDeviceIndex][H_STREAM]);
        DbgOut(_T("Opened USBM400: %s\n"), sStreamPipe);
    }
    
    // Free locally malloc() buffers
    free(sCmdPipe);
    free(sRespPipe);
    free(sStreamPipe);
    
    return (bRtn);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// 'C' Interface - needed by UsbThread 
//
// Sends a command to the USBM400 and returns the response. Using a 'C' function is necessary 
// to make the Threads easier to work with since they are 'C' constructs in MSFT land.
//
// Arguments:
// -----------
// hDevice          Handle to opened USBM400 device
// sCmd             Command string to send to module. Maximum = MAX_CMD_RESP
// sResp            Response received from module. Maximum = MAX_CMD_RESP
//
// Return Value:
// --------------
// TRUE             Success
// FAIL             Error
//
BOOLEAN C_SendCommand(HANDLE hCmdPipe, HANDLE hRespPipe, LPSTR sCmd, LPSTR sResp)
{
    PVOID   pDataBuffer = NULL;
    PCHAR   pcCommand;
    PCHAR   pOutBuffer;
    DWORD   dwBytesWritten, dwBytesRead;
    DWORD   dwLength;
    BOOL    bStatus;
    DWORD   dwError, dwRtn;
    BOOLEAN bRtn = TRUE;
    
    OVERLAPPED  Write_Ovr, Read_Ovr;

    // Check for valid device handle
    if ((hCmdPipe == INVALID_HANDLE_VALUE) || (hRespPipe == INVALID_HANDLE_VALUE)) {
        DbgOut(_T("Error sending USBM400 command. Handle is invalid.\n"));
        return (FALSE);
    }

    // See how many ASCII chars are in the command string
    dwLength = strlen(sCmd);

    // Check for zero length command
    if (dwLength == 0) {
        strncpy(sResp, "no data", MAX_CMD_RESP);
        return (TRUE);
    }

    // FIX: v1.2 Check command length too long
    if (dwLength > 7) {
        DbgOut(_T("Error sending USBM400 command. Command too long %i\n"), dwLength);
        strncpy(sResp, "no data", MAX_CMD_RESP);
        return (FALSE);
    }
    
    // Overlapped structures must be zeroed before using!
    ZeroMemory(&Write_Ovr, sizeof(Write_Ovr));
    ZeroMemory(&Read_Ovr, sizeof(Read_Ovr));
    
    // Create the events for the Read and Write overlapped structures
    Write_Ovr.hEvent = CreateEvent(
                            NULL,   // No special security attributes
                            TRUE,   // TRUE = Manual Reset
                            FALSE,  // FALSE = Initial state not-signaled
                            NULL);  // NULL = No name

    Read_Ovr.hEvent = CreateEvent(
                            NULL,   // No special security attributes
                            TRUE,   // TRUE = Manual Reset
                            FALSE,  // FALSE = Initial state not-signaled
                            NULL);  // NULL = No name

    // Send the command
    bStatus = WriteFile(hCmdPipe, sCmd, dwLength, &dwBytesWritten, &Write_Ovr);                              
    if (bStatus)
        DbgOut(_T("SendCommand write returned immediately\n"));
    if ((!bStatus) && (GetLastError() != ERROR_IO_PENDING)) {
        // Error
        dwError = GetLastError();
        DbgOut(_T("Error sending USBM400 command. Error code %i.\n"), dwError);
        bRtn = FALSE;
        goto Cleanup;
    }

    // Receive any and all responses. 
    bStatus =  ReadFile(hRespPipe, sResp, MAX_CMD_RESP, (PULONG) &dwBytesRead, &Read_Ovr);
    if (bStatus)
        DbgOut(_T("SendCommand read returned immediately\n"));
    if ((!bStatus) && (GetLastError() != ERROR_IO_PENDING)) {
        // Error
        dwError = GetLastError();
        DbgOut(_T("Error receiving USBM400 response. Error code %i.\n"), dwError);
        bRtn = FALSE;
        goto Cleanup;
    }

    // Wait for the Write and Reads to complete (1 second = 1000 millisecond wait)
    dwRtn = WaitForSingleObject(Write_Ovr.hEvent, 1000);
    if (dwRtn == WAIT_TIMEOUT) {
        DbgOut(_T("SendCommand wait on xmit timeout\n"));
        bRtn = FALSE;
        goto Cleanup;
    }
    dwRtn = WaitForSingleObject(Read_Ovr.hEvent, 5000);
    if (dwRtn == WAIT_TIMEOUT) {
        DbgOut(_T("SendCommand wait on rcv timeout\n"));
        bRtn = FALSE;
        goto Cleanup;
    } 
    
    // Get the read status (from the Async read)
    GetOverlappedResult(hRespPipe, &Read_Ovr, &dwBytesRead, FALSE);
    sResp[dwBytesRead] = 0;            // Null terminate for ASCII compliance

    // Cleanup. I hate using goto statements, but this is one of those times it makes the code much more readable.
Cleanup:
    CloseHandle(Write_Ovr.hEvent);
    CloseHandle(Read_Ovr.hEvent);
    return (bRtn);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Sends a command and returns the response.
//
// Arguments:
// -----------
// dwDeviceIndex    Zero based USBM400 device index
// sCmd             Command string to send to module
// sResp            Response received from module
//
// Return Value:
// --------------
// TRUE             Success
// FAIL             Error
//
BOOLEAN USBM400::SendCommand(DWORD dwDeviceIndex, LPSTR sCmd, LPSTR sResp)
{
    BOOLEAN bStatus;

    // Device index range check
    if (dwDeviceIndex > MAX_USBM400_DEVICES) {
        DbgOut(_T("Error sending USBM400 command. Index %i out of range.\n"), dwDeviceIndex);
        return (FALSE);
    }

    // Send the command using the 'C' function call.
    bStatus = ::C_SendCommand(m_hDev[dwDeviceIndex][H_CMD], m_hDev[dwDeviceIndex][H_RESP], sCmd, sResp);

    return (bStatus);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// USBM400 stream mode thread.
//
//
DWORD WINAPI UsbStreamThread(LPVOID lpParameter)
{
    PUSHORT     pOutBuffer[ASYNC_IO_COUNT];
    OVERLAPPED  Ovr[ASYNC_IO_COUNT];
    HANDLE      hEvent[ASYNC_IO_COUNT];
    DWORD       dwBytesReturned, dwAsyncBytesReturned;
    BOOL        bStatus;
    DWORD       dwError = 0;
    DWORD       dwTotalAdcSampleCount = 0;
    DWORD       dwStartTime, dwEndTime, dwWait, i, k;
    CHAR        *sCmd,*sResp ;

    // Parameter block
    PUSB_STREAM_CONTROL  pUsbCtrl = (PUSB_STREAM_CONTROL)lpParameter;

    DbgOut (_T("UsbStreamThread device index %i started. Async I/O count = %i\n"), pUsbCtrl->dwDeviceIndex, ASYNC_IO_COUNT);

    // Allocate and Clear the command and response buffers
    sCmd = (CHAR *)malloc(MAX_CMD_RESP);
    sResp = (CHAR *)malloc(MAX_CMD_RESP);
    ZeroMemory(sCmd, MAX_CMD_RESP);
    ZeroMemory(sResp, MAX_CMD_RESP);

    // Allocate the queue buffer 
    pUsbCtrl->pQueue = (PUSBM400_STREAM_BLOCK_QUEUE)malloc(pUsbCtrl->dwQueueBlocksToAllocate * sizeof(USBM400_STREAM_BLOCK_QUEUE));

    // Setup the queue element pointers
    for (i=0; i < (pUsbCtrl->dwQueueBlocksToAllocate-1); i++) {
        pUsbCtrl->pQueue[i].NextElement = &pUsbCtrl->pQueue[i+1];
    }
    pUsbCtrl->pQueue[i].NextElement = &pUsbCtrl->pQueue[0];

    // Head and tail to start of queue
    pUsbCtrl->pHead = &pUsbCtrl->pQueue[0];
    pUsbCtrl->pTail = &pUsbCtrl->pQueue[0];

    // Allocate the structures for each concurrent ASYNC I/O
    for (i=0; i<ASYNC_IO_COUNT; i++) {
        // Data buffer
        pOutBuffer[i] = (PUSHORT)malloc (256);
        // Must always zero out the overlapped structure before it is used
        ZeroMemory(&Ovr[i], sizeof(OVERLAPPED));
        
        // Create the events for overlapped structure
        hEvent[i] = CreateEvent(
                                NULL,   // No special security attributes
                                TRUE,   // TRUE = Manual Reset
                                FALSE,  // FALSE = Initial state not-signaled
                                NULL);  // NULL = No name    
        // Assign the event to the associated overlapped structure
        Ovr[i].hEvent = hEvent[i];
    }

    // Send the start streaming command
    strncpy(sCmd, "S", MAX_CMD_RESP);
    if (C_SendCommand(pUsbCtrl->hCmd, pUsbCtrl->hResp, sCmd, sResp)) {
        // Nothing to do on success
    } else {
        // SendCommand error
        DbgOut(_T("UsbStreamThread index %i Start stream SendCommand failure.\n"), pUsbCtrl->dwDeviceIndex);
        pUsbCtrl->dwUsbStreamingState |= USB_THREAD_TERMINATE;
        return (1);
    }

    // Indicate the thread is running
    pUsbCtrl->dwUsbStreamingState |= USB_THREAD_RUNNING;

    // Performance calculations
    dwStartTime = GetTickCount();
    pUsbCtrl->dwTotalBytes = 0;
    
    // Kick off the initial asynchronous I/O reads (total = ASYNC_IO_COUNT)
    for (i=0; i<ASYNC_IO_COUNT; i++) {
        ZeroMemory(pOutBuffer[i], 256);
        bStatus =  ReadFile(pUsbCtrl->hStream, pOutBuffer[i], 64, (PULONG) &dwBytesReturned, &Ovr[i]);
        
        // TODO: Check status for pending!
    }

    // Check state variable for USB_THREAD_TERMINATE
    while (!(pUsbCtrl->dwUsbStreamingState & USB_THREAD_TERMINATE)) {

//        DbgOut ("\nUsbStreamThread getting packet %i\n", dwCount++);

        // Wait for any async I/O to complete (or timeout if the USB device dies). Timeout = 1 second (1000 milliseconds)
        dwWait = WaitForMultipleObjects(ASYNC_IO_COUNT, hEvent, FALSE, 1000);
        
        // The USB device stopped responding. Exit the thread
        if (dwWait == WAIT_TIMEOUT) {
            DbgOut(_T("UsbStreamThread async I/O timeout! Bailing UsbStreamThread()\n"));
            break;
        }
        
        // Determine which I/O completed, process the I/O, clear event, then reissue I/O
        k = dwWait - WAIT_OBJECT_0;
        // printf("<%s> W (%04.4d) : Wait returned dwRtn=%i  k=%i\n", outPipe, WriteIoComplete, dwRtn, k);
        
        // Get the read status (from the Async read)
        dwAsyncBytesReturned = 0;
        GetOverlappedResult(pUsbCtrl->hStream, &Ovr[k], &dwAsyncBytesReturned, FALSE);
        
        if (dwAsyncBytesReturned == 0) {
            // Nothing in read buffer, nothing to process
            DbgOut(_T("UsbStreamThread no bytes returned on I/O %i\n"), k);
        }
        
//            DbgOut ("\nUsbStreamThread getting packet %i, bytes returned = %i\n", dwCount++, dwBytesReturned);
        pUsbCtrl->dwTotalBytes += dwAsyncBytesReturned;

        // Check queue full status
        if (pUsbCtrl->pHead->NextElement == pUsbCtrl->pTail) {
            DbgOut(_T("UsbStreamThread %i queue full!\n"), pUsbCtrl->dwDeviceIndex);
            continue;
        } else {
            // USBM400 Data is sent in the following format
            // (24) 16 bit A/D sample
            // (1)  16 bit digital input
            // (1)  32 bit Pulse counter

            // USBM400 Data is sent in the following format
            // (32) 16 bit A/D samples

            // Get A/D data from pOutBuffer and put into queue
            for (i=0; i<ADC_SAMPLE_COUNT; i++) {
                pUsbCtrl->pHead->Data.usAdcSamples[i] = pOutBuffer[k][i];
                dwTotalAdcSampleCount++;
            }

            // Increment queue count and move queue head pointer
            InterlockedIncrement(&pUsbCtrl->lQueueCount);
            pUsbCtrl->pHead = pUsbCtrl->pHead->NextElement;

            // Signal event to GetStreamingData()
            SetEvent(pUsbCtrl->hDataReadyEvent);
        }
        // Reissue the read
        ResetEvent(hEvent[k]);
        ZeroMemory(pOutBuffer[k], 256);
        bStatus =  ReadFile(pUsbCtrl->hStream, pOutBuffer[k], 64, (PULONG) &dwBytesReturned, &Ovr[k]);

        // TODO; Check status for pending!
        
    }  // while (!(pUsbCtrl->dwUsbStreamingState & USB_THREAD_TERMINATE)) {

    // Performance calculations
    dwEndTime = GetTickCount();
    pUsbCtrl->dwTotalMs = dwEndTime - dwStartTime;

    DbgOut(_T("UsbStreamThread %i total samples in %i milliseconds.\n"), dwTotalAdcSampleCount, pUsbCtrl->dwTotalMs);    
    
    
    // Cancel any outstanding I/O to the stream pipe
    CancelIo(pUsbCtrl->hStream);

    // Send the halt streaming command
    strncpy(sCmd, "H", MAX_CMD_RESP);
    if (C_SendCommand(pUsbCtrl->hCmd, pUsbCtrl->hResp, sCmd, sResp)) {
        // Success - nothing to do
    } else {
        // SendCommand error
        DbgOut(_T("UsbStreamThread index %i Halt stream SendCommand failure.\n"), pUsbCtrl->dwDeviceIndex);
    }

    // Free buffers
    for (i=0; i<ASYNC_IO_COUNT; i++) {
        free(pOutBuffer[i]);
        CloseHandle(hEvent[i]);
    }
    free(sCmd);
    free(sResp);
    if (pUsbCtrl->pQueue != NULL) {
        free(pUsbCtrl->pQueue);
        pUsbCtrl->pQueue = NULL;
    }

    // Return any error code
    pUsbCtrl->dwStreamThreadIoError = dwError;

    // Clear streaming state flags
    pUsbCtrl->dwUsbStreamingState &= (~USB_THREAD_RUNNING);
    pUsbCtrl->dwUsbStreamingState &= (~USB_THREAD_TERMINATE);

    DbgOut (_T("UsbStreamThread %i EXIT. dwError = %i\n\n"), pUsbCtrl->dwDeviceIndex, dwError);

    return (0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Starts or stops the streaming thread
//
// Arguments:
// -----------
// dwDeviceIndex            Zero based USBM400 device index
// bStart                   TRUE to start stream thread, FALSE to stop
// dwQueueBlocksToAllocate  Number of stream blocks to allocate for the queue FIFO. 
//                          See USBM400_STREAM_BLOCK definition in USBM400.H
//
// Return Value:
// --------------
// TRUE                     Success
// FAIL                     Error
//
BOOLEAN USBM400::ControlStreaming(DWORD dwDeviceIndex, BOOLEAN bStart, DWORD dwQueueBlocksToAllocate)
{
    DWORD   dwError, dwWait, i;
    double  fBytes, fSeconds, fBytesPerSecond;

    DbgOut(_T("ControlStreaming enter\n"));

    // Device index range check
    if (dwDeviceIndex > MAX_USBM400_DEVICES) {
        DbgOut(_T("ControlStreaming failed. Index %i out of range.\n"), dwDeviceIndex);
        return (FALSE);
    }

    // Check for valid device handles
    for (i=0; i<H_MAX; i++) {
        if (m_hDev[dwDeviceIndex][i] == INVALID_HANDLE_VALUE) {
            DbgOut(_T("ControlStreaming failed. Index %i handle is invalid.\n"), dwDeviceIndex);
            return (FALSE);
        }
    }

    if (bStart) {
        // Is the thread already running?
        if (m_UsbStreamCtrl[dwDeviceIndex].dwUsbStreamingState & USB_THREAD_RUNNING) {
            DbgOut(_T("ControlStreaming index %i thread already started.\n"), dwDeviceIndex);
            return (TRUE);
        }

        // Setup parameter variables
        m_UsbStreamCtrl[dwDeviceIndex].hCmd = m_hDev[dwDeviceIndex][H_CMD];
        m_UsbStreamCtrl[dwDeviceIndex].hResp = m_hDev[dwDeviceIndex][H_RESP];
        m_UsbStreamCtrl[dwDeviceIndex].hStream = m_hDev[dwDeviceIndex][H_STREAM];
        m_UsbStreamCtrl[dwDeviceIndex].dwDeviceIndex = dwDeviceIndex;
        m_UsbStreamCtrl[dwDeviceIndex].dwQueueBlocksToAllocate = dwQueueBlocksToAllocate;
        m_UsbStreamCtrl[dwDeviceIndex].dwUsbStreamingState = USB_THREAD_NOT_STARTED;
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
            DbgOut(_T("ControlStreaming failed creating thread. Index %i error status = %i\n"), dwDeviceIndex, dwError);
            return (FALSE);
        } else {
            // Wait for thread to start before returning
            i = 0;
            while (!(m_UsbStreamCtrl[dwDeviceIndex].dwUsbStreamingState & USB_THREAD_RUNNING)) {
                DbgOut(_T("ControlStreaming waiting for thread to start count %i\n"), i);
                Sleep(100);

                if (m_UsbStreamCtrl[dwDeviceIndex].dwUsbStreamingState & USB_THREAD_TERMINATE) {
                    // The thread killed itself
                    DbgOut(_T("ControlStreaming index %i thread failed to start\n"), dwDeviceIndex);
                    break;
                }

                i++;
                if (i > 50) {
                    DbgOut(_T("ControlStreaming index %i bail waiting for thread to start\n"), dwDeviceIndex);
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

            // Cancel any I/O in progress. Not used in this version ....
            // CancelIo(m_UsbStreamCtrl[dwDeviceIndex].hStream);
            // CancelIo(m_UsbStreamCtrl[dwDeviceIndex].hCmd);
            // CancelIo(m_UsbStreamCtrl[dwDeviceIndex].hResp);

            // Wait for thread to exit. Timeout is 5 seconds (5,000 milliseconds)
            dwWait = WaitForSingleObject(m_hStreamThread[dwDeviceIndex], 5000);

            if (dwWait == WAIT_TIMEOUT) {
                DbgOut(_T("ControlStreaming thread terminate wait timeout. Index %i \n"), dwDeviceIndex);
                return (FALSE);
            } else {

                // Performance data calculations
                fBytes =  m_UsbStreamCtrl[dwDeviceIndex].dwTotalBytes;
                fSeconds =  m_UsbStreamCtrl[dwDeviceIndex].dwTotalMs;
                fSeconds = fSeconds / 1000.0;
                fBytesPerSecond = fBytes / fSeconds;
                // Display results
                DbgOut(_T("Thread performance for index %i:\n"), dwDeviceIndex);
                DbgOut(_T("-> %i total bytes\n-> %i milliseconds\n"), m_UsbStreamCtrl[dwDeviceIndex].dwTotalBytes, m_UsbStreamCtrl[dwDeviceIndex].dwTotalMs);
                DbgOut(_T("-> %f bytes per second\n"), fBytesPerSecond);

                // Close thread handle
                CloseHandle(m_hStreamThread[dwDeviceIndex]);
                m_hStreamThread[dwDeviceIndex] = INVALID_HANDLE_VALUE;

                // Reset thread state
                m_UsbStreamCtrl[dwDeviceIndex].dwUsbStreamingState = USB_THREAD_NOT_STARTED;
            }
        } else {
            // Thread is already stopped
            DbgOut(_T("ControlStreaming thread already stopped. Index %i \n"), dwDeviceIndex);
        }
    }

    return (TRUE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Returns the streaming thread status and blocks ready to be read from FIFO queue
//
// Arguments:
// -----------
// dwDeviceIndex            Zero based USBM400 device index
// *dwQueueBlocksReady      Number of stream data blocks ready in the queue FIFO
// *dwUsbStreamingState     Current stream thread state. See USB_THREAD_RUNNING, USB_THREAD_BUFFER_FULL
//
// Return Value:
// --------------
// TRUE                     Success
// FAIL                     Error
//
BOOLEAN USBM400::GetStreamingStatus(DWORD dwDeviceIndex, DWORD *dwQueueBlocksReady, DWORD *dwUsbStreamingState)
{
    // Device index range check
    if (dwDeviceIndex > MAX_USBM400_DEVICES) {
        DbgOut(_T("GetStreamingStatus failed. Index %i out of range.\n"), dwDeviceIndex);
        return (FALSE);
    }

    // Check for valid device handle. Should we check all or assume that the base device is enough?
    if (m_hDev[dwDeviceIndex][H_BASE] == INVALID_HANDLE_VALUE) {
        DbgOut(_T("GetStreamingStatus failed. Index %i handle is invalid.\n"), dwDeviceIndex);
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
// dwDeviceIndex            Zero based USBM400 device index
// bWait                    TRUE thread waits for dwBlocksRequested, FALSE returns data currently available
// pData                    Array of USBM400_STREAM_BLOCK structures to be filled
//                          Caller must allocate enough 
// dwBlocksRequested        Number of block requested. Must match pData array length
// *dwBlocksReturned        Number of blocks copied into pData
//
// Return Value:
// --------------
// TRUE                     Success
// FAIL                     Error
//
BOOLEAN USBM400::GetStreamingData(DWORD dwDeviceIndex, BOOLEAN bWait, PUSBM400_STREAM_BLOCK pData, DWORD dwBlocksRequested, DWORD *dwBlocksReturned)
{
    DWORD   i, j, k, dwWait;
    DWORD   dwBlocksCopied = 0;

    // Device index range check
    if (dwDeviceIndex > MAX_USBM400_DEVICES) {
        DbgOut(_T("GetStreamingData failed. Index %i out of range.\n"), dwDeviceIndex);
        return (FALSE);
    }

    // Check for valid device handle. Should we check all or assume that the base device is enough?
    if (m_hDev[dwDeviceIndex][H_BASE] == INVALID_HANDLE_VALUE) {
        DbgOut(_T("GetStreamingData failed. Index %i handle is invalid.\n"), dwDeviceIndex);
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
                DbgOut(_T("GetStreamingData thread data ready event timeout. Index %i \n"), dwDeviceIndex);
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
            DbgOut(_T("GetStreamingData unexpected queue empty.\n"), dwDeviceIndex);
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
