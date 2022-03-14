// DaqData.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "DaqData.h"
#include ".\daqdata.h"


// CDaqData
IMPLEMENT_SERIAL(CDaqData, CObject, 1 )

CDaqData::CDaqData()
{
}

CDaqData::~CDaqData()
{
}


// CDaqData member functions

void CDaqData::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);

	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}
