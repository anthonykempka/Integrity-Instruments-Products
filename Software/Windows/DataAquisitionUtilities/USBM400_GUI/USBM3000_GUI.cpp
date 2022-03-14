// USBM3000_GUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

// USBM300 Communications class
#include "..\USBM300 Class\USBM300.h"

#include "USBM3000_GUI.h"
#include "USBM3000_GUIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUSBM3000_GUIApp

BEGIN_MESSAGE_MAP(CUSBM3000_GUIApp, CWinApp)
	//{{AFX_MSG_MAP(CUSBM3000_GUIApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSBM3000_GUIApp construction

CUSBM3000_GUIApp::CUSBM3000_GUIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUSBM3000_GUIApp object

CUSBM3000_GUIApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CUSBM3000_GUIApp initialization

BOOL CUSBM3000_GUIApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CUSBM3000_GUIDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
