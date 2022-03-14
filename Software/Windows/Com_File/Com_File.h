// Com_File.h : main header file for the COM_FILE application
//

#if !defined(AFX_COM_FILE_H__8EC350E4_BF66_11D2_9134_00600896DD50__INCLUDED_)
#define AFX_COM_FILE_H__8EC350E4_BF66_11D2_9134_00600896DD50__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCom_FileApp:
// See Com_File.cpp for the implementation of this class
//

class CCom_FileApp : public CWinApp
{
public:
	CCom_FileApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCom_FileApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCom_FileApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COM_FILE_H__8EC350E4_BF66_11D2_9134_00600896DD50__INCLUDED_)
