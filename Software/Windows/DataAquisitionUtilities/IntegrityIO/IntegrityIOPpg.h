#if !defined(AFX_INTEGRITYIOPPG_H__936E3B00_4D2B_4793_B044_720777FB9ED1__INCLUDED_)
#define AFX_INTEGRITYIOPPG_H__936E3B00_4D2B_4793_B044_720777FB9ED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// IntegrityIOPpg.h : Declaration of the CIntegrityIOPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CIntegrityIOPropPage : See IntegrityIOPpg.cpp.cpp for implementation.

class CIntegrityIOPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CIntegrityIOPropPage)
	DECLARE_OLECREATE_EX(CIntegrityIOPropPage)

// Constructor
public:
	CIntegrityIOPropPage();

// Dialog Data
	//{{AFX_DATA(CIntegrityIOPropPage)
	enum { IDD = IDD_PROPPAGE_INTEGRITYIO };
	long	m_lnCommPort;
	long	m_lnBaudRate;
	long	m_lnCommTimeout;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CIntegrityIOPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTEGRITYIOPPG_H__936E3B00_4D2B_4793_B044_720777FB9ED1__INCLUDED)
