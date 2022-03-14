// IntegrityLineGraph.cpp : Implementation of CIntegrityLineGraphApp and DLL registration.

#include "stdafx.h"
#include "IntegrityLineGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CIntegrityLineGraphApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x9CDEC8B9, 0x5777, 0x4029, { 0x9F, 0x2, 0xFA, 0x47, 0x13, 0xA4, 0x88, 0xC8 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CIntegrityLineGraphApp::InitInstance - DLL initialization

BOOL CIntegrityLineGraphApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CIntegrityLineGraphApp::ExitInstance - DLL termination

int CIntegrityLineGraphApp::ExitInstance()
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
