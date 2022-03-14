// Connection.h: interface for the CConnection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONNECTION_H__75E0CF60_2D18_4E13_880E_018C59EEB125__INCLUDED_)
#define AFX_CONNECTION_H__75E0CF60_2D18_4E13_880E_018C59EEB125__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConnection : public CObject  
{
public:
	void StoreSettings();
	void LoadSettings();
	COLORREF m_crTx;
	COLORREF m_crRx;
	DWORD m_nCommport;
	DWORD m_nBaudRate;
	DWORD m_nDataBits;
	DWORD m_nParity;
	DWORD m_nStopBits;
	BOOL  m_bRTSOn;
	BOOL  m_bDTROn;
	BOOL  m_bAppendLF;
	BOOL  m_bLocalEcho;
	BOOL  m_bHexMode;

	void Serialize(CArchive& ar);
	CConnection();
	virtual ~CConnection();
	DECLARE_SERIAL(CConnection)
};

#endif // !defined(AFX_CONNECTION_H__75E0CF60_2D18_4E13_880E_018C59EEB125__INCLUDED_)
