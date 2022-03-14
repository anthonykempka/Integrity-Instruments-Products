#pragma once

// IntegrityLineGraphPropPage.h : Declaration of the CIntegrityLineGraphPropPage property page class.


// CIntegrityLineGraphPropPage : See IntegrityLineGraphPropPage.cpp for implementation.

class CIntegrityLineGraphPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CIntegrityLineGraphPropPage)
	DECLARE_OLECREATE_EX(CIntegrityLineGraphPropPage)

// Constructor
public:
	CIntegrityLineGraphPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_INTEGRITYLINEGRAPH };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

