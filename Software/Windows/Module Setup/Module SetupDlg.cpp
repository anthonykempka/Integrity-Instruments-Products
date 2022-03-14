// Module SetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Module Setup.h"
#include "Module SetupDlg.h"
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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
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
	DDX_Text(pDX, IDC_STATIC_BUILD_DATE, m_sBuildDate);
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

/////////////////////////////////////////////////////////////////////////////
// CModuleSetupDlg dialog

CModuleSetupDlg::CModuleSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModuleSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModuleSetupDlg)
	m_Data_Direction = _T("");
	m_Module_Address = _T("");
	m_DA_CH0_Default = _T("");
	m_DA_CH1_Default = _T("");
	m_Port1_Default = _T("");
	m_Port2_Default = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CModuleSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModuleSetupDlg)
	DDX_Control(pDX, IDC_232M200_CHECK, m_232M200_Check);
	DDX_Control(pDX, IDC_STREAM_ANALOG, m_Stream_Analog);
	DDX_Control(pDX, IDC_ASYNC_CONFIG, m_Async_Config);
	DDX_Control(pDX, IDC_UNI_SAMPLE8, m_Uni_Sample8);
	DDX_Control(pDX, IDC_UNI_SAMPLE7, m_Uni_Sample7);
	DDX_Control(pDX, IDC_UNI_SAMPLE6, m_Uni_Sample6);
	DDX_Control(pDX, IDC_UNI_SAMPLE5, m_Uni_Sample5);
	DDX_Control(pDX, IDC_UNI_SAMPLE4, m_Uni_Sample4);
	DDX_Control(pDX, IDC_UNI_SAMPLE3, m_Uni_Sample3);
	DDX_Control(pDX, IDC_UNI_SAMPLE2, m_Uni_Sample2);
	DDX_Control(pDX, IDC_UNI_SAMPLE1, m_Uni_Sample1);
	DDX_Control(pDX, IDC_SLOW_ADC, m_Slow_ADC);
	DDX_Control(pDX, IDC_EXPANDER, m_Expander);
	DDX_Control(pDX, IDC_SAMPLE8, m_Sample8);
	DDX_Control(pDX, IDC_SAMPLE7, m_Sample7);
	DDX_Control(pDX, IDC_SAMPLE6, m_Sample6);
	DDX_Control(pDX, IDC_SAMPLE5, m_Sample5);
	DDX_Control(pDX, IDC_SAMPLE4, m_Sample4);
	DDX_Control(pDX, IDC_SAMPLE3, m_Sample3);
	DDX_Control(pDX, IDC_SAMPLE2, m_Sample2);
	DDX_Control(pDX, IDC_SAMPLE1, m_Sample1);
	DDX_Control(pDX, IDC_STREAM_I, m_Stream_Digital);
	DDX_Control(pDX, IDC_STREAM_N, m_Stream_Pulse);
	DDX_Control(pDX, IDC_RS485_Radio, m_RS485_Check);
	DDX_Control(pDX, IDC_RS232_Radio, m_RS232_Check);
    DDX_Control(pDX, IDC_USB_Radio, m_USB_Check);
	DDX_Text(pDX, IDC_DATA_DIRECTION, m_Data_Direction);
	DDV_MaxChars(pDX, m_Data_Direction, 4);
	DDX_Text(pDX, IDC_MODULE_ADDRESS, m_Module_Address);
	DDV_MaxChars(pDX, m_Module_Address, 2);
	DDX_Text(pDX, IDC_DA_CH0_DEFAULT, m_DA_CH0_Default);
	DDV_MaxChars(pDX, m_DA_CH0_Default, 3);
	DDX_Text(pDX, IDC_DA_CH1_DEFAULT, m_DA_CH1_Default);
	DDV_MaxChars(pDX, m_DA_CH1_Default, 3);
	DDX_Text(pDX, IDC_PORT1_DEFAULT, m_Port1_Default);
	DDV_MaxChars(pDX, m_Port1_Default, 2);
	DDX_Text(pDX, IDC_PORT2_DEFAULT, m_Port2_Default);
	DDV_MaxChars(pDX, m_Port2_Default, 2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CModuleSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CModuleSetupDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RS232_Radio, OnRS232Radio)
	ON_BN_CLICKED(IDC_RS485_Radio, OnRS485Radio)
	ON_BN_CLICKED(IDC_COM_SETUP, OnComSetup)
	ON_BN_CLICKED(IDC_READ_CURRENT, OnReadCurrent)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDOK, OnConfigure)
	ON_BN_CLICKED(IDC_USB_Radio, OnUSBRadio)
	ON_BN_CLICKED(IDC_ABOUT_US, OnAboutUs)
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
// CModuleSetupDlg message handlers
//
BOOL CModuleSetupDlg::OnInitDialog()
{
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
	
	// TODO: Add extra initialization here

    
	//Read the Communications Settings registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);

    //
    // Interface types:
    //                  0 = RS232 (default)
    //                  1 = RS485
    //                  2 = USB
    // 
    int Interface_Type;

	Interface_Type = pApp->GetProfileInt("Settings", "Interface", 0);

    switch (Interface_Type) {
    case 1:         // RS485
        m_RS485_Check.SetCheck (1);
        m_USB_Check.SetCheck (0);
        m_RS232_Check.SetCheck (0);
        OnRS485Radio();                 // Enable/Disable async/stream mode controls
        break;
    case 2:         // USB
        m_RS485_Check.SetCheck (0);
        m_USB_Check.SetCheck (1);
        m_RS232_Check.SetCheck (0);
        OnUSBRadio();                   // Enable/Disable async/stream mode controls
        break;
    case 0:         // RS232
    default:
        m_RS485_Check.SetCheck (0);
        m_USB_Check.SetCheck (0);
        m_RS232_Check.SetCheck (1);
        OnRS232Radio();                 // Enable/Disable async/stream mode controls
        break;
    }

    m_Async_Config.LimitText(4);
    m_Stream_Analog.LimitText(2);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CModuleSetupDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CModuleSetupDlg::OnPaint() 
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
HCURSOR CModuleSetupDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CModuleSetupDlg::OnAboutUs() 
{
    //
    // Open up the about dialog
    //
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}

void CModuleSetupDlg::OnRS232Radio() 
{
    //
    // Save in registry
    //
    CWinApp* pApp = AfxGetApp();
    ASSERT_VALID(pApp);

    pApp->WriteProfileInt("Settings", "Interface", 0);

    m_232M200_Check.EnableWindow(TRUE);

    //
    // Enable Async/Stream Mode controls
    //
    m_Sample1.EnableWindow(TRUE);
    m_Sample2.EnableWindow(TRUE);
    m_Sample3.EnableWindow(TRUE);
    m_Sample4.EnableWindow(TRUE);
    m_Sample5.EnableWindow(TRUE);
    m_Sample6.EnableWindow(TRUE);
    m_Sample7.EnableWindow(TRUE);
    m_Sample8.EnableWindow(TRUE);
    m_Uni_Sample1.EnableWindow(TRUE);
    m_Uni_Sample2.EnableWindow(TRUE);
    m_Uni_Sample3.EnableWindow(TRUE);
    m_Uni_Sample4.EnableWindow(TRUE);
    m_Uni_Sample5.EnableWindow(TRUE);
    m_Uni_Sample6.EnableWindow(TRUE);
    m_Uni_Sample7.EnableWindow(TRUE);
    m_Uni_Sample8.EnableWindow(TRUE);
    m_Stream_Pulse.EnableWindow(TRUE);
    m_Stream_Digital.EnableWindow(TRUE);
    m_Async_Config.EnableWindow(TRUE);
    m_Stream_Analog.EnableWindow(TRUE);

    //
    // Handle the current 232M200 check setting
    On232m200Check();
    return;	
}

void CModuleSetupDlg::OnRS485Radio() 
{
    //
    // Save in registry
    //
    CWinApp* pApp = AfxGetApp();
    ASSERT_VALID(pApp);

    pApp->WriteProfileInt("Settings", "Interface", 1);

    m_232M200_Check.EnableWindow(FALSE);

    //
    // Disable Async/Stream Mode controls
    //
    m_Sample1.EnableWindow(FALSE);
    m_Sample2.EnableWindow(FALSE);
    m_Sample3.EnableWindow(FALSE);
    m_Sample4.EnableWindow(FALSE);
    m_Sample5.EnableWindow(FALSE);
    m_Sample6.EnableWindow(FALSE);
    m_Sample7.EnableWindow(FALSE);
    m_Sample8.EnableWindow(FALSE);
    m_Uni_Sample1.EnableWindow(FALSE);
    m_Uni_Sample2.EnableWindow(FALSE);
    m_Uni_Sample3.EnableWindow(FALSE);
    m_Uni_Sample4.EnableWindow(FALSE);
    m_Uni_Sample5.EnableWindow(FALSE);
    m_Uni_Sample6.EnableWindow(FALSE);
    m_Uni_Sample7.EnableWindow(FALSE);
    m_Uni_Sample8.EnableWindow(FALSE);
    m_Stream_Pulse.EnableWindow(FALSE);
    m_Stream_Digital.EnableWindow(FALSE);
    m_Async_Config.EnableWindow(FALSE);
    m_Stream_Analog.EnableWindow(FALSE);
	return;
}

void CModuleSetupDlg::OnUSBRadio() 
{
    //
    // Save in registry
    //
    CWinApp* pApp = AfxGetApp();
    ASSERT_VALID(pApp);

    pApp->WriteProfileInt("Settings", "Interface", 2);

    m_232M200_Check.EnableWindow(FALSE);

    //
    // Enable Async/Stream Mode controls
    //
    m_Sample1.EnableWindow(TRUE);
    m_Sample2.EnableWindow(TRUE);
    m_Sample3.EnableWindow(TRUE);
    m_Sample4.EnableWindow(TRUE);
    m_Sample5.EnableWindow(TRUE);
    m_Sample6.EnableWindow(TRUE);
    m_Sample7.EnableWindow(TRUE);
    m_Sample8.EnableWindow(TRUE);
    m_Uni_Sample1.EnableWindow(TRUE);
    m_Uni_Sample2.EnableWindow(TRUE);
    m_Uni_Sample3.EnableWindow(TRUE);
    m_Uni_Sample4.EnableWindow(TRUE);
    m_Uni_Sample5.EnableWindow(TRUE);
    m_Uni_Sample6.EnableWindow(TRUE);
    m_Uni_Sample7.EnableWindow(TRUE);
    m_Uni_Sample8.EnableWindow(TRUE);
    m_Stream_Pulse.EnableWindow(TRUE);
    m_Stream_Digital.EnableWindow(TRUE);
    m_Async_Config.EnableWindow(TRUE);
    m_Stream_Analog.EnableWindow(TRUE);

    m_232M200_Check.SetCheck(0);

    //
    // Handle the current 232M200 check setting
    On232m200Check();

    return;
}


void CModuleSetupDlg::Create_Sample_Combo_Box(CComboBox *pComboBox, BOOL b232M200)
{
    if (b232M200) {                                     // 232M200 Module
        pComboBox->InsertString(0, "CH0");              // Control Nibble = 0x02
        pComboBox->InsertString(1, "CH1");              // Control Nibble = 0x03
        pComboBox->InsertString(2, "+CH0 -CH1");        // Control Nibble = 0x00
        pComboBox->InsertString(3, "-CH0 +CH1");        // Control Nibble = 0x01

    } else {                                            // 232M300, 485M300, USBM300 Modules
        pComboBox->InsertString(0, "CH0");              // Control Nibble = 0x08
        pComboBox->InsertString(1, "CH1");              // Control Nibble = 0x0C
        pComboBox->InsertString(2, "CH2");              // Control Nibble = 0x09
        pComboBox->InsertString(3, "CH3");              // Control Nibble = 0x0D
        pComboBox->InsertString(4, "CH4");              // Control Nibble = 0x0A
        pComboBox->InsertString(5, "CH5");              // Control Nibble = 0x0E
        pComboBox->InsertString(6, "CH6");              // Control Nibble = 0x0B
        pComboBox->InsertString(7, "CH7");              // Control Nibble = 0x0F
        pComboBox->InsertString(8, "+CH0 -CH1");        // Control Nibble = 0x00
        pComboBox->InsertString(9, "-CH0 +CH1");        // Control Nibble = 0x04
        pComboBox->InsertString(10, "+CH2 -CH3");       // Control Nibble = 0x01
        pComboBox->InsertString(11, "-CH2 +CH3");       // Control Nibble = 0x05
        pComboBox->InsertString(12, "+CH4 -CH5");       // Control Nibble = 0x02
        pComboBox->InsertString(13, "-CH4 +CH5");       // Control Nibble = 0x06
        pComboBox->InsertString(14, "+CH6 -CH7");       // Control Nibble = 0x03
        pComboBox->InsertString(15, "-CH6 +CH7");       // Control Nibble = 0x07
    }
    //
    // Set current selection
    //
    pComboBox->SetCurSel(0);
    return;
}


void CModuleSetupDlg::Config_Sample_Combo_Box(BOOL b232M200) 
{
    //
    // Delete the current ComboBox entries
    //
    m_Sample1.ResetContent();
    m_Sample2.ResetContent();
    m_Sample3.ResetContent();
    m_Sample4.ResetContent();
    m_Sample5.ResetContent();
    m_Sample6.ResetContent();
    m_Sample7.ResetContent();
    m_Sample8.ResetContent();

    //
    // Create each Sample ComboBox
    //
    Create_Sample_Combo_Box(&m_Sample1, b232M200);
    Create_Sample_Combo_Box(&m_Sample2, b232M200);
    Create_Sample_Combo_Box(&m_Sample3, b232M200);
    Create_Sample_Combo_Box(&m_Sample4, b232M200);
    Create_Sample_Combo_Box(&m_Sample5, b232M200);
    Create_Sample_Combo_Box(&m_Sample6, b232M200);
    Create_Sample_Combo_Box(&m_Sample7, b232M200);
    Create_Sample_Combo_Box(&m_Sample8, b232M200);

    return;
}


void CModuleSetupDlg::On232m200Check() 
{
    // Save in registry
    //
    CWinApp* pApp = AfxGetApp();
    ASSERT_VALID(pApp);

    if (m_232M200_Check.GetCheck()) {
        //
        // Setup the Sample ComboBoxes
        //
        Config_Sample_Combo_Box(TRUE);

        //
        // Enable Async/Stream Mode controls
        //
        m_Sample1.EnableWindow(TRUE);
        m_Sample2.EnableWindow(TRUE);
        m_Sample3.EnableWindow(FALSE);
        m_Sample4.EnableWindow(FALSE);
        m_Sample5.EnableWindow(FALSE);
        m_Sample6.EnableWindow(FALSE);
        m_Sample7.EnableWindow(FALSE);
        m_Sample8.EnableWindow(FALSE);
        m_Uni_Sample1.EnableWindow(TRUE);
        m_Uni_Sample2.EnableWindow(TRUE);
        m_Uni_Sample3.EnableWindow(FALSE);
        m_Uni_Sample4.EnableWindow(FALSE);
        m_Uni_Sample5.EnableWindow(FALSE);
        m_Uni_Sample6.EnableWindow(FALSE);
        m_Uni_Sample7.EnableWindow(FALSE);
        m_Uni_Sample8.EnableWindow(FALSE);

        m_Uni_Sample1.SetCheck(TRUE);
        m_Uni_Sample2.SetCheck(TRUE);

    } else {
        //
        // Setup the Sample ComboBoxes
        //
        Config_Sample_Combo_Box(FALSE);

        m_Sample1.EnableWindow(TRUE);
        m_Sample2.EnableWindow(TRUE);
        m_Sample3.EnableWindow(TRUE);
        m_Sample4.EnableWindow(TRUE);
        m_Sample5.EnableWindow(TRUE);
        m_Sample6.EnableWindow(TRUE);
        m_Sample7.EnableWindow(TRUE);
        m_Sample8.EnableWindow(TRUE);
        m_Uni_Sample1.EnableWindow(TRUE);
        m_Uni_Sample2.EnableWindow(TRUE);
        m_Uni_Sample3.EnableWindow(TRUE);
        m_Uni_Sample4.EnableWindow(TRUE);
        m_Uni_Sample5.EnableWindow(TRUE);
        m_Uni_Sample6.EnableWindow(TRUE);
        m_Uni_Sample7.EnableWindow(TRUE);
        m_Uni_Sample8.EnableWindow(TRUE);
    }
    return;
}



void CModuleSetupDlg::OnComSetup() 
{
    CCommSetup dlg;


    if (dlg.DoModal() == IDOK) {


    }

}

void CModuleSetupDlg::OnReadCurrent() 
{
    CWaitCursor Cur;        // Displays the Wait cursor

    // TODO: Add your control notification handler code here

    if (Open_Comport()) {
        if (m_RS485_Check.GetCheck()) {
            Read_Current_Settings(TRUE);
        } else {
            Read_Current_Settings(FALSE);
        }

        ::Close_Comport (hCom);
    }
}

void CModuleSetupDlg::OnDefault() 
{

    m_Module_Address = "01";
    m_Data_Direction = "FFFF";
    m_Async_Config.SetWindowText("0000");
    m_Stream_Analog.SetWindowText("00");

    m_DA_CH0_Default = "000";
    m_DA_CH1_Default = "000";
    m_Port1_Default = "00";
    m_Port2_Default = "00";

    m_Expander.SetCheck(0);
    m_Slow_ADC.SetCheck(0);
    
    m_Sample1.SetCurSel(0);
    m_Sample2.SetCurSel(0);
    m_Sample3.SetCurSel(0);
    m_Sample4.SetCurSel(0);
    m_Sample5.SetCurSel(0);
    m_Sample6.SetCurSel(0);
    m_Sample7.SetCurSel(0);
    m_Sample8.SetCurSel(0);

    m_Uni_Sample1.SetCheck(0);
    m_Uni_Sample2.SetCheck(0);
    m_Uni_Sample3.SetCheck(0);
    m_Uni_Sample4.SetCheck(0);
    m_Uni_Sample5.SetCheck(0);
    m_Uni_Sample6.SetCheck(0);
    m_Uni_Sample7.SetCheck(0);
    m_Uni_Sample8.SetCheck(0);

    m_Stream_Digital.SetCheck(0);
    m_Stream_Pulse.SetCheck(0);
    
    UpdateData(FALSE);
}


void CModuleSetupDlg::OnConfigure() 
{
    CWaitCursor Cur;        // Displays the Wait cursor
    CHAR        sBuffer[100];
    CString     sTemp;

    //
    // Get the updated data
    UpdateData(TRUE);

    m_Module_Address.MakeUpper();
    if (Check_Hex(m_Module_Address, 1, 0xFF, 2) == -1) {
        MessageBox ("Module Address invalid!", "Data Conversion", MB_ICONSTOP);
        return;
    }

    m_Data_Direction.MakeUpper();
    if (Check_Hex(m_Data_Direction, 0, 0xFFFF, 4) == -1) {
        MessageBox ("Data Direction is invalid!", "Data Conversion", MB_ICONSTOP);
        return;
    }

    m_DA_CH0_Default.MakeUpper();
    if (Check_Hex(m_DA_CH0_Default, 0, 0xFFF, 3) == -1) {
        MessageBox ("D/A CH0 default is invalid!", "Data Conversion", MB_ICONSTOP);
        return;
    }
    
    m_DA_CH1_Default.MakeUpper();
    if (Check_Hex(m_DA_CH1_Default, 0, 0xFFF, 3) == -1) {
        MessageBox ("D/A CH1 default is invalid!", "Data Conversion", MB_ICONSTOP);
        return;
    }

    m_Port1_Default.MakeUpper();
    if (Check_Hex(m_Port1_Default, 0, 0xFF, 2) == -1) {
        MessageBox ("Port 1 default is invalid!", "Data Conversion", MB_ICONSTOP);
        return;
    }

    m_Port2_Default.MakeUpper();
    if (Check_Hex(m_Port2_Default, 0, 0xFF, 2) == -1) {
        MessageBox ("Port 2 default is invalid!", "Data Conversion", MB_ICONSTOP);
        return;
    }


    //
    // Only check these if we're in RS232 mode
    //
    if (!m_RS485_Check.GetCheck()) {

        m_Async_Config.GetLine(0, sBuffer, 100);
        sTemp.Format("%s",sBuffer);
        sTemp.MakeUpper();
        if (Check_Hex(sTemp, 0, 0xFFFF, 4) == -1) {
            MessageBox ("Async Configuration is invalid!", "Data Conversion", MB_ICONSTOP);
            return;
        }

        m_Stream_Analog.GetLine(0, sBuffer, 100);
        sTemp.Format("%s",sBuffer);
        sTemp.MakeUpper();
        if (Check_Hex(sTemp, 0, 0x8, 1) == -1) {
            MessageBox ("Stream Analog Configuration is invalid!", "Data Conversion", MB_ICONSTOP);
            return;
        }
    }

    //
    // Reflect the Uppercase converions
    UpdateData(FALSE);

    //
    // Send configuration EEPROM values to module
    if (Open_Comport()) {
        if (m_RS485_Check.GetCheck()) {
            Configure_Module(TRUE);
        } else {
            Configure_Module(FALSE);
        }

        ::Close_Comport (hCom);
    }
}


void CModuleSetupDlg::Read_Current_Settings(BOOL bRS485)
{
    CString Command_Str, Response_Str;
    CString sTemp;
    BOOL    b232M200;

    if (m_232M200_Check.GetCheck()) {
        b232M200 = TRUE;
    } else {
        b232M200 = FALSE;
    }

    //
    // Clear the receive buffer
    ::Clear_Receive_Buffer(hCom);

    //
    // 0x00=Module Address
    Command_Str = "R00\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    m_Module_Address = Response_Str.Right(2);

    //
    // 0x02=Port 1 Direction
    Command_Str = "R02\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    m_Data_Direction = Response_Str.Right(2);

    //
    // 0x03=Port 2 Direction
    Command_Str = "R03\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    m_Data_Direction += Response_Str.Right(2);

    //
    // 0x04/0x05=Async Config
    Command_Str = "R04\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    sTemp = Response_Str.Right(2);

    Command_Str = "R05\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    sTemp += Response_Str.Right(2);
    m_Async_Config.SetWindowText(sTemp);

    //
    // 0x06=Port1 power up setting
    Command_Str = "R06\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    m_Port1_Default = Response_Str.Right(2);

    //
    // 0x07=Port2 power up setting
    Command_Str = "R07\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    m_Port2_Default = Response_Str.Right(2);

    //
    // 0x08=Exander flag
    Command_Str = "R08\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    if (Response_Str.Right(2) == "00") {
        m_Expander.SetCheck(0);
    } else {
        m_Expander.SetCheck(1);
    }

    //
    // 0x09/0x0A=DAC CH0 power up setting
    Command_Str = "R09\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    m_DA_CH0_Default = Response_Str.Right(1);

    Command_Str = "R0A\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    m_DA_CH0_Default += Response_Str.Right(2);

    //
    // 0x0B/0x0C=DAC CH1 power up setting
    Command_Str = "R0B\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    m_DA_CH1_Default = Response_Str.Right(1);

    Command_Str = "R0C\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    m_DA_CH1_Default += Response_Str.Right(2);

    //
    // 0x0D= Slow ADC clock flag
    Command_Str = "R0D\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    if (Response_Str.Right(2) == "00") {
        m_Slow_ADC.SetCheck(0);
    } else {
        m_Slow_ADC.SetCheck(1);
    }

    //
    // 0x10=Stream Analog
    Command_Str = "R10\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    m_Stream_Analog.SetWindowText(Response_Str.Right(2));


    //
    // 0x11=Analog Sample1
    Command_Str = "R11\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }
    m_Sample1.SetCurSel(RtnSampleConfigurationSel(Response_Str.Right(2), b232M200));

    if (Response_Str[1] == '8') {
        m_Uni_Sample1.SetCheck(1);
    } else {
        m_Uni_Sample1.SetCheck(0);
    }

    //
    // 0x11=Analog Sample2
    Command_Str = "R12\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }
    m_Sample2.SetCurSel(RtnSampleConfigurationSel(Response_Str.Right(2), b232M200));

    if (Response_Str[1] == '8') {
        m_Uni_Sample2.SetCheck(1);
    } else {
        m_Uni_Sample2.SetCheck(0);
    }

    //
    // 0x13=Analog Sample3
    Command_Str = "R13\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }
    m_Sample3.SetCurSel(RtnSampleConfigurationSel(Response_Str.Right(2), b232M200));

    if (Response_Str[1] == '8') {
        m_Uni_Sample3.SetCheck(1);
    } else {
        m_Uni_Sample3.SetCheck(0);
    }

    //
    // 0x14=Analog Sample4
    Command_Str = "R14\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }
    m_Sample4.SetCurSel(RtnSampleConfigurationSel(Response_Str.Right(2), b232M200));
    if (Response_Str[1] == '8') {
        m_Uni_Sample4.SetCheck(1);
    } else {
        m_Uni_Sample4.SetCheck(0);
    }

    //
    // 0x15=Analog Sample5
    Command_Str = "R15\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }
    m_Sample5.SetCurSel(RtnSampleConfigurationSel(Response_Str.Right(2), b232M200));

    if (Response_Str[1] == '8') {
        m_Uni_Sample5.SetCheck(1);
    } else {
        m_Uni_Sample5.SetCheck(0);
    }

    //
    // 0x16=Analog Sample6
    Command_Str = "R16\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }
    m_Sample6.SetCurSel(RtnSampleConfigurationSel(Response_Str.Right(2), b232M200));

    if (Response_Str[1] == '8') {
        m_Uni_Sample6.SetCheck(1);
    } else {
        m_Uni_Sample6.SetCheck(0);
    }

    //
    // 0x17=Analog Sample7
    Command_Str = "R17\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }
    m_Sample7.SetCurSel(RtnSampleConfigurationSel(Response_Str.Right(2), b232M200));

    if (Response_Str[1] == '8') {
        m_Uni_Sample7.SetCheck(1);
    } else {
        m_Uni_Sample7.SetCheck(0);
    }

    //
    // 0x18=Analog Sample8
    Command_Str = "R18\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }
    m_Sample8.SetCurSel(RtnSampleConfigurationSel(Response_Str.Right(2), b232M200));

    if (Response_Str[1] == '8') {
        m_Uni_Sample8.SetCheck(1);
    } else {
        m_Uni_Sample8.SetCheck(0);
    }

    //
    // 0x19=Stream Digital
    Command_Str = "R19\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    if (Response_Str.Right(2) == "00") {
        m_Stream_Digital.SetCheck(0);
    } else {
        m_Stream_Digital.SetCheck(1);
    }

    //
    // 0x1A=Stream Pulse
    Command_Str = "R1A\r";
    Response_Str = "RXX\r";
    Send_Command(Command_Str, &Response_Str, bRS485);
    if (Response_Str.IsEmpty()) {
        return;
    }

    Response_Str.TrimRight();
    if (Response_Str.Right(2) == "00") {
        m_Stream_Pulse.SetCheck(0);
    } else {
        m_Stream_Pulse.SetCheck(1);
    }

    UpdateData (FALSE);
}

int CModuleSetupDlg::Open_Comport()
{
    int     ComPort, BaudRate;
    CString sError;

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
            BaudRate = 115200;
            break;
        case 1:
            BaudRate = 57600;
            break;
        case 2:
            BaudRate = 19200;
            break;
        case 3:
        default:
            BaudRate = 9600;
            break;
    }

    //
    // Attempt to open a Comport
    //

    hCom = ::Open_Comport (ComPort, BaudRate);

    if (hCom != INVALID_HANDLE_VALUE) {
        return (TRUE);
    } else {
        sError.Format("Cannot open comport COM%i", ComPort);
        AfxMessageBox(sError, MB_ICONERROR);
        return (FALSE);
    }
}

int CModuleSetupDlg::String_to_Hex(CString str)
{
    int i, val;

    i = ::sscanf ((LPCTSTR) str, "%x", &val);

    if (i != 1) {
        return (-1);
    } else {
        return (val);
    }
}

int CModuleSetupDlg::Check_Hex(CString Str, int LowVal, int HighVal, int MinChars)
{
    int     i, val;
    TCHAR   ch;

    //
    // Check for minimum number of characters
    if (Str.GetLength() < MinChars) {
        return (-1);
    }

    //
    // Check for invalid characters
    Str.MakeUpper();
    for (i=0; i< Str.GetLength(); i++) {
        ch = Str.GetAt(i);

        if ((ch != '0') && (ch != '1') && (ch != '2') && (ch != '3') && (ch != '4') &&
            (ch != '5') && (ch != '6') && (ch != '7') && (ch != '8') && (ch != '9') &&
            (ch != 'A') && (ch != 'B') && (ch != 'C') && (ch != 'D') && (ch != 'E') &&
            (ch != 'F')) {

            return (-1);
        }
    }

    //
    // Convert to integer
    i = ::sscanf ((LPCTSTR) Str, "%x", &val);

    //
    // Check if outside of bounds
    if ((i != 1) || (val < LowVal) || (val > HighVal)) {
        return (-1);
    } else {
        return (val);
    }
}


void CModuleSetupDlg::Configure_Module(BOOL bRS485)
{
    CHAR    sBuffer[100];
    CString sTemp, sCommand, sResponse;
    BOOL    b232M200;

    if (m_232M200_Check.GetCheck()) {
        b232M200 = TRUE;
    } else {
        b232M200 = FALSE;
    }

    //
    // Clear the receive buffer
    ::Clear_Receive_Buffer(hCom);

    //
    // 0x00=Module Address
    sCommand = "W00" + m_Module_Address + "\r";
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x02,0x03=Data Direction
    sCommand = "T" + m_Data_Direction + "\r";
    sResponse = "T\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x04,0x05=Async Config
    m_Async_Config.GetLine(0, sBuffer, 100);
    sTemp.Format("%s",sBuffer);
    sCommand = "W04" + sTemp.Left(2) + "\r";
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    m_Async_Config.GetLine(0, sBuffer, 100);
    sTemp.Format("%s",sBuffer);
    sCommand = "W05" + sTemp.Right(2) + "\r";
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x06=Port 1 default
    sCommand = "W06" + m_Port1_Default + "\r";
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x07=Port 2 default
    sCommand = "W07" + m_Port2_Default + "\r";
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x08=Expander flag 
    if (m_Expander.GetCheck()) {
        sCommand = "W08FF\r";
    } else {
        sCommand = "W0800\r";
    }
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x09,0x0A=DAC CH0 default
    sCommand = "W090" + m_DA_CH0_Default.Left(1) + "\r";
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    sCommand = "W0A" + m_DA_CH0_Default.Right(2) + "\r";
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x0B,0x0C=DAC CH1 default
    sCommand = "W0B0" + m_DA_CH1_Default.Left(1) + "\r";
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    sCommand = "W0C" + m_DA_CH1_Default.Right(2) + "\r";
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x0D=Slow ADC clock flag 
    if (m_Slow_ADC.GetCheck()) {
        sCommand = "W0DFF\r";
    } else {
        sCommand = "W0D00\r";
    }
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x10=Stream Analog
    m_Stream_Analog.GetLine(0, sBuffer, 100);
    sTemp.Format("%s",sBuffer);
    sCommand = "W10" + sTemp + "\r";
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x11=Analog Sample 1
    sCommand = "W11" + RtnSampleConfigurationStr(m_Sample1.GetCurSel(), m_Uni_Sample1.GetCheck(), b232M200);
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }
    //
    // 0x12=Analog Sample 2
    sCommand = "W12" + RtnSampleConfigurationStr(m_Sample2.GetCurSel(), m_Uni_Sample2.GetCheck(), b232M200);
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x13=Analog Sample 3
    sCommand = "W13" + RtnSampleConfigurationStr(m_Sample3.GetCurSel(), m_Uni_Sample3.GetCheck(), b232M200);
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x14=Analog Sample 4
    sCommand = "W14" + RtnSampleConfigurationStr(m_Sample4.GetCurSel(), m_Uni_Sample4.GetCheck(), b232M200);
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x15=Analog Sample 5
    sCommand = "W15" + RtnSampleConfigurationStr(m_Sample5.GetCurSel(), m_Uni_Sample5.GetCheck(), b232M200);
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x16=Analog Sample 6
    sCommand = "W16" + RtnSampleConfigurationStr(m_Sample6.GetCurSel(), m_Uni_Sample6.GetCheck(), b232M200);
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x17=Analog Sample 7
    sCommand = "W17" + RtnSampleConfigurationStr(m_Sample7.GetCurSel(), m_Uni_Sample7.GetCheck(), b232M200);
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x18=Analog Sample 8
    sCommand = "W18" + RtnSampleConfigurationStr(m_Sample8.GetCurSel(), m_Uni_Sample8.GetCheck(), b232M200);
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x19=Stream Digital
    if (m_Stream_Digital.GetCheck()) {
        sCommand = "W1901\r";
    } else {
        sCommand = "W1900\r";
    }
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    //
    // 0x1A=Stream Pulse Count
    if (m_Stream_Pulse.GetCheck()) {
        sCommand = "W1A01\r";
    } else {
        sCommand = "W1A00\r";
    }
    sResponse = "W\r";
    if (!Send_Command(sCommand, &sResponse, bRS485)) {
        return;
    }

    MessageBox ("Module configured successfully!", "Configuration Status", MB_ICONINFORMATION);
    UpdateData (FALSE);
}


int CModuleSetupDlg::RtnSampleConfigurationSel(CString Response_Str, BOOL b232M200)
{
    //
    // NOTE: The following is based on the ComboBox entries set in Create_Sample_Combo_Box()
    //
    if (b232M200) {
        if (Response_Str == "0\r") 
            return (2);
        if (Response_Str == "1\r") 
            return (3);
        if (Response_Str == "2\r") 
            return (0);
        if (Response_Str == "3\r") 
            return (1);
    } else {
        if (Response_Str == "0\r") 
            return (8);
        if (Response_Str == "1\r") 
            return (10);
        if (Response_Str == "2\r") 
            return (12);
        if (Response_Str == "3\r") 
            return (14);
        if (Response_Str == "4\r") 
            return (9);
        if (Response_Str == "5\r") 
            return (11);
        if (Response_Str == "6\r") 
            return (13);
        if (Response_Str == "7\r") 
            return (15);
        if (Response_Str == "8\r") 
            return (0);
        if (Response_Str == "9\r") 
            return (2);
        if (Response_Str == "A\r") 
            return (4);
        if (Response_Str == "B\r") 
            return (6);
        if (Response_Str == "C\r") 
            return (1);
        if (Response_Str == "D\r") 
            return (3);
        if (Response_Str == "E\r") 
            return (5);
        if (Response_Str == "F\r") 
            return (7);
    }
    //
    // default
    //
    return (0);
}

CString CModuleSetupDlg::RtnSampleConfigurationStr(int Sel, BOOL Uni, BOOL b232M200)
{
    int     Channel;
    CString Rtn;

    //
    // NOTE: The following is based on the ComboBox entries set in Create_Sample_Combo_Box()
    //
    if (b232M200) {
        switch (Sel) {
        case 0:
            Channel = 0x02;     // CH0 = 0x02
            break;
        case 1:
            Channel = 0x03;     // CH1 = 0x03
            break;
        case 2:
            Channel = 0x00;     // CH0+ CH1- = 0x00
            break;
        case 3:
            Channel = 0x01;     // CH0- CH1+ = 0x01
            break;
        default:
            Channel = 0x02;     // CH0 = 0x02
            break;
        }
    } else {
        switch (Sel) {
        case 0:
            Channel = 0x08;     // CH0
            break;
        case 1:
            Channel = 0x0C;     // CH1
            break;
        case 2:
            Channel = 0x09;     // CH2
            break;
        case 3:
            Channel = 0x0D;     // CH3
            break;
        case 4:
            Channel = 0x0A;     // CH4
            break;
        case 5:
            Channel = 0x0E;     // CH5
            break;
        case 6:
            Channel = 0x0B;     // CH6
            break;
        case 7:
            Channel = 0x0F;     // CH7
            break;
        case 8:
            Channel = 0x00;     // +CH0
            break;
        case 9:
            Channel = 0x04;     // -CH0
            break;
        case 10:
            Channel = 0x01;     // +CH2
            break;
        case 11:
            Channel = 0x05;     // -CH2
            break;
        case 12: 
            Channel = 0x02;     // +CH4
            break;
        case 13:
            Channel = 0x06;     // -CH4
            break;
        case 14:
            Channel = 0x03;     // +CH6
            break;
        case 15:
            Channel = 0x07;     // -CH6
            break;
        default:
            Channel = 0x08;     // CH0
            break;
        }

        if (Uni) {
            Channel += 0x80;
        }

    }
    Rtn.Format ("%02X\r",Channel);

    return (Rtn);
}


BOOL CModuleSetupDlg::Send_Command(CString sCommand, CString *sResponse, BOOL bRS485 /*= FALSE*/)
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
        *sResponse = "XXXX" + *sResponse;
    }

    dwExpectedReturn = sResponse->GetLength();

    //
    // Send the command
    ::Write_Comport (hCom, sCommand.GetLength(), sCommand.GetBuffer(1));

    //
    // Wait for the the bytes to return or timeout
    ::Read_Comport (hCom, &BytesRead, dwExpectedReturn, Buffer);

    if (BytesRead != dwExpectedReturn) {
        sError.Format("Timeout error in Send_Command(). No response from module on command %s", sCommand);
        AfxMessageBox(sError, MB_ICONERROR);
        //
        // Delete contents of sResponse
        sResponse->Empty();
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

    if (sResponse->Compare(Buffer) == 0) {
        //
        // Strings are identical
        return (TRUE);
    } else {
        //
        // Strings mismatch - send the response string back
        sResponse->Format("%s", Buffer);
        return (FALSE);
    }

    return (TRUE);
}


