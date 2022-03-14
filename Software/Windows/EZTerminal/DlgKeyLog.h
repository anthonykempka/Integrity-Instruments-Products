#if !defined(AFX_DLGKEYLOG_H__4E7B24F8_9EF8_40D6_BC15_CEC3023CCB79__INCLUDED_)
#define AFX_DLGKEYLOG_H__4E7B24F8_9EF8_40D6_BC15_CEC3023CCB79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgKeyLog.h : header file
//

#define WM_CLOSEKL	WM_USER + 5

// 
// Forward class declarations
//

/////////////////////////////////////////////////////////////////////////////
// CDlgKeyLog dialog


class CDlgKeyLog : public CDialog
{
// Construction
public:
	CDlgKeyLog(CWnd* pParent = NULL);   // standard constructor
	CDlgKeyLog(CView *pView);
	BOOL Create();

// Dialog Data
	//{{AFX_DATA(CDlgKeyLog)
	enum { IDD = IDD_DLGKBDDISPLAY };
	CString	m_strKeyLog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgKeyLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgKeyLog)
	afx_msg void OnSetfocusEditKeylog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CView* m_pView;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGKEYLOG_H__4E7B24F8_9EF8_40D6_BC15_CEC3023CCB79__INCLUDED_)
