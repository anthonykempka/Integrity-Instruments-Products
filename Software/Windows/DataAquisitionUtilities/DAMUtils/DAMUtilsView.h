// DAMUtilsView.h : interface of the CDAMUtilsView class
//


#pragma once

#include "moduletypes.h"
//#include "afxcmn.h"
#include "damtabs.h"
#include "integrityioctrl1.h"
//#include "IntegrityIOctrl1.h"



class CDAMUtilsView : public CFormView
{
protected: // create from serialization only
	CDAMUtilsView();
	DECLARE_DYNCREATE(CDAMUtilsView)

public:
	enum{ IDD = IDD_DAMUTILS_FORM };

// Attributes
public:
	CDAMUtilsDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CDAMUtilsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int m_tabCurSel;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	CButton	m_ctrlFixed;
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
protected:
	CDamTabs m_cDamTabs;
public:
	afx_msg void OnBnClickedBtnReadcurrent();
	afx_msg void OnBnClickedBtnConfigure();
	afx_msg void OnBnClickedBtnDefaults();
	afx_msg void OnBnClickedBtnCommsetup();
	afx_msg LRESULT OnTabSelChange(WPARAM wParam, LPARAM lParam);

	afx_msg void OnToolsStreammode();
	afx_msg void OnUpdateToolsStreammode(CCmdUI *pCmdUI);
	//CIntegrityioctrl1 m_axIICtrl;
	
	CIntegrityioctrl1 m_axIICtrl;
	afx_msg void OnToolsDatalogger();
	afx_msg void OnHelpContents();
	afx_msg void OnHelpIndex32778();
	CString m_binDirectory;
};

#ifndef _DEBUG  // debug version in DAMUtilsView.cpp
inline CDAMUtilsDoc* CDAMUtilsView::GetDocument() const
   { return reinterpret_cast<CDAMUtilsDoc*>(m_pDocument); }
#endif

