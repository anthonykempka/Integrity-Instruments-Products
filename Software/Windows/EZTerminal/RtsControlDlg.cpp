// RtsControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EZTerm.h"
#include "RtsControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RtsControlDlg dialog


RtsControlDlg::RtsControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RtsControlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(RtsControlDlg)
	m_DTR_ON = FALSE;
	m_RTS_ON = FALSE;
	//}}AFX_DATA_INIT
}


void RtsControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RtsControlDlg)
	DDX_Check(pDX, IDC_DTRON, m_DTR_ON);
	DDX_Check(pDX, IDC_RTSON, m_RTS_ON);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RtsControlDlg, CDialog)
	//{{AFX_MSG_MAP(RtsControlDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RtsControlDlg message handlers

void RtsControlDlg::OnOK() 
{
    UpdateData (TRUE);  // Update before read

    
	CDialog::OnOK();
}


BOOL RtsControlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
