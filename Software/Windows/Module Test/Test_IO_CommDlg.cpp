// Test_IO_CommDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test_IO.h"
#include "Test_IODlg.h"
#include "Test_IO_CommDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTest_IO_CommDlg dialog


CTest_IO_CommDlg::CTest_IO_CommDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTest_IO_CommDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTest_IO_CommDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTest_IO_CommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTest_IO_CommDlg)
	DDX_Control(pDX, IDC_BAUD_RATE, m_BaudRate);
	DDX_Control(pDX, IDC_COMPORT, m_Comport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTest_IO_CommDlg, CDialog)
	//{{AFX_MSG_MAP(CTest_IO_CommDlg)
	ON_BN_CLICKED(IDOK, OnCommOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest_IO_CommDlg message handlers

void CTest_IO_CommDlg::OnCommOk() 
{

    UpdateData (TRUE);  // Update before read

	//Update the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);


    nComPort = m_Comport.GetCurSel();
    nBaudRate = m_BaudRate.GetCurSel();

	pApp->WriteProfileInt("Settings", "ComPort", nComPort);
	pApp->WriteProfileInt("Settings", "BaudRate", nBaudRate);

    // TODO: Add your control notification handler code here

    CDialog::OnOK();
}


BOOL CTest_IO_CommDlg::OnInitDialog() 
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
    m_Comport.ResetContent();
    for (i=0; i<255; i++) {
        sTemp.Format("COM%i", i+1);
        m_Comport.InsertString(i, sTemp);
    }

    // Default 1 (COM2) if no setting found
	ComPort = pApp->GetProfileInt("Settings", "ComPort", 1);

    // Default 0 (115200) if not setting found
	BaudRate = pApp->GetProfileInt("Settings", "BaudRate", 0);
    
    // Setup the default comport and baudrate for the control list box
    m_Comport.SetCurSel(ComPort);
    m_BaudRate.SetCurSel(BaudRate);

    UpdateData (FALSE); // Update after setting (write)    
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
