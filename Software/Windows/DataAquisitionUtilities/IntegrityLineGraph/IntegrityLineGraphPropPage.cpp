// IntegrityLineGraphPropPage.cpp : Implementation of the CIntegrityLineGraphPropPage property page class.

#include "stdafx.h"
#include "IntegrityLineGraph.h"
#include "IntegrityLineGraphPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CIntegrityLineGraphPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CIntegrityLineGraphPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CIntegrityLineGraphPropPage, "INTEGRITYLINEG.IntegrityLineGPropPage.1",
	0x472aa81c, 0xb23, 0x4e16, 0xa6, 0xb2, 0x8, 0xf7, 0xc7, 0x8, 0x48, 0xf9)



// CIntegrityLineGraphPropPage::CIntegrityLineGraphPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CIntegrityLineGraphPropPage

BOOL CIntegrityLineGraphPropPage::CIntegrityLineGraphPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_INTEGRITYLINEGRAPH_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CIntegrityLineGraphPropPage::CIntegrityLineGraphPropPage - Constructor

CIntegrityLineGraphPropPage::CIntegrityLineGraphPropPage() :
	COlePropertyPage(IDD, IDS_INTEGRITYLINEGRAPH_PPG_CAPTION)
{
}



// CIntegrityLineGraphPropPage::DoDataExchange - Moves data between page and properties

void CIntegrityLineGraphPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CIntegrityLineGraphPropPage message handlers
