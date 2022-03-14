#if !defined(AFX_RTSCONTROLDLG_H__3E503D14_83B9_11D2_9134_444553540000__INCLUDED_)
#define AFX_RTSCONTROLDLG_H__3E503D14_83B9_11D2_9134_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RtsControlDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RtsControlDlg dialog

class RtsControlDlg : public CDialog
{
// Construction
public:
	RtsControlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RtsControlDlg)
	enum { IDD = IDD_RTSCONTROL };
	BOOL	m_DTR_ON;
	BOOL	m_RTS_ON;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RtsControlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RtsControlDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTSCONTROLDLG_H__3E503D14_83B9_11D2_9134_444553540000__INCLUDED_)
