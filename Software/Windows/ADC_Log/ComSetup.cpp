// ComSetup.cpp : implementation file
//

#include "stdafx.h"
#include "ADC_Log.h"
#include "ComSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComSetup dialog


CComSetup::CComSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CComSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComSetup)
	m_Module_Address = 0;
	//}}AFX_DATA_INIT
}


void CComSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComSetup)
	DDX_Control(pDX, IDC_RS232_Radio, m_RS232_Check);
	DDX_Control(pDX, IDC_RS485_Radio, m_RS485_Check);
	DDX_Control(pDX, IDC_BAUD_RATE, m_BaudRate);
	DDX_Control(pDX, IDC_COMPORT, m_Comport);
	DDX_Text(pDX, IDC_MODULE_ADDRESS, m_Module_Address);
	DDV_MinMaxInt(pDX, m_Module_Address, 1, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComSetup, CDialog)
	//{{AFX_MSG_MAP(CComSetup)
	ON_BN_CLICKED(IDC_RS485_Radio, OnRS485Radio)
	ON_BN_CLICKED(IDC_RS232_Radio, OnRS232Radio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComSetup message handlers

void CComSetup::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


void CComSetup::OnOK() 
{
    int ComPort;
    int BaudRate;

    UpdateData (TRUE);  // Update before read

	//Update the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);


    ComPort = m_Comport.GetCurSel();
    BaudRate = m_BaudRate.GetCurSel();

	pApp->WriteProfileInt("Settings", "ComPort", ComPort);
	pApp->WriteProfileInt("Settings", "BaudRate", BaudRate);
    pApp->WriteProfileInt("Settings", "ModuleAddress", m_Module_Address);

    if (m_RS485_Check.GetCheck()) {
        pApp->WriteProfileInt("Settings", "RS485", 1);
    } else {
        pApp->WriteProfileInt("Settings", "RS485", 0);
    }

    CDialog::OnOK();	
}

BOOL CComSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    int ComPort;
    int BaudRate;
    int bRS485_Check;

	//Read the Communications Settings registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);


    // Default 1 (COM2) if no setting found
	ComPort = pApp->GetProfileInt("Settings", "ComPort", 1);

    // Default 0 (115200) if not setting found
	BaudRate = pApp->GetProfileInt("Settings", "BaudRate", 0);
    
    // Setup the default comport and baudrate for the control list box
    m_Comport.SetCurSel(ComPort);
    m_BaudRate.SetCurSel(BaudRate);

    m_Module_Address = pApp->GetProfileInt("Settings", "ModuleAddress", 1);

    // Default 1 (RS485) if no setting found
	bRS485_Check = pApp->GetProfileInt("Settings", "RS485", 1);

    if (bRS485_Check) {
        m_RS485_Check.SetCheck (1);
        m_RS232_Check.SetCheck (0);

    } else {
        m_RS485_Check.SetCheck (0);
        m_RS232_Check.SetCheck (1);

        CEdit* pEdit = (CEdit*) GetDlgItem(IDC_MODULE_ADDRESS);
        pEdit->EnableWindow(FALSE);
    }

    UpdateData (FALSE); // Update after setting (write)    


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CComSetup::OnRS485Radio() 
{
    //
    // Enable Module address window if RS485
    CEdit* pEdit = (CEdit*) GetDlgItem(IDC_MODULE_ADDRESS);
    pEdit->EnableWindow(TRUE);
}

void CComSetup::OnRS232Radio() 
{
    //
    // Disable Module address window if RS232
    CEdit* pEdit = (CEdit*) GetDlgItem(IDC_MODULE_ADDRESS);
    pEdit->EnableWindow(FALSE);
}
