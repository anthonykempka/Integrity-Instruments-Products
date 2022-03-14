// VCExampleDlg.h : header file
//
//{{AFX_INCLUDES()
#include "integrityio.h"
//}}AFX_INCLUDES

#if !defined(AFX_VCEXAMPLEDLG_H__1FC3FD03_9074_46CB_855C_05F21052F7F1__INCLUDED_)
#define AFX_VCEXAMPLEDLG_H__1FC3FD03_9074_46CB_855C_05F21052F7F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVCExampleDlg dialog

class CVCExampleDlg : public CDialog
{
// Construction
public:
	CVCExampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVCExampleDlg)
	enum { IDD = IDD_VCEXAMPLE_DIALOG };
	CIntegrityIO	m_ctrlIntegrity;
	double	m_dADC0Volts;
	double	m_dDAC0Output;
	CString	m_lblPortStatus;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVCExampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVCExampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnTogglep1();
	afx_msg void OnBtnDac0out();
	afx_msg void OnBtnAdc0input();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCEXAMPLEDLG_H__1FC3FD03_9074_46CB_855C_05F21052F7F1__INCLUDED_)
