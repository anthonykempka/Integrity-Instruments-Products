#if !defined(AFX_TERMINALDLG_H__3E503D16_83B9_11D2_9134_444553540000__INCLUDED_)
#define AFX_TERMINALDLG_H__3E503D16_83B9_11D2_9134_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// terminaldlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// terminaldlg dialog

class terminaldlg : public CDialog
{
// Construction
public:
	terminaldlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(terminaldlg)
	enum { IDD = IDD_TERMINALSETTINGS };
	BOOL	m_Append_LF;
	BOOL	m_Local_Echo;
	BOOL	m_bHex_Mode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(terminaldlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(terminaldlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERMINALDLG_H__3E503D16_83B9_11D2_9134_444553540000__INCLUDED_)
