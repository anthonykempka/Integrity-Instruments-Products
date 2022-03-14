// USBM400_GUIDlg.h : header file
//

#if !defined(AFX_USBM400_GUIDLG_H__BC386C08_7648_45E5_B759_53FC7E259A3F__INCLUDED_)
#define AFX_USBM400_GUIDLG_H__BC386C08_7648_45E5_B759_53FC7E259A3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CUSBM400_GUIDlg dialog

class CUSBM400_GUIDlg : public CDialog
{
// Construction
public:
	BOOLEAN m_bStopLoop;
	USBM400 m_USBM400_Dev;
	BOOLEAN m_bIdent_On;
	CUSBM400_GUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUSBM400_GUIDlg)
	enum { IDD = IDD_USBM400_GUI_DIALOG };
	CListBox	m_Device_List;
	CString	m_Command_Edit;
	CString	m_Response_Edit;
	CString	m_sId_Static;
	CString	m_sStream_Static;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUSBM400_GUIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUSBM400_GUIDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSendButton();
	afx_msg void OnIdentButton();
	afx_msg void OnRefreshButton();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDuplButton();
	afx_msg void OnStreamButton();
	afx_msg void OnGetAddress();
	afx_msg void OnSetAddress();
	afx_msg void OnSpeedButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    CString m_sSerialNumber;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USBM400_GUIDLG_H__BC386C08_7648_45E5_B759_53FC7E259A3F__INCLUDED_)
