#pragma once
#define MARGIN_GRAPHLEFT	50
#define MARGIN_GRAPHRIGHT	10
#define MARGIN_GRAPHTOP		20
#define MARGIN_GRAPHBOTTOM	20
#define MARGIN_TEXTLEFT		3
#define MARGIN_TEXTBOTTOM	3
#define MARGIN_BARLEFT		3
#define MARGIN_TEXTOFFSET	8
#define BAR_SPACE			4

// IntegrityBarGraphCtrl.h : Declaration of the CIntegrityBarGraphCtrl ActiveX Control class.
typedef struct __GRAPH_DATA__
{
	double dValue;			// Value represented by bar element
	CString cElementName;	// Name of bar element

}GRAPH_DATA, *LPGRAPHDATA;

// CIntegrityBarGraphCtrl : See IntegrityBarGraphCtrl.cpp for implementation.

class CIntegrityBarGraphCtrl : public COleControl
{
	DECLARE_DYNCREATE(CIntegrityBarGraphCtrl)

// Constructor
public:
	CIntegrityBarGraphCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();

// Implementation
protected:
	~CIntegrityBarGraphCtrl();

	DECLARE_OLECREATE_EX(CIntegrityBarGraphCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CIntegrityBarGraphCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CIntegrityBarGraphCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CIntegrityBarGraphCtrl)		// Type name and misc status

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
		dispidstrElementTitle = 5,		dispiddElement = 4,		dispidlnNumElements = 3,		dispiddblMinValue = 2,		dispiddblMaxValue = 1
	};
protected:
	void OndblMaxValueChanged(void);
	DOUBLE m_dblMaxValue;
	void OndblMinValueChanged(void);
	DOUBLE m_dblMinValue;
	void OnlnNumElementsChanged(void);
	LONG m_lnNumElements;
	DOUBLE GetdElement(USHORT index);
	void SetdElement(USHORT index, DOUBLE newVal);
	// Graph Refrence Line
	int m_nBaseLine;
	CArray <GRAPH_DATA, GRAPH_DATA> m_dblGraphElement;

public:
	double m_dblScale;
protected:
	BSTR GetstrElementTitle(USHORT index);
	void SetstrElementTitle(USHORT index, LPCTSTR newVal);
};

