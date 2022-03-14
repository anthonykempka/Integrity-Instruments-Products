#include "stdafx.h"
#include ".\usbstreamdata.h"

CUSBStreamdata::CUSBStreamdata(void)
{
}

CUSBStreamdata::~CUSBStreamdata(void)
{
}

void CUSBStreamdata::SetValue(unsigned int value, double refrence, unsigned int index)
{
	short sVal;
	double dVal;

	sVal = (short)(((value << 2) ^ 0xFFFF) + 1) * (-1);
	dVal = (double)(sVal >> 2);
	dADCValues[index] = ( dVal * refrence );


}

double CUSBStreamdata::GetValue(unsigned int index)
{
	return dADCValues[index];
}
