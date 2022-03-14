// USBM3000_GUI.h : main header file for the USBM3000_GUI application
//

#if !defined(AFX_USBM3000_GUI_H__47A846AC_0CC0_451E_81A4_B7D20DB95269__INCLUDED_)
#define AFX_USBM3000_GUI_H__47A846AC_0CC0_451E_81A4_B7D20DB95269__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUSBM3000_GUIApp:
// See USBM3000_GUI.cpp for the implementation of this class
//

class CUSBM3000_GUIApp : public CWinApp
{
public:
	CUSBM3000_GUIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUSBM3000_GUIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUSBM3000_GUIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USBM3000_GUI_H__47A846AC_0CC0_451E_81A4_B7D20DB95269__INCLUDED_)
