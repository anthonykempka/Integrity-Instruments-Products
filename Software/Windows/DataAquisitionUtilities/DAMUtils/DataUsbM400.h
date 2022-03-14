#pragma once

#include "afxcoll.h"
#include "Integrityioctrl1.h"
#include ".\datausbm400.h"

// CData485M400 command target
#define SIGNLEPOINT_CH0_TEXT	"CH0"
#define SIGNLEPOINT_CH1_TEXT	"CH1"
#define SIGNLEPOINT_CH2_TEXT	"CH2"
#define SIGNLEPOINT_CH3_TEXT	"CH3"
#define SIGNLEPOINT_CH4_TEXT	"CH4"
#define SIGNLEPOINT_CH5_TEXT	"CH5"
#define SIGNLEPOINT_CH6_TEXT	"CH6"
#define SIGNLEPOINT_CH7_TEXT	"CH7"
#define DIFFERENTIAL_CH0P_TEXT  "CH0+ CH1-"
#define DIFFERENTIAL_CH2P_TEXT  "CH2+ CH3-"
#define DIFFERENTIAL_CH4P_TEXT  "CH4+ CH5-"
#define DIFFERENTIAL_CH6P_TEXT  "CH6+ CH7-"
#define DIFFERENTIAL_CH0N_TEXT  "CH0- CH1+"
#define DIFFERENTIAL_CH2N_TEXT  "CH2- CH3+"
#define DIFFERENTIAL_CH4N_TEXT  "CH4- CH5+"
#define DIFFERENTIAL_CH6N_TEXT  "CH6- CH7+"
#define ADC_GAIN_TEXT_1			"1"
#define ADC_GAIN_TEXT_2			"2"
#define ADC_GAIN_TEXT_4			"4"
#define ADC_GAIN_TEXT_5			"5"
#define ADC_GAIN_TEXT_8			"8"
#define ADC_GAIN_TEXT_10		"10"
#define ADC_GAIN_TEXT_16		"16"
#define ADC_GAIN_TEXT_20		"20"

// CDataUsbM400 command target

class CDataUsbM400 : public CObject
{
	DECLARE_SERIAL(CDataUsbM400)
public:
	const static long PROTOCOL		= 3;		// Communication Protocoll

	const static long EE_UNITADDR	= 0x00;		// Module Address
	const static long EE_PORT1DIR	= 0x02;		// Port Direction EE address
	const static long EE_PORT2DIR	= 0x03;		// Port Direction EE address
	//const static long EE_ASYNCUMC_H = 0x04;		// Asynchronous Update Mode Config High Byte
	//const static long EE_ASYNCUMC_L = 0x05;		// Asynchronous Update Mode Config Low Byte
	const static long EE_P1PDO		= 0x06;		// Port 2 Power Default Output
	const static long EE_P2PDO		= 0x07;		// Port 2 Power Default Output
	const static long EE_EBF		= 0x08;		// Expander Board Flag
	const static long EE_DAPDO_H0	= 0x09;		// D/A Channel 0 Power on Default output High Nibble
	const static long EE_DAPDO_L0	= 0x0A;		// D/A Channel 0 Power on Default output Low Byte
	const static long EE_DAPDO_H1	= 0x0B;		// D/A Channel 1 Power on Default output High Nibble
	const static long EE_DAPDO_L1	= 0x0C;		// D/A Channel 1 Power on Default output Low Byte
	const static long EE_ADSCR		= 0x0D;		// A/D Channels sample clock rate
	const static long EE_ADCREF		= 0x0E;		// Analog to Digital Converter Voltage refrence

	const static int EBF_NORMAL = 0x00;			// Normal Sample rate clock
	const static int EBF_SLOW	= 0xFF;			// Slow sample rate clock

	const static int EXP_NO		= 0x00;			// No Expander board attached
	const static int EXP_YES	= 0xFF;			// Expander board attached
	/*
	const static long EE_CSA		= 0x10;		// Continuous stream analog configuration count
	*/
	const static long EE_CSA_0		= 0x11;		// Analog Query 1
	const static long EE_CSA_1		= 0x12;		// Analog Query 2
	const static long EE_CSA_2		= 0x13;		// Analog Query 3
	const static long EE_CSA_3		= 0x14;		// Analog Query 4
	const static long EE_CSA_4		= 0x15;		// Analog Query 5
	const static long EE_CSA_5		= 0x16;		// Analog Query 6
	const static long EE_CSA_6		= 0x17;		// Analog Query 7
	const static long EE_CSA_7		= 0x18;		// Analog Query 8
	const static long EE_CSA_8		= 0x19;		// Analog Query 9
	const static long EE_CSA_9		= 0x1A;		// Analog Query 10
	const static long EE_CSA_10		= 0x1B;		// Analog Query 11
	const static long EE_CSA_11		= 0x1C;		// Analog Query 12
	const static long EE_CSA_12		= 0x1D;		// Analog Query 13
	const static long EE_CSA_13		= 0x1E;		// Analog Query 14
	const static long EE_CSA_14		= 0x1F;		// Analog Query 15
	const static long EE_CSA_15		= 0x20;		// Analog Query 16

	/*
	const static long EE_CSDI		= 0x19;		// Coninous stream digital input
	const static long EE_CSPC		= 0x1A;		// Continous stream PUlse Counter
	
	*/

	const static int MAX_STREAM_ITEMS = 16;			// Maximum number of stream channels

	const static DWORD DIFFERENTIAL_CH0P=	0x00;	// Differential CH0+ CH1-
	const static DWORD DIFFERENTIAL_CH2P=	0x01;	// Differential CH2+ CH3-
	const static DWORD DIFFERENTIAL_CH4P=	0x02;	// Differential CH4+ CH5-
	const static DWORD DIFFERENTIAL_CH6P=	0x03;	// Differential CH6+ CH8-
	const static DWORD DIFFERENTIAL_CH0N=	0x04;	// Differential CH0- CH1+
	const static DWORD DIFFERENTIAL_CH2N=	0x05;	// Differential CH2- CH3+
	const static DWORD DIFFERENTIAL_CH4N=	0x06;	// Differential CH4- CH5+
	const static DWORD DIFFERENTIAL_CH6N=	0x07;	// Differential CH6- CH8+

	const static DWORD SINGLEPOINT_CH0=		0x08;	// Single Point Channel 0
	const static DWORD SINGLEPOINT_CH1=		0x09;	// Single Point Channel 1
	const static DWORD SINGLEPOINT_CH2=		0x0A;	// Single Point Channel 2
	const static DWORD SINGLEPOINT_CH3=		0x0B;	// Single Point Channel 3
	const static DWORD SINGLEPOINT_CH4=		0x0C;	// Single Point Channel 4
	const static DWORD SINGLEPOINT_CH5=		0x0D;	// Single Point Channel 5
	const static DWORD SINGLEPOINT_CH6=		0x0E;	// Single Point Channel 6
	const static DWORD SINGLEPOINT_CH7=		0x0F;	// Single Point Channel 7

	const static DWORD ADCGAIN_1=			0x00;	// Analog Gain 1
	const static DWORD ADCGAIN_2=			0x10;	// Analog Gain 2
	const static DWORD ADCGAIN_4=			0x20;	// Analog Gain 4
	const static DWORD ADCGAIN_5=			0x30;	// Analog Gain 5
	const static DWORD ADCGAIN_8=			0x40;	// Analog Gain 8
	const static DWORD ADCGAIN_10=			0x50;	// Analog Gain 10
	const static DWORD ADCGAIN_16=			0x60;	// Analog Gain 16
	const static DWORD ADCGAIN_20=			0x70;	// Analog Gain 20

	const static DWORD REF_115=				0x0F;	// 1.15 volt internal refrence
	const static DWORD REF_2048=			0x0E;	// 2.048 volt internal refrence
	const static DWORD REF_25=				0x0C;	// 2.5 volt internal refrence
	const static DWORD REF_EXT=				0x04;	// External refrence.

/*
	const static int DS_ON		= 0xFF;			// Continuous digital stream on
	const static int DS_OFF		= 0x00;			// Continuous digital stream off

	const static int DP_ON		= 0xFF;			// Continuous stream pulse on
	const static int DP_OFF		= 0x00;			// Continuous stream pulse off
*/
public:
	CByteArray m_cbaEEData;
	CString m_csCommPort;
	int m_nBaudRate;
	CIntegrityioctrl1* m_cIICtrl;

public:
	CDataUsbM400();
	virtual ~CDataUsbM400();
	virtual void Serialize(CArchive& ar);

	void EEReset(void);
public:
	int GetModuleAddress(void);
	void SetModuleAddress(int address);
	int GetDIO1Direction(void);
	void SetDIO1Direction(int dir);
	int GetDIO2Direction(void);
	void SetDIO2Direction(int dir);
	int GetP1PwrOnDefault(void);
	void SetP1PwrOnDefault(int pdo);
	int GetP2PwrOnDefault(void);
	void SetP2PwrOnDefault(int pdo);
	int GetExpanderBdFlag(void);
	void SetExpanderBdFlag(int exbf);
	int GetDAC0Pod(void);
	void SetDAC0Pod(int pod);
	int GetDAC1Pod(void);
	void SetDAC1Pod(int pod);
	int GetADCFs(void);
	void SetADCFs(int adcfs);
	int GetAnalogQryCtrl(int index);
	void SetAnalogQryCtrl(int index, int qryctrl);
	int GetAnalogRefrence(void);
	void SetAnalogRefrence(int ref);
	/*
	int GetAsyncUDMode(void);
	void SetAsyncUDMode(int udMode);
	int GetContStrAC(void);
	void SetContStrAC(int ac);
	int GetContDIctrl(void);
	void SetContDIctrl(int dictrl);
	int GetContPCctrl(void);
	void SetContPCctrl(int pcctrl);
	void SetAnalogQueryCount(int count);
	int GetAnalogQueryCount(void);
	*/

	//CObArray m_cObaStreamData;
	
};


