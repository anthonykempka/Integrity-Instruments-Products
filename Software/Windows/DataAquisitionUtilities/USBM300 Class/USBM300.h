///////////////////////////////////////////////////////////////////////////////////////////////////
//
// USBM300.h: interface for the USBM300 class.
//
// Copyright (c) 2003, Integrity Instruments, Inc.
//
// 1-800-450-2001
//
// http://www.integrityusa.com
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_USBM300_H__8A9CC117_1F97_4CB5_AFD2_31BE82C8932C__INCLUDED_)
#define AFX_USBM300_H__8A9CC117_1F97_4CB5_AFD2_31BE82C8932C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
// Local defines
//
#define MAX_USBM300_DEVICES     100
#define MAX_CMD_RESP            16
#define ADC_SAMPLE_COUNT        32

///////////////////////////////////////////////////////////////////////////////////////////////////
// Stream mode data block sent by USBM300
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
typedef struct _USBM300_STREAM_BLOCK
{
    USHORT  usAdcSamples[ADC_SAMPLE_COUNT];
} USBM300_STREAM_BLOCK, *PUSBM300_STREAM_BLOCK;

struct _USBM300_STREAM_BLOCK_QUEUE;

typedef struct _USBM300_STREAM_BLOCK_QUEUE
{
    USBM300_STREAM_BLOCK                Data;
    struct _USBM300_STREAM_BLOCK_QUEUE *NextElement;
} USBM300_STREAM_BLOCK_QUEUE, *PUSBM300_STREAM_BLOCK_QUEUE;

//
// Streaming mode thread and control defines
//
#define     USB_THREAD_NOT_STARTED  0x0000
#define     USB_THREAD_RUNNING      0x0001
#define     USB_THREAD_BUFFER_FULL  0x0010
#define     USB_THREAD_TERMINATE    0x1000

typedef struct _USB_STREAM_CONTROL
{
    DWORD   dwDeviceIndex;              // Zero based USBM300 device index
    HANDLE  hUsbDevice;                 // Handle to the USBM300 device for dwDeviceIndex
    DWORD   dwUsbStreamingState;        // Streaming thread state
    DWORD   dwStreamThreadIoError;      // WinError.H code returned if thread I/O fails
    DWORD   dwTotalBytes;               // Total bytes transfered by stream thread
    DWORD   dwTotalMs;                  // Total milliseconds thread ran (updated at thread exit)
    DWORD   dwQueueBlocksToAllocate;    // Number of blocks in the Stream Data Queue
    LONG    lQueueCount;                // Number of queue entries currently used
    HANDLE  hDataReadyEvent;            // Signaled when the stream thread has data ready
    PUSBM300_STREAM_BLOCK_QUEUE pQueue; // Queue storage
    PUSBM300_STREAM_BLOCK_QUEUE pHead;  // Queue head - modified by UsbStreamThread()
    PUSBM300_STREAM_BLOCK_QUEUE pTail;  // Queue tail - modified by 
} USB_STREAM_CONTROL, *PUSB_STREAM_CONTROL;


///////////////////////////////////////////////////////////////////////////////////////////////////
// USBM300 Class definition
///////////////////////////////////////////////////////////////////////////////////////////////////
class USBM300  
{
public:
	BOOLEAN GetStreamingData(DWORD dwDeviceIndex, BOOLEAN bWait, PUSBM300_STREAM_BLOCK pData, DWORD dwBlocksRequested, DWORD *dwBlocksReturned);
	BOOLEAN GetStreamingStatus(DWORD dwDeviceIndex, DWORD *dwQueueBlocksReady, DWORD *dwUsbStreamingState);
	BOOLEAN ControlStreaming(DWORD dwDeviceIndex, BOOLEAN bStart, DWORD dwQueueBlocksToAllocate);
	BOOLEAN DuplicateAddressPresent();
	ULONG GetClassVersion();
	LONG GetModuleAddress(DWORD dwDeviceIndex);
	BOOLEAN SendCommand(DWORD dwDeviceIndex, LPSTR sCmd, LPSTR sResp);
	void Enum_USBM300_Devices();
	DWORD m_dwUSBM300DeviceCount;

	USBM300();
	virtual ~USBM300();

private:
	void FreeMemoryAndHandles();
	BOOLEAN OpenDevice(DWORD dwDeviceIndex);
	LPSTR   m_sDeviceName[MAX_USBM300_DEVICES];                 // The PnP GUID based device name
    HANDLE  m_hDevice[MAX_USBM300_DEVICES];                     // Handle to m_sDeviceName
    HANDLE  m_hStreamThread[MAX_USBM300_DEVICES];               // Strean thread handle
    DWORD   m_dwStreamThreadId[MAX_USBM300_DEVICES];            // Stream thread ID
    USB_STREAM_CONTROL  m_UsbStreamCtrl[MAX_USBM300_DEVICES];   // Structure used by Stream thread and outside world
};

#endif // !defined(AFX_USBM300_H__8A9CC117_1F97_4CB5_AFD2_31BE82C8932C__INCLUDED_)
