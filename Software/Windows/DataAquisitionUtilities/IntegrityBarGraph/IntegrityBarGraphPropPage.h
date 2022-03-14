#pragma once

// IntegrityBarGraphPropPage.h : Declaration of the CIntegrityBarGraphPropPage property page class.


// CIntegrityBarGraphPropPage : See IntegrityBarGraphPropPage.cpp for implementation.

class CIntegrityBarGraphPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CIntegrityBarGraphPropPage)
	DECLARE_OLECREATE_EX(CIntegrityBarGraphPropPage)

// Constructor
public:
	CIntegrityBarGraphPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_INTEGRITYBARGRAPH };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

