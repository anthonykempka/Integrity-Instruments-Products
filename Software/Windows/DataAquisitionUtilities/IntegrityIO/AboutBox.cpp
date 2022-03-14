// AboutBox.cpp : implementation file
//

#include "stdafx.h"
#include "IntegrityIO.h"
#include "AboutBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutBox dialog


CAboutBox::CAboutBox(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutBox)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutBox, CDialog)
	//{{AFX_MSG_MAP(CAboutBox)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutBox message handlers


BOOL CAboutBox::OnInitDialog() 
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
HBRUSH CAboutBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_DDI_WEB)
	{
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkMode(TRANSPARENT);
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CAboutBox::OnMouseMove(UINT nFlags, CPoint point) 
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

BOOL CAboutBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    if (m_bInStaticText) {
        SetCursor(m_hHandCursor);
		
    } else {
        SetCursor(m_hArrowCursor);
		
    }
	
	//return CDialog::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}

void CAboutBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
    //
    // Check if this mouse point falls in the DDI Web page window (static text)
    //
    if (ChildWindowFromPoint(point) == m_hStatic) {
        OpenLink();
    }
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CAboutBox::OpenLink()
{
    CString sError;

    HINSTANCE hRun = ShellExecute(m_hWnd, _T("open"), "www.integrityusa.com", 0, 0, SW_SHOW);
    if ((int) hRun <= 32)
    {
        sError.Format("ShellExecute() fails with code = %li\n", hRun);
        AfxMessageBox(sError, MB_ICONERROR);
    }	
}

void CAboutBox::DisplayErrors(CString sError)
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

