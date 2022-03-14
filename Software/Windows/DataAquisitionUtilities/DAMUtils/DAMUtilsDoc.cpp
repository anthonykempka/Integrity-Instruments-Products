// DAMUtilsDoc.cpp : implementation of the CDAMUtilsDoc class
//

#include "stdafx.h"
#include "DAMUtils.h"

#include "DAMUtilsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDAMUtilsDoc

IMPLEMENT_DYNCREATE(CDAMUtilsDoc, CDocument)

BEGIN_MESSAGE_MAP(CDAMUtilsDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDAMUtilsDoc, CDocument)
END_DISPATCH_MAP()

// Note: we add support for IID_IDAMUtils to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {5722A921-D07C-4805-A2E2-1AA6FC4F8ACF}
static const IID IID_IDAMUtils =
{ 0x5722A921, 0xD07C, 0x4805, { 0xA2, 0xE2, 0x1A, 0xA6, 0xFC, 0x4F, 0x8A, 0xCF } };

BEGIN_INTERFACE_MAP(CDAMUtilsDoc, CDocument)
	INTERFACE_PART(CDAMUtilsDoc, IID_IDAMUtils, Dispatch)
END_INTERFACE_MAP()


// CDAMUtilsDoc construction/destruction

CDAMUtilsDoc::CDAMUtilsDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CDAMUtilsDoc::~CDAMUtilsDoc()
{
	AfxOleUnlockApp();
}

BOOL CDAMUtilsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CDAMUtilsDoc serialization

void CDAMUtilsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Serialize our Document.
	m_c232M100.Serialize(ar);
	m_c232M200.Serialize(ar);
	m_c232M300.Serialize(ar);
	m_c485M300.Serialize(ar);
	m_cUSBM400.Serialize(ar);
	UpdateAllViews(NULL);
}


// CDAMUtilsDoc diagnostics

#ifdef _DEBUG
void CDAMUtilsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDAMUtilsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDAMUtilsDoc commands
