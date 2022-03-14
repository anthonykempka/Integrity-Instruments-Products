#pragma once
#include "afxcoll.h"
#include "Integrityioctrl1.h"

// CData232M200 command target
#define SIGNLEPOINT_CH0_TEXT	"CH0"
#define SIGNLEPOINT_CH1_TEXT	"CH1"
#define DIFFERENTIAL_CH0P_TEXT  "CH0+ CH1-"
#define DIFFERENTIAL_CH0N_TEXT  "CH0- CH1+"

class CData232M200 : public CObject
{
	DECLARE_SERIAL(CData232M200)

public:

	const static long PROTOCOL		= 1;		// Communication Protocoll

	const static long EE_PORT2DIR	= 0x03;		// Port Direction EE address
	const static long EE_ASYNCUMC_H = 0x04;		// Asynchronous Update Mode Config High Byte
	const static long EE_ASYNCUMC_L = 0x05;		// Asynchronous Update Mode Config Low Byte
	const static long EE_P2PDO		= 0x07;		// Port 2 Power Default Output
	const static long EE_EBF		= 0x08;		// Expander Board Flag
	const static long EE_DAPDO_H0	= 0x09;		// D/A Channel 0 Power on Default output High Nibble
	const static long EE_DAPDO_L0	= 0x0A;		// D/A Channel 0 Power on Default output Low Byte
	const static long EE_DAPDO_H1	= 0x0B;		// D/A Channel 1 Power on Default output High Nibble
	const static long EE_DAPDO_L1	= 0x0C;		// D/A Channel 1 Power on Default output Low Byte
	const static long EE_ADSCR		= 0x0D;		// A/D Channels sample clock rate
	const static long EE_CSA		= 0x10;		// Continuous stream analog configuration count
	const static long EE_CSA_0		= 0x11;		// Analog Query 1
	const static long EE_CSA_1		= 0x12;		// Analog Query 2
	const static long EE_CSDI		= 0x19;		// Coninous stream digital input
	const static long EE_CSPC		= 0x1A;		// Continous stream PUlse Counter
	const static int MAX_STREAM_ITEMS = 2;		// Maximum number of stream channels

	const static DWORD DIFFERENTIAL_CH0P=  0;	// Differential CH0+ CH1-
	const static DWORD DIFFERENTIAL_CH1P=  1;	// Differential CH0- CH1+
	const static DWORD SINGLEPOINT_CH0  =  2;	// Single Point Channel 0
	const static DWORD SINGLEPOINT_CH1  =  3;	// Single Point Channel 1

	const static int EBF_NORMAL = 0x00;			// Normal Sample rate clock
	const static int EBF_SLOW	= 0xFF;			// Slow sample rate clock

	const static int EXP_NO		= 0x00;			// No Expander board attached
	const static int EXP_YES	= 0xFF;			// Expander board attached

	const static int DS_ON		= 0xFF;			// Continuous digital stream on
	const static int DS_OFF		= 0x00;			// Continuous digital stream off

	const static int DP_ON		= 0xFF;			// Continuous stream pulse on
	const static int DP_OFF		= 0x00;			// Continuous stream pulse off



public:

	CData232M200();
	virtual ~CData232M200();
	virtual void Serialize(CArchive& ar);
	CByteArray m_cbaEEData;
	CString m_csCommPort;
	int m_nBaudRate;

	int GetDIODirection(void);
	void SetDIODirection(int dir);
	int GetAsyncUDMode(void);
	void SetAsyncUDMode(int udMode);
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
	int GetContStrAC(void);
	void SetContStrAC(int ac);
	int GetAnalogQryCtrl(int index);
	void SetAnalogQryCtrl(int index, int qryctrl);
	int GetContDIctrl(void);
	void SetContDIctrl(int dictrl);
	int GetContPCctrl(void);
	void SetContPCctrl(int pcctrl);
	void EEReset(void);

	CIntegrityioctrl1* m_cIICtrl;
	void SetAnalogQueryCount(int count);
	int GetAnalogQueryCount(void);
};


