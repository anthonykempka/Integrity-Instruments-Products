// Module Setup.h : main header file for the MODULE SETUP application
//

#if !defined(AFX_MODULESETUP_H__A2D3ED25_B155_11D2_9134_444553540000__INCLUDED_)
#define AFX_MODULESETUP_H__A2D3ED25_B155_11D2_9134_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CModuleSetupApp:
// See Module Setup.cpp for the implementation of this class
//

class CModuleSetupApp : public CWinApp
{
public:
	CModuleSetupApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModuleSetupApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CModuleSetupApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODULESETUP_H__A2D3ED25_B155_11D2_9134_444553540000__INCLUDED_)
