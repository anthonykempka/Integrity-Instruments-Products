#if !defined(AFX_ADCDLG_H__0AACDF41_B1EE_11D2_9134_444553540000__INCLUDED_)
#define AFX_ADCDLG_H__0AACDF41_B1EE_11D2_9134_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AdcDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdcDlg dialog

class CAdcDlg : public CDialog
{
// Construction
public:
	int bRS485_Check;
	int bTimerRunning;
	void RS485_Test_Adc();
	void RS232_Test_Adc();
	HANDLE hCom;
	CAdcDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdcDlg)
	enum { IDD = IDD_ADC_DIALOG };
	CString	m_Samp1;
	CString	m_Samp2;
	CString	m_Samp3;
	CString	m_Samp4;
	CString	m_Samp5;
	CString	m_Samp6;
	CString	m_Samp7;
	CString	m_Samp8;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdcDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADCDLG_H__0AACDF41_B1EE_11D2_9134_444553540000__INCLUDED_)
