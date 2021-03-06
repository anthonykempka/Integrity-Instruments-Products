#if !defined(AFX_INTEGRITYIO_H__EE6D3CE1_2CD5_48E5_B14D_56293A57F67F__INCLUDED_)
#define AFX_INTEGRITYIO_H__EE6D3CE1_2CD5_48E5_B14D_56293A57F67F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CIntegrityIO wrapper class

class CIntegrityIO : public CWnd
{
protected:
	DECLARE_DYNCREATE(CIntegrityIO)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xb9b93b71, 0x3ea9, 0x4bdd, { 0x97, 0x34, 0xcd, 0x28, 0xdf, 0x66, 0xeb, 0x75 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:
	long GetLnComPort();
	void SetLnComPort(long);
	long GetLnBaudRate();
	void SetLnBaudRate(long);
	long GetLnProtocol();
	void SetLnProtocol(long);
	long GetLnModuleAddress();
	void SetLnModuleAddress(long);
	BOOL GetBPortOpen();
	void SetBPortOpen(BOOL);
	long GetLnCommTimeout();
	void SetLnCommTimeout(long);
	double GetDReferenceVoltage();
	void SetDReferenceVoltage(double);
	long GetLnAbout();
	void SetLnAbout(long);
	long GetLnLastError();
	void SetLnLastError(long);
	BOOL GetBDialogDisable();
	void SetBDialogDisable(BOOL);
	long GetLnSoftwareVersion();
	void SetLnSoftwareVersion(long);
	long GetLnFirmwareVersion();
	void SetLnFirmwareVersion(long);
	long GetLnDigitalPort();
	void SetLnDigitalPort(long);
	long GetLnDigitalDirection();
	void SetLnDigitalDirection(long);
	long GetLnPulseCount();
	void SetLnPulseCount(long);
	long GetLnADCControl();
	void SetLnADCControl(long);
	double GetDADCBipolar();
	void SetDADCBipolar(double);
	long GetLnDACControl();
	void SetLnDACControl(long);
	double GetDDACOutput();
	void SetDDACOutput(double);
	long GetLnRcvErrCount();
	void SetLnRcvErrCount(long);
	BOOL GetBPWMUpdate();
	void SetBPWMUpdate(BOOL);
	long GetLnEEPROMAddress();
	void SetLnEEPROMAddress(long);
	long GetLnEEPROMData();
	void SetLnEEPROMData(long);
	BOOL GetBResetModule();
	void SetBResetModule(BOOL);
	long GetLnPWMFreq();
	void SetLnPWMFreq(long);
	long GetLnPWMDuty();
	void SetLnPWMDuty(long);
	double GetDADCUnipolar();
	void SetDADCUnipolar(double);

// Operations
public:
	void SetDIOBit(long bitValue, long bitPosition);
	long GetDIOBit(long bitPosition);
	void SetDIOBitDDR(long bitValue, long bitPosition);
	long GetDIOBitDDR(short bitPosition);
	void AboutBox();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTEGRITYIO_H__EE6D3CE1_2CD5_48E5_B14D_56293A57F67F__INCLUDED_)
