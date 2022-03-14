#pragma once
#include "afxcoll.h"



// CUsbStreamData command target

class CUsbStreamData : public CObject
{
	DECLARE_SERIAL(CUsbStreamData)
public:
	CUsbStreamData();
	virtual ~CUsbStreamData();
	
public:
	CString m_csName;
	DWORD m_dwValue;
public:
	virtual void Serialize(CArchive& ar);
};


