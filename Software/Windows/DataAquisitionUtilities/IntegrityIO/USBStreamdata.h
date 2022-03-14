#pragma once
#include "..\USBM300 Class\USBM300.h"

class CUSBStreamdata
{
public:
	CUSBStreamdata(void);
	~CUSBStreamdata(void);
protected:
	double dADCValues[ADC_SAMPLE_COUNT];
public:
	void SetValue(unsigned int value, double refrence, unsigned int index);
	double GetValue(unsigned int index);
};
