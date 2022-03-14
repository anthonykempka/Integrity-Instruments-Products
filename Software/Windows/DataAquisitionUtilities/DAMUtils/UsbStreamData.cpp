// UsbStreamData.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "UsbStreamData.h"
#include ".\usbstreamdata.h"


// CUsbStreamData
// CDataUsbM400
IMPLEMENT_SERIAL( CUsbStreamData, CObject, 1 )

CUsbStreamData::CUsbStreamData()
: m_csName(_T(""))
, m_dwValue(0)
{
}

CUsbStreamData::~CUsbStreamData()
{
}


// CUsbStreamData member functions

void CUsbStreamData::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << this->m_csName;
		ar << this->m_dwValue;
	}
	else
	{	// loading code
		ar >> this->m_csName;
		ar >> this->m_dwValue;
	}
}


