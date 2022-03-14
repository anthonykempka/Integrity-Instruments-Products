#if !defined(AFX_ABOUTBOX_H__0B4FE348_E3E9_4ACB_96D1_DBDBD129D933__INCLUDED_)
#define AFX_ABOUTBOX_H__0B4FE348_E3E9_4ACB_96D1_DBDBD129D933__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutBox dialog

class CAboutBox : public CDialog
{
// Construction
public:
	void DisplayErrors(CString sError);
	CAboutBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutBox)
	enum { IDD = IDD_ABOUTBOX_INTEGRITYIO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OpenLink();
	CFont font;
	BOOL	m_bInStaticText;
	HCURSOR m_hArrowCursor;
	HCURSOR m_hHandCursor;
	CStatic *m_hStatic;

	// Generated message map functions
	//{{AFX_MSG(CAboutBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTBOX_H__0B4FE348_E3E9_4ACB_96D1_DBDBD129D933__INCLUDED_)
