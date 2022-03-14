// USBM400_GUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

// USBM400 Communications class library
#include "C:\DATA\DRIVERCODE\USBM400_LIB\USBM400LIB.h"

#include "USBM400_GUI.h"
#include "USBM400_GUIDlg.h"

// Setup API library. Inform linker to use "SetupAPI.lib" library
#pragma comment(lib,"USBM400.lib") 


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUSBM400_GUIApp
BEGIN_MESSAGE_MAP(CUSBM400_GUIApp, CWinApp)
	//{{AFX_MSG_MAP(CUSBM400_GUIApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSBM400_GUIApp construction

CUSBM400_GUIApp::CUSBM400_GUIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUSBM400_GUIApp object

CUSBM400_GUIApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CUSBM400_GUIApp initialization

BOOL CUSBM400_GUIApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

    // #IF 0  To remove warning C4996
#if 0
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	CUSBM400_GUIDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
