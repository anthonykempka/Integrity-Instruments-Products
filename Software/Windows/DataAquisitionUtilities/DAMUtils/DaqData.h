#pragma once
#include "data232m200.h"



// CDaqData command target

class CDaqData : public CObject
{
	DECLARE_SERIAL(CDaqData)
public:
	CDaqData();
	virtual ~CDaqData();
	virtual void Serialize(CArchive& ar);
	CData232M200 m_cData232M200;
};


