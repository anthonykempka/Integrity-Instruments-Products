#pragma once
#include "afxwin.h"

// IntegrityLineGraphCtrl.h : Declaration of the CIntegrityLineGraphCtrl ActiveX Control class.
#define MARGIN_GRAPHLEFT	50
#define MARGIN_GRAPHRIGHT	10
#define MARGIN_GRAPHTOP		20
#define MARGIN_GRAPHBOTTOM	40
#define MARGIN_TEXTLEFT		8
#define MARGIN_TEXTBOTTOM	3
#define MARGIN_BARLEFT		3
#define MARGIN_TEXTOFFSET	8
#define BAR_SPACE			4

#define WND_GRAPHID			WM_USER + 100
#define WND_HSCROLLID		WM_USER + 101


typedef struct __GRAPH_DATA__
{
	CArray<double, double> dValueAry;	// Collection of values in element
	CString cElementName;				// Name of bar element
	COLORREF elementColor;				// Line Color
}GRAPH_DATA, *LPGRAPHDATA;

// CIntegrityLineGraphCtrl : See IntegrityLineGraphCtrl.cpp for implementation.

class CIntegrityLineGraphCtrl : public COleControl
{
	DECLARE_DYNCREATE(CIntegrityLineGraphCtrl)

// Constructor
public:
	CIntegrityLineGraphCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();

// Implementation
protected:
	~CIntegrityLineGraphCtrl();

	DECLARE_OLECREATE_EX(CIntegrityLineGraphCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CIntegrityLineGraphCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CIntegrityLineGraphCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CIntegrityLineGraphCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		dispidRemoveAll = 7L,		dispidulElementColor = 6,		dispidlnNumElements = 5,		dispiddblMinValue = 4,		dispiddblMaxValue = 3,		dispidstrElement = 2,		dispiddElement = 1
	};
protected:
	DOUBLE GetdElement(USHORT index);
	void SetdElement(USHORT index, DOUBLE newVal);
	BSTR GetstrElement(USHORT index);
	void SetstrElement(USHORT index, LPCTSTR newVal);
	void OndblMaxValueChanged(void);
	DOUBLE m_dblMaxValue;
	void OndblMinValueChanged(void);
	DOUBLE m_dblMinValue;
	void OnlnNumElementsChanged(void);
	LONG m_lnNumElements;

	CArray<GRAPH_DATA, GRAPH_DATA> m_grElementData;

	ULONG GetulElementColor(SHORT index);
	void SetulElementColor(SHORT index, ULONG newVal);
	CRect m_crGraph;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CScrollBar m_cHScrollBar;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	double m_dblScale;
protected:
	BOOL m_bAutoScroll;
	void RemoveAll(void);
	bool m_bIsDrawing;
};

