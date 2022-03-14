// IntegrityBarGraph.cpp : Implementation of CIntegrityBarGraphApp and DLL registration.

#include "stdafx.h"
#include "IntegrityBarGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CIntegrityBarGraphApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xD38D3179, 0xD74F, 0x49E8, { 0xA1, 0xF1, 0x9A, 0x2A, 0x4, 0xFB, 0x75, 0xE0 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CIntegrityBarGraphApp::InitInstance - DLL initialization

BOOL CIntegrityBarGraphApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CIntegrityBarGraphApp::ExitInstance - DLL termination

int CIntegrityBarGraphApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
