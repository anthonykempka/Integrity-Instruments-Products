#if !defined(AFX_COMSETUP_H__25E7FB8F_B61E_11D2_9134_444553540000__INCLUDED_)
#define AFX_COMSETUP_H__25E7FB8F_B61E_11D2_9134_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ComSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComSetup dialog

class CComSetup : public CDialog
{
// Construction
public:
	CComSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComSetup)
	enum { IDD = IDD_COMM_SETUP };
	CButton	m_RS232_Check;
	CButton	m_RS485_Check;
	CComboBox	m_BaudRate;
	CComboBox	m_Comport;
	int		m_Module_Address;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComSetup)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRS485Radio();
	afx_msg void OnRS232Radio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMSETUP_H__25E7FB8F_B61E_11D2_9134_444553540000__INCLUDED_)
