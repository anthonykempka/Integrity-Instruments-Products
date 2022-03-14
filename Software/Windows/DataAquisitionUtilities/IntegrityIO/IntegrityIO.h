#if !defined(AFX_INTEGRITYIO_H__EB798891_3F92_4A5E_BABB_A3E43A0E1B73__INCLUDED_)
#define AFX_INTEGRITYIO_H__EB798891_3F92_4A5E_BABB_A3E43A0E1B73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// IntegrityIO.h : main header file for INTEGRITYIO.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOApp : See IntegrityIO.cpp for implementation.

class CIntegrityIOApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTEGRITYIO_H__EB798891_3F92_4A5E_BABB_A3E43A0E1B73__INCLUDED)
