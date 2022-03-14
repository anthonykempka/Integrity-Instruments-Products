#pragma once
#include "Integrityioctrl1.h"
#include "afx.h"
#include "afxwin.h"
#include "integritybargraphctrl1.h"


// CDlgUsbStreamManager dialog

class CDlgUsbStreamManager : public CDialog
{
	DECLARE_DYNAMIC(CDlgUsbStreamManager)

public:
	CDlgUsbStreamManager(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUsbStreamManager();

// Dialog Data
	enum { IDD = IDD_DLGUSBSTREAMMGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnStart();
	afx_msg LRESULT OnUpdateUIData(WPARAM wParam, LPARAM lParam);
	CIntegrityioctrl1* m_cIICtrl;
	int m_nUnitAddress;
protected:
	CString m_cstrUnitAddress;
public:
	virtual BOOL OnInitDialog();
	CString m_cstrStatus;
	afx_msg void OnBnClickedBtnstop();
//	afx_msg void OnTimer(UINT nIDEvent);
	CString m_cstrDataBlocks;
	CString m_cstrAvailable;
	CString m_cstrDisplay;
	BOOL m_bLogToFile;
	CString m_cstrLogFilePath;
protected:
	CFile m_cfLogFile;
public:
	CButton m_btnOK;
	CButton m_btnCancel;
	CButton m_btnStart;
	CButton m_btnStop;
protected:
	DWORD m_dwStartTicks;
	DWORD m_dwRxByteCount;
public:
	CString m_cstrBPS;
	CString m_cstrRunTime;
//	CIntegritybargrapctrl1 m_cGraph;
	CIntegritybargrapctrl1 m_cBarGraph;
	static UINT StreamThreadProc(LPVOID pParam);
	CWinThread *m_pStreamThread;
	CEvent m_ceHaltThread;
//	afx_msg void OnBnClickedCheck1();
	long m_lnMessageLock;
	double m_dGraphMax;
};
