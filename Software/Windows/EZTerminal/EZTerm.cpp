// EZTerm.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "EZTerm.h"

#include "MainFrm.h"
#include "EZTermDoc.h"
#include "EZTermView.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEZTermApp

BEGIN_MESSAGE_MAP(CEZTermApp, CWinApp)
	//{{AFX_MSG_MAP(CEZTermApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEZTermApp construction

CEZTermApp::CEZTermApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEZTermApp object

CEZTermApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEZTermApp initialization

BOOL CEZTermApp::InitInstance()
{
	// CG: The following block was added by the Splash Screen component.
\
	{
\
		CCommandLineInfo cmdInfo;
\
		ParseCommandLine(cmdInfo);
\

\
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
\
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey("Integrity_Instruments");

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEZTermDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CEZTermView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_txtLic;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	void OpenLink();
	void DisplayErrors(CString sError);
	CFont font;
	BOOL	m_bInStaticText;
	HCURSOR m_hArrowCursor;
	HCURSOR m_hHandCursor;
	CStatic *m_hStatic;

};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_txtLic = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_EDIT1, m_txtLic);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CEZTermApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	
	aboutDlg.m_txtLic.Format( 
		"LICENSE AGREEMENT FOR INTEGRITY INSTRUMENTS SOFTWARE\r\n\r\n" \
		"This legal agreement between you, the customer or end user, and INTEGRITY INSTRUMENTS gives you certain rights, benefits, and obligations.  By breaking any sealed package or by using the product, you agree to be bound by the terms of this agreement and indicate that you have read, understood, and will comply with these terms.\r\n\r\n" \
		"1.	Product - The term product refers to the software supplied regardless of the media or means by which it is supplied.\r\n\r\n" \
		"2.  License - The license agreement that INTEGRITY INSTRUMENTS grants is to install, use, and modify the product in whatever manner you choose.\r\n\r\n" \
		"3.  You may not sell, lease, distribute, or release the product, in whole or in part, without explicit written permission from INTEGRITY INSTRUMENTS.\r\n\r\n" \
		"4.  INTEGRITY INSTRUMENTS makes no warranties to the product's contents, disclaims any implied warranties of any kind, except that the product will perform functionally in accordance with the written documentation supplied.\r\n\r\n" \
		"5.  INTEGRITY INSTRUMENTS shall not be liable to any person or entity for any direct, indirect, special, incidental or consequential damages resulting from the use or misuse of its product, including but not limited to loss of business, business profits, or business credibility." );
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CEZTermApp message handlers


BOOL CAboutDlg::OnInitDialog() 
{
	
	LOGFONT lf;
	CDialog::OnInitDialog();
	
//
    // Set the edit box font to a non-variable pitch font
    // Gain a pointer to the static text
    //
    memset(&lf, 0, sizeof(LOGFONT));
    m_hStatic = (CStatic*) GetDlgItem(IDC_STATIC_DDI_WEB);


    lf.lfHeight = 10;                       // request a 10-pixel-height font
    lf.lfUnderline = TRUE;                  //
    lf.lfWeight = FW_BOLD;
    strcpy(lf.lfFaceName, "MS Sans Serif"); 
    VERIFY(font.CreateFontIndirect(&lf));   // create the font



	//
    // Set the new font for the static text (WEB LINK)
    //
    m_hStatic->SetFont(&font);
	
    m_hArrowCursor = AfxGetApp()->LoadStandardCursor (IDC_ARROW);
	m_hHandCursor = AfxGetApp()->LoadCursor (IDC_HAND_CURSOR);

	TRACE("Hand Instance: 0x%08X\n", m_hHandCursor);
	TRACE("Arrow Instance: 0x%08X\n", m_hArrowCursor);


    m_bInStaticText = FALSE;		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_DDI_WEB)
	{
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkMode(TRANSPARENT);
	}
	return hbr;
}

void CAboutDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
    //
    // Check if this mouse point falls in the DDI Web page window (static text)
    //
    if (ChildWindowFromPoint(point) == m_hStatic) {
        OpenLink();
    }
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CAboutDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	//
    // Check if this mouse point falls in the bitmap window
    //
	
    if (ChildWindowFromPoint(point) == m_hStatic) {
        m_bInStaticText = TRUE;
    } else {
        m_bInStaticText = FALSE;
    }	
	CDialog::OnMouseMove(nFlags, point);
}

BOOL CAboutDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    if (m_bInStaticText) {
        SetCursor(m_hHandCursor);
		
    } else {
        SetCursor(m_hArrowCursor);
		
    }
	
	//return CDialog::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}

void CAboutDlg::DisplayErrors(CString sError)
{
    CString sMsg;
    LPVOID  lpMsgBuf;

    sMsg.Format("%s\r\n", sError);

    FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        0, // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL 
    );

    sMsg += (LPTSTR)lpMsgBuf;

    // Process any inserts in lpMsgBuf.
    // ...
    // Display the string.
    AfxMessageBox(sMsg, MB_OK | MB_ICONERROR | MB_TOPMOST);
    // Free the buffer.
    LocalFree( lpMsgBuf );

 

}

void CAboutDlg::OpenLink()
{
    CString sError;

    HINSTANCE hRun = ShellExecute(m_hWnd, _T("open"), "www.integrityusa.com", 0, 0, SW_SHOW);
    if ((int) hRun <= 32)
    {
        sError.Format("ShellExecute() fails with code = %li\n", hRun);
        AfxMessageBox(sError, MB_ICONERROR);
    }	

}

BOOL CEZTermApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}
