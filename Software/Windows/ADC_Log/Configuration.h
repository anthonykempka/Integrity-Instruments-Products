#if !defined(AFX_CONFIGURATION_H__BE3EBD80_B699_11D2_9134_00600896DD50__INCLUDED_)
#define AFX_CONFIGURATION_H__BE3EBD80_B699_11D2_9134_00600896DD50__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Configuration.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfiguration dialog

class CConfiguration : public CDialog
{
// Construction
public:
	CString m_Response_Str;
	CString m_Command_Str;
	HANDLE hCom;
	void RS232_Offset();
	void RS485_Offset();
	CConfiguration(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfiguration)
	enum { IDD = IDD_CONFIG_DIALOG };
	CComboBox	m_Sample8;
	CComboBox	m_Sample7;
	CComboBox	m_Sample6;
	CComboBox	m_Sample5;
	CComboBox	m_Sample4;
	CComboBox	m_Sample3;
	CComboBox	m_Sample2;
	CComboBox	m_Sample1;
	CComboBox	m_Vref;
	CButton	m_Uni_Sample8;
	CButton	m_Uni_Sample7;
	CButton	m_Uni_Sample6;
	CButton	m_Uni_Sample5;
	CButton	m_Uni_Sample4;
	CButton	m_Uni_Sample3;
	CButton	m_Uni_Sample2;
	CButton	m_Uni_Sample1;
	DWORD	m_Log_Interval;
	int		m_Filter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfiguration)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfiguration)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnOffset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGURATION_H__BE3EBD80_B699_11D2_9134_00600896DD50__INCLUDED_)
