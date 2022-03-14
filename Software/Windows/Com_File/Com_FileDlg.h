// Com_FileDlg.h : header file
//

#if !defined(AFX_COM_FILEDLG_H__8EC350E6_BF66_11D2_9134_00600896DD50__INCLUDED_)
#define AFX_COM_FILEDLG_H__8EC350E6_BF66_11D2_9134_00600896DD50__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "afxmt.h"

/////////////////////////////////////////////////////////////////////////////
// CCom_FileDlg dialog

class CCom_FileDlg : public CDialog
{
// Construction
public:
	BOOL bComportOpen;
	int Log_Interval;
	HANDLE hXmtFile;
	HANDLE hRcvFile;
	HANDLE hCom;
	BOOL bTimerRunning;
	CCom_FileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCom_FileDlg)
	enum { IDD = IDD_COM_FILE_DIALOG };
	CButton	m_CommSetup;
	CButton	m_XmtBrowse;
	CButton	m_RcvBrowse;
	CButton	m_Halt;
	CButton	m_Begin;
	CString	m_RcvFileName;
	CString	m_XmtFileName;
	int		m_RcvBytes;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCom_FileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCom_FileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCommSetup();
	afx_msg void OnBegin();
	afx_msg void OnHalt();
	afx_msg void OnXmtBrowse();
	afx_msg void OnRcvBrowse();
	afx_msg void OnSendFile();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CCriticalSection Comm_cs;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COM_FILEDLG_H__8EC350E6_BF66_11D2_9134_00600896DD50__INCLUDED_)
