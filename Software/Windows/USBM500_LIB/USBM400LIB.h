///////////////////////////////////////////////////////////////////////////////////////////////////
//
// USBM400.h: interface for the USBM400 class.
//
// Copyright (c) 2003-2008, Integrity Instruments, Inc.
//
// 1-800-450-2001
//
// http://www.integrityusa.com
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_USBM400_H__8A9CC117_1F97_4CB5_AFD2_31BE82C8932C__INCLUDED_)
#define AFX_USBM400_H__8A9CC117_1F97_4CB5_AFD2_31BE82C8932C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Local defines
#define MAX_LENGTH              256
#define MAX_USBM400_DEVICES     20
#define MAX_CMD_RESP            16
#define ADC_SAMPLE_COUNT        32

// Number of concurrent asyncronous I/O operations in the streaming thread
#define ASYNC_IO_COUNT  4

// m_hDev handles to
#define H_BASE      0
#define H_CMD       1
#define H_RESP      2
#define H_STREAM    3
#define H_MAX       3

// USB Pipe definitions
// --------------------
// Command  = PIPE1: Host_PC->PIC
// Response = PIPE0: PIC->Host_PC
// Stream   = PIPE2: PIC->Host_PC

///////////////////////////////////////////////////////////////////////////////////////////////////
// Stream mode data block sent by USBM400
// Each block consists of:
// ------------------------
// 24 12 bit A/D samples (stored as 16 bits)
// 1  16 bit Digital Input Status
// 1  32 bit Pulse Counter Status
// 54 total bytes
//
// Stream mode data block sent by USBM400
// Each block consists of:
// ------------------------
// 32 12 bit A/D samples (stored as 16 bits)
// 64 total bytes
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _USBM400_STREAM_BLOCK
{
    USHORT  usAdcSamples[ADC_SAMPLE_COUNT];
} USBM400_STREAM_BLOCK, *PUSBM400_STREAM_BLOCK;

struct _USBM400_STREAM_BLOCK_QUEUE;

typedef struct _USBM400_STREAM_BLOCK_QUEUE
{
    USBM400_STREAM_BLOCK                Data;
    struct _USBM400_STREAM_BLOCK_QUEUE *NextElement;
} USBM400_STREAM_BLOCK_QUEUE, *PUSBM400_STREAM_BLOCK_QUEUE;

//
// Streaming mode thread and control defines
//
#define     USB_THREAD_NOT_STARTED  0x0000
#define     USB_THREAD_RUNNING      0x0001
#define     USB_THREAD_BUFFER_FULL  0x0010
#define     USB_THREAD_TERMINATE    0x1000

typedef struct _USB_STREAM_CONTROL
{
    DWORD   dwDeviceIndex;              // Zero based USBM400 device index
    HANDLE  hCmd;                       // Handle to the USBM400 command pipe
    HANDLE  hResp;                      // Handle to the USBM400 response pipe 
    HANDLE  hStream;                    // Handle to the USBM400 stream pipe
    DWORD   dwUsbStreamingState;        // Streaming thread state
    DWORD   dwStreamThreadIoError;      // WinError.H code returned if thread I/O fails
    DWORD   dwTotalBytes;               // Total bytes transfered by stream thread
    DWORD   dwTotalMs;                  // Total milliseconds thread ran (updated at thread exit)
    DWORD   dwQueueBlocksToAllocate;    // Number of blocks in the Stream Data Queue
    LONG    lQueueCount;                // Number of queue entries currently used
    HANDLE  hDataReadyEvent;            // Signaled when the stream thread has data ready
    PUSBM400_STREAM_BLOCK_QUEUE pQueue; // Queue storage
    PUSBM400_STREAM_BLOCK_QUEUE pHead;  // Queue head ptr - modified by UsbStreamThread()
    PUSBM400_STREAM_BLOCK_QUEUE pTail;  // Queue tail ptr - modified by GetStreamingData()
} USB_STREAM_CONTROL, *PUSB_STREAM_CONTROL;


///////////////////////////////////////////////////////////////////////////////////////////////////
// USBM400 Class definition
///////////////////////////////////////////////////////////////////////////////////////////////////
class USBM400  
{
public:
    // Standard constructor/destructor
    USBM400();
    virtual ~USBM400();
    
    // Returns the class version number
    ULONG GetClassVersion();
    
    // Enumerates and sets up the structures to access the USBM400 devices.
    // The total number of USBM400 modules is found in member variable m_dwDeviceCount
    void EnumDevices();
    DWORD m_dwDeviceCount;

    // Methods that access the device based on the Device Index (zero based)
    BOOLEAN SendCommand(DWORD dwDeviceIndex, 
                        LPSTR sCmd, 
                        LPSTR sResp);
    BOOLEAN GetStreamingData(DWORD dwDeviceIndex, 
                             BOOLEAN bWait, 
                             PUSBM400_STREAM_BLOCK pData, 
                             DWORD dwBlocksRequested, 
                             DWORD *dwBlocksReturned);
    BOOLEAN GetStreamingStatus(DWORD dwDeviceIndex, 
                               DWORD *dwQueueBlocksReady, 
                               DWORD *dwUsbStreamingState);
    BOOLEAN ControlStreaming(DWORD dwDeviceIndex, 
                             BOOLEAN bStart, 
                             DWORD dwQueueBlocksToAllocate);

private:
    void FreeMemoryAndHandles();
    BOOLEAN OpenDevice(DWORD dwDeviceIndex);
    TCHAR   *m_sDeviceName[MAX_USBM400_DEVICES];                // The PnP GUID based device name
    HANDLE  m_hDev[MAX_USBM400_DEVICES][H_MAX];                 // Handles to USBM400 driver
    HANDLE  m_hStreamThread[MAX_USBM400_DEVICES];               // Strean thread handle
    DWORD   m_dwStreamThreadId[MAX_USBM400_DEVICES];            // Stream thread ID
    USB_STREAM_CONTROL  m_UsbStreamCtrl[MAX_USBM400_DEVICES];   // Structure used by Stream thread and outside world
};

#endif // !defined(AFX_USBM400_H__8A9CC117_1F97_4CB5_AFD2_31BE82C8932C__INCLUDED_)
