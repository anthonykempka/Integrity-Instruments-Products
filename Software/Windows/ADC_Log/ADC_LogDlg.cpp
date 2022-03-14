// ADC_LogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ADC_Log.h"
#include "ADC_LogDlg.h"
#include "ComSetup.h"
#include "Comm32.h"
#include "Configuration.h"

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
// CADC_LogDlg dialog

CADC_LogDlg::CADC_LogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CADC_LogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CADC_LogDlg)
	m_FileName = _T("");
	m_LastCommand = _T("");
	m_LastResponse = _T("");
	m_SampleCount = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CADC_LogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADC_LogDlg)
	DDX_Control(pDX, IDC_BROWSE, m_Browse);
	DDX_Control(pDX, IDC_CONFIG, m_Config);
	DDX_Control(pDX, IDC_COMM_SETUP, m_CommSetup);
	DDX_Control(pDX, IDC_HALT, m_Halt);
	DDX_Control(pDX, IDC_BEGIN, m_Begin);
	DDX_Text(pDX, IDC_FILENAME, m_FileName);
	DDX_Text(pDX, IDC_LAST_COMMAND, m_LastCommand);
	DDX_Text(pDX, IDC_LAST_RESPONSE, m_LastResponse);
	DDX_Text(pDX, IDC_SAMPLE_COUNT, m_SampleCount);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CADC_LogDlg, CDialog)
	//{{AFX_MSG_MAP(CADC_LogDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_BEGIN, OnBegin)
	ON_BN_CLICKED(IDC_COMM_SETUP, OnCommSetup)
	ON_BN_CLICKED(IDC_CONFIG, OnConfig)
	ON_BN_CLICKED(IDC_HALT, OnHalt)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADC_LogDlg message handlers

BOOL CADC_LogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Read the Communications Settings registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);

    // Default NULL if no setting found
	m_FileName = pApp->GetProfileString("Settings", "Filename");


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
    bAskCancel = FALSE;
    m_SampleCount = 0;
	
	// TODO: Add extra initialization here

    SetWindowText ("ADC_Log - Halted");

    UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CADC_LogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CADC_LogDlg::OnPaint() 
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
HCURSOR CADC_LogDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CADC_LogDlg::OnCancel() 
{
    int MsgRtn;

    if (bTimerRunning) {        // Only kill timer if one was made

        bAskCancel = TRUE;
        MsgRtn = AfxMessageBox ("File logging active.\n\nAre you sure you want to exit?", MB_OKCANCEL);
        bAskCancel = FALSE;
        if (MsgRtn == IDCANCEL) {
            return;
        }

        bTimerRunning = FALSE;
        if (bRS485_Check) {
            KillTimer(1);
        } else {
            KillTimer(2);
        }
        ::Close_Comport (hCom);
        ::CloseHandle(hLogFile);
    }
	
	CDialog::OnCancel();
}

void CADC_LogDlg::OnBrowse() 
{
    static char BASED_CODE szFilter[] = "Comma Quote Files (*.csv)|*.csv|All Files (*.*)|*.*||";

    //Update the Dialog objects from member vars
    UpdateData(TRUE);
    
    CFileDialog cDlg(TRUE, "CSV", m_FileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

    if(IDOK == cDlg.DoModal())
    {
        m_FileName = cDlg.GetPathName();

        //Update Member vars from dialog objects
        UpdateData(FALSE);

        //
        // Save off filename in registry

    	CWinApp* pApp = AfxGetApp();
	    ASSERT_VALID(pApp);

        pApp->WriteProfileString("Settings", "Filename", m_FileName);
    }	
}

void CADC_LogDlg::OnBegin() 
{
    int     BaudRate, ComPort;
    int     MsgRtn, i, j;
    DWORD   BytesWritten;
    CTime   t;
    CString s;
 
    if (m_FileName == "") {
        MessageBox ("Log File Name must be entered!", "Error", MB_ICONERROR);
        m_Browse.SetFocus();
        return;
    }

    hLogFile = :: CreateFile (m_FileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
                              NULL, OPEN_EXISTING, 0, NULL);

    if (hLogFile != INVALID_HANDLE_VALUE) {

        MsgRtn = AfxMessageBox ("YES to append to existing Log file.\n\nNO to over-write existing Log File\n\nCANCEL to exit", MB_YESNOCANCEL);
        if (MsgRtn == IDCANCEL) {
            ::CloseHandle (hLogFile);
            return;
        }

        if (MsgRtn == IDNO) {

            //
            // This resets the EOF to the begining
            ::SetEndOfFile(hLogFile);

            //
            // Display the info message
            t = CTime::GetCurrentTime();
            s = t.Format( "%B %d %Y  %H:%M" );
            FileStr = "ADC-x Log file created by ADC_Log.exe  ";
            FileStr += s + "\r\n";
            ::WriteFile(hLogFile, FileStr.GetBuffer(1), FileStr.GetLength(), &BytesWritten, NULL);
        }

    } else {
        //
        // Create a new file
        hLogFile = :: CreateFile (m_FileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
                                  NULL, CREATE_NEW, 0, NULL);

        if (hLogFile == INVALID_HANDLE_VALUE) {
            MessageBox ("Cannot create Log file!", "Error", MB_ICONERROR);
            return;
        }

        //
        // Display the info message
        t = CTime::GetCurrentTime();
        s = t.Format( "%B %d %Y  %H:%M" );
        FileStr = "ADC-x Log file created by ADC_Log.exe  ";
        FileStr += s + "\r\n";        

        ::WriteFile(hLogFile, FileStr.GetBuffer(1), FileStr.GetLength(), &BytesWritten, NULL);
    }

    //
    // Bump to the end of the file
    ::SetFilePointer(hLogFile, 0, NULL, FILE_END);

    //
    // Sanity check for timer already running

    if (bTimerRunning) {        // Don't do anything if it's already running.
        AfxMessageBox("Sampling already started Bozo!");
        return;
    }


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
    
    // Default 1 (RS485) if no setting found
	bRS485_Check = pApp->GetProfileInt("Settings", "RS485", 1);

    //
    // Convert from zero based offset to baud rate
    switch (BaudRate) {
        case 0:
            BaudRate = 115200;
            m_RcvSleepTime = 10;
            break;
        case 1:
            BaudRate = 57600;
            m_RcvSleepTime = 10;
            break;
        case 2:
            BaudRate = 19200;
            m_RcvSleepTime = 15;
            break;
        case 3:
        default:
            BaudRate = 9600;
            m_RcvSleepTime = 20;
            break;
    }

    ModuleAddress = pApp->GetProfileInt("Settings", "ModuleAddress", 1);

    //
    // Read the Sample configuration from registry

	Config_Sample1 = pApp->GetProfileInt("Settings", "Sample1", 0);
    Config_Sample2 = pApp->GetProfileInt("Settings", "Sample2", 0);
    Config_Sample3 = pApp->GetProfileInt("Settings", "Sample3", 0);
    Config_Sample4 = pApp->GetProfileInt("Settings", "Sample4", 0);
    Config_Sample5 = pApp->GetProfileInt("Settings", "Sample5", 0);
    Config_Sample6 = pApp->GetProfileInt("Settings", "Sample6", 0);
    Config_Sample7 = pApp->GetProfileInt("Settings", "Sample7", 0);
    Config_Sample8 = pApp->GetProfileInt("Settings", "Sample8", 0);


    Unipolar_Sample1 = pApp->GetProfileInt("Settings", "Unipolar1", 0);
    Unipolar_Sample2 = pApp->GetProfileInt("Settings", "Unipolar2", 0);
    Unipolar_Sample3 = pApp->GetProfileInt("Settings", "Unipolar3", 0);
    Unipolar_Sample4 = pApp->GetProfileInt("Settings", "Unipolar4", 0);
    Unipolar_Sample5 = pApp->GetProfileInt("Settings", "Unipolar5", 0);
    Unipolar_Sample6 = pApp->GetProfileInt("Settings", "Unipolar6", 0);
    Unipolar_Sample7 = pApp->GetProfileInt("Settings", "Unipolar7", 0);
    Unipolar_Sample8 = pApp->GetProfileInt("Settings", "Unipolar8", 0);

    //
    // First get the 0 based index of the Voltage Reference, the convert to number

    int VRefSel;

    VRefSel = pApp->GetProfileInt("Settings", "Vref", 3);

    switch (VRefSel) {
    case 0:
        Vref = 1.200;
        break;
    case 1:
        Vref = 2.500;
        break;
    case 2:
        Vref = 4.096;
        break;
    case 3:
    default:
        Vref = 5.000;
        break;
    }

    //
    // Log_Interval is in .1 second increments = must be converted to milliseconds.
    Log_Interval = pApp->GetProfileInt("Settings", "Interval", 100);
    Log_Interval *= 100;

    // Filter: default = 0
    FilterCount = pApp->GetProfileInt("Settings", "Filter", 0);

    //
    // Filter array initialization
    for (i=0;i<8; i++) {
        FilterArrayPosition[i] = 0;
        for (j=0;j<100; j++) {
            FilterArray[i][j] = 0.0;
        }
    }

    //
    // Attempt to open a Comport
    //

    hCom = ::Open_Comport (ComPort, BaudRate);

    if (hCom != INVALID_HANDLE_VALUE) {

        //
        // Setup the window timer
        //

        if (bRS485_Check) {
            //
            // Obtain offset calibration
            if (RS485_Get_Offset()) {
                //
                // Initial sample
                RS485_Sample();
                SetTimer (1, Log_Interval, NULL);       // RS485 Test
                bTimerRunning = TRUE;
            }
        } else {
            //
            // Obtain offset calibration
            if (RS232_Get_Offset()) {
                //
                // Initial sample
                RS232_Sample();
                SetTimer (2, Log_Interval, NULL);       // RS232 Test
                bTimerRunning = TRUE;
            }
        }
    } else {
        MessageBox ("Cannot open Comport!", "Error", MB_ICONERROR);
        bTimerRunning = FALSE;
    }

    if (bTimerRunning) {
        //
        // Enable/disable buttons
        m_Halt.EnableWindow(TRUE);
        m_Halt.SetFocus();
        m_Begin.EnableWindow(FALSE);
        m_CommSetup.EnableWindow(FALSE);
        m_Config.EnableWindow(FALSE);

        SetWindowText ("ADC_Log - Logging");
    } else {
        ::CloseHandle (hLogFile);
        if (hCom != INVALID_HANDLE_VALUE) {
            ::Close_Comport (hCom);
        }
    }       
}

void CADC_LogDlg::OnCommSetup() 
{
    CComSetup dlg;

    dlg.DoModal();
}

void CADC_LogDlg::OnConfig() 
{
    CConfiguration dlg;

    dlg.DoModal();
}

void CADC_LogDlg::OnHalt() 
{
	// TODO: Add your control notification handler code here

    m_Halt.EnableWindow(FALSE);
    m_Begin.EnableWindow(TRUE);
    m_Begin.SetFocus();
    m_CommSetup.EnableWindow(TRUE);
    m_Config.EnableWindow(TRUE);

    SetWindowText ("ADC_Log - Halted");

    if (bTimerRunning) {        // Only kill timer if one was made
        bTimerRunning = FALSE;
        if (bRS485_Check) {
            KillTimer(1);
        } else {
            KillTimer(2);
        }
        ::Close_Comport (hCom);
        ::CloseHandle (hLogFile);
    }
}


void CADC_LogDlg::RS232_Sample()
{
    DWORD   BytesRead, BytesWritten;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);
    int     i, j;
    int     Config_Sample, Unipolar_Sample;
    unsigned int  Sample_Count;
    double  Fp_Sample;
    BOOL    First = TRUE;

    //
    // Start by clearing out the File write string

    FileStr = "";

    //
    // Check each of the possible 8 samples

    for (i=0; i<8; i++) {

        switch (i) {
            case 0:
                Config_Sample = Config_Sample1;
                Unipolar_Sample = Unipolar_Sample1;
                break;
            case 1:
                Config_Sample = Config_Sample2;
                Unipolar_Sample = Unipolar_Sample2;
                break;
            case 2:
                Config_Sample = Config_Sample3;
                Unipolar_Sample = Unipolar_Sample3;
                break;
            case 3:
                Config_Sample = Config_Sample4;
                Unipolar_Sample = Unipolar_Sample4;
                break;
            case 4:
                Config_Sample = Config_Sample5;
                Unipolar_Sample = Unipolar_Sample5;
                break;
            case 5:
                Config_Sample = Config_Sample6;
                Unipolar_Sample = Unipolar_Sample6;
                break;
            case 6:
                Config_Sample = Config_Sample7;
                Unipolar_Sample = Unipolar_Sample7;
                break;
            case 7:
                Config_Sample = Config_Sample8;
                Unipolar_Sample = Unipolar_Sample8;
                break;
        }

        switch (Config_Sample) {
            case 0:
                break;
            case 1:
                Config_Sample = 0x8;    // CH0
                break;
            case 2:
                Config_Sample = 0xC;    // CH1
                break;
            case 3:
                Config_Sample = 0x9;    // CH2
                break;
            case 4:
                Config_Sample = 0xD;    // CH3
                break;
            case 5:
                Config_Sample = 0xA;    // CH4
                break;
            case 6:
                Config_Sample = 0xE;    // CH5
                break;
            case 7:
                Config_Sample = 0xB;    // CH6
                break;
            case 8:
                Config_Sample = 0xF;    // CH7
                break;
            case 9:
                Config_Sample = 0x0;    // +CH0  -CH1
                break;
            case 10:
                Config_Sample = 0x4;    // -CH0  +CH1
                break;
            case 11:
                Config_Sample = 0x1;    // +CH2  -CH3
                break;
            case 12:
                Config_Sample = 0x5;    // -CH2  +CH3
                break;
            case 13:
                Config_Sample = 0x2;    // +CH4  -CH5
                break;
            case 14:
                Config_Sample = 0x6;    // -CH4  +CH5
                break;
            case 15:
                Config_Sample = 0x3;    // +CH6  -CH7
                break;
            case 16:
                Config_Sample = 0x7;    // -CH6  +CH7
                break;
        }

        //
        // Do we need to sample?

        if (Config_Sample) {

            //
            // Construct the command string
            
            if (Unipolar_Sample) {
                Command_Str.Format("U%1X\r", Config_Sample);
            } else {
                Command_Str.Format("Q%1X\r", Config_Sample);
            }

            ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
            Sleep (m_RcvSleepTime);
            ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
            Buffer[BytesRead] = 0;          // Null terminate for printing

            if (BytesRead != 6) {
                continue;
            }

            ::sscanf(&Buffer[2], "%X", &Sample_Count);

            if (Unipolar_Sample) {
                Fp_Sample = Sample_Count * (Vref/4096.0);
            } else {
                if (Sample_Count >= 2048) {
                    Fp_Sample = (Sample_Count-4097.0+OffsetCalibration) * (Vref/2048.0);
                } else {
                    Fp_Sample = (Sample_Count+OffsetCalibration) * (Vref/2048.0);
                }
            }

            //
            // Do we filter?
            if (FilterCount > 1) {
                FilterArray [i][FilterArrayPosition[i]] = Fp_Sample;

                //
                // Increment and wrap arrar position
                FilterArrayPosition[i] += 1;

                if (FilterArrayPosition[i] >= FilterCount) {
                    FilterArrayPosition[i] = 0;
                }

                Fp_Sample = 0.0;

                //
                // Perform filter average
                for (j=0; j<FilterCount; j++) {
                    Fp_Sample += FilterArray[i][j];
                }

                Fp_Sample = Fp_Sample / FilterCount;
            }

            //
            // First value doesn't get leading ',' for delimiter
            if (First) {
                First = FALSE;
                SampleStr.Format("%lf", Fp_Sample);
                FileStr = FileStr + SampleStr;
            } else {
                SampleStr.Format(",%lf", Fp_Sample);
                FileStr = FileStr + SampleStr;
            }
        }
    } // for (i...)

    FileStr = FileStr + "\r\n";

    ::WriteFile(hLogFile, FileStr.GetBuffer(1), FileStr.GetLength(), &BytesWritten, NULL);

    m_SampleCount++;

    //
    // Update data on Dialog
    m_LastCommand = Command_Str;
    m_LastResponse = Buffer;
    UpdateData (FALSE);
}

void CADC_LogDlg::RS485_Sample()
{
    DWORD   BytesRead, BytesWritten;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);
    int     i, j;
    int     Config_Sample, Unipolar_Sample;
    unsigned int  Sample_Count;
    double  Fp_Sample;
    BOOL    First = TRUE;

    //
    // Start by clearing out the File write string

    FileStr = "";

    //
    // Check each of the possible 8 samples

    for (i=0; i<8; i++) {

        switch (i) {
            case 0:
                Config_Sample = Config_Sample1;
                Unipolar_Sample = Unipolar_Sample1;
                break;
            case 1:
                Config_Sample = Config_Sample2;
                Unipolar_Sample = Unipolar_Sample2;
                break;
            case 2:
                Config_Sample = Config_Sample3;
                Unipolar_Sample = Unipolar_Sample3;
                break;
            case 3:
                Config_Sample = Config_Sample4;
                Unipolar_Sample = Unipolar_Sample4;
                break;
            case 4:
                Config_Sample = Config_Sample5;
                Unipolar_Sample = Unipolar_Sample5;
                break;
            case 5:
                Config_Sample = Config_Sample6;
                Unipolar_Sample = Unipolar_Sample6;
                break;
            case 6:
                Config_Sample = Config_Sample7;
                Unipolar_Sample = Unipolar_Sample7;
                break;
            case 7:
                Config_Sample = Config_Sample8;
                Unipolar_Sample = Unipolar_Sample8;
                break;
        }

        switch (Config_Sample) {
            case 0:
                break;
            case 1:
                Config_Sample = 0x8;    // CH0
                break;
            case 2:
                Config_Sample = 0xC;    // CH1
                break;
            case 3:
                Config_Sample = 0x9;    // CH2
                break;
            case 4:
                Config_Sample = 0xD;    // CH3
                break;
            case 5:
                Config_Sample = 0xA;    // CH4
                break;
            case 6:
                Config_Sample = 0xE;    // CH5
                break;
            case 7:
                Config_Sample = 0xB;    // CH6
                break;
            case 8:
                Config_Sample = 0xF;    // CH7
                break;
            case 9:
                Config_Sample = 0x0;    // +CH0  -CH1
                break;
            case 10:
                Config_Sample = 0x4;    // -CH0  +CH1
                break;
            case 11:
                Config_Sample = 0x1;    // +CH2  -CH3
                break;
            case 12:
                Config_Sample = 0x5;    // -CH2  +CH3
                break;
            case 13:
                Config_Sample = 0x2;    // +CH4  -CH5
                break;
            case 14:
                Config_Sample = 0x6;    // -CH4  +CH5
                break;
            case 15:
                Config_Sample = 0x3;    // +CH6  -CH7
                break;
            case 16:
                Config_Sample = 0x7;    // -CH6  +CH7
                break;
        }

        //
        // Do we need to sample?

        if (Config_Sample) {

            //
            // Construct the command string
            
            if (Unipolar_Sample) {
                Command_Str.Format("%02X00U%1X\r", ModuleAddress, Config_Sample);
            } else {
                Command_Str.Format("%02X00Q%1X\r", ModuleAddress, Config_Sample);
            }

            ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
            Sleep (m_RcvSleepTime);
            ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
            Buffer[BytesRead] = 0;          // Null terminate for printing

            if (BytesRead != 10) {
                continue;
            }

            ::sscanf(&Buffer[6], "%X", &Sample_Count);

            if (Unipolar_Sample) {
                Fp_Sample = Sample_Count * (Vref/4096.0);
            } else {
                if (Sample_Count >= 2048) {
                    Fp_Sample = (Sample_Count-4097.0+OffsetCalibration) * (Vref/2048.0);
                } else {
                    Fp_Sample = (Sample_Count+OffsetCalibration) * (Vref/2048.0);
                }
            }

            //
            // Do we filter?
            if (FilterCount > 1) {
                FilterArray [i][FilterArrayPosition[i]] = Fp_Sample;

                //
                // Increment and wrap arrar position
                FilterArrayPosition[i] += 1;

                if (FilterArrayPosition[i] >= FilterCount) {
                    FilterArrayPosition[i] = 0;
                }

                Fp_Sample = 0.0;

                //
                // Perform filter average
                for (j=0; j<FilterCount; j++) {
                    Fp_Sample += FilterArray[i][j];
                }

                Fp_Sample = Fp_Sample / FilterCount;
            }

            //
            // First value doesn't get leading ',' for delimiter
            if (First) {
                First = FALSE;
                SampleStr.Format("%lf", Fp_Sample);
                FileStr = FileStr + SampleStr;
            } else {
                SampleStr.Format(",%lf", Fp_Sample);
                FileStr = FileStr + SampleStr;
            }
        }
    } // for (i...)

    FileStr = FileStr + "\r\n";

    ::WriteFile(hLogFile, FileStr.GetBuffer(1), FileStr.GetLength(), &BytesWritten, NULL);

    m_SampleCount++;

    //
    // Update data on Dialog
    m_LastCommand = Command_Str;
    m_LastResponse = Buffer;
    UpdateData (FALSE);
}


void CADC_LogDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

    if (!bAskCancel) {

        //
        // Check if we're RS232 or RS485
        if (nIDEvent == 1) {
                RS485_Sample();
        }

        if (nIDEvent == 2) {
                RS232_Sample();
        }
    }
	
	CDialog::OnTimer(nIDEvent);
}

int CADC_LogDlg::RS485_Get_Offset()
{
    DWORD   BytesRead;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);
    int     Offset;

    //
    // Start with 0 calibration
    OffsetCalibration = 0;

    Command_Str.Format ("%02X00R0F\r", ModuleAddress);
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (m_RcvSleepTime);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);

    //
    // Null terminate for printing
    Buffer[BytesRead] = 0;

    if (BytesRead != 8) {
        MessageBox ("Offset calibration not received from module!", "Error", MB_ICONERROR);
        return (FALSE);
    }

    ::sscanf(&Buffer[5], "%X", &Offset);

    //
    // Check for negative numbers and calculate
    if (Offset >= 128) {
        OffsetCalibration = (Offset-256);
    } else {
        OffsetCalibration = Offset;
    }

    return (TRUE);
}

int CADC_LogDlg::RS232_Get_Offset()
{
    DWORD   BytesRead;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);
    int     Offset;

    //
    // Start with 0 calibration
    OffsetCalibration = 0;

    Command_Str.Format ("R0F\r");
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (m_RcvSleepTime);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);

    //
    // Null terminate for printing
    Buffer[BytesRead] = 0;

    if (BytesRead != 4) {
        MessageBox ("Offset calibration not received from module!", "Error", MB_ICONERROR);
        return (FALSE);
    }

    ::sscanf(&Buffer[1], "%X", &Offset);

    //
    // Check for negative numbers and calculate
    if (Offset >= 128) {
        OffsetCalibration = (Offset-256);
    } else {
        OffsetCalibration = Offset;
    }
    return (TRUE);
}
