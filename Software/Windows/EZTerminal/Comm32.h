// Comm32.h: interface for the CComm32 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMM32_H__DDFD0CA4_DCBC_11D3_B53B_00105A010632__INCLUDED_)
#define AFX_COMM32_H__DDFD0CA4_DCBC_11D3_B53B_00105A010632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CComm32  
{
public:
	BOOL getOpenStatus();
	HANDLE getCommHandle();
	BOOL Get_Comport_Signals(DWORD *CTS_On, DWORD *DSR_On, DWORD *CD_On, DWORD *RI_On);
	BOOL Set_Comport_Signals (BOOL RTS_On, BOOL DTR_On);
	BOOL Read_Comport (DWORD *BytesRead, DWORD Length, char *Buffer);
	BOOL Write_Comport(DWORD Length, char *Buffer);
	void Close_Comport ();
	BOOL Open_Comport(DWORD iComport, DWORD iBaudRate, DWORD iDataBits, DWORD iStopBits, DWORD iParity);
	CComm32();
	virtual ~CComm32();

	DWORD   DCB_Baud_Rate;
	CString Com_Name;

protected:
	HANDLE hDriver_Handle;
	BOOL bComport_Open;
};

#endif // !defined(AFX_COMM32_H__DDFD0CA4_DCBC_11D3_B53B_00105A010632__INCLUDED_)
