// DlgCommSetup.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "DlgCommSetup.h"


// CDlgCommSetup dialog

IMPLEMENT_DYNAMIC(CDlgCommSetup, CDialog)
CDlgCommSetup::CDlgCommSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCommSetup::IDD, pParent)
	, m_csCommPort(_T("COM1"))
	, m_csBaudRate(_T("115200"))
{
}

CDlgCommSetup::~CDlgCommSetup()
{
}

void CDlgCommSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_CB_COMMPORT, m_csCommPort);
	DDX_CBString(pDX, IDC_CB_BAUDRATE, m_csBaudRate);
}


BEGIN_MESSAGE_MAP(CDlgCommSetup, CDialog)
END_MESSAGE_MAP()


// CDlgCommSetup message handlers
