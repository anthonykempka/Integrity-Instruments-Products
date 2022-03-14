// Test_IODlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test_IO.h"
#include "Test_IODlg.h"
#include "Test_IO_CommDlg.h"

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

    CFont       font;
    HCURSOR     m_hHandCursor;
    HCURSOR     m_hArrowCursor;
    CStatic     *m_hStatic;
    BOOL        m_bInStaticText;

    void        OpenLink();

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
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
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
	DDX_Text(pDX, IDC_BUILD_DATE, m_sBuildDate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest_IODlg dialog

CTest_IODlg::CTest_IODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTest_IODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTest_IODlg)
	m_Command_Str = _T("");
	m_Response_Str = _T("");
	m_sTestEdit = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTest_IODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTest_IODlg)
	DDX_Control(pDX, IDC_232M200_CHECK, m_232M200_Check);
	DDX_Control(pDX, IDC_USB_Radio, m_USB_Check);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
	DDX_Control(pDX, IDC_RS485_Radio, m_RS485_Check);
	DDX_Control(pDX, IDC_RS232_Radio, m_RS232_Check);
	DDX_Text(pDX, IDC_COMMAND_EDIT, m_Command_Str);
	DDX_Text(pDX, IDC_RESPONSE_EDIT, m_Response_Str);
	DDX_Text(pDX, IDC_EDIT1, m_sTestEdit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTest_IODlg, CDialog)
	//{{AFX_MSG_MAP(CTest_IODlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COMM_BUTTON, OnCommButton)
	ON_BN_CLICKED(IDC_RS485_Radio, OnRS485Radio)
	ON_BN_CLICKED(IDC_RS232_Radio, OnRS232Radio)
	ON_BN_CLICKED(IDC_SPEED_TEST, OnSpeedTest)
	ON_BN_CLICKED(IDC_USB_Radio, OnUSBRadio)
	ON_BN_CLICKED(IDC_STREAM_SPEED_TEST, OnStreamSpeedTest)
	ON_BN_CLICKED(ID_TEST_DIGITAL, OnTestDigital)
	ON_BN_CLICKED(IDC_DA_TEST, OnDaTest)
	ON_BN_CLICKED(IDC_ADC_SNR, OnAdcSnr)
	ON_BN_CLICKED(IDC_232M200_CHECK, On232m200Check)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers
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
    m_hStatic = (CStatic*) GetDlgItem(IDC_STATIC_INTEGRITY_WEB);

    lf.lfHeight = 10;                       // request a 10-pixel-height font
    lf.lfUnderline = TRUE;                  // Underline and bold the font
    lf.lfWeight = FW_BOLD;
    strcpy(lf.lfFaceName, "MS Sans Serif"); 
    VERIFY(font.CreateFontIndirect(&lf));   // create the font

    //
    // Set the new font for the static text (WEB LINK)
    //
    m_hStatic ->SetFont(&font);

//    m_brush.CreateSolidBrush(RGB(200,200,200));

    //
    // Load the cursors.
    // Note: The Win98 and earlier do not have IDC_HAND cursor defined, so use our own.
    //
    m_hHandCursor = AfxGetApp()->LoadCursor(IDC_HAND_CURSOR);
    m_hArrowCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

    m_bInStaticText = FALSE;

    //
    // Set the current build date
    //
    m_sBuildDate.Format("Build Date:  %s", __DATE__);
    UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here

    // Are we painting the IDC_STATIC_INTEGRITY_WEB control?
    // Use CWnd::GetDlgCtrlID() to perform the most efficient test
    //
    if (pWnd->GetDlgCtrlID() == IDC_STATIC_INTEGRITY_WEB)
    {
        // Set the text color to BLUE
        pDC->SetTextColor(RGB(0, 0, 255));

        // Set the background mode for text to transparent so any background will show through
        pDC->SetBkMode(TRANSPARENT);

        // Return handle to our CBrush object - background color
        //hbr = m_brush;
    }
	
	// TODO: Return a different brush if the default is not desired
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

    return TRUE;

//	return CDialog::OnSetCursor(pWnd, nHitTest, message);
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

    return;
}



/////////////////////////////////////////////////////////////////////////////
// CTest_IODlg message handlers

BOOL CTest_IODlg::OnInitDialog()
{
    int Interface_Number;

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
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//Read the Communications Settings registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);

    // Default 0 (RS232) if no setting found
	Interface_Number = pApp->GetProfileInt("Settings", "Interface", 0);

    switch (Interface_Number) {
    case 2: // USB
        m_RS485_Check.SetCheck (0);
        m_RS232_Check.SetCheck (0);
        m_USB_Check.SetCheck(1);
        break;
    case 1: // RS485
        m_RS485_Check.SetCheck (1);
        m_RS232_Check.SetCheck (0);
        m_USB_Check.SetCheck(0);
        OnRS485Radio();
        break;
    case 0: // RS232
    default:
        m_RS485_Check.SetCheck (0);
        m_RS232_Check.SetCheck (1);
        m_USB_Check.SetCheck(0);
        break;
    }

    // Default 0 (Not 232M200) if no setting found
	Interface_Number = pApp->GetProfileInt("Settings", "232M200", 0);

    if (Interface_Number) {
        m_232M200_Check.SetCheck(TRUE);
    } else {
        m_232M200_Check.SetCheck(FALSE);
    }

	// TODO: Add extra initialization here

    pThis = this;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTest_IODlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTest_IODlg::OnPaint() 
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
HCURSOR CTest_IODlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTest_IODlg::OnCommButton() 
{
    CTest_IO_CommDlg dlg;


    if (dlg.DoModal() == IDOK) {


    }
    return;
}

void CTest_IODlg::OnRS232Radio() 
{
    //
    // Enable Stream Test button
    //
    CButton *hButton;
    hButton = (CButton*) GetDlgItem(IDC_STREAM_SPEED_TEST);
    hButton->EnableWindow(TRUE);

    //
    // Save in registry
    //
    CWinApp* pApp = AfxGetApp();
    ASSERT_VALID(pApp);

    pApp->WriteProfileInt("Settings", "Interface", 0);
    return;
}

void CTest_IODlg::OnRS485Radio() 
{
    //
    // Disable Stream Test button
    //
    CButton *hButton;
    hButton = (CButton*) GetDlgItem(IDC_STREAM_SPEED_TEST);
    hButton->EnableWindow(FALSE);

    //
    // Save in registry
    //
    CWinApp* pApp = AfxGetApp();
    ASSERT_VALID(pApp);

    pApp->WriteProfileInt("Settings", "Interface", 1);
    return;
}

void CTest_IODlg::OnUSBRadio() 
{
    //
    // Enable Stream Test button
    //
    CButton *hButton;
    hButton = (CButton*) GetDlgItem(IDC_STREAM_SPEED_TEST);
    hButton->EnableWindow(TRUE);

    //
    // Save in registry
    //
    CWinApp* pApp = AfxGetApp();
    ASSERT_VALID(pApp);

    pApp->WriteProfileInt("Settings", "Interface", 2);
    return;
}


void CTest_IODlg::On232m200Check() 
{
//    CButton *hButton;

    CWinApp* pApp = AfxGetApp();
    ASSERT_VALID(pApp);

//    hButton = (CButton*) GetDlgItem(IDC_STREAM_SPEED_TEST);

    if 	(m_232M200_Check.GetCheck()) {
//       hButton->EnableWindow(FALSE);
       pApp->WriteProfileInt("Settings", "232M200", 1);
    } else {
//       hButton->EnableWindow(TRUE);
       pApp->WriteProfileInt("Settings", "232M200", 0);
    }
    return;
}


void CTest_IODlg::OnAdcSnr() 
{
    CWaitCursor Cur;        // Displays the Wait cursor
    CString     sError;

	//Read the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);


    // Default 1 (COM2) if no setting found
	m_ComPort = pApp->GetProfileInt("Settings", "ComPort", 1);

    // Default 0 (115200) if not setting found
	m_BaudRate = pApp->GetProfileInt("Settings", "BaudRate", 0);

    //
    // Adjust zero based offset to COMx number
    m_ComPort += 1;
        
    //
    // Convert from zero based offset to baud rate
    switch (m_BaudRate) {
        case 0:
            m_BaudRate = 115200;
            break;
        case 1:
            m_BaudRate = 57600;
            break;
        case 2:
            m_BaudRate = 19200;
            break;
        case 3:
        default:
            m_BaudRate = 9600;
            break;
    }

    //
    // Attempt to open a Comport
    //

    hCom = ::Open_Comport (m_ComPort, m_BaudRate);

    if (hCom != INVALID_HANDLE_VALUE) {

        //
        // Perform test battery here
        //

        if (m_RS485_Check.GetCheck ()) {

            //
            // RS485 noise test
            //
            Snr_Test(TRUE);

        } else {

            //
            // RS232 noise test
            //
            Snr_Test(FALSE);
        }

        ::Close_Comport (hCom);

    } else {
        sError.Format("Cannot open comport COM%i", m_ComPort);
        AfxMessageBox(sError,  MB_ICONERROR);
    }
    return;	
}


void CTest_IODlg::Snr_Test(BOOL bRS485)
{
    CString sError;
    DWORD   BytesRead;
    DWORD   BytesExpected;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);
    int     i, Min, Max;
    int     Total, Avg;
    #define NumberSamples   500
    #define NumberSamplesFP 500.0
    int     Samples[NumberSamples];
    double  AvgNoise;
    CString Results, sResponse;

    //
    // Warn the user to supply signal to CH0
    sError.Format("Analog Noise Test\n%i total samples\n\nPlease attach CH0 to Voltage source", NumberSamples);
    if (AfxMessageBox(sError, MB_OKCANCEL | MB_ICONWARNING) != IDOK) {
        return;
    }

    //
    // Clear any data in the receive buffers first
    //
    ::Clear_Receive_Buffer(hCom);

    //
    // Setup the progress bar
    //
    m_ProgressBar.SetRange (0, NumberSamples);
    m_ProgressBar.SetPos(0);


    //
    // Set D/A output CH0 to 0x800 in case the D/A loopback connector is on
    //
    m_Command_Str = "L0800\r";          // CH0 D/A output
    sResponse = "L\r";
    Send_Command(m_Command_Str, sResponse, bRS485);


    Min = 0xFFF;
    Max = 0x000;
    Total = 0;

    //
    // Unipolar analog command for CH0
    //
    if (m_232M200_Check.GetCheck()) {
        m_Command_Str = "U2\r";
    } else {
        m_Command_Str = "U8\r";
    }

    //
    // RS-485 addressing
    //
    if (bRS485) {
        m_Command_Str = "FF00" + m_Command_Str;
        BytesExpected = 10;
    } else {
        BytesExpected = 6;
    }

    //
    // Take NumberSamples readings 
    //
    for (i=0; i<NumberSamples; i++) {

        //
        // Update the progress bar every 50 samples
        //
        if (!(i % 100)) {
            m_ProgressBar.SetPos(i);
        }

        //
        // Send the command to the module
        //
        ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));

        //
        // Wait for the bytes to return or timeout
        ::Read_Comport (hCom, &BytesRead, BytesExpected, Buffer);

        if (BytesRead == 0) {
            sError = "Timeout error. No response from module";
            AfxMessageBox(sError, MB_ICONERROR);
            return;
        }

        //
        // Null terminate for printing
        Buffer[BytesRead] = 0;
        m_Response_Str = Buffer;

        if (bRS485) {
            ::sscanf(&Buffer[6], "%X", &Samples[i]);
        } else {
            ::sscanf(&Buffer[2], "%X", &Samples[i]);
        }

        //
        // Check Min/Max
        if (Samples[i] < Min) {
            Min = Samples[i];
        }
        if (Samples[i] > Max) {
            Max = Samples[i];
        }

        //
        // Sum total
        Total += Samples[i];
    }

    //
    // Final progress bar position
    //
    m_ProgressBar.SetPos(i);

    Avg = Total / NumberSamples;
    AvgNoise = 0.0;

    for (i=0; i<NumberSamples; i++) {
        //
        // Stuff samples into edit box
        //
        sError.Format ("Sample %03i = 0x%04X\r\n", i, Samples[i]);

        m_sTestEdit += sError;
        if ((Samples[i] - Avg) < 0) {
            AvgNoise += -(Samples[i] - Avg);
        } else {
            AvgNoise += Samples[i] - Avg;
        }
    }

    AvgNoise = AvgNoise / NumberSamplesFP;
    
    Results.Format ("Number of Samples = %i\r\nAverage Sample = %03X\r\nMin Sample = %03X\r\nMax Sample = %03X\r\nAverage Noise = %f\r\n\r\n", NumberSamples , Avg, Min, Max, AvgNoise);
    AfxMessageBox(Results, MB_OK | MB_ICONINFORMATION);

    m_sTestEdit += Results;

    UpdateData (FALSE);         // Display the last response
}

void CTest_IODlg::OnSpeedTest() 
{
    CWaitCursor Cur;        // Displays the Wait cursor
    CString     sError;

	//Read the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);


    // Default 1 (COM2) if no setting found
	m_ComPort = pApp->GetProfileInt("Settings", "ComPort", 1);

    // Default 0 (115200) if not setting found
	m_BaudRate = pApp->GetProfileInt("Settings", "BaudRate", 0);

    //
    // Adjust zero based offset to COMx number
    m_ComPort += 1;
        
    //
    // Convert from zero based offset to baud rate
    switch (m_BaudRate) {
        case 0:
            m_BaudRate = 115200;
            break;
        case 1:
            m_BaudRate = 57600;
            break;
        case 2:
            m_BaudRate = 19200;
            break;
        case 3:
        default:
            m_BaudRate = 9600;
            break;
    }

    //
    // Attempt to open a Comport
    //

    hCom = ::Open_Comport (m_ComPort, m_BaudRate);

    if (hCom != INVALID_HANDLE_VALUE) {

        //
        // Perform test battery here
        //

        if (m_RS485_Check.GetCheck ()) {

            //
            // RS485 Speed Test
            //
            Polled_Speed(TRUE);

        } else {

            //
            // RS232 Speed Test
            //
            Polled_Speed(FALSE);
        }


        ::Close_Comport (hCom);

    } else {
        sError.Format("Cannot open comport COM%i", m_ComPort);
        AfxMessageBox(sError,  MB_ICONERROR);
    }

    return;	
}

void CTest_IODlg::Polled_Speed(BOOL bRS485)
{
    CString sError;
    DWORD   BytesRead, i;
    DWORD   BytesExpected;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);
    #define SpeedSamples   500
    #define fpSpeedSamples 500.0
    CString Results;

    LARGE_INTEGER   PerfFreq, PerfCountStart, PerfCountEnd, PerfCountDelta;
    double          anaTimeInSeconds, anaIOsPerSecond, digTimeInSeconds, digIOsPerSecond;

    //
    // Notify the user that the test is about to begin
    sError.Format("RS-232 Module Polled Speed Test\n%i Samples per test", SpeedSamples);
    if (AfxMessageBox(sError, MB_OKCANCEL | MB_ICONWARNING) != IDOK) {
        return;
    }

    //
    // Clear any data in the receive buffers first
    //
    ::Clear_Receive_Buffer(hCom);

    //
    // Setup the progress bar
    //
    m_ProgressBar.SetRange (0, SpeedSamples);
    m_ProgressBar.SetPos(0);

    QueryPerformanceFrequency (&PerfFreq);
    QueryPerformanceCounter(&PerfCountStart);

    //
    // Establish the command to send to the module
    //
    if (m_232M200_Check.GetCheck()) {
        m_Command_Str = "U2\r";         // CH0 in the 232M200 Module
    } else {
        m_Command_Str = "U8\r";         // CH1 in the 232M300 Module
    }

    //
    // Add address for RS-485
    //
    if (bRS485) {
        m_Command_Str = "FF00" + m_Command_Str;
        BytesExpected = 10;
    } else {
        BytesExpected = 6;
    }

    //
    // Take NumberSamples readings 
    //
    for (i=0; i<SpeedSamples; i++) {

        //
        // Update the progress bar every 250 samples
        //
        if (!(i % 100)) {
            m_ProgressBar.SetPos(i);
        }

        //
        // Send the command to the module
        //
        ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));

        //
        // Wait for the bytes to return or timeout
        ::Read_Comport (hCom, &BytesRead, BytesExpected, Buffer);

        if (BytesRead == 0) {
            sError = "Timeout error. No response from module";
            AfxMessageBox(sError, MB_ICONERROR);
            return;
        }
    }

    //
    // Calculate I/O speed
    QueryPerformanceCounter(&PerfCountEnd);
    PerfCountDelta.QuadPart = PerfCountEnd.QuadPart - PerfCountStart.QuadPart;
    anaTimeInSeconds = (double)PerfCountDelta.QuadPart / (double)PerfFreq.QuadPart;
    anaIOsPerSecond = 1.0 / anaTimeInSeconds * fpSpeedSamples;

    //
    // Set final bar position
    m_ProgressBar.SetPos(i);

    //
    // Clear any data in the receive buffers first
    //
    ::Clear_Receive_Buffer(hCom);

    //
    // Setup the progress bar again
    //
    m_ProgressBar.SetRange (0, SpeedSamples);
    m_ProgressBar.SetPos(0);

    QueryPerformanceFrequency (&PerfFreq);
    QueryPerformanceCounter(&PerfCountStart);

    //
    // Establish the command to send to the module
    m_Command_Str = "I\r";

    //
    // Add address for RS-485
    //
    if (bRS485) {
        m_Command_Str = "FF00" + m_Command_Str;
        BytesExpected = 10;
    } else {
        BytesExpected = 6;
    }

    //
    // Take NumberSamples readings 
    //
    for (i=0; i<SpeedSamples; i++) {

        //
        // Update the progress bar every 250 samples
        //
        if (!(i % 100)) {
            m_ProgressBar.SetPos(i);
        }

        //
        // Send the command to the module
        //
        ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));

        //
        // Wait for the 6 bytes to return or timeout
        ::Read_Comport (hCom, &BytesRead, BytesExpected, Buffer);

        if (BytesRead == 0) {
            sError = "Timeout error. No response from module";
            AfxMessageBox(sError, MB_ICONERROR);
            return;
        }
    }

    //
    // Calculate I/O speed
    QueryPerformanceCounter(&PerfCountEnd);
    PerfCountDelta.QuadPart = PerfCountEnd.QuadPart - PerfCountStart.QuadPart;
    digTimeInSeconds = (double)PerfCountDelta.QuadPart / (double)PerfFreq.QuadPart;
    digIOsPerSecond = 1.0 / digTimeInSeconds * fpSpeedSamples;

    //
    // Final progress bar position
    m_ProgressBar.SetPos(i);

    Results.Format ("%li Polled samples per test\r\nAnalog Sample time = %f\r\nAnalog SPS = %f\r\nDigital Sample time = %f\r\nDigital I/O PS = %f\r\n\r\n", SpeedSamples, anaTimeInSeconds, anaIOsPerSecond, digTimeInSeconds, digIOsPerSecond);
    AfxMessageBox(Results, MB_OK | MB_ICONINFORMATION);

    m_sTestEdit += Results;

    UpdateData (FALSE);         // Display the last response
}


void CTest_IODlg::OnStreamSpeedTest() 
{
    CWaitCursor Cur;        // Displays the Wait cursor
    CString     sError;

	//Read the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);


    // Default 1 (COM2) if no setting found
	m_ComPort = pApp->GetProfileInt("Settings", "ComPort", 1);

    // Default 0 (115200) if not setting found
	m_BaudRate = pApp->GetProfileInt("Settings", "BaudRate", 0);

    //
    // Adjust zero based offset to COMx number
    m_ComPort += 1;
        
    //
    // Convert from zero based offset to baud rate
    switch (m_BaudRate) {
        case 0:
            m_BaudRate = 115200;
            break;
        case 1:
            m_BaudRate = 57600;
            break;
        case 2:
            m_BaudRate = 19200;
            break;
        case 3:
        default:
            m_BaudRate = 9600;
            break;
    }

    //
    // Attempt to open a Comport
    //

    hCom = ::Open_Comport (m_ComPort, m_BaudRate);

    if (hCom != INVALID_HANDLE_VALUE) {

        //
        // Perform test battery here
        //
        if (m_RS485_Check.GetCheck ()) {

            //
            // Stream mode not applicable!
            //
            AfxMessageBox("Stream Mode not applicable for RS-485 Modules!",  MB_ICONERROR);

        } else {

            //
            // RS232 Speed Test
            //
            Stream_Speed(FALSE);
        }

        ::Close_Comport (hCom);

    } else {
        sError.Format("Cannot open comport COM%i", m_ComPort);
        AfxMessageBox(sError,  MB_ICONERROR);
    }

    return;	
	
}


void CTest_IODlg::Stream_Speed(BOOL bRS485)
{
    CString sError;
    DWORD   BytesRead, BytesToRead;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);
    #define TestTime        5
    #define fpTestTime      5.0
    CString Results;

    CString sCommand_Str, sResponse_Str;

    COMSTAT     Our_Comstat;
    DWORD       Com_Errors;

    DWORD       TestStartTime, TestEndTime, CurrentTime;
    DWORD       SecondsCount = 0;

    double      digIOsPerSecond, digTotalIOs, anaIOsPerSecond, anaTotalIOs;

    //
    // Notify the user that the test is about to begin
    sError.Format("RS-232 Module Stream Mode Speed Test\n%i Seconds Test Time", TestTime);
    if (AfxMessageBox(sError, MB_OKCANCEL | MB_ICONWARNING) != IDOK) {
        return;
    }

    //
    // Setup the progress bar
    //
    m_ProgressBar.SetRange (0, TestTime);
    m_ProgressBar.SetStep (1);
    m_ProgressBar.SetPos(0);

    //
    // Make sure Async config is 0000
    // Reset I/O module so changes take effect
    // Note: The unit may be streaming already, so we ignore the responses
    //
    m_Command_Str = "W0400\r";       // Setup Async configuration to 0000
    ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));
    Sleep(100);

    m_Command_Str = "W0500\r";       // Setup Async configuration to 0000
    ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));
    Sleep(100);

    m_ProgressBar.StepIt();
    m_Command_Str = "Z\r";          // Reset the CPU
    ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));
    Sleep(1000);                    // Sleep 3 seconds, Clear any data in the receive buffers first
    m_ProgressBar.StepIt();
    Sleep(1000);
    m_ProgressBar.StepIt();
    Sleep(1000);
    m_ProgressBar.StepIt();
    ::Clear_Receive_Buffer(hCom);

    //
    // Begin the Digital I/O Continuous Stream Mode test
    //

    //
    // 1) Setup EEPROM values
    // 2) Send the start stream command
    sCommand_Str = "W1000\r";       // Analog count = 0
    sResponse_Str = "W\r";
    Send_Command(sCommand_Str, sResponse_Str);

    sCommand_Str = "W19FF\r";       // Enable digital status
    sResponse_Str = "W\r";
    Send_Command(sCommand_Str, sResponse_Str);

    sCommand_Str = "W1A00\r";       // Disable pulse counter status
    sResponse_Str = "W\r";
    Send_Command(sCommand_Str, sResponse_Str);

    m_Command_Str = "S\r";          // Start the Stream mode
    ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));

    m_ProgressBar.SetPos(0);        // Progress bar back to start

    //
    // Loop for the time alloted
    TestStartTime = GetTickCount();
    TestEndTime = TestStartTime + (TestTime * 1000);   // Test time in seconds
    digTotalIOs = 0.0;
    CurrentTime = GetTickCount();

    while (CurrentTime < TestEndTime) {

        CurrentTime = GetTickCount();

        if (((CurrentTime - TestStartTime) / 1000) != SecondsCount) {
            SecondsCount = (CurrentTime - TestStartTime) / 1000;
            m_ProgressBar.SetPos(SecondsCount);
        }

        //
        // Wait for at least 1 byte to be returned
        //
        ClearCommError (hCom, &Com_Errors, &Our_Comstat);

        if (Our_Comstat.cbInQue >= 1) {

            if (Our_Comstat.cbInQue > BufferSize) {
                BytesToRead = BufferSize;
            } else {
                BytesToRead = Our_Comstat.cbInQue;
            }

            //
            // Read the returned data - we're only interested in the number of bytes read
            //
            ::Read_Comport (hCom, &BytesRead, BytesToRead, Buffer);
            digTotalIOs += (double)BytesRead;
        }
    }

    //
    // Send the halt stream command
    m_Command_Str = "H\r";
    ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));
    Sleep(100);

    //
    // Final progress bar position
    m_ProgressBar.SetPos(TestTime);

    //
    // Clear any data in the receive buffers first
    //
    ::Clear_Receive_Buffer(hCom);

    //
    // Calculate the I/O's per second - each response is 6 characters
    //
    digTotalIOs = digTotalIOs / 6.0;
    digIOsPerSecond = digTotalIOs / fpTestTime;


    //
    // Begin the Analog I/O Continuous Stream Mode test
    //

    //
    // 1) Setup EEPROM values
    // 2) Send the start stream command
    sCommand_Str = "W1001\r";       // Analog count = 0
    sResponse_Str = "W\r";
    Send_Command(sCommand_Str, sResponse_Str);

    sCommand_Str = "W1101\r";       // Analog Sample #1 Config nibble = 0x01
    sResponse_Str = "W\r";
    Send_Command(sCommand_Str, sResponse_Str);

    sCommand_Str = "W1900\r";       // Disable digital status
    sResponse_Str = "W\r";
    Send_Command(sCommand_Str, sResponse_Str);

    sCommand_Str = "W1A00\r";       // Disable pulse counter status
    sResponse_Str = "W\r";
    Send_Command(sCommand_Str, sResponse_Str);

    m_Command_Str = "S\r";          // Start the Stream mode
    ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));

    m_ProgressBar.SetPos(0);        // Progress bar back to start

    //
    // Loop for the time alloted
    TestStartTime = GetTickCount();
    TestEndTime = TestStartTime + (TestTime * 1000);   // Test time in seconds
    anaTotalIOs = 0.0;
    CurrentTime = GetTickCount();

    while (CurrentTime < TestEndTime) {

        CurrentTime = GetTickCount();

        if (((CurrentTime - TestStartTime) / 1000) != SecondsCount) {
            SecondsCount = (CurrentTime - TestStartTime) / 1000;
            m_ProgressBar.SetPos(SecondsCount);
        }

        //
        // Wait for at least 1 byte to be returned
        //
        ClearCommError (hCom, &Com_Errors, &Our_Comstat);

        if (Our_Comstat.cbInQue >= 1) {

            if (Our_Comstat.cbInQue > BufferSize) {
                BytesToRead = BufferSize;
            } else {
                BytesToRead = Our_Comstat.cbInQue;
            }

            //
            // Read the returned data - we're only interested in the number of bytes read
            //
            ::Read_Comport (hCom, &BytesRead, BytesToRead, Buffer);
            anaTotalIOs += (double)BytesRead;
        }
    }

    //
    // Send the halt stream command
    m_Command_Str = "H\r";
    ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));
    Sleep(100);

    //
    // Final progress bar position
    m_ProgressBar.SetPos(TestTime);

    //
    // Clear any data in the receive buffers first
    //
    ::Clear_Receive_Buffer(hCom);

    //
    // Calculate the I/O's per second - each response is 6 characters
    //
    anaTotalIOs = anaTotalIOs / 6.0;
    anaIOsPerSecond = anaTotalIOs / fpTestTime;



    Results.Format ("%li Seconds stream test time\r\nTotal Digital I/O's = %f\r\nDigital I/O PS = %f\r\nTotal Analog Samples = %f\r\nAnalog SPS = %f\r\n\r\n", 
        TestTime, digTotalIOs, digIOsPerSecond, anaTotalIOs, anaIOsPerSecond);
    AfxMessageBox(Results, MB_OK | MB_ICONINFORMATION);

    m_sTestEdit += Results;

    UpdateData (FALSE);         // Display the last response
}


BOOL CTest_IODlg::Send_Command(CString sCommand, CString sResponse, BOOL bRS485 /*= FALSE*/)
{
    DWORD       dwExpectedReturn;
    CString     sError;

    DWORD       BytesRead;
    char        Buffer[100];
    DWORD       BufferSize = sizeof (Buffer);

    ZeroMemory(Buffer, BufferSize);

    //
    // Is this an RS485 command?
    if (bRS485) {
        sCommand = "FF00" + sCommand;
        sResponse = "XXXX" + sResponse;
    }

    dwExpectedReturn = sResponse.GetLength();

    //
    // Send the command
    ::Write_Comport (hCom, sCommand.GetLength(), sCommand.GetBuffer(1));

    //
    // Wait for the the bytes to return or timeout
    ::Read_Comport (hCom, &BytesRead, dwExpectedReturn, Buffer);

    if (BytesRead != dwExpectedReturn) {
        sError.Format("Timeout error in Send_Command(). No response from module on command %s", sCommand);
        AfxMessageBox(sError, MB_ICONERROR);
        return (FALSE);
    }

    //
    // Pad response addressing with XXXX since we don't know what the module response address is
    //
    if (bRS485) {
        Buffer[0] = 'X';
        Buffer[1] = 'X';
        Buffer[2] = 'X';
        Buffer[3] = 'X';
    }

    if (sResponse.Compare(Buffer) == 0) {
        //
        // Strings are identical
        return (TRUE);
    } else {
        //
        // Strings mismatch
        return (FALSE);
    }

    return (TRUE);
}



CTest_IODlg * CTest_IODlg::pThis = NULL;  

void CALLBACK EXPORT CTest_IODlg::TimerProc(
  HWND hwnd,         // handle to window
  UINT uMsg,         // WM_TIMER message
  UINT_PTR idEvent,  // timer identifier
  DWORD dwTime       // current system time
)
{

    OutputDebugString("Hello world!\n");
    if (NULL != (CTest_IODlg *) pThis) {
        pThis->m_ProgressBar.StepIt();
    }

    return;
}

void CTest_IODlg::OnTestDigital() 
{
    CWaitCursor Cur;        // Displays the Wait cursor
    CString     sError;

	//Read the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);


    // Default 1 (COM2) if no setting found
	m_ComPort = pApp->GetProfileInt("Settings", "ComPort", 1);

    // Default 0 (115200) if not setting found
	m_BaudRate = pApp->GetProfileInt("Settings", "BaudRate", 0);

    //
    // Adjust zero based offset to COMx number
    m_ComPort += 1;
        
    //
    // Convert from zero based offset to baud rate
    switch (m_BaudRate) {
        case 0:
            m_BaudRate = 115200;
            break;
        case 1:
            m_BaudRate = 57600;
            break;
        case 2:
            m_BaudRate = 19200;
            break;
        case 3:
        default:
            m_BaudRate = 9600;
            break;
    }

    //
    // Attempt to open a Comport
    //

    hCom = ::Open_Comport (m_ComPort, m_BaudRate);

    if (hCom != INVALID_HANDLE_VALUE) {

        //
        // Perform test battery here
        //

        if (m_RS485_Check.GetCheck ()) {
            //
            // RS485 Digital I/O Test and Configure
            //
            Test_Digital(TRUE);
        } else {

            //
            // RS232 Digital I/O Test and Configure
            //
            Test_Digital(FALSE);
        }

        ::Close_Comport (hCom);

    } else {
        sError.Format("Cannot open comport COM%i", m_ComPort);
        AfxMessageBox(sError,  MB_ICONERROR);
    }
    return;	
}


void CTest_IODlg::Test_Digital(BOOL bRS485)
{
    CString sResponse, sFailText;
    DWORD   i;
    BOOL    bFail = FALSE;
    DWORD   LoopCount = 29;

    //
    // Setup the progress bar
    //
    m_ProgressBar.SetRange (0, (SHORT)LoopCount);
    m_ProgressBar.SetPos(0);

    //
    // Clear any junk out of the receive buffers
    //
    ::Clear_Receive_Buffer(hCom);

    //
    // Loop for each test
    //
    for (i=0; i<=LoopCount; i++) {

        m_ProgressBar.SetPos(i);

        switch (i) {
        case 0: 
            m_Command_Str = "V\r";                  // Get firmware version
            sResponse = "V30\r";
			sFailText = "Firmware version not 3.0";
            break;
        case 1:
            m_Command_Str = "W0001\r";              // Set module address 0x01
            sResponse = "W\r";
            sFailText = "EEPROM location 00 not written to 01";
            break;
        case 2:
            m_Command_Str = "W0100\r";
            sResponse = "W\r";                      // Async response address to 0x00
			sFailText = "EEPROM location 01 not written to 00";
            break;
        case 3:
            m_Command_Str = "W02FF\r";              //  Port 1 direction = all inputs
            sResponse = "W\r";
			sFailText = "EEPROM location 02 not written to FF";
            break;
        case 4:
            m_Command_Str = "W03FF\r";              // Port 2 direction = all inputs
            sResponse = "W\r";
			sFailText = "EEPROM location 03 not written to FF";
            break;
        case 5:
            m_Command_Str = "W0400\r";              // EEPROM 0x04/0x05 is Async Config = 0x0000
            sResponse = "W\r";
			sFailText = "EEPROM location 04 not written to 00";
            break;
        case 6:
            m_Command_Str = "W0500\r";
            sResponse = "W\r";
			sFailText = "EEPROM location 05 not written to 00";
            break;
        case 7:                                     
            m_Command_Str = "W0600\r";              // Port 1 power on default
            sResponse = "W\r";
			sFailText = "EEPROM location 06 not written to 00";
            break;
        case 8:                                     
            m_Command_Str = "W0700\r";              // Port 2 power on default
            sResponse = "W\r";
			sFailText = "EEPROM location 07 not written to 00";
            break;
        case 9:                                     
            m_Command_Str = "W0800\r";              // Expander flag - No expander
            sResponse = "W\r";
			sFailText = "EEPROM location 08 not written to 00";
            break;
        case 10:                                     
            m_Command_Str = "W0900\r";              // D/A CH0 power on default = 0x000
            sResponse = "W\r";
			sFailText = "EEPROM location 09 not written to 00";
            break;
        case 11:                                     
            m_Command_Str = "W0A00\r";
            sResponse = "W\r";
			sFailText = "EEPROM location 0A not written to 00";
            break;
        case 12:                                     
            m_Command_Str = "W0B00\r";              // D/A CH1 power on default = 0x000
            sResponse = "W\r";
			sFailText = "EEPROM location 0B not written to 00";
            break;
        case 13:                                     
            m_Command_Str = "W0C00\r";
            sResponse = "W\r";
			sFailText = "EEPROM location 0C not written to 00";
            break;
        case 14:                                     
            m_Command_Str = "W0D00\r";              // Normal A/D clock speed
            sResponse = "W\r";
			sFailText = "EEPROM location 0D not written to 00";
            break;
        case 15:                                     
            m_Command_Str = "W1000\r";              // Async Analog count = 0
            sResponse = "W\r";
			sFailText = "EEPROM location 10 not written to 00";
            break;
        case 16:                                     
            m_Command_Str = "W1900\r";              // Async digital input = disabled
            sResponse = "W\r";
			sFailText = "EEPROM location 19 not written to 00";
            break;
        case 17:                                     
            m_Command_Str = "W1A00\r";              // Async pusle count = disabled
            sResponse = "W\r";
			sFailText = "EEPROM location 1A not written to 00";
            break;
        case 18:                                     
            m_Command_Str = "Z\r";                  // Reset the I/O Module
            sResponse = "Z\r";
			sFailText = "Reset not performed";
            break;
        case 19:
            m_sTestEdit += "EEPROM configured and module Reset\r\n";
            if (!m_232M200_Check.GetCheck()) {
                m_Command_Str = "T00FF\r";          // 232M300, 485M300
            } else {
                m_Command_Str = "T00F0\r";          // 232M200
            }
            sResponse = "T\r";
			sFailText = "Command T00FF failed";
            break;
        case 20:
            if (!m_232M200_Check.GetCheck()) {
                m_Command_Str = "OAA00\r";          // 232M300, 485M300
            } else {
                m_Command_Str = "O000A\r";          // 232M200
            }
            sResponse = "O\r";
			sFailText = "Command OAA00 failed";
            break;
        case 21:
            if (!m_232M200_Check.GetCheck()) {
                m_Command_Str = "I\r";              // 232M300, 485M300
                sResponse = "IAAAA\r";
            } else {
                m_Command_Str = "I\r";              // 232M200
                sResponse = "I00AA\r";
            }
			sFailText = "Command I not equal to IAAAA -- Digital Loopback fail";
            break;
        case 22:
            if (!m_232M200_Check.GetCheck()) {
                m_Command_Str = "O5500\r";          // 232M300, 485M300
                sResponse = "O\r";
            } else {
                m_Command_Str = "O0005\r";          // 232M200
                sResponse = "O\r";
            }
			sFailText = "Command O5500 failed";
            break;
        case 23:
            if (!m_232M200_Check.GetCheck()) {
                m_Command_Str = "I\r";              // 232M300, 485M300
                sResponse = "I5555\r";
            } else {
                m_Command_Str = "I\r";              // 232M200
                sResponse = "I0055\r";
            }
			sFailText = "Command I not equal to I5555 -- Loopback fail";
            break;
        case 24:
            m_Command_Str = "O0000\r";              // outputs all off
            sResponse = "O\r";
			sFailText = "Command O0000 failed";
            break;
        case 25:
            m_sTestEdit += "Digital loopback passed\r\n";
            m_Command_Str = "TFFFF\r";              // Digital I/O direction all inputs
            sResponse = "T\r";
			sFailText = "Command TFFFF failed";
            break;
        case 26:
            m_Command_Str = "M\r";                  // Clear pulse counter
            sResponse = "M\r";
			sFailText = "Command M failed";
            break;
        case 27:
            m_Command_Str = "PFE080\r";             // Turn on PWM
            sResponse = "P\r";
			sFailText = "Command PFE004 failed";
            break;
        case 28:
            m_Command_Str = "P00000\r";             // Turn off PWM
            sResponse = "P\r";
			sFailText = "Command P00000 failed";
            break;
        case 29:
            m_Command_Str = "N\r";                  // Get the pulse count input
            sResponse = "N00000000\r";
            sFailText = "PWM - Pulse Count loopback failed";
            break;
        default:
            sFailText = "Invalid test number";
            bFail = TRUE;
            break;
        }

        // 
        // Invert response expectation for Pulse Count
        //
        if (m_Command_Str == "N\r") {
            //
            // Send command and check response - WE DON'T WANT ZERO COUNT = N00000000
            if (Send_Command(m_Command_Str, sResponse, m_RS485_Check.GetCheck())) {
                bFail = TRUE;
                break;
            }
            m_sTestEdit += "PWM and Counter loopback passed\r\n";
        } else {
            //
            // Send command and check response
            if (!Send_Command(m_Command_Str, sResponse, m_RS485_Check.GetCheck())) {
                bFail = TRUE;
                break;
            }
        }

        //
        // Delay a few seconds on reset and clear the power up message
        //
        if (m_Command_Str == "Z\r") {
            Sleep (3000);
            ::Clear_Receive_Buffer(hCom);
        }   

        //
        // Delay after turned on PWM
        //
//        if (m_Command_Str == "PFE080\r") {
//            Sleep (500);
//        }
    } // for (i=0....)

    //
    // Final progress bar position
    m_ProgressBar.SetPos(LoopCount);

    if (bFail) {
        MessageBox (sFailText, "Test status", MB_ICONERROR);
    } else {
        MessageBox ("Digital Test and Module Configure OK", "Test status", MB_ICONINFORMATION);
    }

    m_sTestEdit += "\r\n";

    UpdateData(FALSE);
    return;
}


void CTest_IODlg::OnDaTest() 
{
    CWaitCursor Cur;        // Displays the Wait cursor
    CString     sError;

	//Read the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);


    // Default 1 (COM2) if no setting found
	m_ComPort = pApp->GetProfileInt("Settings", "ComPort", 1);

    // Default 0 (115200) if not setting found
	m_BaudRate = pApp->GetProfileInt("Settings", "BaudRate", 0);

    //
    // Adjust zero based offset to COMx number
    m_ComPort += 1;
        
    //
    // Convert from zero based offset to baud rate
    switch (m_BaudRate) {
        case 0:
            m_BaudRate = 115200;
            break;
        case 1:
            m_BaudRate = 57600;
            break;
        case 2:
            m_BaudRate = 19200;
            break;
        case 3:
        default:
            m_BaudRate = 9600;
            break;
    }

    //
    // Attempt to open a Comport
    //

    hCom = ::Open_Comport (m_ComPort, m_BaudRate);

    if (hCom != INVALID_HANDLE_VALUE) {

        //
        // Perform test battery here
        //

        if (m_RS485_Check.GetCheck ()) {

            //
            // RS485 D/A Test
            //
            DA_Test(TRUE);

        } else {

            //
            // RS232 D/A Test
            //
            DA_Test(FALSE);
        }

        ::Close_Comport (hCom);

    } else {
        sError.Format("Cannot open comport COM%i", m_ComPort);
        AfxMessageBox(sError,  MB_ICONERROR);
    }
    return;	
}


void CTest_IODlg::DA_Test(BOOL bRS485) 
{
    DWORD   BytesExpected;
    CString sError;
    DWORD   BytesRead;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);

    CString sResponse;
    CString Results;

    DWORD   LoopCount = 39;         // Loop Count = 12 bit max 4096 (multiply * 100);
    DWORD   i;

    DWORD   DA_Val, AD_Val, DA_Val2, AD_Val2;

    BOOL    bFail = FALSE;

    //
    // Warn the user to supply signal to CH0
    sError.Format("D-to-A Loopback Test\nConnect A-D to D-A\n%i loops to execute\n\n", LoopCount);
    if (AfxMessageBox(sError, MB_OKCANCEL | MB_ICONWARNING) != IDOK) {
        return;
    }

    //
    // Clear any data in the receive buffers first
    //
    ::Clear_Receive_Buffer(hCom);

    //
    // Setup the progress bar
    //
    m_ProgressBar.SetRange (0, (SHORT)LoopCount);
    m_ProgressBar.SetPos(0);


    //
    // Take NumberSamples readings 
    //
    for (i=0; i<LoopCount; i++) {

        //
        // Update the progress bar
        //
        m_ProgressBar.SetPos(i);

        //
        // Establish the D/A output value
        //
        DA_Val = i * 100;
        DA_Val2 = (i+1) * 100;

        m_Command_Str.Format("L0%03X\r", DA_Val);       // CH0 D/A output
        sResponse = "L\r";

        if (!Send_Command(m_Command_Str, sResponse, bRS485)) {
            return;
        }

        m_Command_Str.Format("L1%03X\r", DA_Val2);      // CH1 D/A output
        sResponse = "L\r";

        if (!Send_Command(m_Command_Str, sResponse, bRS485)) {
            return;
        }

        //
        // Establish the A/D command to send to the module - CH0
        //
        if (m_232M200_Check.GetCheck()) {
            m_Command_Str = "U2\r";                     // CH0 in the 232M200 Module
        } else {
            m_Command_Str = "U8\r";                     // CH0 in the 232M300 Module
        }

        //
        // RS-485 addressing
        //
        if (bRS485) {
            m_Command_Str = "FF00" + m_Command_Str;
            BytesExpected = 10;
        } else {
            BytesExpected = 6;
        }

        //
        // Send the command to the module
        //
        ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));

        //
        // Wait for the bytes to return or timeout
        ::Read_Comport (hCom, &BytesRead, BytesExpected, Buffer);

        if (BytesRead == 0) {
            sError = "Timeout error. No response from module";
            AfxMessageBox(sError, MB_ICONERROR);
            bFail = TRUE;
            break;
        }

        //
        // Null terminate for printing
        Buffer[BytesRead] = 0;
        m_Response_Str = Buffer;

        if (bRS485) {
            ::sscanf(&Buffer[6], "%X", &AD_Val);
        } else {
            ::sscanf(&Buffer[2], "%X", &AD_Val);
        }

        //
        // Establish the A/D command to send to the module - CH1
        //
        if (m_232M200_Check.GetCheck()) {
            m_Command_Str = "U3\r";                     // CH1 in the 232M200 Module
        } else {
            m_Command_Str = "UC\r";                     // CH1 in the 232M300 Module
        }

        //
        // RS-485 addressing
        //
        if (bRS485) {
            m_Command_Str = "FF00" + m_Command_Str;
            BytesExpected = 10;
        } else {
            BytesExpected = 6;
        }

        //
        // Send the command to the module
        //
        ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));

        //
        // Wait for the bytes to return or timeout
        ::Read_Comport (hCom, &BytesRead, BytesExpected, Buffer);

        if (BytesRead == 0) {
            sError = "Timeout error. No response from module";
            AfxMessageBox(sError, MB_ICONERROR);
            bFail = TRUE;
            break;
        }

        //
        // Null terminate for printing
        Buffer[BytesRead] = 0;
        m_Response_Str = Buffer;

        if (bRS485) {
            ::sscanf(&Buffer[6], "%X", &AD_Val2);
        } else {
            ::sscanf(&Buffer[2], "%X", &AD_Val2);
        }

        Results.Format ("Output=%03X Input=%03X  Output2=%03X Input2=%03X\r\n", DA_Val, AD_Val, DA_Val2, AD_Val2);
        m_sTestEdit += Results;

        if ( (abs(AD_Val - DA_Val) > 30) ||
             (abs(AD_Val2 - DA_Val2) > 30) ) {
            sError = "DA Loopback value out of range";
            AfxMessageBox(sError, MB_ICONERROR);
            bFail = TRUE;
            break;
        }

    }

    //
    // Final progress bar position
    //
    m_ProgressBar.SetPos(LoopCount);

    m_sTestEdit += "\r\n";

    if (!bFail) {
        MessageBox ("A-to-D Test OK", "Test status", MB_ICONINFORMATION);
    }
   
    UpdateData (FALSE);         // Display the last response
}


