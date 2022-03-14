// DAMUtilsDoc.h : interface of the CDAMUtilsDoc class
//


#pragma once
#include "data232m200.h"
#include "data232m300.h"
#include "data485m300.h"
#include "dataUsbM400.h"
#include "data232m100.h"

class CDAMUtilsDoc : public CDocument
{
protected: // create from serialization only
	CDAMUtilsDoc();
	DECLARE_DYNCREATE(CDAMUtilsDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CDAMUtilsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CData232M200 m_c232M200;
	CData232M300 m_c232M300;
	CData485M300 m_c485M300;
	CDataUsbM400 m_cUSBM400;
	CData232M100 m_c232M100;

};


