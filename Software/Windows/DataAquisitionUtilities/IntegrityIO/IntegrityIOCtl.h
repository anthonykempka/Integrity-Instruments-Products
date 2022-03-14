/* 
 * Copyright (c) 2004-2005, Integrity Instruments, Inc.
 * All rights reserved. 
 */

#include "afxmt.h"
#if !defined(AFX_INTEGRITYIOCTL_H__B0AADD0F_8FC5_4C6A_A188_3085FABC8ADE__INCLUDED_)
#define AFX_INTEGRITYIOCTL_H__B0AADD0F_8FC5_4C6A_A188_3085FABC8ADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "USBStreamdata.h"

#define IIVERSION		0x00010000
#define PORT_CLOSED		0
#define PORT_OPENIDLE	1
#define PORT_OPENTX		2
#define PORT_OPENRX		3

#define OFFSET_ADDRESS  0x0F


// IntegrityIOCtl.h : Declaration of the CIntegrityIOCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOCtrl : See IntegrityIOCtl.cpp for implementation.


class CIntegrityIOCtrl : public COleControl
{
	DECLARE_DYNCREATE(CIntegrityIOCtrl)

// Constructor
public:
	CIntegrityIOCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIntegrityIOCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	LONG GetIndexFromAddress();
	USBM300 m_pUSB;
	CBitmap m_cRXBM;
	CBitmap m_cTXBM;
	CBitmap m_cDOnBM;
	CBitmap m_cDOffBM;
	DWORD WaitForResponse(unsigned char *buffer, unsigned short bufSize);
	CRect m_crBounds;
	void EmptyRxBuffer();
	char data[16];
	short m_nStreamPtr;
	BOOL m_bStreamModeFlag;
	short m_nEEAddress;
	short m_nDACControl;
	void SetOffsetCalibration();
	long m_lnOffsetCal;
	short m_nADCControl;
	void WaitForDraw();
	void SetRxOff();
	void SetRxOn();
	void SetTxOff();
	CString SendCommand(LPCTSTR cmd);
	long m_lnComTimeout;
	void CloseComm();
	short m_nDrawState;
	void SetTxOn();
	BOOL OpenComm();
	HANDLE m_hComm;

	long m_lnPWMDuty;
	long m_lnPWMFreq;

	BOOL m_bDialogsDisabled;
	double m_dReferenceVoltage;
	double m_dDACReferenceVoltage;
	long m_lnModuleAddress;
	long m_lnProtocol;
	long m_lnBaudRate;
	long m_lnComport;
	~CIntegrityIOCtrl();

	DECLARE_OLECREATE_EX(CIntegrityIOCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CIntegrityIOCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CIntegrityIOCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CIntegrityIOCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CIntegrityIOCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CIntegrityIOCtrl)
	afx_msg long GetLnComPort();
	afx_msg void SetLnComPort(long nNewValue);
	afx_msg long GetLnBaudRate();
	afx_msg void SetLnBaudRate(long nNewValue);
	afx_msg long GetLnProtocol();
	afx_msg void SetLnProtocol(long nNewValue);
	afx_msg long GetLnModuleAddress();
	afx_msg void SetLnModuleAddress(long nNewValue);
	afx_msg BOOL GetBPortOpen();
	afx_msg void SetBPortOpen(BOOL bNewValue);
	afx_msg long GetLnCommTimeout();
	afx_msg void SetLnCommTimeout(long nNewValue);
	afx_msg double GetDReferenceVoltage();
	afx_msg void SetDReferenceVoltage(double newValue);
	afx_msg long GetLnAbout();
	afx_msg void SetLnAbout(long nNewValue);
	afx_msg long GetLnLastError();
	afx_msg void SetLnLastError(long nNewValue);
	afx_msg BOOL GetBDialogDisable();
	afx_msg void SetBDialogDisable(BOOL bNewValue);
	afx_msg long GetLnSoftwareVersion();
	afx_msg void SetLnSoftwareVersion(long nNewValue);
	afx_msg long GetLnFirmwareVersion();
	afx_msg void SetLnFirmwareVersion(long nNewValue);
	afx_msg long GetLnDigitalPort();
	afx_msg void SetLnDigitalPort(long nNewValue);
	afx_msg long GetLnDigitalDirection();
	afx_msg void SetLnDigitalDirection(long nNewValue);
	afx_msg long GetLnPulseCount();
	afx_msg void SetLnPulseCount(long nNewValue);
	afx_msg long GetLnADCControl();
	afx_msg void SetLnADCControl(long nNewValue);
	afx_msg double GetDADCBipolar();
	afx_msg void SetDADCBipolar(double newValue);
	afx_msg long GetLnDACControl();
	afx_msg void SetLnDACControl(long nNewValue);
	afx_msg double GetDDACOutput();
	afx_msg void SetDDACOutput(double newValue);
	afx_msg long GetLnRcvErrCount();
	afx_msg void SetLnRcvErrCount(long nNewValue);
	afx_msg BOOL GetBPWMUpdate();
	afx_msg void SetBPWMUpdate(BOOL bNewValue);
	afx_msg long GetLnEEPROMAddress();
	afx_msg void SetLnEEPROMAddress(long nNewValue);
	afx_msg long GetLnEEPROMData();
	afx_msg void SetLnEEPROMData(long nNewValue);
	afx_msg BOOL GetBResetModule();
	afx_msg void SetBResetModule(BOOL bNewValue);
	afx_msg long GetLnPWMFreq();
	afx_msg void SetLnPWMFreq(long nNewValue);
	afx_msg long GetLnPWMDuty();
	afx_msg void SetLnPWMDuty(long nNewValue);
	afx_msg double GetDADCUnipolar();
	afx_msg void SetDADCUnipolar(double newValue);
	afx_msg void SetDIOBit(long bitValue, long bitPosition);
	afx_msg long GetDIOBit(long bitPosition);
	afx_msg void SetDIOBitDDR(long bitValue, long bitPosition);
	afx_msg long GetDIOBitDDR(short bitPosition);
	double GetdDACReferenceVoltage(void);
	void SetdDACReferenceVoltage(double newVal);
	LONG GetlnUSBStreamBlockCount(void);
	void SetlnUSBStreamBlockCount(LONG newVal);
	LONG GetUSBDataBlock(DOUBLE* adcData, LONG startIndex, LONG count);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CIntegrityIOCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	CRect m_crInvalid;
	BOOL m_bGetRects;
	enum {
		dispiddADCScale = 40,		dispidnADCBits = 39,	//{{AFX_DISP_ID(CIntegrityIOCtrl)
	dispidLnComPort = 1L,
	dispidLnBaudRate = 2L,
	dispidLnProtocol = 3L,
	dispidLnModuleAddress = 4L,
	dispidBPortOpen = 5L,
	dispidLnCommTimeout = 6L,
	dispidDReferenceVoltage = 7L,
	dispidLnAbout = 8L,
	dispidLnLastError = 9L,
	dispidBDialogDisable = 10L,
	dispidLnSoftwareVersion = 11L,
	dispidLnFirmwareVersion = 12L,
	dispidLnDigitalPort = 13L,
	dispidLnDigitalDirection = 14L,
	dispidLnPulseCount = 15L,
	dispidLnADCControl = 16L,
	dispidDADCBipolar = 17L,
	dispidLnDACControl = 18L,
	dispidDDACOutput = 19L,
	dispidLnRcvErrCount = 20L,
	dispidBPWMUpdate = 21L,
	dispidLnEEPROMAddress = 22L,
	dispidLnEEPROMData = 23L,
	dispidBResetModule = 24L,
	dispidLnPWMFreq = 25L,
	dispidLnPWMDuty = 26L,
	dispidDADCUnipolar = 27L,
	dispidSetDIOBit = 28L,
	dispidGetDIOBit = 29L,
	dispidSetDIOBitDDR = 30L,
	dispidGetDIOBitDDR = 31L,
	dispidbInvalidate = 32L,
	dispidlnAnalogGain = 33L,		
	dispidbModeStream = 34L,		
	dispiddDACReferenceVoltage = 35L,
	dispidGetUSBModuleAddress = 36L,
	dispidlnUSBStreamBlockCount = 37,
	dispidGetUSBDataBlock = 38L,
//}}AFX_DISP_ID
	};
protected:
	void OnbInvalidateChanged(void);
	VARIANT_BOOL m_bInvalidate;
	LONG GetlnAnalogGain(void);
	void SetlnAnalogGain(LONG newVal);
	long m_lnAnalogGain;
	BOOL GetbModeStream(void);
	void SetbModeStream(BOOL newVal);
	boolean m_bModeStream;
	LONG GetUSBModuleAddress(LONG moduleIndex);

	CEvent m_ceHaltThread;
	CWinThread *m_pStreamThread;

	static UINT usbThreadProc(LPVOID pParam);
	long m_lnUSBStreamBlockCount;

	CArray<CUSBStreamdata,CUSBStreamdata> m_cUSBStreamData;
	
	short m_nAdcTwosCompShift;
	SHORT m_nADCBits;
	DOUBLE m_dADCScale;
	SHORT GetnADCBits(void);
	void SetnADCBits(SHORT newVal);
	DOUBLE GetdADCScale(void);
	void SetdADCScale(DOUBLE newVal);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTEGRITYIOCTL_H__B0AADD0F_8FC5_4C6A_A188_3085FABC8ADE__INCLUDED)
