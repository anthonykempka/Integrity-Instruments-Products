// EZTermView.h : interface of the CEZTermView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EZTERMVIEW_H__2E90E252_BC8F_40BF_8F31_4524BA1F4EED__INCLUDED_)
#define AFX_EZTERMVIEW_H__2E90E252_BC8F_40BF_8F31_4524BA1F4EED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDlgKeyLog;

class CEZTermView : public CScrollView
{
protected: // create from serialization only
	CEZTermView();
	DECLARE_DYNCREATE(CEZTermView)

// Attributes
public:
	CEZTermDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEZTermView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	CStatusBar* pStatus;
	BOOL m_bRedraw;
	virtual ~CEZTermView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	CDlgKeyLog* m_pDlgKeyLog;
	DWORD m_dwTimerCount;
	CSize m_cLastSize;
	CString PrintableCharacters;
	//{{AFX_MSG(CEZTermView)
	afx_msg void OnSettingsCommport();
	afx_msg void OnSettingsRtsdtr();
	afx_msg void OnSettingsTerminalemulation();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSettingsColorsRecieve();
	afx_msg void OnSettingsColorsTransmit();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSettingsKeyboardlog();
	afx_msg void OnUpdateSettingsKeyboardlog(CCmdUI* pCmdUI);
	afx_msg void OnSettingsHexmode();
	afx_msg void OnUpdateSettingsHexmode(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in EZTermView.cpp
inline CEZTermDoc* CEZTermView::GetDocument()
   { return (CEZTermDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EZTERMVIEW_H__2E90E252_BC8F_40BF_8F31_4524BA1F4EED__INCLUDED_)
