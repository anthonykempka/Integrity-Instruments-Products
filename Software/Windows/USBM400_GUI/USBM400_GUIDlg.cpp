// USBM400_GUIDlg.cpp : implementation file
//

#include "stdafx.h"

// Disable warnings about string functions like strcpy() and sprintf()
#pragma warning (disable: 4996)

// USBM400 Communications class library
#include "C:\DATA\DRIVERCODE\USBM400_LIB\USBM400LIB.h"

#include "USBM400_GUI.h"
#include "USBM400_GUIDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//
class CAboutDlg : public CDialog
{
public:
	void OpenLink();
	HCURSOR     m_hArrowCursor;
	HCURSOR     m_hHandCursor;
	BOOLEAN     m_bInStaticText;
    CFont       font;
    CStatic     *m_hLink;

	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_sBuildDate;
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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_sBuildDate = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_BUILD_DATE_STATIC, m_sBuildDate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog functions
//

/////////////////////////////////////////////////////////////////////////////
// WM_INIT Handler
//
BOOL CAboutDlg::OnInitDialog() 
{
    LOGFONT lf;

    //
    // Call the base class
    //
	CDialog::OnInitDialog();
	
    //
    // Set the edit box font to a non-variable pitch font
    // Gain a handle to the static text
    //
    memset(&lf, 0, sizeof(LOGFONT));
    m_hLink = (CStatic*) GetDlgItem(IDC_STATIC_INTEGRITY_WEB);

    lf.lfHeight = 10;                       // request a 10-pixel-height font
    lf.lfUnderline = TRUE;                  // Underline and bold the font
    lf.lfWeight = FW_BOLD;
    strcpy(lf.lfFaceName, "MS Sans Serif"); 
    VERIFY(font.CreateFontIndirect(&lf));   // create the font

    //
    // Set the new font for the static text (WEB LINK)
    //
    m_hLink->SetFont(&font);

//    m_brush.CreateSolidBrush(RGB(200,200,200));   // Black color

    //
    // Load the cursors.
    // Note: The Win98 and earlier do not have IDC_HAND cursor defined, so use our own.
    //
    m_hHandCursor = AfxGetApp()->LoadCursor(IDC_HAND_CURSOR);
    m_hArrowCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

    // Not in hyperlink to start with
    m_bInStaticText = FALSE;

    //
    // Set the current build date
    //
    m_sBuildDate.Format("Build Date:  %s", __DATE__);
    UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// WM_CTLCOLOR Handler
//
HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
    // Are we painting the IDC_STATIC_INTEGRITY_WEB control?
    // Use CWnd::GetDlgCtrlID() to perform the most efficient test
    //
    if (pWnd->GetDlgCtrlID() == IDC_STATIC_INTEGRITY_WEB)
    {
        // Set the text color to BLUE
        pDC->SetTextColor(RGB(0, 0, 255));

        // Set the background mode for text to transparent so any background will show through
        pDC->SetBkMode(TRANSPARENT);
    }
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


/////////////////////////////////////////////////////////////////////////////
// WM_LBUTTONUP Handler
//
void CAboutDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
    //
    // Check if this mouse point falls in the Hyperlink window (static text)
    //
    if (ChildWindowFromPoint(point) == m_hLink) {
        OpenLink();
    }
	
	CDialog::OnLButtonUp(nFlags, point);
}


/////////////////////////////////////////////////////////////////////////////
// WM_MOUSEMOVE Handler
//
void CAboutDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
    //
    // Check if this mouse point falls in the bitmap window
    //
    if (ChildWindowFromPoint(point) == m_hLink) {
        m_bInStaticText = TRUE;
    } else {
        m_bInStaticText = FALSE;
    }

	CDialog::OnMouseMove(nFlags, point);
}


/////////////////////////////////////////////////////////////////////////////
// WM_SETCURSOR Handler
//
BOOL CAboutDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    if (m_bInStaticText) {
        SetCursor(m_hHandCursor);
    } else {
        SetCursor(m_hArrowCursor);
    }

    return TRUE;
//
//  Don't call the base class. We're modifying the cursor here.
//
//	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


/////////////////////////////////////////////////////////////////////////////
// Opens a hyper link to the INTEGRITYUSA.COM website
//
void CAboutDlg::OpenLink()
{
    CString sError;

    HINSTANCE hRun = ShellExecute(m_hWnd, _T("open"), "www.integrityusa.com", 0, 0, SW_SHOW);
    if ((int) hRun <= 32)
    {
        sError.Format("ShellExecute() fails with code = %li\n", hRun);
        AfxMessageBox(sError, MB_ICONERROR);
    }	

    return;
}



/////////////////////////////////////////////////////////////////////////////
// CUSBM400_GUIDlg dialog functions
//
CUSBM400_GUIDlg::CUSBM400_GUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUSBM400_GUIDlg::IDD, pParent)
    , m_sSerialNumber(_T(""))
{
	//{{AFX_DATA_INIT(CUSBM400_GUIDlg)
	m_Command_Edit = _T("V");
	m_Response_Edit = _T("");
	m_sId_Static = _T("");
	m_sStream_Static = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUSBM400_GUIDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CUSBM400_GUIDlg)
    DDX_Control(pDX, IDC_DEVICE_LIST, m_Device_List);
    DDX_Text(pDX, IDC_COMMAND_EDIT, m_Command_Edit);
    DDV_MaxChars(pDX, m_Command_Edit, 7);
    DDX_Text(pDX, IDC_RESPONSE_EDIT, m_Response_Edit);
    DDX_Text(pDX, IDC_ID_STATIC, m_sId_Static);
    DDX_Text(pDX, IDC_STREAM_STATIC, m_sStream_Static);
    //}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_SN_EDIT, m_sSerialNumber);
	DDV_MaxChars(pDX, m_sSerialNumber, 8);
}

BEGIN_MESSAGE_MAP(CUSBM400_GUIDlg, CDialog)
	//{{AFX_MSG_MAP(CUSBM400_GUIDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND_BUTTON, OnSendButton)
	ON_BN_CLICKED(IDC_IDENT_BUTTON, OnIdentButton)
	ON_BN_CLICKED(IDC_REFRESH_BUTTON, OnRefreshButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STREAM_BUTTON, OnStreamButton)
	ON_BN_CLICKED(IDC_GET_ADR_BUTTON, OnGetAddress)
	ON_BN_CLICKED(IDC_SET_ADR_BUTTON, OnSetAddress)
	ON_BN_CLICKED(IDC_SPEED_BUTTON, OnSpeedButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSBM400_GUIDlg message handlers

BOOL CUSBM400_GUIDlg::OnInitDialog()
{
    DWORD   i;
    CString sTemp;

	CDialog::OnInitDialog();

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
//	SetIcon(m_hIcon, FALSE);		// Set small icon
	
    // Identify button initial settings
    m_sId_Static = "Press ''Ident'' to identify a unit";
    m_bIdent_On = FALSE;

    // Add the USBM400 device list
    for (i=1; i<= m_USBM400_Dev.m_dwDeviceCount; i++) {
        sTemp.Format("USBM400 device #%i", i);
        m_Device_List.AddString(sTemp);
    }

    if (m_USBM400_Dev.m_dwDeviceCount >= 1) {
        m_Device_List.SetCurSel(0);
    }

    m_bStopLoop = FALSE;

	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUSBM400_GUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUSBM400_GUIDlg::OnPaint() 
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
HCURSOR CUSBM400_GUIDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CUSBM400_GUIDlg::OnCancel() 
{
    //
    // Kill the timer if it's running.
    //
    if (m_bIdent_On) {
        KillTimer(1);
    }

    m_bStopLoop = TRUE;

	CDialog::OnCancel();
}


void CUSBM400_GUIDlg::OnRefreshButton() 
{   
    CWaitCursor cWait;
    DWORD   i;
    CString sTemp;

    // Cleanup the list box. Start fresh.
    m_Device_List.ResetContent();

    m_USBM400_Dev.EnumDevices();

    // Add the USBM400 device list
    for (i=1; i<= m_USBM400_Dev.m_dwDeviceCount; i++) {
        sTemp.Format("USBM400 device #%i", i);
        m_Device_List.AddString(sTemp);
    }

    // Update the dialog with the new data
    UpdateData(FALSE);
}


void CUSBM400_GUIDlg::OnTimer(UINT nIDEvent) 
{
    int     DeviceIndex;
    char    sResp[MAX_CMD_RESP];

    if (nIDEvent == 1) {
        //
        // Send a data packet to the unit
        //
        DeviceIndex = m_Device_List.GetCurSel();
        if (DeviceIndex != LB_ERR) {
            if (!m_USBM400_Dev.SendCommand(DeviceIndex, "V", sResp)) {
                //
                // Stop the timer on error
                //
                KillTimer(1);
            }
        }
    }
	
	CDialog::OnTimer(nIDEvent);
}


void CUSBM400_GUIDlg::OnIdentButton() 
{
    CButton *pButton;
    int     DeviceIndex;

    //
    // Check if a device is selected in m_Device_List
    //
    DeviceIndex = m_Device_List.GetCurSel();
    if (DeviceIndex == LB_ERR) {
        MessageBox("A USBM400 device instance must be selected", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
        return;
    }

    pButton = (CButton *)GetDlgItem(IDC_IDENT_BUTTON);

    if (!m_bIdent_On) {
        SetTimer(1, 20, NULL);
        m_sId_Static = "Press ''Ident'' to stop";
        pButton->SetWindowText("Ident Stop");
        m_bIdent_On = TRUE;
    } else {
        KillTimer(1);
        m_sId_Static = "Press ''Ident'' to identify a unit";
        pButton->SetWindowText("Ident");
        m_bIdent_On = FALSE;
    }
    UpdateData(FALSE);
}


void CUSBM400_GUIDlg::OnSendButton() 
{
    CWaitCursor cWait;
    int     DeviceIndex;
    char    sResp[MAX_CMD_RESP];
    LPSTR   sCmd;
    CEdit* pEdit = (CEdit*) GetDlgItem(IDC_RESPONSE_EDIT);

    //
    // Check if a device is selected in m_Device_List
    //
    DeviceIndex = m_Device_List.GetCurSel();
    if (DeviceIndex == LB_ERR) {
        MessageBox("A USBM400 device instance must be selected", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
        return;
    }

    //
    // Get any updates to the command edit box and verify 
    //
    if (!UpdateData(TRUE)) {
        return;
    }

    //
    // Clear the Response string, redraw the window, and give the user some time to see it blank.
    //
    m_Response_Edit = "";
    UpdateData(FALSE);
    pEdit->RedrawWindow();
    Sleep(100);

    // Retrieve the command from the edit string. Make sure it is upper case
    m_Command_Edit.MakeUpper();
    sCmd = m_Command_Edit.GetBuffer(MAX_CMD_RESP);

    //
    // Send the command
    //
    if (!m_USBM400_Dev.SendCommand(DeviceIndex, sCmd, sResp)) {
        m_Response_Edit = "Error";
    } else {
        m_Response_Edit = sResp;
    }

    m_Command_Edit.ReleaseBuffer();

    //
    // Update the dialog with the new response string
    //
    UpdateData(FALSE);
    return;	
}

VOID DbgOut(LPSTR FormatString, ...);


void CUSBM400_GUIDlg::OnStreamButton() 
{
    CWaitCursor cWait;
    int         DeviceIndex;
    DWORD       dwThreadState, dwStreamBlocksAvailable;
    ULONG       ulBlockCount = 0;
    char        sResp[MAX_CMD_RESP];
    char        sCmd[MAX_CMD_RESP];

    USBM400_STREAM_BLOCK  sStreamData;
    HANDLE      hDataFile = INVALID_HANDLE_VALUE;
    DWORD       dwStartTime, dwEndTime, dwCurrentTime, i, dwBlocksRtn;
    DWORD       dwWriteSize, dwBytesWritten;
    BOOLEAN     bStatus;
    LPSTR       szFileData;
    CString     sTemp;
    DWORD       dwSeconds = 10;
    SYSTEMTIME  CurTime;

    CStatic* pStatic = (CStatic*) GetDlgItem(IDC_STREAM_STATIC);

    //
    // Check if a device is selected in m_Device_List
    //
    DeviceIndex = m_Device_List.GetCurSel();
    if (DeviceIndex == LB_ERR) {
        MessageBox("A USBM400 device instance must be selected", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
        return;
    }

    // Allocate the buffer to write to data file
    szFileData = (LPSTR)malloc(1024);
    if (szFileData == NULL) {
        MessageBox("Cannot create file write buffer", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
        return;
    }

    // Temporary data file to store information
    hDataFile = CreateFile("stream_data.txt",
                           GENERIC_READ | GENERIC_WRITE,           // Access mode
                           0,                       // Share mode - not shared
                           NULL,                    // Default security
                           CREATE_ALWAYS,           // Overwrite existing file
                           FILE_ATTRIBUTE_NORMAL,   // Normal attributes
                           NULL);                   // No template

    if (hDataFile == INVALID_HANDLE_VALUE) {
        MessageBox("Cannot create temp data file", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
        return;
    }

    // Date and time header in file
    GetLocalTime(&CurTime);
	sprintf(szFileData, "File created: %02d/%02d/%02d %02d:%02d.%02d\r\n\r\n", 
        CurTime.wMonth, CurTime.wDay, CurTime.wYear, CurTime.wHour, CurTime.wMinute, CurTime.wSecond);
    dwWriteSize = strlen(szFileData);
    WriteFile(hDataFile, szFileData, dwWriteSize, &dwBytesWritten, NULL);

    // Obtain the current analog sample configuration from the module
	sprintf(szFileData, "Current analog settings. 16 analog samples taken 2 times for each block\r\n");
    dwWriteSize = strlen(szFileData);
    WriteFile(hDataFile, szFileData, dwWriteSize, &dwBytesWritten, NULL);
    // Read the configuration out of EEPROM
    for (i=0; i<16; i++) {
        // Analog sample configuration EEPROM address 0x11...0x20
        sprintf(sCmd, "R%02X", i+0x11);
        if (!m_USBM400_Dev.SendCommand(DeviceIndex, sCmd, sResp)) {
            // Error
        }
        // Data is returned as "Rxx", so skip past "R"
	    sprintf(szFileData, "Sample #%02d/%02d: 0x%s\r\n", i, i+16, &sResp[1]);
        dwWriteSize = strlen(szFileData);
        WriteFile(hDataFile, szFileData, dwWriteSize, &dwBytesWritten, NULL);
    }

	sprintf(szFileData, "\r\nData follows...\r\n");
    dwWriteSize = strlen(szFileData);
    WriteFile(hDataFile, szFileData, dwWriteSize, &dwBytesWritten, NULL);

    // Start the streaming mode. Check current thread state
    if (m_USBM400_Dev.GetStreamingStatus(DeviceIndex, &dwStreamBlocksAvailable, &dwThreadState)) {

        if (dwThreadState & USB_THREAD_RUNNING) {
            // Thread already running. Error
            MessageBox("Stream mode thread already running", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
            if (hDataFile != INVALID_HANDLE_VALUE) {
                CloseHandle(hDataFile);
            }
            return;
        } else {
            // No thread running. Start thread
            m_USBM400_Dev.ControlStreaming(DeviceIndex, TRUE, 500);
        }
    } else {
        MessageBox("Error getting stream thread status", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
    }

    // Time for running = 10 seconds
    dwStartTime = GetTickCount();
    dwEndTime = dwStartTime + dwSeconds*1000;        // 10 seconds = 10,000 milliseconds
    dwCurrentTime = GetTickCount();

    while (dwEndTime > dwCurrentTime) {
        // Wait for data to arrive
        bStatus = m_USBM400_Dev.GetStreamingData(DeviceIndex,   // Current selected index
                                                 TRUE,          // Wait for data to arrive
                                                 &sStreamData,  // Data buffer for stream blocks
                                                 1,             // one block requested
                                                 &dwBlocksRtn); // Number of blocks returned
        // Error getting stream data
        if (!bStatus) {
            MessageBox("Error getting stream data", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
            break;
        } 

        ulBlockCount++;
        
        if (!(ulBlockCount % 250)) {
            // Let the user know what we're doing and where the data is going
            m_sStream_Static.Format("File:  STREAM_DATA.TXT\r\nStreaming %i seconds...", (dwEndTime - dwCurrentTime)/1000);
            UpdateData(FALSE);
            pStatic->RedrawWindow();
        }

        // Write data to disk
	    sprintf(szFileData, "\r\nTime Stamp: %i\r\n", GetTickCount());
        dwWriteSize = strlen(szFileData);
        WriteFile(hDataFile, szFileData, dwWriteSize, &dwBytesWritten, NULL);

        for (i=0; i<ADC_SAMPLE_COUNT; i++) {
	        sprintf(szFileData, "ADC #%02i      = 0x%04X\r\n", i, sStreamData.usAdcSamples[i]);
            dwWriteSize = strlen(szFileData);
            WriteFile(hDataFile, szFileData, dwWriteSize, &dwBytesWritten, NULL);
        }

        // Update current time
        dwCurrentTime = GetTickCount();
    }

    // Stop streaming mode. Check current thread state
    if (m_USBM400_Dev.GetStreamingStatus(DeviceIndex, &dwStreamBlocksAvailable, &dwThreadState)) {

        if (dwThreadState & USB_THREAD_RUNNING) {
            // Thread already running. Stop it.
            m_USBM400_Dev.ControlStreaming(DeviceIndex, FALSE, 500);
        } else {
            // No thread running. Error
            MessageBox("Stream mode thread not running", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
        }
    } else {
        MessageBox("Error getting stream thread status", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
    }

    // Close the temp data file
    if (hDataFile != INVALID_HANDLE_VALUE) {
        CloseHandle(hDataFile);
    }

    // Cleanup malloc()
    free(szFileData);

    // Indicate results to user
    sTemp.Format("Processed %i stream blocks (%i bytes) in %i milliseconds\r\n", 
        ulBlockCount, ulBlockCount*sizeof(sStreamData), (dwEndTime-dwStartTime));
    MessageBox(sTemp, "USBM400 Stream test results", MB_OK | MB_ICONINFORMATION);

    UpdateData(FALSE);
    return;
}


void CUSBM400_GUIDlg::OnGetAddress() 
{
    CWaitCursor cWait;
    int         DeviceIndex;
    char        sResp[MAX_CMD_RESP];
    char        sCmd[MAX_CMD_RESP];
    DWORD       i, j;
    char        c;

    CEdit* pEdit = (CEdit*) GetDlgItem(IDC_SN_EDIT);

    // Check if a device is selected in m_Device_List
    DeviceIndex = m_Device_List.GetCurSel();
    if (DeviceIndex == LB_ERR) {
        MessageBox("A USBM400 device instance must be selected", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
        return;
    }

    // Clear the Module SN string, redraw the window, and give the user some time to see it blank.
    m_sSerialNumber = "";
    UpdateData(FALSE);
    pEdit->SetWindowText("");
    pEdit->RedrawWindow();
    Sleep(100);

    // Get module serial number. 8 ASCII characters from EEPROM address 0x30 to 0x38
    for (i=0x30; i<0x38; i++) {
        sprintf(sCmd, "R%02X", i);
        if (!m_USBM400_Dev.SendCommand(DeviceIndex, sCmd, sResp)) {
            MessageBox("Error getting module serial number", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
            break;
        } else {
            sscanf(&sResp[1], "%02X", &j);
            // Check if manufacturing default value 0xFF (not an ASCII character!)
            if (j == 0xFF) 
                c = '?';
            else 
                c = (char)j;
            m_sSerialNumber += c;
        }
    }

    UpdateData(FALSE);
    return;
}


void CUSBM400_GUIDlg::OnSetAddress() 
{
    CWaitCursor cWait;
    int         DeviceIndex;
    char        sResp[MAX_CMD_RESP];
    char        sCmd[MAX_CMD_RESP];
    char        *sSN;
    DWORD       i;
    CEdit* pEdit = (CEdit*) GetDlgItem(IDC_SN_EDIT);

    // Check if a device is selected in m_Device_List
    DeviceIndex = m_Device_List.GetCurSel();
    if (DeviceIndex == LB_ERR) {
        MessageBox("A USBM400 device instance must be selected", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
        return;
    }

    // Update the edit box
    UpdateData(TRUE);

    // Check the length. We need 8 characters
    if (m_sSerialNumber.GetLength() != 8) {
        MessageBox("Serial Numbers must be 8 digits in length.", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
        return;
    }

    // Validate that the serial numbers are 0...9
    sSN = m_sSerialNumber.GetBuffer(m_sSerialNumber.GetLength());
    for (i=0; i<m_sSerialNumber.GetLength(); i++) {
        if ((sSN[i] < '0') || (sSN[i] > '9')) {
            MessageBox("Serial Numbers must be comprised of digits 0...9", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
            return;
        }
    }

    // Show we're sending, redraw the window, and give the user some time to see it blank.
    pEdit->SetWindowText("Setting S/N...");
    pEdit->RedrawWindow();
    Sleep(100);

    // Build commands to write EEPROM address 0x30...0x38 with module serial number
    for (i=0; i<m_sSerialNumber.GetLength(); i++) {
        sprintf(sCmd, "W%02X%02X", i+0x30, sSN[i]);
        if (!m_USBM400_Dev.SendCommand(DeviceIndex, sCmd, sResp)) {
            MessageBox("Error setting Module Address", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
        }
    }

    // Original module address is re-written in edit box
    UpdateData(FALSE);
    return;
}

void CUSBM400_GUIDlg::OnSpeedButton() 
{
#if 1
    int         DeviceIndex;
    char        sResp[MAX_CMD_RESP];
    char        sCmd[MAX_CMD_RESP];
    CWaitCursor cWait;
    DWORD       dwStartTime, dwEndTime, dwCurrentTime;
    DWORD       dwSeconds = 10;
    DWORD       dwLoopCount = 0;
    BOOLEAN     bErrorStop = FALSE;
    CStatic* pStatic = (CStatic*) GetDlgItem(IDC_STREAM_STATIC);

    //
    // Check if a device is selected in m_Device_List
    //
    DeviceIndex = m_Device_List.GetCurSel();
    if (DeviceIndex == LB_ERR) {
        MessageBox("A USBM400 device instance must be selected", "ERROR USBM400 GUI", MB_OK | MB_ICONERROR);
        return;
    }

    strcpy(sCmd, "I");

    // Time for running 
    dwStartTime = GetTickCount();
    dwEndTime = dwStartTime + dwSeconds*1000;        // 1 second = 1,000 milliseconds

    do {
        //
        // Send a data packet to the unit
        //
        if (!m_USBM400_Dev.SendCommand(DeviceIndex, sCmd, sResp)) {
            //
            // Stop the loop on error
            //
            bErrorStop = TRUE;
            break;
        } else {
            dwLoopCount++;
        }

        // Update current time
        dwCurrentTime = GetTickCount();

        if (!(dwLoopCount % 250)) {
            // Let the user know what we're doing and where the data is going
            m_sStream_Static.Format("Testing command-response speed\r\nRunning %i seconds...", (dwEndTime - dwCurrentTime)/1000);
            UpdateData(FALSE);
            pStatic->RedrawWindow();
        }

    } while (dwCurrentTime < dwEndTime);

    if (!bErrorStop) {
        m_sStream_Static.Format("%i commands in %i seconds.\r\nCommand = %s\r\n%i commands per second", dwLoopCount, dwSeconds, sCmd, dwLoopCount/dwSeconds);
    } else {
        m_sStream_Static.Format("Error encountered testing speed");
    }

    UpdateData(FALSE);

#else
    BOOL        bResult;
    MSG         sMsg;
    char        sResp[MAX_CMD_RESP];

    while (!m_bStopLoop) {
        // Grab messages every 100 loops
        if (dwCount++ > 100) {
            dwCount = 0;
            bResult = PeekMessage(&sMsg, NULL, 0, 0, PM_NOREMOVE);
            if (bResult) {
                GetMessage(&sMsg, NULL, 0, 0);
                TranslateMessage(&sMsg);
                DispatchMessage(&sMsg);
            }
        }
        //
        // Send a data packet to the unit
        //
        if (!m_USBM400_Dev.SendCommand(DeviceIndex, "U08", sResp)) {
            //
            // Stop the loop on error
            //
            break;
        } else {
            dwLoopCount++;
        }
    }
    DbgOut("Loop exit count = %i\n", dwLoopCount);
#endif

    return;	
}

