// EZTermDoc.h : interface of the CEZTermDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EZTERMDOC_H__8F5CB305_97E0_4F64_B4A9_3B23B0728650__INCLUDED_)
#define AFX_EZTERMDOC_H__8F5CB305_97E0_4F64_B4A9_3B23B0728650__INCLUDED_

#include "Comm32.h"	// Added by ClassView
#include "Connection.h"	// Added by ClassView
#include "ConsoleData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEZTermDoc : public CDocument
{
protected: // create from serialization only
	CEZTermDoc();
	DECLARE_DYNCREATE(CEZTermDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEZTermDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTextSize(CDC *pDC);
	CComm32 m_cComm;
	void insertRxChars();
	int m_nBufTail;
	int m_nBufHead;
	void StopMonitor();
	void StartMonitor(CDC* pDC);
	CEvent m_ceStopMonitor;
	BOOL m_bDisplayMode;
	CFont m_cFont;
	CSize getSize();
	void drawDocument(CDC* pDC);
	void insertChar(UINT nChar, UINT source);
	CObList m_cDataObjects;
	CConnection m_cConnection;
	virtual ~CEZTermDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	BOOL m_bRedraw;


// Generated message map functions
protected:
	CSize m_cSizeTotal;
	CFile m_cfLogFile;
	BOOL m_bLoggingToFile;
	CString m_strLogFileName;
	UINT_PTR m_upTimer;
	unsigned char buffer[RX_BUFSIZE];
	BOOL m_bMonitorActive;
	static UINT commMonitor(LPVOID pParam);
	CConsoleData* addNewObject(UINT source);
	UINT m_nMaxLines;
	UINT m_nLastSource;
	//{{AFX_MSG(CEZTermDoc)
	afx_msg void OnLoggingSetfilename();
	afx_msg void OnLoggingStartlogging();
	afx_msg void OnUpdateLoggingStartlogging(CCmdUI* pCmdUI);
	afx_msg void OnLoggingStoplogging();
	afx_msg void OnUpdateLoggingStoplogging(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EZTERMDOC_H__8F5CB305_97E0_4F64_B4A9_3B23B0728650__INCLUDED_)
