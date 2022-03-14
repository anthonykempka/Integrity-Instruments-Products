// DAMUtilsView.cpp : implementation of the CDAMUtilsView class
//

#include "stdafx.h"
#include "DAMUtils.h"

#include "DAMUtilsDoc.h"
#include "DAMUtilsView.h"
#include ".\damutilsview.h"

#include ".\ModuleTypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDAMUtilsView

IMPLEMENT_DYNCREATE(CDAMUtilsView, CFormView)

BEGIN_MESSAGE_MAP(CDAMUtilsView, CFormView)
	ON_BN_CLICKED(IDC_RBMODULETYPE, OnBnClickedRbmoduletype)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RBADCREF, OnBnClickedRbadcref)
	ON_BN_CLICKED(IDC_RADIO7, OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO10, OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO9, OnBnClickedRadio9)
END_MESSAGE_MAP()

// CDAMUtilsView construction/destruction

CDAMUtilsView::CDAMUtilsView()
	: CFormView(CDAMUtilsView::IDD)
	, m_nModuleType(0)
	, m_nADCReference(0)
{
	// TODO: add construction code here

}

CDAMUtilsView::~CDAMUtilsView()
{
}

void CDAMUtilsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RBMODULETYPE, m_nModuleType);
	DDX_Radio(pDX, IDC_RBADCREF, m_nADCReference);
}

BOOL CDAMUtilsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CDAMUtilsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();
}


// CDAMUtilsView diagnostics

#ifdef _DEBUG
void CDAMUtilsView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDAMUtilsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDAMUtilsDoc* CDAMUtilsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDAMUtilsDoc)));
	return (CDAMUtilsDoc*)m_pDocument;
}
#endif //_DEBUG


// CDAMUtilsView message handlers

/*****************************************************************************
 * Module Type Radio Button Set
 *	Button Clicked Handlers
 *
 * Radio Button Group is used in conjuction with DDX to form an integer
 * Types are defined in ModuleTypes.h
 *
 *****************************************************************************/
void CDAMUtilsView::OnBnClickedRbmoduletype()
{
	OnRbModuleTypeChanged();
}

void CDAMUtilsView::OnBnClickedRadio2()
{
	OnRbModuleTypeChanged();
}

void CDAMUtilsView::OnBnClickedRadio3()
{
	OnRbModuleTypeChanged();
}

void CDAMUtilsView::OnBnClickedRadio4()
{
	OnRbModuleTypeChanged();
}

void CDAMUtilsView::OnRbModuleTypeChanged(void)
{
	UpdateData();
}
/*****************************************************************************/




/*****************************************************************************
 * ADC Voltage Reference Radio Button Set
 *	Button Clicked Handlers
 *
 * Radio Button Group is used in conjuction with DDX to form an integer
 * Types are defined in ModuleTypes.h
 *
 *****************************************************************************/
void CDAMUtilsView::OnBnClickedRbadcref()
{
	OnADCReferenceChanged();
}

void CDAMUtilsView::OnBnClickedRadio7()
{
	OnADCReferenceChanged();
}

void CDAMUtilsView::OnBnClickedRadio8()
{
	OnADCReferenceChanged();
}

void CDAMUtilsView::OnBnClickedRadio9()
{
	OnADCReferenceChanged();
}

void CDAMUtilsView::OnBnClickedRadio10()
{
	OnADCReferenceChanged();
}

void CDAMUtilsView::OnADCReferenceChanged(void)
{
	UpdateData();
}
/*****************************************************************************/
