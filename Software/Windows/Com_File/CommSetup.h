#if !defined(AFX_COMMSETUP_H__8EC350EE_BF66_11D2_9134_00600896DD50__INCLUDED_)
#define AFX_COMMSETUP_H__8EC350EE_BF66_11D2_9134_00600896DD50__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CommSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommSetup dialog

class CCommSetup : public CDialog
{
// Construction
public:
	CCommSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCommSetup)
	enum { IDD = IDD_COMM_SETUP };
	CComboBox	m_BaudRate;
	CComboBox	m_Comport;
	int		m_Interval;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommSetup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMSETUP_H__8EC350EE_BF66_11D2_9134_00600896DD50__INCLUDED_)
