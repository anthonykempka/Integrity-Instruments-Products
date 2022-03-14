// Connection.cpp: implementation of the CConnection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EZTerm.h"
#include "Connection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CConnection, CObject, VERSIONABLE_SCHEMA | 2 )

CConnection::CConnection()
{
	// Load the settings from the registry
	LoadSettings();
}

CConnection::~CConnection()
{
	StoreSettings();	
}

void CConnection::Serialize(CArchive &ar)
{
	TRACE("Entering CConnection::Serialize\n");

	if(ar.IsStoring())
	{
		ar << m_nCommport;
		ar << m_nBaudRate;
		ar << m_nDataBits;
		ar << m_nParity;
		ar << m_nStopBits;
		ar << (BYTE)m_bRTSOn;
		ar << (BYTE)m_bDTROn;
		ar << (BYTE)m_bAppendLF;
		ar << (BYTE)m_bLocalEcho;
		ar << (BYTE)m_bHexMode;
		ar << (DWORD)m_crTx;
		ar << (DWORD)m_crRx;
	}
	else
	{
		ar >> m_nCommport;
		ar >> m_nBaudRate;
		ar >> m_nDataBits;
		ar >> m_nParity;
		ar >> m_nStopBits;
		ar >> (BYTE&)m_bRTSOn;
		ar >> (BYTE&)m_bDTROn;
		ar >> (BYTE&)m_bAppendLF;
		ar >> (BYTE&)m_bLocalEcho;
		ar >> (BYTE&)m_bHexMode;
		ar >> (DWORD&)m_crTx;
		ar >> (DWORD&)m_crRx;
	}
}

void CConnection::LoadSettings()
{
	//Read the Communications Settings registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);

    // Default 1 (COM2) if no setting found
	m_nCommport = pApp->GetProfileInt("Settings", "ComPort", 1);

    // Default 0 (300) if not setting found
	m_nBaudRate = pApp->GetProfileInt("Settings", "BaudRate", 0);

    // Default 0 (8 data bits) if no setting found
    m_nDataBits = pApp->GetProfileInt("Settings", "DataBits", 0);

    // Default 0 (NO PARITY) if no setting found
    m_nParity = pApp->GetProfileInt("Settings", "Parity", 0);

    // Default 0 (1 stop bit) if no setting found
    m_nStopBits = pApp->GetProfileInt("Settings", "StopBits", 0);

    // Default 0 (RTS OFF) if no setting found
	m_bRTSOn = pApp->GetProfileInt("Settings", "Force_RTS_On", 0);

    // Default 1 (DTR ON) if no setting found
	m_bDTROn = pApp->GetProfileInt("Settings", "Force_DTR_On", 1);

    // Default 1 (Append Line Feed) if no setting found
	m_bAppendLF = pApp->GetProfileInt("Settings", "Append_LF", 1);

    // Default 1 (Append Line Feed) if no setting found
	m_bLocalEcho = pApp->GetProfileInt("Settings", "Local_Echo", 1);

    // Default 0 (Not HEX mode) if no setting found
	m_bHexMode = pApp->GetProfileInt("Settings", "Hex_Mode", 0);

	// Default Transmit Color
	m_crTx = (COLORREF)pApp->GetProfileInt("Settings", "Tx_Color", RGB(217,0,0));
	m_crRx = (COLORREF)pApp->GetProfileInt("Settings", "Rx_Color", RGB(0,140,0));

}

void CConnection::StoreSettings()
{
	//Update the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);

    
	// Communication Parameters
	pApp->WriteProfileInt("Settings", "ComPort", m_nCommport);
    
	pApp->WriteProfileInt("Settings", "BaudRate", m_nBaudRate);

	pApp->WriteProfileInt("Settings", "DataBits", m_nDataBits);

	pApp->WriteProfileInt("Settings", "Parity", m_nParity);

	pApp->WriteProfileInt("Settings", "StopBits", m_nStopBits);

	
	// Handshaking lines
	pApp->WriteProfileInt("Settings", "Force_RTS_On", m_bRTSOn);
	pApp->WriteProfileInt("Settings", "Force_DTR_On", m_bDTROn);

	// Terminal Emulation
	pApp->WriteProfileInt("Settings", "Append_LF", m_bAppendLF);
	pApp->WriteProfileInt("Settings", "Local_Echo", m_bLocalEcho);
    pApp->WriteProfileInt("Settings", "Hex_Mode", m_bHexMode);
	
	// Set Color selections.
	pApp->WriteProfileInt("Settings", "Tx_Color", (UINT)m_crTx);
	pApp->WriteProfileInt("Settings", "Rx_Color", (UINT)m_crRx);
}
