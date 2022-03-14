// IntegrityLineGraphCtrl.cpp : Implementation of the CIntegrityLineGraphCtrl ActiveX Control class.

#include "stdafx.h"
#include "IntegrityLineGraph.h"
#include "IntegrityLineGraphCtrl.h"
#include "IntegrityLineGraphPropPage.h"
#include ".\integritylinegraphctrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CIntegrityLineGraphCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CIntegrityLineGraphCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CIntegrityLineGraphCtrl, COleControl)
	DISP_FUNCTION_ID(CIntegrityLineGraphCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_PARAM_ID(CIntegrityLineGraphCtrl, "dElement", dispiddElement, GetdElement, SetdElement, VT_R8, VTS_UI2)
	DISP_PROPERTY_PARAM_ID(CIntegrityLineGraphCtrl, "strElement", dispidstrElement, GetstrElement, SetstrElement, VT_BSTR, VTS_UI2)
	DISP_PROPERTY_NOTIFY_ID(CIntegrityLineGraphCtrl, "dblMaxValue", dispiddblMaxValue, m_dblMaxValue, OndblMaxValueChanged, VT_R8)
	DISP_PROPERTY_NOTIFY_ID(CIntegrityLineGraphCtrl, "dblMinValue", dispiddblMinValue, m_dblMinValue, OndblMinValueChanged, VT_R8)
	DISP_PROPERTY_NOTIFY_ID(CIntegrityLineGraphCtrl, "lnNumElements", dispidlnNumElements, m_lnNumElements, OnlnNumElementsChanged, VT_I4)
	DISP_PROPERTY_PARAM_ID(CIntegrityLineGraphCtrl, "ulElementColor", dispidulElementColor, GetulElementColor, SetulElementColor, VT_UI4, VTS_I2)
	DISP_FUNCTION_ID(CIntegrityLineGraphCtrl, "RemoveAll", dispidRemoveAll, RemoveAll, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CIntegrityLineGraphCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CIntegrityLineGraphCtrl, 1)
	PROPPAGEID(CIntegrityLineGraphPropPage::guid)
END_PROPPAGEIDS(CIntegrityLineGraphCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CIntegrityLineGraphCtrl, "INTEGRITYLINEGRA.IntegrityLineGraCtrl.1",
	0xb68ee2fb, 0xa4d5, 0x4580, 0x93, 0x1c, 0x3d, 0x43, 0xe8, 0x5d, 0x72, 0x4d)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CIntegrityLineGraphCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DIntegrityLineGraph =
		{ 0x97EFD0FF, 0x4A44, 0x43C6, { 0xB2, 0xBC, 0x96, 0x5B, 0x3B, 0xEA, 0xBA, 0xCA } };
const IID BASED_CODE IID_DIntegrityLineGraphEvents =
		{ 0x7F5C1214, 0x301A, 0x4244, { 0x81, 0xC2, 0x30, 0xDB, 0x79, 0x6A, 0x38, 0x6E } };



// Control type information

static const DWORD BASED_CODE _dwIntegrityLineGraphOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CIntegrityLineGraphCtrl, IDS_INTEGRITYLINEGRAPH, _dwIntegrityLineGraphOleMisc)



// CIntegrityLineGraphCtrl::CIntegrityLineGraphCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CIntegrityLineGraphCtrl

BOOL CIntegrityLineGraphCtrl::CIntegrityLineGraphCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_INTEGRITYLINEGRAPH,
			IDB_INTEGRITYLINEGRAPH,
			afxRegInsertable | afxRegApartmentThreading,
			_dwIntegrityLineGraphOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CIntegrityLineGraphCtrl::CIntegrityLineGraphCtrl - Constructor

CIntegrityLineGraphCtrl::CIntegrityLineGraphCtrl()
: m_dblScale(0)
, m_bAutoScroll(TRUE)
, m_bIsDrawing(false)
{
	InitializeIIDs(&IID_DIntegrityLineGraph, &IID_DIntegrityLineGraphEvents);
	

	m_grElementData.SetSize(0);
	m_dblMaxValue = 5;
	m_dblMinValue = 0;
	m_lnNumElements = 10;

}



// CIntegrityLineGraphCtrl::~CIntegrityLineGraphCtrl - Destructor

CIntegrityLineGraphCtrl::~CIntegrityLineGraphCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CIntegrityLineGraphCtrl::OnDraw - Drawing function

void CIntegrityLineGraphCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	CString str;
	COLORREF lineColor;
	int startIndex, lineRes;

	int deviceCaps = pdc->GetDeviceCaps(RASTERCAPS);
	CBitmap bmp;
	BITMAP bmpInfo;

	int eIndex, vIndex, grX;
	double value = 0.0;
	double refrenceLine;
	CPen *penColor, *oldPen;
	CBrush *brushColor, *oldBrush;
	int maxGrx, maxVIndex, maxElements;
	//int maxIndex;
	// Create the font to use for display
	TEXTMETRIC tm;
	CFont fontText;
	CSize sizeText;

	
	m_bIsDrawing = true;
	if(deviceCaps & RC_BITBLT)
	{

		
		fontText.CreateFont(
			14,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			"Arial");                 // lpszFacename


		




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

		

		CFont *pOldFont = (CFont*)dcMemory.SelectObject(&fontText);
		dcMemory.GetTextMetrics(&tm);




		if(m_grElementData.GetSize())
		{
			m_dblScale = ((m_crGraph.Height()) / (m_dblMaxValue - m_dblMinValue));
			//barWidth = (m_crGraph.Width() / m_dblGraphElement.GetSize()) - BAR_SPACE;
			refrenceLine = m_crGraph.bottom - ( (0 - m_dblMinValue) * m_dblScale) ;
		}

		


		dcMemory.FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
		


			// Draw a rectangle around the drawing area
		dcMemory.Rectangle(rcBounds);


		dcMemory.FillRect(&m_crGraph, CBrush::FromHandle((HBRUSH)GetStockObject(DKGRAY_BRUSH)));

		
		double startValue = m_dblMinValue;
		double increment = (m_dblMaxValue - m_dblMinValue) / 4;
		
		for(eIndex = 0; eIndex < 5; eIndex++)
		{
			str.Format("%0.3f", startValue);
			dcMemory.TextOut(rcBounds.left + MARGIN_TEXTLEFT, m_crGraph.bottom - ((m_crGraph.Height() / 4) * eIndex) - MARGIN_TEXTOFFSET, str);
			startValue += increment;
		}
		
		
		// Graph Border and gradient lines
		penColor = new CPen(0,1, RGB(0,0x80,0x30));
		oldPen = dcMemory.SelectObject(penColor);

		brushColor = CBrush::FromHandle((HBRUSH)GetStockObject(DKGRAY_BRUSH));
		oldBrush = dcMemory.SelectObject(brushColor);

		dcMemory.Rectangle(&m_crGraph);
		

		lineRes = (int)(m_crGraph.Height() * 0.1);
		for(eIndex = 0; eIndex < 10; eIndex++)
		{
			dcMemory.MoveTo(m_crGraph.left, m_crGraph.bottom - (eIndex * lineRes));
			dcMemory.LineTo(m_crGraph.right, m_crGraph.bottom - (eIndex * lineRes));
		}

		// Find the starting point for the verticle graph lines
		// and setup the scrollbar
		lineRes = (int)(m_crGraph.Width() *  0.1);
		if(m_grElementData.GetSize() > 0)
		{

			maxElements = 0;
			for(eIndex = 0; eIndex < m_grElementData.GetSize(); eIndex++)
			{
				if(maxElements < (int)m_grElementData[eIndex].dValueAry.GetSize())
				{
					maxElements = m_grElementData[eIndex].dValueAry.GetSize();
					//maxIndex = eIndex;
				}
			}

			//if((int)m_grElementData[0].dValueAry.GetSize() > m_crGraph.Width())
			if(maxElements > m_crGraph.Width())
			{
				//startIndex = (lineRes - (int)(m_grElementData[maxIndex].dValueAry.GetSize() % lineRes));
				startIndex = (lineRes - (maxElements % lineRes));

				// Position at end
				// Set the scroll Range
				if(m_bAutoScroll)
				{
					//m_cHScrollBar.SetScrollRange(0, (int)m_grElementData[maxIndex].dValueAry.GetSize(), FALSE);
					//m_cHScrollBar.SetScrollPos((int)m_grElementData[maxIndex].dValueAry.GetSize());
					m_cHScrollBar.SetScrollRange(0, maxElements, FALSE);
					m_cHScrollBar.SetScrollPos(maxElements);
				}
				else
				{
					startIndex = (lineRes - (m_cHScrollBar.GetScrollPos() % lineRes));
				}
			}
			else
			{
				// set relative position Position 
				if(m_bAutoScroll)
				{
					m_cHScrollBar.SetScrollRange(0, (int)m_crGraph.Width(), FALSE);
					//m_cHScrollBar.SetScrollPos(m_crGraph.Width() - (m_crGraph.Width() - (int)m_grElementData[0].dValueAry.GetSize()));
					m_cHScrollBar.SetScrollPos(m_crGraph.Width() - (m_crGraph.Width() - maxElements));
				}
				startIndex = 0;
			}
		}
		else
		{
			startIndex = 0;
		}

		// Draw the verticle graph lines
		for(eIndex = startIndex; eIndex < m_crGraph.Width()-1; eIndex+=lineRes)
		{
			dcMemory.MoveTo(m_crGraph.left + eIndex, m_crGraph.bottom);
			dcMemory.LineTo(m_crGraph.left + eIndex, m_crGraph.top);
		}


		dcMemory.SelectObject(oldPen);
		dcMemory.SelectObject(oldBrush);

		delete penColor;




		// Create the clipping region so that we don't draw outside of our
		// graph rectangle
		CRgn cRgn;
		cRgn.CreateRectRgn(m_crGraph.left, m_crGraph.top, m_crGraph.right, m_crGraph.bottom);
		dcMemory.SelectClipRgn(&cRgn, RGN_AND);

		vIndex = maxVIndex = 0;
		grX = maxGrx = 0;

		// Loop through each element and data value 
		// drawing a graph line for each
		for(eIndex = 0; eIndex < m_grElementData.GetSize(); eIndex++)
		{
			grX = m_crGraph.left+1;		// Start at the left
			
			// Find the starting index, don't forget that there are two pixels in the drawing area that make up the
			// left and right borders
			if(m_bAutoScroll || m_grElementData[eIndex].dValueAry.GetSize() < m_crGraph.Width())
			{
				startIndex = (int)(m_grElementData[eIndex].dValueAry.GetSize() > m_crGraph.Width() ? 
						m_grElementData[eIndex].dValueAry.GetSize() - (m_crGraph.Width()-2) : 0);
			}
			else
			{
				startIndex = m_cHScrollBar.GetScrollPos();
				
			}

			
			// Get the value of the first value to graph
			maxElements = m_grElementData[eIndex].dValueAry.GetSize();
			startIndex = startIndex >= maxElements ? maxElements - 1 : startIndex;
			if(maxElements > 0)
			{
				value = m_grElementData[eIndex].dValueAry[startIndex];
			}
			//dcMemory.MoveTo(grX, (int)(m_crGraph.bottom + -(value * m_dblScale)) );
			dcMemory.MoveTo(grX, (int)(refrenceLine + -(value * m_dblScale)) );
			grX++;

			lineColor = (COLORREF)m_grElementData[eIndex].elementColor;
			penColor = new CPen(0,1, lineColor);
			oldPen = dcMemory.SelectObject(penColor);
			for(vIndex = startIndex + 1; (vIndex < m_grElementData[eIndex].dValueAry.GetSize()) && (vIndex < (startIndex + m_crGraph.Width() - 2)); vIndex++)
			{
				value = m_grElementData[eIndex].dValueAry[vIndex];
				//dcMemory.LineTo(grX, (int)(m_crGraph.bottom + -(value * m_dblScale)));
				dcMemory.LineTo(grX, (int)(refrenceLine + -(value * m_dblScale)));
				grX++;
				
				
			}

			// Store the max values
			maxVIndex = maxVIndex < vIndex ? vIndex : maxVIndex;
			maxGrx = maxGrx < grX ? grX : maxGrx;

			dcMemory.SelectObject(oldPen);
			delete penColor;
		}

		grX = maxGrx;		// Restore the largest value of grX
							// in case some channels are off.
		vIndex = maxVIndex; // Same for the vIndex

		// Draw the indexes just inside the graph area
		dcMemory.SetTextColor(RGB(0, 0XFF, 0) );
		dcMemory.SetBkMode(TRANSPARENT);
		
		str.Format("%d", startIndex);
		sizeText = dcMemory.GetTextExtent(str);
		dcMemory.TextOut(m_crGraph.left + MARGIN_TEXTLEFT, m_crGraph.bottom - sizeText.cy - MARGIN_TEXTBOTTOM, str);

		str.Format("%d", vIndex);
		CSize sizeText2 = dcMemory.GetTextExtent(str);
		int maxPos;

		if(grX - m_crGraph.left <= sizeText.cx + 5)
		{
			maxPos = grX + sizeText.cx + 5;
		}
		else
		{
			if(grX >= m_crGraph.right - sizeText2.cx - MARGIN_TEXTLEFT)
			{
				maxPos = m_crGraph.right - sizeText2.cx - MARGIN_TEXTLEFT;
			}
			else
			{
				maxPos = grX;
			}
		}
		dcMemory.TextOut(maxPos, m_crGraph.bottom - sizeText2.cy - MARGIN_TEXTBOTTOM, str);

		// Copy the bits from the in-memory DC into the on-
		// screen DC to actually do the painting. Use the centerpoint
		// we computed for the target offset.
		pdc->BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory,	0, 0, SRCCOPY);

		
		// Deselect the font
		pdc->SelectObject(pOldFont);

		dcMemory.SelectObject(pOldBitmap);


	}
	else
	{
		// --
		// -- Video device is noe capable of raster operations
		// --


		pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));


			// Draw a rectangle around the drawing area
		pdc->Rectangle(rcBounds);

		// Draw the min, max, and inner values
		str.Format("%0.3f", this->m_dblMaxValue);		// Max
		pdc->TextOut(rcBounds.left + MARGIN_TEXTLEFT, m_crGraph.top - MARGIN_TEXTOFFSET , str);

		str.Format("%0.3f", this->m_dblMinValue);		// Min
		pdc->TextOut(rcBounds.left + MARGIN_TEXTLEFT, m_crGraph.bottom - MARGIN_TEXTOFFSET, str);
		
		str.Format("%0.3f", ((m_dblMaxValue - m_dblMinValue) / 2) );	// Inner
		pdc->TextOut(rcBounds.left + MARGIN_TEXTLEFT, m_crGraph.top + ((m_crGraph.Height()) / 2) - MARGIN_TEXTOFFSET, str);

		str.Format("%0.3f", (m_dblMaxValue * 0.75) );	// Inner top
		pdc->TextOut(rcBounds.left + MARGIN_TEXTLEFT, m_crGraph.top + ((m_crGraph.Height()) / 4) - MARGIN_TEXTOFFSET, str);

		str.Format("%0.3f", (m_dblMaxValue * 0.25) );	// Inner Bottom
		pdc->TextOut(rcBounds.left + MARGIN_TEXTLEFT, m_crGraph.bottom - ((m_crGraph.Height()) / 4) - MARGIN_TEXTOFFSET, str);


		// Graph Border and gradient lines
		pdc->MoveTo(m_crGraph.left, m_crGraph.bottom);
		pdc->LineTo(m_crGraph.left, m_crGraph.top);

		pdc->MoveTo(m_crGraph.left, m_crGraph.top + ((m_crGraph.Height()) / 2));
		pdc->LineTo(m_crGraph.right, m_crGraph.top + ((m_crGraph.Height()) / 2));

		pdc->MoveTo(m_crGraph.left, m_crGraph.top + (int)(m_crGraph.Height() * 0.25));
		pdc->LineTo(m_crGraph.right, m_crGraph.top + (int)(m_crGraph.Height() * 0.25));

		pdc->MoveTo(m_crGraph.left, m_crGraph.bottom - (int)(m_crGraph.Height() * 0.25));
		pdc->LineTo(m_crGraph.right, m_crGraph.bottom - (int)(m_crGraph.Height() * 0.25));

		pdc->MoveTo(m_crGraph.left, m_crGraph.bottom);
		pdc->LineTo(m_crGraph.right, m_crGraph.bottom);

		pdc->MoveTo(m_crGraph.left, m_crGraph.top);
		pdc->LineTo(m_crGraph.right, m_crGraph.top);


		// Loop through each element and data value 
		// drawing a graph line for each
		int eIndex, vIndex, grX;
		double value = 0.0;
		CPen *penColor, *oldPen;
		for(eIndex = 0; eIndex < m_grElementData.GetSize(); eIndex++)
		{
			grX = m_crGraph.left+1;		// Start at the left
			startIndex = (int)(m_grElementData[eIndex].dValueAry.GetSize() > m_crGraph.Width() ? m_grElementData[eIndex].dValueAry.GetSize() - m_crGraph.Width() : 0);
			if(m_grElementData[eIndex].dValueAry.GetSize() > 0)
			{
				value = m_grElementData[eIndex].dValueAry[startIndex];
			}
			pdc->MoveTo(grX, (int)(m_crGraph.bottom + -(value * m_dblScale)) );
			grX++;

			lineColor = (COLORREF)m_grElementData[eIndex].elementColor;
			penColor = new CPen(0,1, lineColor);
			oldPen = pdc->SelectObject(penColor);
			for(vIndex = startIndex + 1; vIndex < m_grElementData[eIndex].dValueAry.GetSize(); vIndex++)
			{
				value = m_grElementData[eIndex].dValueAry[vIndex];
				pdc->LineTo(grX, (int)(m_crGraph.bottom + -(value * m_dblScale)));
				grX++;
			}

			pdc->SelectObject(oldPen);
			delete penColor;
		}





	}
	m_bIsDrawing = false;
}

// --
// -- Standard Windows message mappings
// --
int CIntegrityLineGraphCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	// A scrollbar object must be created for this process as the object
	// is derived directly from CWnd and not CView. The scrollbar is created
	// as a child window so that messages my be received by this (parent) window.
	VERIFY(m_cHScrollBar.Create(SBS_HORZ | SBS_BOTTOMALIGN | WS_CHILD | WS_VISIBLE, 
      CRect(lpCreateStruct->x + 1, lpCreateStruct->y, lpCreateStruct->cx, lpCreateStruct->cy-1), this, WND_HSCROLLID));

	// Create a rectangle the size of the graph to be drawn
	// This object is created once to improve performance
	m_crGraph.SetRect(lpCreateStruct->x + MARGIN_GRAPHLEFT, 
					lpCreateStruct->y + MARGIN_GRAPHTOP,
					lpCreateStruct->cx - MARGIN_GRAPHRIGHT - 1,
					lpCreateStruct->cy - MARGIN_GRAPHBOTTOM);

	

	return 0;
}

void CIntegrityLineGraphCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	CRect scrlRect;
	int scrlHeight;

	// Create a rectangle the size of the graph to be drawn
	// This object is created once to improve performance
	m_crGraph.SetRect(MARGIN_GRAPHLEFT, 
					MARGIN_GRAPHTOP,
					cx - MARGIN_GRAPHRIGHT,
					cy - MARGIN_GRAPHBOTTOM);


	// Get the current scroll bar rect
	m_cHScrollBar.GetWindowRect(&scrlRect);

	// Use height to calculate new position
	scrlHeight = scrlRect.Height();

	// Create new position for scrollbar
	scrlRect.bottom = cy-1;
	scrlRect.top = cy - 1 - scrlHeight;
	scrlRect.left = 1;
	scrlRect.right = cx -1;
	
	// Reposition the scrollbar
	m_cHScrollBar.MoveWindow(scrlRect);

/*
	SCROLLINFO   info;
	m_cHScrollBar.GetScrollInfo(&info, SIF_ALL);
	info.fMask = SIF_ALL;
	info.nPage = m_crGraph.Width() - 2;
	m_cHScrollBar.SetScrollInfo(&info, FALSE);
*/	
}

// --
// -- Scrolling logic
// -- 
void CIntegrityLineGraphCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Get the minimum and maximum scroll-bar positions.
	int minpos;
	int maxpos;

	
	
	maxpos = pScrollBar->GetScrollLimit();
	pScrollBar->GetScrollRange(&minpos, &maxpos); 


	// Get the current position of scroll box.
	int curpos = pScrollBar->GetScrollPos();

	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
		m_bAutoScroll = false;
		curpos = minpos;
		break;

	case SB_RIGHT:      // Scroll to far right.
		m_bAutoScroll = false;
		curpos = maxpos;
		break;

	case SB_ENDSCROLL:   // End scroll.
		if(curpos >= maxpos)
		{
			m_bAutoScroll = true;
		}
		else
		{
			m_bAutoScroll = false;
		}
		break;

	case SB_LINELEFT:      // Scroll left.
		m_bAutoScroll = false;
		if (curpos > minpos)
			curpos--;
		break;

	case SB_LINERIGHT:   // Scroll right.
		m_bAutoScroll = false;
		if (curpos < maxpos)
			curpos++;
		break;

	case SB_PAGELEFT:    // Scroll one page left.
	{
		// Get the page size. 
		SCROLLINFO   info;
		pScrollBar->GetScrollInfo(&info, SIF_ALL);

		if (curpos > minpos)
			curpos = min(maxpos, curpos - (int) m_crGraph.Width()-2);
		m_bAutoScroll = false;
	}
		break;

	case SB_PAGERIGHT:      // Scroll one page right.
	{
		// Get the page size. 
		SCROLLINFO   info;
		pScrollBar->GetScrollInfo(&info, SIF_ALL);

		if (curpos < maxpos)
			curpos = min(maxpos, curpos + (int) m_crGraph.Width()-2);
		m_bAutoScroll = false;
	}
		break;

	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		m_bAutoScroll = false;
		curpos = nPos;      // of the scroll box at the end of the drag operation.
		break;

	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		m_bAutoScroll = false;
		curpos = nPos;     // position that the scroll box has been dragged to.
		break;
	}

	// Set the new position of the thumb (scroll box).
	pScrollBar->SetScrollPos(curpos, TRUE);
	COleControl::OnHScroll(nSBCode, nPos, pScrollBar);
}


// CIntegrityLineGraphCtrl::DoPropExchange - Persistence support

void CIntegrityLineGraphCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CIntegrityLineGraphCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
DWORD CIntegrityLineGraphCtrl::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();


	// The control will not be redrawn when making the transition
	// between the active and inactivate state.
	//dwFlags |= noFlickerActivate;
	return dwFlags;
}



// CIntegrityLineGraphCtrl::OnResetState - Reset control to default state

void CIntegrityLineGraphCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CIntegrityLineGraphCtrl::AboutBox - Display an "About" box to the user

void CIntegrityLineGraphCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_INTEGRITYLINEGRAPH);
	dlgAbout.DoModal();
}



// CIntegrityLineGraphCtrl message handlers

// --
// -- Returns the value of a graph element 
// -- by index
// --
DOUBLE CIntegrityLineGraphCtrl::GetdElement(USHORT index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return 0;
}

// --
// -- Sets the value of a graph element at an index
// --
void CIntegrityLineGraphCtrl::SetdElement(USHORT index, DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// Add a value to the indexed element
	m_grElementData[index].dValueAry.Add(newVal);
	if(!m_bIsDrawing)
		Invalidate();

	SetModifiedFlag();
}


BSTR CIntegrityLineGraphCtrl::GetstrElement(USHORT index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

void CIntegrityLineGraphCtrl::SetstrElement(USHORT index, LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here

	SetModifiedFlag();
}

void CIntegrityLineGraphCtrl::OndblMaxValueChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	m_dblScale = ((m_crGraph.Height()) / m_dblMaxValue == 0 ? 1 / m_crGraph.Height() : m_dblMaxValue);
	Invalidate();
	SetModifiedFlag();
}

void CIntegrityLineGraphCtrl::OndblMinValueChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	Invalidate();

	SetModifiedFlag();
}

void CIntegrityLineGraphCtrl::OnlnNumElementsChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// Remove all elements from the container
	m_grElementData.RemoveAll();

	// Set the container collection size
	m_grElementData.SetSize(m_lnNumElements);

	SetModifiedFlag();
}

ULONG CIntegrityLineGraphCtrl::GetulElementColor(SHORT index)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return m_grElementData[index].elementColor;
}

void CIntegrityLineGraphCtrl::SetulElementColor(SHORT index, ULONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	m_grElementData[index].elementColor = newVal;
	SetModifiedFlag();
}





void CIntegrityLineGraphCtrl::RemoveAll(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	m_grElementData.RemoveAll();
	Invalidate();
}
