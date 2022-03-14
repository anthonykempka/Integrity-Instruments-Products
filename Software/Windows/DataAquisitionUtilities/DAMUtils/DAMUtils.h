// DAMUtils.h : main header file for the DAMUtils application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CDAMUtilsApp:
// See DAMUtils.cpp for the implementation of this class
//

class CDAMUtilsApp : public CWinApp
{
public:
	CDAMUtilsApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CDAMUtilsApp theApp;