// terminaldlg.cpp : implementation file
//

#include "stdafx.h"
#include "EZTerm.h"
#include "terminaldlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// terminaldlg dialog


terminaldlg::terminaldlg(CWnd* pParent /*=NULL*/)
	: CDialog(terminaldlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(terminaldlg)
	m_Append_LF = FALSE;
	m_Local_Echo = FALSE;
	m_bHex_Mode = FALSE;
	//}}AFX_DATA_INIT
}


void terminaldlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(terminaldlg)
	DDX_Check(pDX, IDC_APPEND_LF_CHECK, m_Append_LF);
	DDX_Check(pDX, IDC_LCOAL_ECHO_CHECK, m_Local_Echo);
	DDX_Check(pDX, IDC_HEX_MODE_CHECK, m_bHex_Mode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(terminaldlg, CDialog)
	//{{AFX_MSG_MAP(terminaldlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// terminaldlg message handlers

void terminaldlg::OnOK() 
{
    UpdateData (TRUE);  // Update before read

	CDialog::OnOK();
}

BOOL terminaldlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
