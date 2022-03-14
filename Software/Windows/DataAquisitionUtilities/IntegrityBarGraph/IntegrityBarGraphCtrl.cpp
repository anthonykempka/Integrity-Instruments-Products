// IntegrityBarGraphCtrl.cpp : Implementation of the CIntegrityBarGraphCtrl ActiveX Control class.

#include "stdafx.h"
#include "IntegrityBarGraph.h"
#include "IntegrityBarGraphCtrl.h"
#include "IntegrityBarGraphPropPage.h"
#include ".\integritybargraphctrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CIntegrityBarGraphCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CIntegrityBarGraphCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CIntegrityBarGraphCtrl, COleControl)
	DISP_FUNCTION_ID(CIntegrityBarGraphCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_NOTIFY_ID(CIntegrityBarGraphCtrl, "dblMaxValue", dispiddblMaxValue, m_dblMaxValue, OndblMaxValueChanged, VT_R8)
	DISP_PROPERTY_NOTIFY_ID(CIntegrityBarGraphCtrl, "dblMinValue", dispiddblMinValue, m_dblMinValue, OndblMinValueChanged, VT_R8)
	DISP_PROPERTY_NOTIFY_ID(CIntegrityBarGraphCtrl, "lnNumElements", dispidlnNumElements, m_lnNumElements, OnlnNumElementsChanged, VT_I4)
	DISP_PROPERTY_PARAM_ID(CIntegrityBarGraphCtrl, "dElement", dispiddElement, GetdElement, SetdElement, VT_R8, VTS_UI2)
	DISP_PROPERTY_PARAM_ID(CIntegrityBarGraphCtrl, "strElementTitle", dispidstrElementTitle, GetstrElementTitle, SetstrElementTitle, VT_BSTR, VTS_UI2)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CIntegrityBarGraphCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CIntegrityBarGraphCtrl, 1)
	PROPPAGEID(CIntegrityBarGraphPropPage::guid)
END_PROPPAGEIDS(CIntegrityBarGraphCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CIntegrityBarGraphCtrl, "INTEGRITYBARGRAP.IntegrityBarGrapCtrl.1",
	0x6c8af639, 0xe23a, 0x4041, 0xb6, 0xb2, 0x33, 0x3, 0xd7, 0xd0, 0xe0, 0x66)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CIntegrityBarGraphCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DIntegrityBarGraph =
		{ 0x43F8802F, 0x69F3, 0x428C, { 0x87, 0x5A, 0x9C, 0x1E, 0xBB, 0xBE, 0xC, 0x85 } };
const IID BASED_CODE IID_DIntegrityBarGraphEvents =
		{ 0x45D064D8, 0x9DE5, 0x4A47, { 0x84, 0x72, 0xF5, 0xFB, 0xC4, 0xFD, 0x6, 0x39 } };



// Control type information

static const DWORD BASED_CODE _dwIntegrityBarGraphOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CIntegrityBarGraphCtrl, IDS_INTEGRITYBARGRAPH, _dwIntegrityBarGraphOleMisc)



// CIntegrityBarGraphCtrl::CIntegrityBarGraphCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CIntegrityBarGraphCtrl

BOOL CIntegrityBarGraphCtrl::CIntegrityBarGraphCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_INTEGRITYBARGRAPH,
			IDB_INTEGRITYBARGRAPH,
			afxRegInsertable | afxRegApartmentThreading,
			_dwIntegrityBarGraphOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CIntegrityBarGraphCtrl::CIntegrityBarGraphCtrl - Constructor

CIntegrityBarGraphCtrl::CIntegrityBarGraphCtrl()
: m_nBaseLine(0)
, m_dblScale(0)
{
	InitializeIIDs(&IID_DIntegrityBarGraph, &IID_DIntegrityBarGraphEvents);
	
	m_dblMaxValue = 5.0;
	m_dblMinValue = 0;
	m_lnNumElements = 10;

	m_dblGraphElement.SetSize(m_lnNumElements);

}



// CIntegrityBarGraphCtrl::~CIntegrityBarGraphCtrl - Destructor

CIntegrityBarGraphCtrl::~CIntegrityBarGraphCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CIntegrityBarGraphCtrl::OnDraw - Drawing function

void CIntegrityBarGraphCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	int index;

	CString str;
	int barWidth;
	int deviceCaps = pdc->GetDeviceCaps(RASTERCAPS);
	CBitmap bmp;
	BITMAP bmpInfo;

	// Create a rectangle the size of the graph to be drawn
	CRect crGraph(rcBounds.left + MARGIN_GRAPHLEFT, 
					rcBounds.top + MARGIN_GRAPHTOP,
					rcBounds.right - MARGIN_GRAPHRIGHT,
					rcBounds.bottom - MARGIN_GRAPHBOTTOM);
	
	
	CRect crBar;						// Object represinting one element of the bar graphe
	CBrush cBrush(RGB(0,128, 128));		// Create a brush to be used for filling the 
										// bar graph rectangles.



	if(deviceCaps & RC_BITBLT)
	{

		// --
		// -- Video device is capable of raster operations, as 
		// -- well as in memory device contects, so draw to an
		// -- offscreen bitmap, then transfer to on screen device
		// -- context
		// --
		bmp.CreateBitmap(rcBounds.Width(), rcBounds.Height(), 1, 32, NULL);
		bmp.GetBitmap(&bmpInfo);

		// Create an in-memory DC compatible with the
		// display DC we're using to paint
		CDC dcMemory;
		dcMemory.CreateCompatibleDC(pdc);

		// Select the bitmap into the in-memory DC
		CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);


		// Clear the drawing area.
		dcMemory.FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

		if(m_dblGraphElement.GetSize())
		{
			m_dblScale = ((crGraph.Height()) / m_dblMaxValue);
			barWidth = (crGraph.Width() / m_dblGraphElement.GetSize()) - BAR_SPACE;
		}



		
		// Draw a rectangle around the drawing area
		dcMemory.Rectangle(rcBounds);

		// Draw the min and max values
		str.Format("%0.3f", this->m_dblMaxValue);
		dcMemory.TextOut(rcBounds.left + MARGIN_TEXTLEFT, crGraph.top - MARGIN_TEXTOFFSET , str);

		str.Format("%0.3f", this->m_dblMinValue);
		dcMemory.TextOut(rcBounds.left + MARGIN_TEXTLEFT, crGraph.bottom - MARGIN_TEXTOFFSET, str);
		
		str.Format("%0.3f", ((m_dblMaxValue - m_dblMinValue) / 2) );
		dcMemory.TextOut(rcBounds.left + MARGIN_TEXTLEFT, crGraph.top + ((crGraph.Height()) / 2) - MARGIN_TEXTOFFSET, str);

		dcMemory.MoveTo(crGraph.left, crGraph.bottom);
		dcMemory.LineTo(crGraph.left, crGraph.top);
		dcMemory.MoveTo(crGraph.left, crGraph.top + ((crGraph.Height()) / 2));
		dcMemory.LineTo(crGraph.right, crGraph.top + ((crGraph.Height()) / 2));
		dcMemory.MoveTo(crGraph.left, crGraph.bottom);
		dcMemory.LineTo(crGraph.right, crGraph.bottom);

		
		for(index = 0; index < m_dblGraphElement.GetSize(); index++)
		{
			crBar.bottom = crGraph.bottom;
			crBar.top = (crGraph.bottom) - (LONG)(m_dblGraphElement[index].dValue * m_dblScale);
			crBar.left = MARGIN_BARLEFT + crGraph.left + (index * (barWidth + BAR_SPACE));
			crBar.right = crBar.left + barWidth;
			dcMemory.FillRect(&crBar, &cBrush );
			dcMemory.TextOut(crBar.left, crGraph.bottom + MARGIN_TEXTBOTTOM, m_dblGraphElement[index].cElementName);
		}
	
	
		// Copy the bits from the in-memory DC into the on-
		// screen DC to actually do the painting. Use the centerpoint
		// we computed for the target offset.
		pdc->BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory,	0, 0, SRCCOPY);


		dcMemory.SelectObject(pOldBitmap);
	
	
	}
	else
	{
		// Clear the drawing area.
		pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

		if(m_dblGraphElement.GetSize())
		{
			m_dblScale = ((crGraph.Height()) / m_dblMaxValue);
			barWidth = (crGraph.Width() / m_dblGraphElement.GetSize()) - BAR_SPACE;
		}



		
		// Draw a rectangle around the drawing area
		pdc->Rectangle(rcBounds);

		// Draw the min and max values
		str.Format("%0.3f", this->m_dblMaxValue);
		pdc->TextOut(rcBounds.left + MARGIN_TEXTLEFT, crGraph.top - MARGIN_TEXTOFFSET , str);

		str.Format("%0.3f", this->m_dblMinValue);
		pdc->TextOut(rcBounds.left + MARGIN_TEXTLEFT, crGraph.bottom - MARGIN_TEXTOFFSET, str);
		
		str.Format("%0.3f", ((m_dblMaxValue - m_dblMinValue) / 2) );
		pdc->TextOut(rcBounds.left + MARGIN_TEXTLEFT, crGraph.top + ((crGraph.Height()) / 2) - MARGIN_TEXTOFFSET, str);

		pdc->MoveTo(crGraph.left, crGraph.bottom);
		pdc->LineTo(crGraph.left, crGraph.top);
		pdc->MoveTo(crGraph.left, crGraph.top + ((crGraph.Height()) / 2));
		pdc->LineTo(crGraph.right, crGraph.top + ((crGraph.Height()) / 2));
		pdc->MoveTo(crGraph.left, crGraph.bottom);
		pdc->LineTo(crGraph.right, crGraph.bottom);

		
		for(index = 0; index < m_dblGraphElement.GetSize(); index++)
		{
			crBar.bottom = crGraph.bottom;
			crBar.top = (crGraph.bottom) - (LONG)(m_dblGraphElement[index].dValue * m_dblScale);
			crBar.left = MARGIN_BARLEFT + crGraph.left + (index * (barWidth + BAR_SPACE));
			crBar.right = crBar.left + barWidth;
			pdc->FillRect(&crBar, &cBrush );
			pdc->TextOut(crBar.left, crGraph.bottom + MARGIN_TEXTBOTTOM, m_dblGraphElement[index].cElementName);
		}

	}

}



// CIntegrityBarGraphCtrl::DoPropExchange - Persistence support

void CIntegrityBarGraphCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CIntegrityBarGraphCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
DWORD CIntegrityBarGraphCtrl::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();


	// The control can optimize its OnDraw method, by not restoring
	// the original GDI objects in the device context.
	dwFlags |= canOptimizeDraw;
	return dwFlags;
}



// CIntegrityBarGraphCtrl::OnResetState - Reset control to default state

void CIntegrityBarGraphCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CIntegrityBarGraphCtrl::AboutBox - Display an "About" box to the user

void CIntegrityBarGraphCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_INTEGRITYBARGRAPH);
	dlgAbout.DoModal();
}



// CIntegrityBarGraphCtrl message handlers

void CIntegrityBarGraphCtrl::OndblMaxValueChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	

	SetModifiedFlag();
}

void CIntegrityBarGraphCtrl::OndblMinValueChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	

	SetModifiedFlag();
}

void CIntegrityBarGraphCtrl::OnlnNumElementsChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_dblGraphElement.RemoveAll();
	m_dblGraphElement.SetSize(m_lnNumElements);

	SetModifiedFlag();
}

DOUBLE CIntegrityBarGraphCtrl::GetdElement(USHORT index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DOUBLE rValue = 0.0;

	if(index >= 0 && index <= m_dblGraphElement.GetUpperBound())
	{
		rValue = m_dblGraphElement[index].dValue;
	}
	

	return rValue;
}

void CIntegrityBarGraphCtrl::SetdElement(USHORT index, DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(index >= 0 && index <= this->m_dblGraphElement.GetUpperBound())
	{
		m_dblGraphElement[index].dValue = newVal;
		Invalidate();
	}

	SetModifiedFlag();
}


BSTR CIntegrityBarGraphCtrl::GetstrElementTitle(USHORT index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	if(index >= 0 && index <= this->m_dblGraphElement.GetUpperBound())
	{
		strResult = m_dblGraphElement[index].cElementName;
	}


	return strResult.AllocSysString();
}

void CIntegrityBarGraphCtrl::SetstrElementTitle(USHORT index, LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	if(index >= 0 && index <= this->m_dblGraphElement.GetUpperBound())
	{
		m_dblGraphElement[index].cElementName.Format("%s", newVal);
	}

	SetModifiedFlag();
}
