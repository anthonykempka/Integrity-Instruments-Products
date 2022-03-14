// DlgKeyLog.cpp : implementation file
//

#include "stdafx.h"
#include "EZTerm.h"
#include "DlgKeyLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgKeyLog dialog


CDlgKeyLog::CDlgKeyLog(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgKeyLog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgKeyLog)
	m_strKeyLog = _T("");
	//}}AFX_DATA_INIT
	m_pView = NULL;
}

CDlgKeyLog::CDlgKeyLog(CView *pView)
{
	m_pView = pView;

}
BOOL CDlgKeyLog::Create()
{
	return CDialog::Create(CDlgKeyLog::IDD);
}
void CDlgKeyLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgKeyLog)
	DDX_Text(pDX, IDC_EDIT_KEYLOG, m_strKeyLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgKeyLog, CDialog)
	//{{AFX_MSG_MAP(CDlgKeyLog)
	ON_EN_SETFOCUS(IDC_EDIT_KEYLOG, OnSetfocusEditKeylog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKeyLog message handlers

void CDlgKeyLog::OnSetfocusEditKeylog() 
{
	m_pView->SetFocus();	
}
