// IntegrityIOPpg.cpp : Implementation of the CIntegrityIOPropPage property page class.

#include "stdafx.h"
#include "IntegrityIO.h"
#include "IntegrityIOPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CIntegrityIOPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CIntegrityIOPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CIntegrityIOPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CIntegrityIOPropPage, "INTEGRITYIO.IntegrityIOPropPage.1",
	0x8912dda5, 0x6907, 0x4d25, 0x8b, 0xb2, 0x4a, 0x42, 0xf0, 0xec, 0x9a, 0xe2)


/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOPropPage::CIntegrityIOPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CIntegrityIOPropPage

BOOL CIntegrityIOPropPage::CIntegrityIOPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_INTEGRITYIO_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOPropPage::CIntegrityIOPropPage - Constructor

CIntegrityIOPropPage::CIntegrityIOPropPage() :
	COlePropertyPage(IDD, IDS_INTEGRITYIO_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CIntegrityIOPropPage)
	m_lnCommPort = 0;
	m_lnBaudRate = 0;
	m_lnCommTimeout = 0;
	//}}AFX_DATA_INIT

	SetHelpInfo(_T("Names to appear in the control"), _T("INTEGRITYIO.HLP"), 0);
}


/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOPropPage::DoDataExchange - Moves data between page and properties

void CIntegrityIOPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CIntegrityIOPropPage)
	DDP_Text(pDX, IDC_EDITCOMMPORT, m_lnCommPort, _T("lnComPort") );
	DDX_Text(pDX, IDC_EDITCOMMPORT, m_lnCommPort);
	DDV_MinMaxLong(pDX, m_lnCommPort, 1, 255);
	DDP_Text(pDX, IDC_EDITBAUDRATE, m_lnBaudRate, _T("lnBaudRate") );
	DDX_Text(pDX, IDC_EDITBAUDRATE, m_lnBaudRate);
	DDV_MinMaxLong(pDX, m_lnBaudRate, 9600, 115200);
	DDP_Text(pDX, IDC_EDITCOMMTIMEOUT, m_lnCommTimeout, _T("lnCommTimeout") );
	DDX_Text(pDX, IDC_EDITCOMMTIMEOUT, m_lnCommTimeout);
	DDV_MinMaxLong(pDX, m_lnCommTimeout, 100, 100000);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
	
	
}


/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOPropPage message handlers
