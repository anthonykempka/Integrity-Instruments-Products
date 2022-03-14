// CommSetup.cpp : implementation file
//

#include "stdafx.h"
#include "Module Setup.h"
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
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCommSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommSetup)
	DDX_Control(pDX, IDC_BAUD_RATE, m_BaudRate);
	DDX_Control(pDX, IDC_COMPORT, m_ComPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommSetup, CDialog)
	//{{AFX_MSG_MAP(CCommSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommSetup message handlers

void CCommSetup::OnOK() 
{
    UpdateData (TRUE);  // Update before read

	//Update the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);


    nComPort = m_ComPort.GetCurSel();
    nBaudRate = m_BaudRate.GetCurSel();

	pApp->WriteProfileInt("Settings", "ComPort", nComPort);
	pApp->WriteProfileInt("Settings", "BaudRate", nBaudRate);

	CDialog::OnOK();
}

BOOL CCommSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    int     ComPort, BaudRate, i;
    CString sTemp;

	//Read the Communications Settings registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);

    //
    // Create the 255 possible comport entries
    //
    m_ComPort.ResetContent();
    for (i=0; i<255; i++) {
        sTemp.Format("COM%i", i+1);
        m_ComPort.InsertString(i, sTemp);
    }

    // Default 1 (COM2) if no setting found
	ComPort = pApp->GetProfileInt("Settings", "ComPort", 1);

    // Default 0 (115200) if not setting found
	BaudRate = pApp->GetProfileInt("Settings", "BaudRate", 0);
    
    // Setup the default comport and baudrate for the control list box
    m_ComPort.SetCurSel(ComPort);
    m_BaudRate.SetCurSel(BaudRate);

    UpdateData (FALSE); // Update after setting (write)    
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
