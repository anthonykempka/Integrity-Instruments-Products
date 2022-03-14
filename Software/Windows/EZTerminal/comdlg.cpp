// comdlg.cpp : implementation file
//

#include "stdafx.h"
#include "EZTerm.h"
#include "comdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// comdlg dialog


comdlg::comdlg(CWnd* pParent /*=NULL*/)
	: CDialog(comdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(comdlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nBaudIndex = 0;
	m_nComportIndex = 0;
	m_nDataBitIndex = 0;
	m_nParityIndex = 0;
	m_nStopBitIndex = 0;

}


void comdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(comdlg)
	DDX_Control(pDX, IDC_STOPBITS, m_StopBits);
	DDX_Control(pDX, IDC_PARITY, m_Parity);
	DDX_Control(pDX, IDC_DATABITS, m_DataBits);
	DDX_Control(pDX, IDC_COMPORT, m_Comport);
	DDX_Control(pDX, IDC_BAUDRATE, m_BaudRate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(comdlg, CDialog)
	//{{AFX_MSG_MAP(comdlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// comdlg message handlers

void comdlg::OnOK() 
{

    UpdateData (TRUE);  // Update before read

	//Update the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);

    m_nComportIndex = m_Comport.GetCurSel();
    m_nBaudIndex = m_BaudRate.GetCurSel();
    m_nDataBitIndex = m_DataBits.GetCurSel();
    m_nParityIndex = m_Parity.GetCurSel();
    m_nStopBitIndex = m_StopBits.GetCurSel();

	CDialog::OnOK();
}

BOOL comdlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    int i;
    CString Comx;

	//Read the Communications Settings registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);

    //
    // Setup the COMx Combo box
    //
    m_Comport.ResetContent();
    for (i=1; i<256; i++) {
        Comx.Format("COM%i", i);
        m_Comport.AddString(Comx);
    }
	
	m_BaudRate.SetCurSel(m_nBaudIndex);
	m_Comport.SetCurSel(m_nComportIndex);
	m_DataBits.SetCurSel(m_nDataBitIndex);
	m_Parity.SetCurSel(m_nParityIndex);
	m_StopBits.SetCurSel(m_nStopBitIndex);

    UpdateData (FALSE); // Update after setting (write)    

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
