// ADC_Log.h : main header file for the ADC_LOG application
//

#if !defined(AFX_ADC_LOG_H__25E7FB85_B61E_11D2_9134_444553540000__INCLUDED_)
#define AFX_ADC_LOG_H__25E7FB85_B61E_11D2_9134_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CADC_LogApp:
// See ADC_Log.cpp for the implementation of this class
//

class CADC_LogApp : public CWinApp
{
public:
	CADC_LogApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADC_LogApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CADC_LogApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADC_LOG_H__25E7FB85_B61E_11D2_9134_444553540000__INCLUDED_)
