/* IntegrityIOCtl.cpp : Implementation of the CIntegrityIOCtrl ActiveX Control class.
 * Copyright (c) 2004-2005, Integrity Instruments, Inc.
 * All rights reserved. 
 */

#include "stdafx.h"

#include "..\USBM300 Class\USBM300.h"



#include "IntegrityIO.h"
#include "IntegrityIOCtl.h"
#include "IntegrityIOPpg.h"
#include "AboutBox.h"
#include ".\integrityioctl.h"
//#include ".\integrityioctl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <math.h>

IMPLEMENT_DYNCREATE(CIntegrityIOCtrl, COleControl)

VOID DbgOut(LPSTR FormatString, ...); // From USBM300

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CIntegrityIOCtrl, COleControl)
	//{{AFX_MSG_MAP(CIntegrityIOCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CIntegrityIOCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CIntegrityIOCtrl)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnComPort", GetLnComPort, SetLnComPort, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnBaudRate", GetLnBaudRate, SetLnBaudRate, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnProtocol", GetLnProtocol, SetLnProtocol, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnModuleAddress", GetLnModuleAddress, SetLnModuleAddress, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "bPortOpen", GetBPortOpen, SetBPortOpen, VT_BOOL)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnCommTimeout", GetLnCommTimeout, SetLnCommTimeout, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "dReferenceVoltage", GetDReferenceVoltage, SetDReferenceVoltage, VT_R8)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnAbout", GetLnAbout, SetLnAbout, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnLastError", GetLnLastError, SetLnLastError, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "bDialogDisable", GetBDialogDisable, SetBDialogDisable, VT_BOOL)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnSoftwareVersion", GetLnSoftwareVersion, SetLnSoftwareVersion, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnFirmwareVersion", GetLnFirmwareVersion, SetLnFirmwareVersion, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnDigitalPort", GetLnDigitalPort, SetLnDigitalPort, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnDigitalDirection", GetLnDigitalDirection, SetLnDigitalDirection, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnPulseCount", GetLnPulseCount, SetLnPulseCount, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnADCControl", GetLnADCControl, SetLnADCControl, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "dADCBipolar", GetDADCBipolar, SetDADCBipolar, VT_R8)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnDACControl", GetLnDACControl, SetLnDACControl, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "dDACOutput", GetDDACOutput, SetDDACOutput, VT_R8)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnRcvErrCount", GetLnRcvErrCount, SetLnRcvErrCount, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "bPWMUpdate", GetBPWMUpdate, SetBPWMUpdate, VT_BOOL)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnEEPROMAddress", GetLnEEPROMAddress, SetLnEEPROMAddress, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnEEPROMData", GetLnEEPROMData, SetLnEEPROMData, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "bResetModule", GetBResetModule, SetBResetModule, VT_BOOL)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnPWMFreq", GetLnPWMFreq, SetLnPWMFreq, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "lnPWMDuty", GetLnPWMDuty, SetLnPWMDuty, VT_I4)
	DISP_PROPERTY_EX(CIntegrityIOCtrl, "dADCUnipolar", GetDADCUnipolar, SetDADCUnipolar, VT_R8)
	DISP_FUNCTION(CIntegrityIOCtrl, "SetDIOBit", SetDIOBit, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CIntegrityIOCtrl, "GetDIOBit", GetDIOBit, VT_I4, VTS_I4)
	DISP_FUNCTION(CIntegrityIOCtrl, "SetDIOBitDDR", SetDIOBitDDR, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CIntegrityIOCtrl, "GetDIOBitDDR", GetDIOBitDDR, VT_I4, VTS_I2)
	DISP_FUNCTION_ID(CIntegrityIOCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_NOTIFY_ID(CIntegrityIOCtrl, "bInvalidate", dispidbInvalidate, m_bInvalidate, OnbInvalidateChanged, VT_BOOL)
	DISP_PROPERTY_EX_ID(CIntegrityIOCtrl, "lnAnalogGain", dispidlnAnalogGain, GetlnAnalogGain, SetlnAnalogGain, VT_I4)
	DISP_PROPERTY_EX_ID(CIntegrityIOCtrl, "bModeStream", dispidbModeStream, GetbModeStream, SetbModeStream, VT_BOOL)
	DISP_PROPERTY_EX_ID(CIntegrityIOCtrl, "dDACReferenceVoltage", dispiddDACReferenceVoltage, GetdDACReferenceVoltage, SetdDACReferenceVoltage, VT_R8)
	DISP_FUNCTION_ID(CIntegrityIOCtrl, "GetUSBModuleAddress", dispidGetUSBModuleAddress, GetUSBModuleAddress, VT_I4, VTS_I4)
	DISP_PROPERTY_EX_ID(CIntegrityIOCtrl, "lnUSBStreamBlockCount", dispidlnUSBStreamBlockCount, GetlnUSBStreamBlockCount, SetlnUSBStreamBlockCount, VT_I4)
	DISP_FUNCTION_ID(CIntegrityIOCtrl, "GetUSBDataBlock", dispidGetUSBDataBlock, GetUSBDataBlock, VT_I4, VTS_PR8 VTS_I4 VTS_I4)
	//}}AFX_DISPATCH_MAP
	DISP_PROPERTY_EX_ID(CIntegrityIOCtrl, "nADCBits", dispidnADCBits, GetnADCBits, SetnADCBits, VT_I2)
	DISP_PROPERTY_EX_ID(CIntegrityIOCtrl, "dADCScale", dispiddADCScale, GetdADCScale, SetdADCScale, VT_R8)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CIntegrityIOCtrl, COleControl)
	//{{AFX_EVENT_MAP(CIntegrityIOCtrl)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CIntegrityIOCtrl, 1)
	PROPPAGEID(CIntegrityIOPropPage::guid)
END_PROPPAGEIDS(CIntegrityIOCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CIntegrityIOCtrl, "INTEGRITYIO.IntegrityIOCtrl.1",
	0xb9b93b71, 0x3ea9, 0x4bdd, 0x97, 0x34, 0xcd, 0x28, 0xdf, 0x66, 0xeb, 0x75)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CIntegrityIOCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DIntegrityIO =
		{ 0x29dc770f, 0x1391, 0x4d90, { 0xa8, 0xd, 0xfe, 0x1f, 0x2c, 0xc7, 0xf5, 0xa3 } };
const IID BASED_CODE IID_DIntegrityIOEvents =
		{ 0x79ea71b5, 0x94ed, 0x463b, { 0x9f, 0x34, 0xe3, 0x62, 0x79, 0xaa, 0x89, 0xa0 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwIntegrityIOOleMisc =
	OLEMISC_SIMPLEFRAME |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CIntegrityIOCtrl, IDS_INTEGRITYIO, _dwIntegrityIOOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOCtrl::CIntegrityIOCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CIntegrityIOCtrl

BOOL CIntegrityIOCtrl::CIntegrityIOCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_INTEGRITYIO,
			IDB_INTEGRITYIO,
			afxRegInsertable | afxRegApartmentThreading,
			_dwIntegrityIOOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOCtrl::CIntegrityIOCtrl - Constructor

CIntegrityIOCtrl::CIntegrityIOCtrl()
: m_lnAnalogGain(0)
, m_lnUSBStreamBlockCount(0)
, m_nAdcTwosCompShift(0)
{
	InitializeIIDs(&IID_DIntegrityIO, &IID_DIntegrityIOEvents);

	EnableSimpleFrame();

	// TODO: Initialize your control's instance data here.
	m_dReferenceVoltage = 5.0;
	m_lnBaudRate = 115200;
	m_lnComport = 1;
	m_lnModuleAddress = 1;
	m_lnProtocol = 1;
	m_lnPWMDuty = 50;
	m_lnPWMFreq = 100;
	m_bDialogsDisabled = FALSE;
	m_lnComTimeout = 1000;

	m_hComm = NULL;
	m_nDrawState = PORT_CLOSED;
	m_nDACControl = 0;
	m_nADCControl = 0;
	m_nEEAddress = 255;
	m_bStreamModeFlag = FALSE;
	m_bGetRects = TRUE;

	//m_pUSB = NULL;
	m_bInvalidate = true;
	m_bModeStream = false;
	m_dDACReferenceVoltage = 5.0;

	m_nAdcTwosCompShift = 4;				// Shift variable for twos complement conversion
	m_nADCBits = 12;
	m_dADCScale = 1.0;
}


/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOCtrl::~CIntegrityIOCtrl - Destructor

CIntegrityIOCtrl::~CIntegrityIOCtrl()
{
	// TODO: Cleanup your control's instance data here.
	CloseComm();

	//if(NULL != m_pUSB) delete m_pUSB;
}


/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOCtrl::OnDraw - Drawing function

void CIntegrityIOCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	CDC dcMemory;

	// Create a nice looking frame
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(LTGRAY_BRUSH)));

	// Load the appropriate bitmap
	dcMemory.CreateCompatibleDC(pdc);
	switch(m_nDrawState)
	{
	case PORT_CLOSED:		// Port closed
		//bitmap.LoadBitmap(IDB_INTEGRITYDISPOFF);
		dcMemory.SelectObject(&m_cDOffBM);	
		break;
	case PORT_OPENIDLE:		// Port open/idle
		//bitmap.LoadBitmap(IDB_INTEGRITYDISPON);
		dcMemory.SelectObject(&m_cDOnBM);	
		break;
	case PORT_OPENTX:		// Port open/tx
		//bitmap.LoadBitmap(IDB_INTEGRITYDISPTX);
		dcMemory.SelectObject(&m_cTXBM);	
		break;
	case PORT_OPENRX:		// Port open/rx
		//bitmap.LoadBitmap(IDB_INTEGRITYDISPRX);
		dcMemory.SelectObject(&m_cRXBM);	
		break;	//Port Open/rx
	}
	// Display the loaded bitmap
	
	pdc->StretchBlt(rcBounds.left, rcBounds.top, rcBounds.right, rcBounds.bottom,&dcMemory,0,0,85,32,SRCCOPY);

}


/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOCtrl::DoPropExchange - Persistence support

void CIntegrityIOCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
	

}


/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOCtrl::OnResetState - Reset control to default state

void CIntegrityIOCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOCtrl::AboutBox - Display an "About" box to the user

void CIntegrityIOCtrl::AboutBox()
{
	//CDialog dlgAbout(IDD_ABOUTBOX_INTEGRITYIO);
	CAboutBox dlgAbout;
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CIntegrityIOCtrl message handlers
/************************* GetLnComPort *************************************
 *
 * Return the current commport settings
 *
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnComPort() 
{
	/*
	 * Return the current commport setting
	 */
	return m_lnComport;
}

/************************* SetLnComPort *************************************
 *
 * Set the current commport settings
 *
 ****************************************************************************/
void CIntegrityIOCtrl::SetLnComPort(long nNewValue) 
{
	/*
	 * Update the current comport setting to
	 * the new value
	 */

	// Check for an invalid value
	if( ((nNewValue < 0) || (nNewValue > 255)) && !m_bDialogsDisabled )
	{
		AfxMessageBox("Invalid Comport value");
		return;
	}
		
	m_lnComport = nNewValue;

	// If the port is already open, then reset the connection
	// using the new baud rate
	if(GetBPortOpen())
	{
		SetBPortOpen(FALSE);
		SetBPortOpen(TRUE);
	}

	SetModifiedFlag();
}
/************************* GetLnBaudRate *************************************
 *
 * Return the current communication speed setting
 *
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnBaudRate() 
{
	/*
	 * Return the current Baud rate setting
	 */

	return m_lnBaudRate;
}

/************************* SetLnBaudRate *************************************
 *
 * Set the current communication speed setting
 * Calling this function will automatically reset the baud rate if the
 * port is alread open.
 ****************************************************************************/
void CIntegrityIOCtrl::SetLnBaudRate(long nNewValue) 
{
	/*
	 * Update the current baud rate
	 */

	switch(nNewValue)
	{
	case 9600:
	case 19200:
	case 57600:
	case 115200:
		m_lnBaudRate = nNewValue;
		break;
	default:
		if(!m_bDialogsDisabled)
		{
			AfxMessageBox("Invalid Baud Rate setting");
		}
		break;
	}
	
	// If the port is already open, then reset the connection
	// using the new baud rate
	if(GetBPortOpen())
	{
		SetBPortOpen(FALSE);
		SetBPortOpen(TRUE);
	}
	SetModifiedFlag();
}
/************************* GetLnProtocol *************************************
 *
 * Return the current communication protocol setting
 *
 *	1. RS-232
 *	2. RS-485
 *	3. USB
 *
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnProtocol() 
{
	/*
	 * Return the current protocol setting.
	 */
	return m_lnProtocol;
}

/************************* SetLnProtocol ************************************
 *
 * Set the current communication protocol setting
 *
 *	1. RS-232
 *	2. RS-485
 *	3. USB
 * Note:
 *	Invalid parameters are ignored
 ****************************************************************************/
void CIntegrityIOCtrl::SetLnProtocol(long nNewValue) 
{
	if( ((nNewValue < 1) || (nNewValue > 3)) && !m_bDialogsDisabled)
	{
		AfxMessageBox("Invalid protocol specification");
		return;
	}

	m_lnProtocol = nNewValue;

	SetModifiedFlag();
}
/************************* GetLnModuleAddress *******************************
 *
 * Return the current module address
 *
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnModuleAddress() 
{
	/*
	 * Return the current module address
	 */
	return m_lnModuleAddress;
}

/************************* SetLnModuleAddress *******************************
 *
 * Set the module address for all following module accesses
 * (Not valid for RS-232 modules)
 *
 * Valid Addresses are in the range of:
 *		1...254
 *		0 is the host address
 *		255 is a broadcast address
 *
 ****************************************************************************/
void CIntegrityIOCtrl::SetLnModuleAddress(long nNewValue) 
{
	/*
	 * set the module address for control
	 */
	//if( ((nNewValue < 1) || (nNewValue > 254)) && !m_bDialogsDisabled)
	if( ((nNewValue < 1) || (nNewValue > 255)) && !m_bDialogsDisabled)
	{
		AfxMessageBox("Invalid module address");
		return;
	}
	m_lnModuleAddress = nNewValue;
	SetModifiedFlag();
}

/************************* GetBPortOpen ************************************
 *
 * Return a flag based on the ability to communicate using the serial port.
 *
 ****************************************************************************/
BOOL CIntegrityIOCtrl::GetBPortOpen() 
{
	BOOL bFlag;
	switch(m_lnProtocol)
	{
	default:
	case 1:
	case 2:
		bFlag = ((m_hComm != NULL) && (m_hComm != INVALID_HANDLE_VALUE)) ? TRUE : FALSE;
		break;
	case 3:
		//bFlag = (m_pUSB != NULL) ? TRUE : FALSE;
		bFlag = TRUE;
		break;
	}
	return bFlag;
	
}

/************************* SetBPortOpen ************************************
 *
 * Attempt to open a communication channle using the preset parameters
 * 
 * m_hComm will contain a valid handle on successful completion.
 *
 * m_lnComport -- Port number to open 1...255
 * m_lnBaudRate - Communication speed,
 *				  9600, 19200, 57600, or 115200
 *
 *
 ****************************************************************************/
void CIntegrityIOCtrl::SetBPortOpen(BOOL bNewValue) 
{
	/*
	 * Open or close communications based on the value
	 * passed by the program
	 */
	BOOL bFlag = bNewValue;
	switch(m_lnProtocol)
	{
	default:		// Serial port based protocols
	case 1:
	case 2:
		if(bFlag == TRUE)
		{
			if(OpenComm())
			{
				m_nDrawState = PORT_OPENIDLE;
				SetOffsetCalibration();
			}
		}
		else
		{
			CloseComm();
			m_nDrawState = PORT_CLOSED;
		}
		break;
	case 3:
		if(bFlag == TRUE)
		{
			// Create an instance of the USB Object
			/*
			if(NULL == (m_pUSB = new USBM300()) && !m_bDialogsDisabled)
			{
				AfxMessageBox("Error creating USB connection!");
				DbgOut("IntegrityIOCtl.SetPortOpen(): Error creating USBM300 Object\n");
			}
			*/

			// Check for multiple units with the same address
			//if(m_pUSB->DuplicateAddressPresent())
			if(m_pUSB.DuplicateAddressPresent())
			{
				if(!m_bDialogsDisabled)
					AfxMessageBox("There are multiple units with the same address");

				DbgOut("There are multiple units with the same address\n");

			}
//			if (m_pUSB != NULL )
//			{
				m_nDrawState = PORT_OPENIDLE;
				DbgOut("USB Connection Created\n");
//			}
		}
		else
		{
			/*
			if(NULL != m_pUSB) 
			{
				delete m_pUSB;
				m_pUSB = NULL;
				DbgOut("USB Connection Deleted\n");
			}
			*/
			m_nDrawState = PORT_CLOSED;
		}

		break;
	}
	if(m_bInvalidate)
		Invalidate(TRUE);
	SetModifiedFlag();
}

/************************* GetLnCommTimeout *********************************
 *
 * Return the current timeout value for recieving data from a module in 
 * milliseconds
 *
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnCommTimeout() 
{
	/*
	 * Return the current comm timout value
	 */
	return m_lnComTimeout;
}


/************************* SetLnCommTimeout *********************************
 *
 * Set the number of milliseconds to wait for a packet to be returned from
 * a module. 
 *
 * Must be a positive integer value.
 *
 ****************************************************************************/
void CIntegrityIOCtrl::SetLnCommTimeout(long nNewValue) 
{
	/*
	 * Set the current communication timeout value
	 */
	if(nNewValue > 0)
	{
		m_lnComTimeout = nNewValue;
	}

	SetModifiedFlag();
}


/************************* GetDReferenceVoltage *****************************
 *
 * Return the module Refrence voltage used for analog I/O.
 *
 ****************************************************************************/
double CIntegrityIOCtrl::GetDReferenceVoltage() 
{
	/*
	 * Return the current refrence voltage
	 */
	
	return m_dReferenceVoltage;
}

/************************* SetDReferenceVoltage *****************************
 *
 * Set the refrence voltage to be used for analog I/O. 
 *
 * Caution: Any value may be used here, if not set correctly then upredictable
 *			results WILL occur.
 *
 ****************************************************************************/
void CIntegrityIOCtrl::SetDReferenceVoltage(double newValue) 
{
	/*
	 * Set the current reference voltage
	 */
	m_dReferenceVoltage = newValue;

	// reset the offest calibration coefficient.
	SetOffsetCalibration();
	SetModifiedFlag();
}

long CIntegrityIOCtrl::GetLnAbout() 
{
	
	/*
	 * Display about box and return the 
	 * value from DoModal();
	 */
	
	return 0;
}



/************************* SetLnAbout  **************************************
 *
 * Show the About box.
 *
 ****************************************************************************/
void CIntegrityIOCtrl::SetLnAbout(long nNewValue) 
{
	/*
	 * Don't do anything
	 */
	CDialog dlgAbout(IDD_ABOUTBOX_INTEGRITYIO);
	dlgAbout.DoModal();
	//SetModifiedFlag();
}


/************************* GetLnLastError ***********************************
 *
 * Return the value of the last error that occured.
 *
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnLastError() 
{
	// TODO: Add your property handler here
	
	return GetLastError();
}

void CIntegrityIOCtrl::SetLnLastError(long nNewValue) 
{
	/*
	 * Don't do anything
	 */

	//SetModifiedFlag();
}

/************************* GetBDialogDisable *********************************
 *
 * Get the current state of the Disable Dialog/Message Boxes flag.
 *
 ****************************************************************************/
BOOL CIntegrityIOCtrl::GetBDialogDisable() 
{
	// TODO: Add your property handler here
	return m_bDialogsDisabled;
}

/************************* SetBDialogDisable *********************************
 *
 * Setting the m_bDialogsDisabled flag will disable all message boxes except
 * the about box. This means that there will be no visual indicators when 
 * somthing goes wrong.
 *
 ****************************************************************************/
void CIntegrityIOCtrl::SetBDialogDisable(BOOL bNewValue) 
{
	m_bDialogsDisabled= bNewValue;
	SetModifiedFlag();
}


/************************* GetLnSoftwareVersion ******************************
 *
 * Return the current software version (this).
 * 
 * Format:
 *	0xAAAABBBB
 * Where:
 *	A = major version
 *  B = minor version
 *
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnSoftwareVersion() 
{

	return IIVERSION;
}

void CIntegrityIOCtrl::SetLnSoftwareVersion(long nNewValue) 
{
	/*
	 * Do Nothign
	 */

	//SetModifiedFlag();
}

/************************* GetLnFirmwareVersion ******************************
 *
 * Return the current firmware version (module).
 * 
 * Format:
 *	0xAB
 * Where:
 *	A = major version
 *  B = minor version
 *
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnFirmwareVersion() 
{
	long version;
	
	
	version = 0;
	
	if(GetBPortOpen())
	{
		CString str = SendCommand("V");
		if(str.GetLength() > 0)
		{
			sscanf((const char*)(LPCTSTR)str, "V%d", &version);
		}
	}

	return version;
}

void CIntegrityIOCtrl::SetLnFirmwareVersion(long nNewValue) 
{
	/*
	 * Do nothing
	 */
	//SetModifiedFlag();
}

/************************* GetLnDigitalPort *********************************
 *
 * Read the current state of the digital port (module)
 * 
 * Value: 
 *		0xAABB
 * Where:
 *		AA = Port 1
 *		BB = Port 2
 *
 * Always returns 0 if the port is not open, or communication fails.
 *
 * Port 2 is not available on the 232M200
 *
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnDigitalPort() 
{
	long port;
	port = 0;
	if(GetBPortOpen())
	{
		CString str = SendCommand("I");
		TRACE("Digital Port Data... \n" );
		if(str.GetLength() > 0)
		{
			sscanf((const char*)(LPCTSTR) str, "I%X", &port);
			TRACE("\tReceived: %s\n", str);
		}
	}

	return port;
}

/************************* SetLnDigitalPort *********************************
 *
 * Set the current state of the digital port (module)
 * 
 * Value: 
 *		0xAABB
 * Where:
 *		AA = Port 1
 *		BB = Port 2
 *
 * Does nothing if the port is not open.
 *
 * Port 2 is not available on the 232M200
 *
 ****************************************************************************/
void CIntegrityIOCtrl::SetLnDigitalPort(long nNewValue) 
{
	CString str, rStr;
	str.Format("O%04X", nNewValue);
	if(GetBPortOpen())
	{
		rStr =  SendCommand(str);
		TRACE("Setting Digital Port Data...\n");
		TRACE("\tReceived: %s\n", rStr);
	}

	SetModifiedFlag();
}

/************************* GetLnDigitalDirection *****************************
 *
 * Set the current state of the digital port directions (module)
 * 
 * Value: 
 *		0xAABB
 * Where:
 *		AA = Port 1
 *		BB = Port 2
 *
 * Always returns 0 if the port is not open, or communication fails.
 *
 * Port 2 is not available on the 232M200
 *
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnDigitalDirection() 
{
	long pDir = 0;
	if(GetBPortOpen())
	{
		CString str = SendCommand("G");
		TRACE("Getting Digital Direction...\n");
		if(str.GetLength() > 0)
		{
			sscanf((const char*)(LPCTSTR)str, "G%X", &pDir);
			TRACE("\tReceived: %s\n", str);
		}
	}
	return pDir;
}

/************************* SetLnDigitalDirection *****************************
 *
 * Set the current state of the digital port direction (module)
 * 
 * Value: 
 *		0xAABB
 * Where:
 *		AA = Port 1
 *		BB = Port 2
 *
 * Does nothing if the port is not open.
 *
 * Port 2 is not available on the 232M200
 *
 ****************************************************************************/
void CIntegrityIOCtrl::SetLnDigitalDirection(long nNewValue) 
{
	CString str, rStr;
	if(GetBPortOpen())
	{
		str.Format("T%04X", nNewValue);
		rStr = SendCommand(str);
		TRACE ("Setting digital Direction\n");
	}
	SetModifiedFlag();
}

/************************* GetLnPulseCount *****************************
 *
 * Get the current value of the pulse counter(module)
 * 
 * returns a 32 bit value.
 * 
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnPulseCount() 
{
	long rVal = 0;	
	
	if(GetBPortOpen())
	{
		CString cmd = SendCommand("N");
		if(cmd.GetLength() > 0)
		{
			sscanf((const char*)(LPCTSTR)cmd, "N%X", &rVal);		
		}
	}
	return rVal;
}

/************************* SetLnPulseCount **********************************
 *
 * Any write to this method will cause the pulse counter to be reset.
 * 
 ****************************************************************************/
void CIntegrityIOCtrl::SetLnPulseCount(long nNewValue) 
{
	if(GetBPortOpen())
	{
		SendCommand("M");
	}
	SetModifiedFlag();
}

/************************* GetLnADCControl **********************************
 *
 * Get the current ADC control nibble setting.
 * 
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnADCControl() 
{
	
	return m_nADCControl;
}

/************************* SetLnADCControl **********************************
 *
 * Set the current ADC control nibble setting.
 * Values:
 *		0 - Diffarential: CH0+ CH1-
 *		1 - Diffarential: CH2+ CH3-
 *		2 - Diffarential: CH4+ CH5-
 *		3 - Diffarential: CH6+ CH7-
 *		4 - Diffarential: CH0- CH1+
 *		5 - Diffarential: CH2- CH3+
 *		6 - Diffarential: CH4- CH5+
 *		7 - Diffarential: CH6- CH7+
 *
 *		8 - Single Point: CH0
 *		9 - Single Point: CH2
 *		A - Single Point: CH4
 *		B - Single Point: CH6
 *		C - Single Point: CH1
 *		D - Single Point: CH3
 *		E - Single Point: CH5
 *		F - Single Point: CH7
 * 
 ****************************************************************************/
void CIntegrityIOCtrl::SetLnADCControl(long nNewValue) 
{
	if( (nNewValue < 0) || (nNewValue > 15))
	{
		if(m_bDialogsDisabled)
		{
			AfxMessageBox("Invalid ADC Control value!");
		}
		return;
	}
	m_nADCControl = (short)nNewValue;
	SetModifiedFlag();
}


/************************* GetDADCBipolar ***********************************
 *
 * Perform a Bipolar ADC sample and return the value as a voltage 
 * to the calling function. 
 *
 * Results are based on the current control nibble setting as well
 * as the current refrence voltage setting
 * 
 ****************************************************************************/
double CIntegrityIOCtrl::GetDADCBipolar() 
{
	CString cmd, str;
	long val;
	double dVal, dOffset;
	double rVal = 0.0;
	if(GetBPortOpen())
	{
		switch(m_lnProtocol)
		{
		case 1:			// 232
		case 2:			// 485
			cmd.Format("Q%01X", m_nADCControl);
			str = SendCommand(cmd);
			if(str.GetLength() > 1)
			{
				// Create a protoype string for the sscanf function
				cmd.Format("Q%01X%%X", m_nADCControl);

				// Get the returned value
				sscanf((const char*)(LPCTSTR)str, (const char*)(LPCTSTR)cmd, &val);
				dVal = (double)val;
				dOffset = (double)m_lnOffsetCal;
				// Convert to the floating point representation
				rVal = (dVal * (m_dReferenceVoltage/2048.0) + dOffset);
			}
			break;
		case 3:		// USB
			if(!m_bDialogsDisabled)
			{
				AfxMessageBox("Q Command not supported in USB unit");
			}
			break;
		}
	}
	return rVal;
}

void CIntegrityIOCtrl::SetDADCBipolar(double newValue) 
{
	//SetModifiedFlag();
}


/************************* GetDADCBipolar ***********************************
 *
 * Perform a Bipolar ADC sample and return the value as a voltage 
 * to the calling function. 
 *
 * Results are based on the current control nibble setting as well
 * as the current refrence voltage setting
 * 
 ****************************************************************************/
double CIntegrityIOCtrl::GetDADCUnipolar() 
{
	CString str, cmd;
	long val;
	short sVal;
	unsigned short uVal;
	double dVal;
	double rVal = 0.0;
	double dBitCoeff;
	
	if(GetBPortOpen())
	{
		switch(m_lnProtocol)
		{
		case 1:
		case 2:	
			dBitCoeff = pow(2,m_nADCBits)-1;
			cmd.Format("U%01X", m_nADCControl);
			break;
		case 3:
			dBitCoeff = (pow(2,m_nADCBits)/2)-1;
			cmd.Format("U%01X%01X", m_lnAnalogGain, m_nADCControl);
			break;
		}
		str = SendCommand(cmd);
		if(str.GetLength() > 1)
		{
			// Create a protoype string for the sscanf function
			switch(m_lnProtocol)
			{
			case 1:
			case 2:
				cmd.Format("U%01X%%X", m_nADCControl);
				break;
			case 3:
				cmd.Format("U%01X%01X%%X", m_lnAnalogGain, m_nADCControl);
				break;
			}

			val = 0;
			// Get the returned value
			sscanf((const char*)(LPCTSTR)str, (const char*)(LPCTSTR)cmd, &val);
			
			// Convert to the floating point representation

			if(m_lnProtocol == 3)
			{
				sVal = (short)(((val << m_nAdcTwosCompShift) ^ 0xFFFF) + 1) * (-1);
				dVal = (double)(sVal >> m_nAdcTwosCompShift);
				rVal = ( dVal * (m_dReferenceVoltage/dBitCoeff) );
			}
			else
			{
				//uVal = (short)(((val << m_nAdcTwosCompShift) ^ 0xFFFF));
				//dVal = (double)(uVal >> m_nAdcTwosCompShift);
				
				rVal = ( (double)val * (m_dReferenceVoltage / dBitCoeff) );
			}
			rVal *= m_dADCScale;
		}
	}
	return rVal;
}

void CIntegrityIOCtrl::SetDADCUnipolar(double newValue) 
{
	//SetModifiedFlag();
}


/************************* GetLnDACControl **********************************
 *
 * Return the current value of the DAC control nibble
 * 
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnDACControl() 
{
	// TODO: Add your property handler here

	return (long)m_nDACControl;
}

/************************* GetLnDACControl **********************************
 *
 * Set the current value of the DAC control nibble
 *
 * Values:
 *		0 - Channel 0 output
 *		1 - Channel 1 output
 *
 * Function ignores any unknown DAC channel values
 * 
 ****************************************************************************/
void CIntegrityIOCtrl::SetLnDACControl(long nNewValue) 
{
	
	if(nNewValue == 0 || nNewValue == 1)
	{
		m_nDACControl = (short)nNewValue;
	}

	SetModifiedFlag();
}

double CIntegrityIOCtrl::GetDDACOutput() 
{
	return 0.0;
}

/************************* GetLnDACControl **********************************
 *
 * Set the current value of the DAC output
 *
 * Valid range = 0 to m_dRefrenceVoltage
 * 
 ****************************************************************************/
void CIntegrityIOCtrl::SetDDACOutput(double newValue) 
{
	
	long output;
	CString cmd;

	if(GetBPortOpen())
	{
		output = (long)((newValue / m_dDACReferenceVoltage) * 4095);
		cmd.Format("L%d%03X", m_nDACControl, output);
		SendCommand(cmd);
	}
	SetModifiedFlag();
}

/************************* GetLnRcvErrCount *********************************
 *
 * Get the current receive error count from a module
 *
 * 
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnRcvErrCount() 
{
	CString str;
	long rVal = 0;
	if(GetBPortOpen())
	{
		switch(m_lnProtocol)
		{
		case 1:
		case 2:
			str = SendCommand("K");
			if(str.GetLength() > 0)
			{
				sscanf((const char*)(LPCTSTR)str, "K%X", &rVal);
			}
			break;
		case 3:
			if(!m_bDialogsDisabled)
			{
				AfxMessageBox("K Command not supported by USB unit");
			}
			break;
		}
	}
	return rVal;
}

/************************* SetLnRcvErrCount *********************************
 *
 * Any attempt to write to the receive error count variable will
 * result in a reset command being sent to a module
 * 
 ****************************************************************************/
void CIntegrityIOCtrl::SetLnRcvErrCount(long nNewValue) 
{
	if(GetBPortOpen())
	{
		switch(m_lnProtocol)
		{
		case 1:
		case 2:
			SendCommand("J");
			break;
		case 3:
			if(!m_bDialogsDisabled)
			{
				AfxMessageBox("J Command not supported by USB Unit");
			}
		}
	}
	//SetModifiedFlag();
}

/************************* GetLnPWMFreq *************************************
 *
 * Return the current PWM frequency value
 * 
 * 
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnPWMFreq() 
{
	// TODO: Add your property handler here

	return m_lnPWMFreq;
}

/************************* SetLnPWMFreq *************************************
 *
 * Return the current PWM frequency value
 * 
 * 
 ****************************************************************************/
void CIntegrityIOCtrl::SetLnPWMFreq(long nNewValue) 
{
	m_lnPWMFreq = nNewValue;
	SetModifiedFlag();
}

/************************* GetLnPWMFreq *************************************
 *
 * Return the current PWM duty cycle
 * 
 * 
 ****************************************************************************/
long CIntegrityIOCtrl::GetLnPWMDuty() 
{
	return m_lnPWMDuty;
}

void CIntegrityIOCtrl::SetLnPWMDuty(long nNewValue) 
{
	
	m_lnPWMDuty = nNewValue;

	SetModifiedFlag();
}

BOOL CIntegrityIOCtrl::GetBPWMUpdate() 
{
	// TODO: Add your property handler here

	return FALSE;
}

void CIntegrityIOCtrl::SetBPWMUpdate(BOOL bNewValue) 
{
	// TODO: Add your property handler here

	//SetModifiedFlag();
	CString cmd;
	if(GetBPortOpen())
	{
		switch(m_lnProtocol)
		{
		case 1:
		case 2:
			cmd.Format("P%02X%03X", m_lnPWMFreq, m_lnPWMDuty);
			SendCommand(cmd);
			break;
		case 3:
			if(!m_bDialogsDisabled)
			{
				AfxMessageBox("P Command not Supported by USB unit.");
			}
			break;
		}
	}
	SetModifiedFlag();
}

long CIntegrityIOCtrl::GetLnEEPROMAddress() 
{
	// TODO: Add your property handler here

	return (long)m_nEEAddress;
}

void CIntegrityIOCtrl::SetLnEEPROMAddress(long nNewValue) 
{
	// TODO: Add your property handler here

	SetModifiedFlag();
	m_nEEAddress = (short)nNewValue;

}

long CIntegrityIOCtrl::GetLnEEPROMData() 
{
	// TODO: Add your property handler here
	CString cmd, str;
	long rVal;
	rVal = -1;
	if(GetBPortOpen())
	{
		cmd.Format("R%02X", m_nEEAddress);

		str = SendCommand(cmd);
		if(str.GetLength() > 1)
		{
			sscanf((const char*)(LPCTSTR)str, "R%X", &rVal);
		}
	}
	return rVal;
}

void CIntegrityIOCtrl::SetLnEEPROMData(long nNewValue) 
{
	CString cmd;
	if(GetBPortOpen())
	{
		cmd.Format("W%02X%02X", m_nEEAddress, nNewValue);
		SendCommand(cmd);
	}
	SetModifiedFlag();
}

LONG CIntegrityIOCtrl::GetlnAnalogGain(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return m_lnAnalogGain;
}

void CIntegrityIOCtrl::SetlnAnalogGain(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
		if( ((newVal < 0) || (newVal > 7)) && !m_bDialogsDisabled )
	{
		AfxMessageBox("Invalid Analog Gain value");
		return;
	}
	m_lnAnalogGain = newVal;

	SetModifiedFlag();
}


BOOL CIntegrityIOCtrl::GetBResetModule() 
{
	// TODO: Add your property handler here

	return FALSE;
}

void CIntegrityIOCtrl::SetBResetModule(BOOL bNewValue) 
{
	// TODO: Add your property handler here

	//SetModifiedFlag();
	if(GetBPortOpen())
	{
		switch(m_lnProtocol)
		{
		case 1:
		case 2:
			SendCommand("Z");
			break;
		case 3:
			if(!m_bDialogsDisabled)
			{
				AfxMessageBox("Z Command not available in USB unit!");
			}
			break;
		}
	}
	SetModifiedFlag();
}
/******************************************************************************
 * bModeStream - Returns the status of the stream mode
 ******************************************************************************/
BOOL CIntegrityIOCtrl::GetbModeStream(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return m_bModeStream;
}

/******************************************************************************
 * bModeStream - Enable/Disable Stream mode
 ******************************************************************************/
void CIntegrityIOCtrl::SetbModeStream(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DWORD exitCode;
	// TODO: Add your property handler code here

	switch(newVal)
	{
	case TRUE:
		switch(m_lnProtocol)
		{
		case 1:
			// Start the RS-232 Stream mode thread
			break;
		case 3:
			// Start the USB stream mode thread
			if(!m_bModeStream)
				m_ceHaltThread.ResetEvent();
				m_pStreamThread =AfxBeginThread(usbThreadProc, this);
			break;
		}
		m_bModeStream = TRUE;
		break;
	case FALSE:
		switch(m_lnProtocol)
		{
		case 1:
			// Terminate RS-232 stream mode thread
			break;
		case 3:
			// Terminate the USB Stream Mode thread
			m_ceHaltThread.SetEvent();
			while(TRUE)
			{
				GetExitCodeThread(m_pStreamThread->m_hThread, &exitCode);
				if(exitCode != STILL_ACTIVE) break;
			}
			m_ceHaltThread.ResetEvent();
			break;
		}
		m_bModeStream = FALSE;
		break;
	default:
		if(!m_bDialogsDisabled)
		{
			AfxMessageBox("Invalid Flag Value:\nPlease use TRUE / FALSE values.");
		}
		break;
	}
	

	SetModifiedFlag();
}

BOOL CIntegrityIOCtrl::OpenComm()
{
	CString commPort;

	DCB dcb;

	commPort.Format("\\\\.\\COM%d", m_lnComport);

	// Make Sure that the port is closed
	CloseComm();

	// Get a connection to the serial port driver
	m_hComm = CreateFile((LPCTSTR)commPort,			// Commport Name
				GENERIC_READ | GENERIC_WRITE,		// Read/Write access
				0,									// Non sharing
				NULL,								// Default securit
				OPEN_EXISTING,						// The port should exist
				FILE_ATTRIBUTE_NORMAL,				// Non Overlapped
				NULL);								// No template

	if ( ((INVALID_HANDLE_VALUE == m_hComm) || (NULL == m_hComm)) && !m_bDialogsDisabled)
	{
		AfxMessageBox("Error Opening Communications");
		return FALSE;
	}
	
	// Create a device control block
	dcb.DCBlength = sizeof( DCB );
	GetCommState(m_hComm, &dcb);
	switch(m_lnBaudRate)
	{
	case 9600:
		dcb.BaudRate = CBR_9600;
		break;
	case 19200:
		dcb.BaudRate = CBR_19200;
		break;
	case 57600:
		dcb.BaudRate = CBR_57600;
		break;
	case 115200:
		dcb.BaudRate = CBR_115200;
		break;
	default:
		dcb.BaudRate = CBR_115200;
		break;
	}
	
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.ByteSize = 8;

	if(!SetCommState(m_hComm, &dcb))
	{
		CloseHandle(m_hComm);
		TRACE("Error setting up commport!\n");
		if(!m_bDialogsDisabled)
		{
			AfxMessageBox("Error setting up communication parameters");
		}
		return FALSE;
	}

	PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
	return TRUE;
}

void CIntegrityIOCtrl::CloseComm()
{
	if( (m_hComm != NULL) && (m_hComm != INVALID_HANDLE_VALUE) )
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
}

/*********************** SendCommand ********************************
 * 
 * Input:
 *	String structured as the RS-232 command
 *
 * Output:
 *	String from unit structured as an RS-232 command
 *
 * Description:
 *	This function prepends the RS-484 control to the RS-232 style command.
 *  This function also appends a CR character for the calling function
 *  Function scans for the CR (0x0D) character.
 *  Function may timeout before receiving a response
 *  An empty string is returned on failure
 *
 * Note:
 *    
 *	This function may produce unexpected results while in 
 *  continuous stream mode.
 *
 ********************************************************************/
CString CIntegrityIOCtrl::SendCommand(LPCTSTR cmd)
{
	CString str;
	DWORD dwBytesWritten;
	DWORD dwResBytes;
	unsigned char buf[64];
	unsigned short ptr;
	char sResp[MAX_CMD_RESP];

	switch(m_lnProtocol)
	{
	case 1:			// RS-232 Protocol
		// Turn on the TX led
		SetTxOn();

		EmptyRxBuffer();		// Clear the receive buffer

		// Send the command out, append a CR
		str = cmd;
		WriteFile(m_hComm, (LPCTSTR)str, str.GetLength(), &dwBytesWritten, NULL);
		WriteFile(m_hComm, (LPCTSTR)"\r", 1, &dwBytesWritten, NULL);
		
		// Turn off the TX led
		SetTxOff();
		
		// Wait for the response from the unit
		dwResBytes = WaitForResponse(buf, sizeof(buf));

		if(dwResBytes > 0)
		{
			ptr = (unsigned short)dwResBytes;
			// Command recieved success
			buf[ptr] = '\0';	// null terminate
			str = buf;			// return to caller
		}
		else
		{
			// Command Failed
			str = "";
		}
		break;

	case 2:			// RS-485 Protocol

		// Turn on the TX led
		SetTxOn();
		EmptyRxBuffer();		// Clear the receive buffer
		
		// Format the RS-485 Command string, don't forget the CR
		str.Format("%02X%02X%s\r",m_lnModuleAddress, 0, cmd);
		WriteFile(m_hComm, (LPCTSTR)str, str.GetLength(), &dwBytesWritten, NULL);
		
		// Turn off the TX led
		SetTxOff();
		
		// Wait for the response from the unit
		dwResBytes = WaitForResponse(buf, sizeof(buf));

		if(dwResBytes > 0)
		{
			ptr = (unsigned short)dwResBytes;
			// Command recieved success
			buf[ptr] = '\0';	// null terminate

			// Strip the RS-485 Protocol Data off.
			str.Format("%s", &buf[4]);
		}
		else
		{
			// Command Failed
			str = "";
		}

		break;
	case 3:			// USB Protocol
		// Turn on the TX led
		SetTxOn();

		// Send the command out, append a CR
		str.Format("%s",cmd);
		LPSTR sCmd = str.GetBuffer(MAX_CMD_RESP);
		int devIndex = GetIndexFromAddress();
		
		//if(!m_pUSB->SendCommand(GetIndexFromAddress(), str.GetBuffer(MAX_CMD_RESP), sResp))
		if(!m_pUSB.SendCommand(devIndex, sCmd, sResp))
		{
			str = "Error";
		}
		else
		{
			str.Format("%s", sResp);
		}


		str.ReleaseBuffer();
		// Turn off the TX led
		SetTxOff();
		break;	
	}
	return str;
}

DWORD CIntegrityIOCtrl::WaitForResponse(unsigned char *buffer, unsigned short bufSize)
{
	unsigned short ptr;
	BOOL done;
	DWORD dwRVal;
	DWORD dwErrors;
	DWORD tickCount, dwBytesRead;
	COMSTAT comStat;

	// Loop until a packet is recieved or
	// we run out of time
	ptr = 0;
	done = FALSE;
	tickCount = GetTickCount();
	while((GetTickCount() < (tickCount + m_lnComTimeout)) && !done && ptr < bufSize)
	{
		ClearCommError(m_hComm, &dwErrors, &comStat);
		if(comStat.cbInQue > 0)
		{
			// Only turn on the Receive LED once
			if(ptr == 0)
			{
				SetRxOn();
			}

			// Get a character from the buffer
			ReadFile(m_hComm, &buffer[ptr], 1, &dwBytesRead, NULL);

			// EOF ?? 
			if(buffer[ptr] == '\r')
			{
				// Yes, escape loop
				done = TRUE;
			}
			else
			{
				// No, continue Scan
				ptr++;
			}
		}
	}
	// Turn off the receive LED
	SetRxOff();


	dwRVal = 0;
	if(done)
	{
		dwRVal = (DWORD)ptr;
	}

	return dwRVal;
}



/************************ SetTxOn **********************************
 * Set the Draw State to show that the host is transmitting to a 
 * module.
 *******************************************************************/
void CIntegrityIOCtrl::SetTxOn()
{

	// Set Draw State
	m_nDrawState = PORT_OPENTX;

	// Wait for OnDraw
	WaitForDraw();

}
/************************ SetTxOff() *******************************
 * Set the Draw State to show that data transmittion is complete
 *******************************************************************/
void CIntegrityIOCtrl::SetTxOff()
{

	// Set Draw State
	m_nDrawState = PORT_OPENIDLE;


	// Wait for OnDraw
	WaitForDraw();
	
}
/************************ SetTxOn **********************************
 * Set the Draw State to show that the host is receiving data from a
 * module.
 *******************************************************************/
void CIntegrityIOCtrl::SetRxOn()
{

	// Set Draw State
	m_nDrawState = PORT_OPENRX;

	// Wait for OnDraw
	WaitForDraw();

}

/************************ SetTxOff() *******************************
 * Set the Draw State to show that data reception is complete.
 *******************************************************************/
void CIntegrityIOCtrl::SetRxOff()
{
	// Set Draw State
	m_nDrawState = PORT_OPENIDLE;

	// Wait for OnDraw
	WaitForDraw();

}

/******************** Wait For Draw *************************
 * Function forces a redraw using the current device context
 * The m_nDrawState variable must be set prior to calling
 * this function. 
 ************************************************************/
void CIntegrityIOCtrl::WaitForDraw()
{
	if(m_bInvalidate)
	{
		CRect bounds, invalid;

		// Get the current device contect
		CDC* pDC = GetDC();
		
		// Get the bounding rectangles
		pDC->GetBoundsRect(&bounds, 0);
		invalid = bounds;

		
		TRACE("Waiting for OnDraw\n");

		// Draw the new view
		OnDraw(pDC, bounds, invalid);

		TRACE("OnDraw Complete\n");
	}
	
}

/******************** SetOffsetCalibration ******************
 *
 * Set the offset calibration coefficient based on the 
 * factory default.
 *
 ************************************************************/
void CIntegrityIOCtrl::SetOffsetCalibration()
{
	CString str, cmd;
	long offsVal;
	// Get the offset calibration value.
	if(GetBPortOpen())
	{
		// make sure that the port is open
		cmd.Format("R%d", OFFSET_ADDRESS);
		str = SendCommand(cmd);
		if(str.GetLength() > 0)
		{
			sscanf((const char*)(LPCTSTR)str, "R%X", &offsVal);
			if(offsVal >= 128)
			{
				m_lnOffsetCal = (long)((offsVal - 256) * (m_dReferenceVoltage / 2048.0));
			}
			else
			{
				m_lnOffsetCal = (long)(offsVal * (m_dReferenceVoltage / 2048.0));
			}
		}
	}

}

void CIntegrityIOCtrl::EmptyRxBuffer()
{
	DWORD dwBytesRead;
	DWORD dwErrors;
	COMSTAT comStat;
	char *dat;

	ClearCommError(m_hComm, &dwErrors, &comStat);

	if(comStat.cbInQue > 0)
	{
		dat = new char[comStat.cbInQue];
		ReadFile(m_hComm, dat, comStat.cbInQue, &dwBytesRead, NULL);
		delete[] dat;
	}
}


/****************************************************************
 * SetDIOBit
 *
 *	bitValue: 1 or 0
 *  bitPosition: 0 ... 15
 ****************************************************************/
void CIntegrityIOCtrl::SetDIOBit(long bitValue, long bitPosition) 
{
	long bitVal;
	bitVal = GetLnDigitalPort() & ~(1 << bitPosition);
	bitVal |= bitValue << bitPosition;
	SetLnDigitalPort(bitVal);

}

long CIntegrityIOCtrl::GetDIOBit(long bitPosition) 
{
	return (GetLnDigitalPort() >> bitPosition) & 1;
}

void CIntegrityIOCtrl::SetDIOBitDDR(long bitValue, long bitPosition) 
{
	long ddr;
	ddr = GetLnDigitalDirection() & ~(1 << bitPosition);
	ddr |= bitValue << bitPosition;
	SetLnDigitalDirection(ddr);
}

long CIntegrityIOCtrl::GetDIOBitDDR(short bitPosition) 
{
	return (GetLnDigitalDirection() >> bitPosition) & 1;
}




int CIntegrityIOCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_cDOffBM.LoadBitmap(IDB_INTEGRITYDISPOFF);
	m_cDOnBM.LoadBitmap(IDB_INTEGRITYDISPON);
	m_cTXBM.LoadBitmap(IDB_INTEGRITYDISPTX);
	m_cRXBM.LoadBitmap(IDB_INTEGRITYDISPRX);
	
	return 0;
}

LONG CIntegrityIOCtrl::GetIndexFromAddress()
{
	LONG i=0, rValue=-1;
	
	for(i=0; i<(LONG)m_pUSB.m_dwUSBM300DeviceCount; i++)
	{
		if(m_lnModuleAddress == m_pUSB.GetModuleAddress(i)) 
		{
			rValue = i;
			break;		
		}
	}

	return rValue;
}

void CIntegrityIOCtrl::OnbInvalidateChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here

	SetModifiedFlag();
}



DOUBLE CIntegrityIOCtrl::GetdDACReferenceVoltage(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return m_dDACReferenceVoltage;
}

void CIntegrityIOCtrl::SetdDACReferenceVoltage(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	m_dDACReferenceVoltage = newVal;
	SetModifiedFlag();
}


LONG CIntegrityIOCtrl::GetUSBModuleAddress(LONG moduleIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return m_pUSB.GetModuleAddress(moduleIndex);
}

LONG CIntegrityIOCtrl::GetlnUSBStreamBlockCount(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return m_lnUSBStreamBlockCount;
}

void CIntegrityIOCtrl::SetlnUSBStreamBlockCount(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here

	SetModifiedFlag();
}

// --
// -- Function used to retrieve one block of stream data
// -- Each block consists of 32 double values.
// -- On exit, the current block (top of fifo) will be removed 
// -- from memory, count of the remaining blocks will be returned
// --
LONG CIntegrityIOCtrl::GetUSBDataBlock(DOUBLE* adcData, LONG startIndex, LONG count)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	long index;
	long templateCount;
	CUSBStreamdata data;
	templateCount = m_cUSBStreamData.GetCount();

	if(templateCount > 0)
	{
		// Retrieve the data from the collection
		data = m_cUSBStreamData[0];

		// Transfer the data from our collection object
		// to the array passed in by the main process
		for(index = startIndex; index < count; index++)
		{
			adcData[index] = data.GetValue(index);
		}

 		// Remove the item from our fifo
		m_cUSBStreamData.RemoveAt(0);

		// Decrement the count, one item has been removed
		templateCount--;
	}

	// Return number of items in our collection
	return templateCount;
}


//////////////////////////////////////////////////////////////////////////////
// Thread Procedures for handling stream mode communication
//////////////////////////////////////////////////////////////////////////////
UINT CIntegrityIOCtrl::usbThreadProc(LPVOID pParam)
{
    int         DeviceIndex;
    DWORD       dwThreadState, dwStreamBlocksAvailable;
    ULONG       ulBlockCount = 0;
    USBM300_STREAM_BLOCK  sStreamData;
	BOOL		bStatus;
	DWORD		dwBlocksReturned;
	double		refrence;

	CUSBStreamdata usbData;
	int index;

	// A pointer to this object will be passed as a parameter
	// to the create thread API call
	CIntegrityIOCtrl *pThis = (CIntegrityIOCtrl*)pParam;

	// Reset the total block counter
	pThis->m_lnUSBStreamBlockCount = 0;

	// Setup the USB Stream Mode
	DeviceIndex = pThis->GetIndexFromAddress();

	// Check the thread status, error out if status cannot be determined
	if(pThis->m_pUSB.GetStreamingStatus(DeviceIndex, &dwStreamBlocksAvailable, &dwThreadState))
	{
		if(dwThreadState & USB_THREAD_RUNNING)
		{
			// Thread is already running
		}
		else
		{
			// The thread is not running, start One
			pThis->m_pUSB.ControlStreaming(DeviceIndex, TRUE, 512);
		}
	}
	else
	{
		// Cannot determine USB stream thread status
		ExitThread(-1);
	}

	
	
	// Create the coefficient for calculating
	// the actual voltage in advance. This significantly
	// improves the performace of the conversion algorithm.
	refrence = pThis->m_dReferenceVoltage/8191.0;

	// Throw away any pre existing data.
	pThis->m_cUSBStreamData.RemoveAll();

	
	// Loop while waiting for process owner to signal the end of streaming
	while(WAIT_OBJECT_0 != ::WaitForSingleObject(pThis->m_ceHaltThread, 0))
	{
		
		// Check stream status
		bStatus = pThis->m_pUSB.GetStreamingData(DeviceIndex, TRUE,
						&sStreamData, 1, &dwBlocksReturned);

		// There is data to be retrieved.
		if(bStatus)
		{
			pThis->m_lnUSBStreamBlockCount++;
			for(index = 0; index < ADC_SAMPLE_COUNT; index++)
			{
				usbData.SetValue(sStreamData.usAdcSamples[index], refrence, index);
			}
			pThis->m_cUSBStreamData.Add(usbData);

			if(pThis->m_cUSBStreamData.GetCount() > 1024) 
			{
				pThis->m_cUSBStreamData.RemoveAt(0);
				TRACE("Removed 1 item\r\n");
			}
		}

	}

	// Terminate the stream mode
	if(pThis->m_pUSB.GetStreamingStatus(DeviceIndex, &dwStreamBlocksAvailable, &dwThreadState))
	{
		if(dwThreadState & USB_THREAD_RUNNING)
		{
			pThis->m_pUSB.ControlStreaming(DeviceIndex, FALSE, 512);
		}
	}


	ExitThread(0);
	return 0;
}



SHORT CIntegrityIOCtrl::GetnADCBits(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	
	return m_nADCBits;
}

void CIntegrityIOCtrl::SetnADCBits(SHORT newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	m_nADCBits = newVal;
	m_nAdcTwosCompShift = 16 - m_nADCBits;			// Used to determine the
													// Shift coefficient for the 2's 
													// complement conversion
	SetModifiedFlag();
}

DOUBLE CIntegrityIOCtrl::GetdADCScale(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here

	return m_dADCScale;
}

void CIntegrityIOCtrl::SetdADCScale(DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here
	m_dADCScale = newVal;
	SetModifiedFlag();
}
