// CommSetup.cpp : implementation file
//

#include "stdafx.h"
#include "Com_File.h"
#include "CommSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommSetup dialog


CCommSetup::CCommSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CCommSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommSetup)
	m_Interval = 0;
	//}}AFX_DATA_INIT
}


void CCommSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommSetup)
	DDX_Control(pDX, IDC_BAUD_RATE, m_BaudRate);
	DDX_Control(pDX, IDC_COMPORT, m_Comport);
	DDX_Text(pDX, IDC_INTERVAL, m_Interval);
	DDV_MinMaxInt(pDX, m_Interval, 1, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommSetup, CDialog)
	//{{AFX_MSG_MAP(CCommSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommSetup message handlers

BOOL CCommSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    int ComPort;
    int BaudRate;

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

    m_Interval = pApp->GetProfileInt("Settings", "Interval", 1);

    UpdateData (FALSE); // Update after setting (write)    

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommSetup::OnOK() 
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
    pApp->WriteProfileInt("Settings", "Interval", m_Interval);

    CDialog::OnOK();	
}
