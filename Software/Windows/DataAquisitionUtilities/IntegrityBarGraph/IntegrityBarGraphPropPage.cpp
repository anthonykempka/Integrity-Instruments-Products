// IntegrityBarGraphPropPage.cpp : Implementation of the CIntegrityBarGraphPropPage property page class.

#include "stdafx.h"
#include "IntegrityBarGraph.h"
#include "IntegrityBarGraphPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CIntegrityBarGraphPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CIntegrityBarGraphPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CIntegrityBarGraphPropPage, "INTEGRITYBARGR.IntegrityBarGrPropPage.1",
	0xac2f96f3, 0x28bb, 0x4c0b, 0x9e, 0xe1, 0xda, 0x58, 0x16, 0xa6, 0x1f, 0x8c)



// CIntegrityBarGraphPropPage::CIntegrityBarGraphPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CIntegrityBarGraphPropPage

BOOL CIntegrityBarGraphPropPage::CIntegrityBarGraphPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_INTEGRITYBARGRAPH_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CIntegrityBarGraphPropPage::CIntegrityBarGraphPropPage - Constructor

CIntegrityBarGraphPropPage::CIntegrityBarGraphPropPage() :
	COlePropertyPage(IDD, IDS_INTEGRITYBARGRAPH_PPG_CAPTION)
{
}



// CIntegrityBarGraphPropPage::DoDataExchange - Moves data between page and properties

void CIntegrityBarGraphPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CIntegrityBarGraphPropPage message handlers
