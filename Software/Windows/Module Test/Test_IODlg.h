// Test_IODlg.h : header file
//

#if !defined(AFX_TEST_IODLG_H__F83BA9A7_AF80_11D2_9134_444553540000__INCLUDED_)
#define AFX_TEST_IODLG_H__F83BA9A7_AF80_11D2_9134_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CTest_IODlg dialog

class CTest_IODlg : public CDialog
{
// Construction
public:
	HANDLE hCom;
	int m_ComPort;
	int m_BaudRate;

    void Stream_Speed(BOOL bRS485);
    void Polled_Speed(BOOL bRS485);
	void Snr_Test(BOOL bRS485);
	void Test_Digital(BOOL bRS485);
    void DA_Test(BOOL bRS485) ;

    BOOL Send_Command(CString sCommand, CString sResponse, BOOL bRS485 = FALSE);

    static CTest_IODlg * pThis;
    static void CALLBACK EXPORT TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

	CTest_IODlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTest_IODlg)
	enum { IDD = IDD_TEST_IO_DIALOG };
	CButton	m_232M200_Check;
	CButton	m_USB_Check;
	CProgressCtrl	m_ProgressBar;
	CButton	m_RS485_Check;
	CButton	m_RS232_Check;
	CString	m_Command_Str;
	CString	m_Response_Str;
	CString	m_sTestEdit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_IODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTest_IODlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCommButton();
	afx_msg void OnRS485Radio();
	afx_msg void OnRS232Radio();
	afx_msg void OnSpeedTest();
	afx_msg void OnUSBRadio();
	afx_msg void OnStreamSpeedTest();
	afx_msg void OnTestDigital();
	afx_msg void OnDaTest();
	afx_msg void OnAdcSnr();
	afx_msg void On232m200Check();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_IODLG_H__F83BA9A7_AF80_11D2_9134_444553540000__INCLUDED_)
