// LineGraphTestDlg.h : header file
//

#pragma once
#include "integritylinegractrl1.h"
#include "integritybargrapctrl1.h"


// CLineGraphTestDlg dialog
class CLineGraphTestDlg : public CDialog
{
// Construction
public:
	CLineGraphTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LINEGRAPHTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CIntegritylinegractrl1 m_cGraph;
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	CIntegritybargrapctrl1 m_cBargraph;
	CIntegritylinegractrl1 m_cLineGraph2;
	CString m_strMinValue;
	CString m_strMaxValue;
	afx_msg void OnEnChangeTxtminvalue();
	afx_msg void OnEnChangeTxtmaxvalue();
};
