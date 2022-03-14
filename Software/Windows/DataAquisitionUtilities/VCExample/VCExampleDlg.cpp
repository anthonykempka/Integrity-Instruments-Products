// VCExampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCExample.h"
#include "VCExampleDlg.h"

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
// CVCExampleDlg dialog

CVCExampleDlg::CVCExampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVCExampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVCExampleDlg)
	m_dADC0Volts = 0.0;
	m_dDAC0Output = 0.0;
	m_lblPortStatus = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVCExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVCExampleDlg)
	DDX_Control(pDX, IDC_INTEGRITYIOCTRL1, m_ctrlIntegrity);
	DDX_Text(pDX, IDC_TXT_ADC0INPUT, m_dADC0Volts);
	DDX_Text(pDX, IDC_TXT_DAC0OUTPUT, m_dDAC0Output);
	DDX_Text(pDX, IDC_LBL_PORTSTATUS, m_lblPortStatus);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVCExampleDlg, CDialog)
	//{{AFX_MSG_MAP(CVCExampleDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_TOGGLEP1, OnBtnTogglep1)
	ON_BN_CLICKED(IDC_BTN_DAC0OUT, OnBtnDac0out)
	ON_BN_CLICKED(IDC_BTN_ADC0INPUT, OnBtnAdc0input)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCExampleDlg message handlers

BOOL CVCExampleDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVCExampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVCExampleDlg::OnPaint() 
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
HCURSOR CVCExampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVCExampleDlg::OnBtnStart() 
{
	
	// Set the serial port to COM1
	m_ctrlIntegrity.SetLnComPort(1);

	// Set the communication speed to 115200
	m_ctrlIntegrity.SetLnBaudRate(115200);

	// Open the serial port
	m_ctrlIntegrity.SetBPortOpen(TRUE);

	// Configure Port 2 bit 0 as an input
	// Bit position = 0
	// Bit value = 1
	m_ctrlIntegrity.SetDIOBitDDR(1, 0);

    // Configure Port 1 bit 0 as an output
    // Bit Position = 8
    // Bit Value = 0
	m_ctrlIntegrity.SetDIOBitDDR(0, 8);

	// Set all port 1 outputs to 0
	m_ctrlIntegrity.SetLnDigitalPort(0);

}

void CVCExampleDlg::OnBtnStop() 
{
	// Close the serial port
	m_ctrlIntegrity.SetBPortOpen(FALSE);
}

void CVCExampleDlg::OnBtnTogglep1() 
{
	long p2In;
	
	// Read in Port 2 Bit 0 and invert it
	p2In = m_ctrlIntegrity.GetDIOBit(0);
	p2In ^= 1;
	
	// Set Port 1 Bit 0 to the inverted Port 2 Bit 0
	m_ctrlIntegrity.SetDIOBit(p2In, 8);

	// Read in Port 2 Bit 0
	p2In = m_ctrlIntegrity.GetDIOBit(0);

	// Update the Port 2 Bit 0 input label
	if(p2In == 1)
	{
		m_lblPortStatus = "On";
	}
	else
	{
		m_lblPortStatus = "Off";
	}
	
	// Update the lable so that the new state shows
	UpdateData(FALSE);
}

void CVCExampleDlg::OnBtnDac0out() 
{
	
	// Update the object variables
	UpdateData (TRUE);

	// Set D/A control word for channel 0 output
	m_ctrlIntegrity.SetLnDACControl(0);

	// Output the value from the D/A edit control
	m_ctrlIntegrity.SetDDACOutput(m_dDAC0Output);

	// Read back the dac voltage and display it
	// Set the A/D control word for Channel 0 single point
	m_ctrlIntegrity.SetLnADCControl(8);
	// Set the object variable
	m_dADC0Volts = m_ctrlIntegrity.GetDADCUnipolar();

	// Show new values.
	UpdateData(FALSE);

	
}

void CVCExampleDlg::OnBtnAdc0input() 
{
	// Update the object variables
	UpdateData(TRUE);

	// Read back the dac voltage and display it
	// Set the A/D control word for Channel 0 single point
	m_ctrlIntegrity.SetLnADCControl(8);
	// Set the object variable
	m_dADC0Volts = m_ctrlIntegrity.GetDADCUnipolar();

	// Show new values.
	UpdateData(FALSE);

	
}
