// Com_FileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Com_File.h"
#include "Com_FileDlg.h"
#include "CommSetup.h"
#include "Comm32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCom_FileDlg dialog

CCom_FileDlg::CCom_FileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCom_FileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCom_FileDlg)
	m_RcvFileName = _T("");
	m_XmtFileName = _T("");
	m_RcvBytes = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCom_FileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCom_FileDlg)
	DDX_Control(pDX, IDC_COMM_SETUP, m_CommSetup);
	DDX_Control(pDX, IDC_XMT_BROWSE, m_XmtBrowse);
	DDX_Control(pDX, IDC_RCV_BROWSE, m_RcvBrowse);
	DDX_Control(pDX, IDC_HALT, m_Halt);
	DDX_Control(pDX, IDC_BEGIN, m_Begin);
	DDX_Text(pDX, IDC_RCV_FILENAME, m_RcvFileName);
	DDX_Text(pDX, IDC_XMT_FILENAME, m_XmtFileName);
	DDX_Text(pDX, IDC_RCV_BYTES, m_RcvBytes);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCom_FileDlg, CDialog)
	//{{AFX_MSG_MAP(CCom_FileDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COMM_SETUP, OnCommSetup)
	ON_BN_CLICKED(IDC_BEGIN, OnBegin)
	ON_BN_CLICKED(IDC_HALT, OnHalt)
	ON_BN_CLICKED(IDC_XMT_BROWSE, OnXmtBrowse)
	ON_BN_CLICKED(IDC_RCV_BROWSE, OnRcvBrowse)
	ON_BN_CLICKED(IDC_SEND_FILE, OnSendFile)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCom_FileDlg message handlers

BOOL CCom_FileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Read the Communications Settings registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);

    // Default NULL if no setting found
	m_RcvFileName = pApp->GetProfileString("Settings", "RcvFilename");

    // Default NULL if no setting found
	m_XmtFileName = pApp->GetProfileString("Settings", "XmtFilename");

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
    //
    // Disable the HALT button since we're not sampling yet
    //
    m_Halt.EnableWindow(FALSE);

    bTimerRunning = FALSE;
    bComportOpen = FALSE;

    m_RcvBytes = 0;
	
	// TODO: Add extra initialization here

    SetWindowText ("Com_File - Halted");

    UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCom_FileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCom_FileDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCom_FileDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCom_FileDlg::OnCommSetup() 
{
    CCommSetup dlg;

    dlg.DoModal();
	
}

void CCom_FileDlg::OnBegin() 
{
    int     BaudRate, ComPort;
    int     MsgRtn;

    //Update the Dialog objects from member vars
    UpdateData(TRUE);

    if (m_RcvFileName == "") {
        MessageBox ("Receive File Name must be entered!", "Error", MB_ICONERROR);
        m_RcvBrowse.SetFocus();
        return;
    }

    hRcvFile = :: CreateFile (m_RcvFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
                              NULL, OPEN_EXISTING, 0, NULL);

    if (hRcvFile != INVALID_HANDLE_VALUE) {

        MsgRtn = AfxMessageBox ("YES to append to existing file.\n\nNO to over-write existing File\n\nCANCEL to exit", MB_YESNOCANCEL);
        if (MsgRtn == IDCANCEL) {
            ::CloseHandle (hRcvFile);
            return;
        }

        if (MsgRtn == IDNO) {

            //
            // This resets the EOF to the begining
            ::SetEndOfFile(hRcvFile);
        }

    } else {
        //
        // Create a new file
        hRcvFile = :: CreateFile (m_RcvFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
                                  NULL, CREATE_NEW, 0, NULL);

        if (hRcvFile == INVALID_HANDLE_VALUE) {
            MessageBox ("Cannot create receive file!", "Error", MB_ICONERROR);
            return;
        }

    }

    //
    // Bump to the end of the file
    ::SetFilePointer(hRcvFile, 0, NULL, FILE_END);

    m_RcvBytes = 0;
    UpdateData (FALSE);

    //
    // Sanity check for timer already running

    if (bTimerRunning) {        // Don't do anything if it's already running.
        AfxMessageBox("Sampling already started Bozo!");
        return;
    }

    //Read the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);

    //
    // Only open a comport if it isn't open
    if (!bComportOpen) {

        // Default 1 (COM2) if no setting found
	    ComPort = pApp->GetProfileInt("Settings", "ComPort", 1);

        // Default 0 (115200) if not setting found
	    BaudRate = pApp->GetProfileInt("Settings", "BaudRate", 0);

        //
        // Adjust zero based offset to COMx number
        ComPort += 1;
    
        //
        // Convert from zero based offset to baud rate
        switch (BaudRate) {
            case 0:
                BaudRate = 110;
                break;
            case 1:
                BaudRate = 300;
                break;
            case 2:
                BaudRate = 600;
                break;
            case 3:
                BaudRate = 1200;
                break;
            case 4: 
                BaudRate = 2400;
                break;
            case 5:
                BaudRate = 4800;
                break;
            case 6:
                BaudRate = 9600;
                break;
            case 7:
                BaudRate = 19200;
                break;
            case 8:
                BaudRate = 38400;
                break;
            case 9:
                BaudRate = 57600;
                break;
            case 10: 
            default:
                BaudRate = 115200;
                break;
        }

        //
        // Attempt to open a Comport
        //

        hCom = ::Open_Comport (ComPort, BaudRate);

        if (hCom != INVALID_HANDLE_VALUE) {

            bComportOpen = TRUE;
        } else {
            MessageBox ("Cannot open Comport!", "Error", MB_ICONERROR);
            ::CloseHandle (hRcvFile);
            bComportOpen = FALSE;
        }
    }

    if (bComportOpen) {
        //
        // Enable/disable buttons
        m_Halt.EnableWindow(TRUE);
        m_Halt.SetFocus();
        m_Begin.EnableWindow(FALSE);
        m_CommSetup.EnableWindow(FALSE);

        SetWindowText ("Com_File - Logging");

        //
        // Log_Interval is in .1 second increments = must be converted to milliseconds.
        Log_Interval = pApp->GetProfileInt("Settings", "Interval", 1);
        Log_Interval *= 1000;

        SetTimer (1, Log_Interval, NULL);

        bTimerRunning = TRUE;
    }
}

void CCom_FileDlg::OnHalt() 
{
    m_Halt.EnableWindow(FALSE);
    m_Begin.EnableWindow(TRUE);
    m_Begin.SetFocus();
    m_CommSetup.EnableWindow(TRUE);

    SetWindowText ("Com_File - Halted");

    if (bTimerRunning) {        // Only kill timer if one was made
        bTimerRunning = FALSE;
        KillTimer(1);
        ::CloseHandle (hRcvFile);
    }

    if (bComportOpen) {         // Only close comport if one is open
        ::Close_Comport (hCom);
        bComportOpen = FALSE;
    }

    UpdateData(FALSE);
}

void CCom_FileDlg::OnXmtBrowse() 
{
    static char BASED_CODE szFilter[] = "All Files (*.*)|*.*|Data Files (*.dat)|*.dat|ASCII Files (*.asc)|*.asc|Text Files (*.txt)|*.txt||";

    //Update the Dialog objects from member vars
    UpdateData(TRUE);
    
    CFileDialog cDlg(TRUE, NULL, m_XmtFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

    if(IDOK == cDlg.DoModal())
    {
        m_XmtFileName = cDlg.GetPathName();

        //Update Member vars from dialog objects
        UpdateData(FALSE);

        //
        // Save off filename in registry

    	CWinApp* pApp = AfxGetApp();
	    ASSERT_VALID(pApp);

        pApp->WriteProfileString("Settings", "XmtFilename", m_XmtFileName);
    }	
	
}

void CCom_FileDlg::OnRcvBrowse() 
{
    static char BASED_CODE szFilter[] = "All Files (*.*)|*.*|Data Files (*.dat)|*.dat|ASCII Files (*.asc)|*.asc|Text Files (*.txt)|*.txt||";

    //Update the Dialog objects from member vars
    UpdateData(TRUE);
    
    CFileDialog cDlg(TRUE, NULL, m_RcvFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

    if(IDOK == cDlg.DoModal())
    {
        m_RcvFileName = cDlg.GetPathName();

        //Update Member vars from dialog objects
        UpdateData(FALSE);

        //
        // Save off filename in registry

    	CWinApp* pApp = AfxGetApp();
	    ASSERT_VALID(pApp);

        pApp->WriteProfileString("Settings", "RcvFilename", m_RcvFileName);
    }	
	
}

void CCom_FileDlg::OnSendFile() 
{
    CWaitCursor Cur;        // Displays the Wait cursor
    DWORD   BytesRead;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);
    int     ComPort, BaudRate;

    //Update the Dialog objects from member vars
    UpdateData(TRUE);

    //
    // Check if there is a transmit filename
    if (m_XmtFileName == "") {
        MessageBox ("Transmit File Name must be entered!", "Error", MB_ICONERROR);
        m_XmtBrowse.SetFocus();
        return;
    }

    //
    // See if we can open the file to transmit
    hXmtFile = :: CreateFile (m_XmtFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
                              NULL, OPEN_EXISTING, 0, NULL);

    if (hXmtFile == INVALID_HANDLE_VALUE) {
        MessageBox ("Cannot open transmit file!", "Error", MB_ICONERROR);
        return;
    }

    //
    // Check if Comport is already open
    if (!bComportOpen) {
    
	    //Read the registry entrys
	    CWinApp* pApp = AfxGetApp();
	    ASSERT_VALID(pApp);

        // Default 1 (COM2) if no setting found
	    ComPort = pApp->GetProfileInt("Settings", "ComPort", 1);

        // Default 0 (115200) if not setting found
	    BaudRate = pApp->GetProfileInt("Settings", "BaudRate", 0);

        //
        // Adjust zero based offset to COMx number
        ComPort += 1;
    
        //
        // Convert from zero based offset to baud rate
        switch (BaudRate) {
            case 0:
                BaudRate = 110;
                break;
            case 1:
                BaudRate = 300;
                break;
            case 2:
                BaudRate = 600;
                break;
            case 3:
                BaudRate = 1200;
                break;
            case 4: 
                BaudRate = 2400;
                break;
            case 5:
                BaudRate = 4800;
                break;
            case 6:
                BaudRate = 9600;
                break;
            case 7:
                BaudRate = 19200;
                break;
            case 8:
                BaudRate = 38400;
                break;
            case 9:
                BaudRate = 57600;
                break;
            case 10: 
            default:
                BaudRate = 115200;
                break;
        }

        //
        // Attempt to open a Comport
        //

        hCom = ::Open_Comport (ComPort, BaudRate);

        if (hCom != INVALID_HANDLE_VALUE) {
            bComportOpen = TRUE;
        } else {
            MessageBox ("Cannot open Comport!", "Error", MB_ICONERROR);
            ::CloseHandle (hXmtFile);
            bComportOpen = FALSE;
        }
    }

    if (bComportOpen) {

        Comm_cs.Lock();

        //
        // Keep reading from file and writing to comport until all gone
        do {
            ::ReadFile (hXmtFile, Buffer, BufferSize, &BytesRead, NULL);

            //
            // Only smit data is there is some
            if (BytesRead > 0) {
                ::Write_Comport (hCom, BytesRead, Buffer);
            }

        } while (BytesRead != 0);

        Comm_cs.Unlock();

        //
        // Close open file
        ::CloseHandle (hXmtFile);
    }
}

void CCom_FileDlg::OnTimer(UINT nIDEvent) 
{
    DWORD   BytesRead, BytesWritten;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);

    //
    // Check the Timer event ID
    if (nIDEvent == 1) {

        Comm_cs.Lock();

        //
        // Read comport data until it's all gone
        do {

            ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);

            //
            // Only write data to file if there is some
            if (BytesRead > 0) {
                ::WriteFile(hRcvFile, Buffer, BytesRead, &BytesWritten, NULL);
                m_RcvBytes += BytesRead;
            }
        } while (BytesRead > 0);

        Comm_cs.Unlock();

        UpdateData (FALSE);
    }
	
	CDialog::OnTimer(nIDEvent);
}



void CCom_FileDlg::OnCancel() 
{
    if (bTimerRunning) {        // Only kill timer if one was made
        bTimerRunning = FALSE;
        KillTimer(1);
        ::CloseHandle (hRcvFile);
    }

    if (bComportOpen) {         // Only close comport if one is open
        ::Close_Comport (hCom);
        bComportOpen = FALSE;
    }
	
	CDialog::OnCancel();
}
