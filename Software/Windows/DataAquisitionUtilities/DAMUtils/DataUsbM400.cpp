// DataUsbM400.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "DataUsbM400.h"
#include "UsbStreamData.h"

// CDataUsbM400
IMPLEMENT_SERIAL( CDataUsbM400, CObject, 1 )

CDataUsbM400::CDataUsbM400()
: m_csCommPort(_T("COM1"))
, m_nBaudRate(115200)
, m_cIICtrl(NULL)
{
	EEReset();
}

CDataUsbM400::~CDataUsbM400()
{
	/*
	for(int i = 0; i < m_cObaStreamData.GetSize(); i++)
	{
		delete m_cObaStreamData[i];
	}
	*/
}


// CDataUsbM400 member functions
void CDataUsbM400::EEReset(void)
{
	// Clear data
	m_cbaEEData.RemoveAll();

	m_cbaEEData.Add(0x01);	//0x00
	m_cbaEEData.Add(0xFF);	//0x01
	m_cbaEEData.Add(0xFF);	//0x02
	m_cbaEEData.Add(0xFF);	//0x03
	m_cbaEEData.Add(0xFF);	//0x04
	m_cbaEEData.Add(0xFF);	//0x05
	m_cbaEEData.Add(0x00);	//0x06
	m_cbaEEData.Add(0x00);	//0x07
	m_cbaEEData.Add(0x00);	//0x08
	m_cbaEEData.Add(0x00);	//0x09
	m_cbaEEData.Add(0x00);	//0x0A
	m_cbaEEData.Add(0x00);	//0x0B
	m_cbaEEData.Add(0x00);	//0x0C
	m_cbaEEData.Add(0x00);	//0x0D
	m_cbaEEData.Add(0x0C);	//0x0E
	m_cbaEEData.Add(0xFF);	//0x0F
	m_cbaEEData.Add(0xFF);	//0x10
	m_cbaEEData.Add(0x08);	//0x11
	m_cbaEEData.Add(0x09);	//0x12
	m_cbaEEData.Add(0x0A);	//0x13
	m_cbaEEData.Add(0x0B);	//0x14
	m_cbaEEData.Add(0x0C);	//0x15
	m_cbaEEData.Add(0x0D);	//0x16
	m_cbaEEData.Add(0x0E);	//0x17
	m_cbaEEData.Add(0x0F);	//0x18
	m_cbaEEData.Add(0x08);	//0x19
	m_cbaEEData.Add(0x09);	//0x1A
	m_cbaEEData.Add(0x0A);	//0x1B
	m_cbaEEData.Add(0x0B);	//0x1C
	m_cbaEEData.Add(0x0C);	//0x1D
	m_cbaEEData.Add(0x0D);	//0x1E
	m_cbaEEData.Add(0x0E);	//0x1F
	m_cbaEEData.Add(0x0F);	//0x20

	/*
	m_cObaStreamData.RemoveAll();
	m_cObaStreamData.SetSize(16);

	for(int i=0; i < MAX_STREAM_ITEMS; i++)
	{
		m_cObaStreamData[i] = new CUsbStreamData();
	}
	
	((CUsbStreamData*)m_cObaStreamData[0])->m_csName = SIGNLEPOINT_CH0_TEXT;
	((CUsbStreamData*)m_cObaStreamData[0])->m_dwValue = GetAnalogQryCtrl(0);

	((CUsbStreamData*)m_cObaStreamData[1])->m_csName = SIGNLEPOINT_CH1_TEXT;
	((CUsbStreamData*)m_cObaStreamData[1])->m_dwValue = GetAnalogQryCtrl(1);

	((CUsbStreamData*)m_cObaStreamData[2])->m_csName = SIGNLEPOINT_CH2_TEXT;
	((CUsbStreamData*)m_cObaStreamData[2])->m_dwValue = GetAnalogQryCtrl(2);

	((CUsbStreamData*)m_cObaStreamData[3])->m_csName = SIGNLEPOINT_CH3_TEXT;
	((CUsbStreamData*)m_cObaStreamData[3])->m_dwValue = GetAnalogQryCtrl(3);

	((CUsbStreamData*)m_cObaStreamData[4])->m_csName = SIGNLEPOINT_CH4_TEXT;
	((CUsbStreamData*)m_cObaStreamData[4])->m_dwValue = GetAnalogQryCtrl(4);

	((CUsbStreamData*)m_cObaStreamData[5])->m_csName = SIGNLEPOINT_CH5_TEXT;
	((CUsbStreamData*)m_cObaStreamData[5])->m_dwValue = GetAnalogQryCtrl(5);

	((CUsbStreamData*)m_cObaStreamData[6])->m_csName = SIGNLEPOINT_CH6_TEXT;
	((CUsbStreamData*)m_cObaStreamData[6])->m_dwValue = GetAnalogQryCtrl(6);

	((CUsbStreamData*)m_cObaStreamData[7])->m_csName = SIGNLEPOINT_CH7_TEXT;
	((CUsbStreamData*)m_cObaStreamData[7])->m_dwValue = GetAnalogQryCtrl(7);

	((CUsbStreamData*)m_cObaStreamData[8])->m_csName = SIGNLEPOINT_CH0_TEXT;
	((CUsbStreamData*)m_cObaStreamData[8])->m_dwValue = GetAnalogQryCtrl(8);

	((CUsbStreamData*)m_cObaStreamData[9])->m_csName = SIGNLEPOINT_CH1_TEXT;
	((CUsbStreamData*)m_cObaStreamData[9])->m_dwValue = GetAnalogQryCtrl(9);

	((CUsbStreamData*)m_cObaStreamData[10])->m_csName = SIGNLEPOINT_CH2_TEXT;
	((CUsbStreamData*)m_cObaStreamData[10])->m_dwValue = GetAnalogQryCtrl(10);

	((CUsbStreamData*)m_cObaStreamData[11])->m_csName = SIGNLEPOINT_CH3_TEXT;
	((CUsbStreamData*)m_cObaStreamData[11])->m_dwValue = GetAnalogQryCtrl(11);

	((CUsbStreamData*)m_cObaStreamData[12])->m_csName = SIGNLEPOINT_CH4_TEXT;
	((CUsbStreamData*)m_cObaStreamData[12])->m_dwValue = GetAnalogQryCtrl(12);

	((CUsbStreamData*)m_cObaStreamData[13])->m_csName = SIGNLEPOINT_CH5_TEXT;
	((CUsbStreamData*)m_cObaStreamData[13])->m_dwValue = GetAnalogQryCtrl(13);

	((CUsbStreamData*)m_cObaStreamData[14])->m_csName = SIGNLEPOINT_CH6_TEXT;
	((CUsbStreamData*)m_cObaStreamData[14])->m_dwValue = GetAnalogQryCtrl(14);

	((CUsbStreamData*)m_cObaStreamData[15])->m_csName = SIGNLEPOINT_CH7_TEXT;
	((CUsbStreamData*)m_cObaStreamData[15])->m_dwValue = GetAnalogQryCtrl(15);
	*/
}

void CDataUsbM400::Serialize(CArchive& ar)
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
	//m_cObaStreamData.Serialize(ar);
}

/*
 * Returns the Module Address from EE image
 */
int CDataUsbM400::GetModuleAddress(void) 
{
	return (int)m_cbaEEData[EE_UNITADDR];
}

/*
 * Sets the Module Address in EE image
 */
void CDataUsbM400::SetModuleAddress(int address)
{
	m_cbaEEData[EE_UNITADDR] = (BYTE)address;
}

/*
 * Returns the Digital I/O Direction from EE image
 */
int CDataUsbM400::GetDIO1Direction(void) 
{
	return (int)m_cbaEEData[EE_PORT1DIR];
}

/*
 * Sets the Digital I/O Direction in EE image
 */
void CDataUsbM400::SetDIO1Direction(int dir)
{
	m_cbaEEData[EE_PORT1DIR] = (BYTE)dir;
}
/*
 * Returns the Digital I/O Direction from EE image
 */
int CDataUsbM400::GetDIO2Direction(void)
{
	return (int)m_cbaEEData[EE_PORT2DIR];
}

/*
 * Sets the Digital I/O Direction in EE image
 */
void CDataUsbM400::SetDIO2Direction(int dir)
{
	m_cbaEEData[EE_PORT2DIR] = (BYTE)dir;
}


/*
 * Return the Port2 Power On default configuration from EE image
 */
int CDataUsbM400::GetP1PwrOnDefault(void)
{
	return (int)m_cbaEEData[EE_P1PDO];
}

/*
 * Sets the Port2 Power On default configuration in EE image
 */
void CDataUsbM400::SetP1PwrOnDefault(int pdo)
{
	m_cbaEEData[EE_P1PDO] = (BYTE)pdo;
}

/*
 * Return the Port2 Power On default configuration from EE image
 */
int CDataUsbM400::GetP2PwrOnDefault(void)
{
	return (int)m_cbaEEData[EE_P2PDO];
}

/*
 * Sets the Port2 Power On default configuration in EE image
 */
void CDataUsbM400::SetP2PwrOnDefault(int pdo)
{
	m_cbaEEData[EE_P2PDO] = (BYTE)pdo;
}


/*
 * Return the Expander board flags configuration from EE image
 */
int CDataUsbM400::GetExpanderBdFlag(void)
{
	return (int)m_cbaEEData[EE_EBF];
}


/*
 * Sets the Expander board flags configuration in EE image
 */
void CDataUsbM400::SetExpanderBdFlag(int exbf)
{
	m_cbaEEData[EE_EBF] = (BYTE)exbf;
}


/*
 * Return the DAC0 default output configuration from EE image
 */
int CDataUsbM400::GetDAC0Pod(void)
{
	int rval;
	rval = (int)m_cbaEEData[EE_DAPDO_H0]<<8;
	rval |= (int)m_cbaEEData[EE_DAPDO_L0];

	return rval;
}

/*
 * Sets the DAC0 default output configuration in EE image
 */
void CDataUsbM400::SetDAC0Pod(int pod)
{
	m_cbaEEData[EE_DAPDO_H0] = (BYTE)(pod >> 8);
	m_cbaEEData[EE_DAPDO_L0] = (BYTE)(pod & 0xFF);
}
/*
 * Return the DAC0 default output configuration from EE image
 */
int CDataUsbM400::GetDAC1Pod(void)
{
	int rval;
	rval = (int)m_cbaEEData[EE_DAPDO_H1]<<8;
	rval |= (int)m_cbaEEData[EE_DAPDO_L1];

	return rval;
}

/*
 * Sets the DAC0 default output configuration in EE image
 */
void CDataUsbM400::SetDAC1Pod(int pod)
{
	m_cbaEEData[EE_DAPDO_H1] = (BYTE)(pod >> 8);
	m_cbaEEData[EE_DAPDO_L1] = (BYTE)(pod & 0xFF);
}

/*
 * Return the ADC Channels sample rate clock configuration from EE image
 */
int CDataUsbM400::GetADCFs(void)
{
	return (int)m_cbaEEData[EE_ADSCR];
}

/*
 * Sets the ADC Channels sample rate clock configuration in EE image
 */
void CDataUsbM400::SetADCFs(int adcfs)
{
	m_cbaEEData[EE_ADSCR] = (BYTE)adcfs;
}

/*
 * Return the ADC Analog Query Control Byte configuration from EE image
 * Valid Index 0 - 1
 */
int CDataUsbM400::GetAnalogQryCtrl(int index)
{
	return m_cbaEEData[EE_CSA_0 + index];
}
/*
 * Sets the ADC Analog Query Control Byte configuration in EE image
 * Valid Index 0 - 1
 */
void CDataUsbM400::SetAnalogQryCtrl(int index, int qryctrl)
{
	m_cbaEEData[EE_CSA_0 + index] = (BYTE)qryctrl;
}

int CDataUsbM400::GetAnalogRefrence(void)
{
	return (int)m_cbaEEData[EE_ADCREF];
}
void CDataUsbM400::SetAnalogRefrence(int ref)
{
	m_cbaEEData[EE_ADCREF] = (BYTE)ref;
}

/********************  REMOVED STREAMING BY #IF *****************************/
#if 0

/*
 * Return the Asyncronous update mode configuration from EE image
 */
int CDataUsbM400::GetAsyncUDMode(void)
{
	int rval;
	rval = (int)m_cbaEEData[EE_ASYNCUMC_H] << 8;
	rval |= (int)m_cbaEEData[EE_ASYNCUMC_L];
	return rval;
}

/*
 * Sets the Asyncronous update mode configuration in EE image
 */
void CDataUsbM400::SetAsyncUDMode(int udMode)
{
	m_cbaEEData[EE_ASYNCUMC_H] = (BYTE)udMode >> 8;
	m_cbaEEData[EE_ASYNCUMC_L] = (BYTE)udMode & 0x00FF;
}


/*
 * Return the ADC Continuou Stream Analog Count configuration from EE image
 */
int CDataUsbM400::GetContStrAC(void)
{
	return (int)m_cbaEEData[EE_CSA];
}
/*
 * Sets the ADC ADC Continuou Stream Analog Count configuration in EE image
 */
void CDataUsbM400::SetContStrAC(int ac)
{
	m_cbaEEData[EE_CSA] = (BYTE)ac;
}

/*
 * Return the Continuous Stream Digital Input configuration from EE image
 */
int CDataUsbM400::GetContDIctrl(void)
{
	return (int)m_cbaEEData[EE_CSDI];
}


/*
 * Sets the Continuous Stream Digital Input configuration in EE image
 */
void CDataUsbM400::SetContDIctrl(int dictrl)
{
	m_cbaEEData[EE_CSDI] = (BYTE)dictrl;
}



/*
 * Return the Continuous Stream Pulse Count configuration from EE image
 */
int CDataUsbM400::GetContPCctrl(void)
{
	return (int)m_cbaEEData[EE_CSPC];
}

/*
 * Sets the Continuous Stream Pulse Count configuration in EE image
 */
void CDataUsbM400::SetContPCctrl(int pcctrl)
{
	m_cbaEEData[EE_CSPC] = (BYTE)pcctrl;
}


void CDataUsbM400::SetAnalogQueryCount(int count)
{
	m_cbaEEData[EE_CSA] = (BYTE)count;
}

int CDataUsbM400::GetAnalogQueryCount(void)
{
	return (int)m_cbaEEData[EE_CSA] ;
}
#endif
