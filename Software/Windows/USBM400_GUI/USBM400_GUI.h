// USBM400_GUI.h : main header file for the USBM400_GUI application
//

#if !defined(AFX_USBM400_GUI_H__47A846AC_0CC0_451E_81A4_B7D20DB95269__INCLUDED_)
#define AFX_USBM400_GUI_H__47A846AC_0CC0_451E_81A4_B7D20DB95269__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUSBM400_GUIApp:
// See USBM400_GUI.cpp for the implementation of this class
//

class CUSBM400_GUIApp : public CWinApp
{
public:
	CUSBM400_GUIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUSBM400_GUIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUSBM400_GUIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USBM400_GUI_H__47A846AC_0CC0_451E_81A4_B7D20DB95269__INCLUDED_)
