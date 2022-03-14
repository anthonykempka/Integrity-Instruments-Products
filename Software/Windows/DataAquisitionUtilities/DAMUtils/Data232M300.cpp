// Data232M300.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "Data232M300.h"
#include ".\data232m300.h"


// CData232M300
IMPLEMENT_SERIAL( CData232M300, CObject, 1 )

CData232M300::CData232M300()
: m_csCommPort(_T("COM1"))
, m_nBaudRate(115200)
, m_cIICtrl(NULL)
{
	EEReset();
}

CData232M300::~CData232M300()
{
}


// CData232M300 member functions

void CData232M300::EEReset(void)
{
	// Clear data
	m_cbaEEData.RemoveAll();

	m_cbaEEData.Add(0xFF);	//0x00
	m_cbaEEData.Add(0xFF);	//0x01
	m_cbaEEData.Add(0xFF);	//0x02
	m_cbaEEData.Add(0xFF);	//0x03
	m_cbaEEData.Add(0x00);	//0x04
	m_cbaEEData.Add(0x00);	//0x05
	m_cbaEEData.Add(0x00);	//0x06
	m_cbaEEData.Add(0x00);	//0x07
	m_cbaEEData.Add(0x00);	//0x08
	m_cbaEEData.Add(0x00);	//0x09
	m_cbaEEData.Add(0x00);	//0x0A
	m_cbaEEData.Add(0x00);	//0x0B
	m_cbaEEData.Add(0x00);	//0x0C
	m_cbaEEData.Add(0x00);	//0x0D
	m_cbaEEData.Add(0xFF);	//0x0E
	m_cbaEEData.Add(0xFF);	//0x0F
	m_cbaEEData.Add(0x00);	//0x10
	m_cbaEEData.Add(SINGLEPOINT_CH0);	//0x11
	m_cbaEEData.Add(SINGLEPOINT_CH1);	//0x12
	m_cbaEEData.Add(SINGLEPOINT_CH2);	//0x13
	m_cbaEEData.Add(SINGLEPOINT_CH3);	//0x14
	m_cbaEEData.Add(SINGLEPOINT_CH4);	//0x15
	m_cbaEEData.Add(SINGLEPOINT_CH5);	//0x16
	m_cbaEEData.Add(SINGLEPOINT_CH6);	//0x17
	m_cbaEEData.Add(SINGLEPOINT_CH7);	//0x18
	m_cbaEEData.Add(0x00);	//0x19
	m_cbaEEData.Add(0x00);	//0x1A
	m_cbaEEData.Add(0xFF);	//0x1B
	m_cbaEEData.Add(0xFF);	//0x1C
	m_cbaEEData.Add(0xFF);	//0x1D
	m_cbaEEData.Add(0xFF);	//0x1E
	m_cbaEEData.Add(0xFF);	//0x1F
	m_cbaEEData.Add(0xFF);	//0x20

}
void CData232M300::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);

	if (ar.IsStoring())
	{	// storing code
		ar << m_csCommPort;
		ar << m_nBaudRate;
	}
	else
	{	// loading code
		ar >> m_csCommPort;
		ar >> m_nBaudRate;
	}

	m_cbaEEData.Serialize(ar);
}
/*
 * Returns the Digital I/O Direction from EE image
 */
int CData232M300::GetDIO1Direction(void) 
{
	return (int)m_cbaEEData[EE_PORT1DIR];
}

/*
 * Sets the Digital I/O Direction in EE image
 */
void CData232M300::SetDIO1Direction(int dir)
{
	m_cbaEEData[EE_PORT1DIR] = (BYTE)dir;
}
/*
 * Returns the Digital I/O Direction from EE image
 */
int CData232M300::GetDIO2Direction(void)
{
	return (int)m_cbaEEData[EE_PORT2DIR];
}

/*
 * Sets the Digital I/O Direction in EE image
 */
void CData232M300::SetDIO2Direction(int dir)
{
	m_cbaEEData[EE_PORT2DIR] = (BYTE)dir;
}

/*
 * Return the Asyncronous update mode configuration from EE image
 */
int CData232M300::GetAsyncUDMode(void)
{
	int rval;
	rval = (int)m_cbaEEData[EE_ASYNCUMC_H] << 8;
	rval |= (int)m_cbaEEData[EE_ASYNCUMC_L];
	return rval;
}

/*
 * Sets the Asyncronous update mode configuration in EE image
 */
void CData232M300::SetAsyncUDMode(int udMode)
{
	m_cbaEEData[EE_ASYNCUMC_H] = (BYTE)udMode >> 8;
	m_cbaEEData[EE_ASYNCUMC_L] = (BYTE)udMode & 0x00FF;
}

/*
 * Return the Port2 Power On default configuration from EE image
 */
int CData232M300::GetP1PwrOnDefault(void)
{
	return (int)m_cbaEEData[EE_P1PDO];
}

/*
 * Sets the Port2 Power On default configuration in EE image
 */
void CData232M300::SetP1PwrOnDefault(int pdo)
{
	m_cbaEEData[EE_P1PDO] = (BYTE)pdo;
}

/*
 * Return the Port2 Power On default configuration from EE image
 */
int CData232M300::GetP2PwrOnDefault(void)
{
	return (int)m_cbaEEData[EE_P2PDO];
}

/*
 * Sets the Port2 Power On default configuration in EE image
 */
void CData232M300::SetP2PwrOnDefault(int pdo)
{
	m_cbaEEData[EE_P2PDO] = (BYTE)pdo;
}


/*
 * Return the Expander board flags configuration from EE image
 */
int CData232M300::GetExpanderBdFlag(void)
{
	return (int)m_cbaEEData[EE_EBF];
}


/*
 * Sets the Expander board flags configuration in EE image
 */
void CData232M300::SetExpanderBdFlag(int exbf)
{
	m_cbaEEData[EE_EBF] = (BYTE)exbf;
}


/*
 * Return the DAC0 default output configuration from EE image
 */
int CData232M300::GetDAC0Pod(void)
{
	int rval;
	rval = (int)m_cbaEEData[EE_DAPDO_H0]<<8;
	rval |= (int)m_cbaEEData[EE_DAPDO_L0];

	return rval;
}

/*
 * Sets the DAC0 default output configuration in EE image
 */
void CData232M300::SetDAC0Pod(int pod)
{
	m_cbaEEData[EE_DAPDO_H0] = (BYTE)(pod >> 8);
	m_cbaEEData[EE_DAPDO_L0] = (BYTE)(pod & 0xFF);
}
/*
 * Return the DAC0 default output configuration from EE image
 */
int CData232M300::GetDAC1Pod(void)
{
	int rval;
	rval = (int)m_cbaEEData[EE_DAPDO_H1]<<8;
	rval |= (int)m_cbaEEData[EE_DAPDO_L1];

	return rval;
}

/*
 * Sets the DAC0 default output configuration in EE image
 */
void CData232M300::SetDAC1Pod(int pod)
{
	m_cbaEEData[EE_DAPDO_H1] = (BYTE)(pod >> 8);
	m_cbaEEData[EE_DAPDO_L1] = (BYTE)(pod & 0xFF);
}

/*
 * Return the ADC Channels sample rate clock configuration from EE image
 */
int CData232M300::GetADCFs(void)
{
	return (int)m_cbaEEData[EE_ADSCR];
}

/*
 * Sets the ADC Channels sample rate clock configuration in EE image
 */
void CData232M300::SetADCFs(int adcfs)
{
	m_cbaEEData[EE_ADSCR] = (BYTE)adcfs;
}


/*
 * Return the ADC Continuou Stream Analog Count configuration from EE image
 */
int CData232M300::GetContStrAC(void)
{
	return (int)m_cbaEEData[EE_CSA];
}
/*
 * Sets the ADC ADC Continuou Stream Analog Count configuration in EE image
 */
void CData232M300::SetContStrAC(int ac)
{
	m_cbaEEData[EE_CSA] = (BYTE)ac;
}

/*
 * Return the ADC Analog Query Control Byte configuration from EE image
 * Valid Index 0 - 1
 */
int CData232M300::GetAnalogQryCtrl(int index)
{
	return m_cbaEEData[EE_CSA_0 + index];
}
/*
 * Sets the ADC Analog Query Control Byte configuration in EE image
 * Valid Index 0 - 1
 */
void CData232M300::SetAnalogQryCtrl(int index, int qryctrl)
{
	m_cbaEEData[EE_CSA_0 + index] = (BYTE)qryctrl;
}
/*
 * Return the Continuous Stream Digital Input configuration from EE image
 */
int CData232M300::GetContDIctrl(void)
{
	return (int)m_cbaEEData[EE_CSDI];
}


/*
 * Sets the Continuous Stream Digital Input configuration in EE image
 */
void CData232M300::SetContDIctrl(int dictrl)
{
	m_cbaEEData[EE_CSDI] = (BYTE)dictrl;
}



/*
 * Return the Continuous Stream Pulse Count configuration from EE image
 */
int CData232M300::GetContPCctrl(void)
{
	return (int)m_cbaEEData[EE_CSPC];
}

/*
 * Sets the Continuous Stream Pulse Count configuration in EE image
 */
void CData232M300::SetContPCctrl(int pcctrl)
{
	m_cbaEEData[EE_CSPC] = (BYTE)pcctrl;
}


void CData232M300::SetAnalogQueryCount(int count)
{
	m_cbaEEData[EE_CSA] = (BYTE)count;
}

int CData232M300::GetAnalogQueryCount(void)
{
	return (int)m_cbaEEData[EE_CSA] ;
}


