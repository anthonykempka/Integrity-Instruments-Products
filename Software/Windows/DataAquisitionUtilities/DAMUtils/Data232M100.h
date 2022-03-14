#pragma once
#include "afxcoll.h"
#include "Integrityioctrl1.h"



// CData232M100 command target
#define SIGNLEPOINT_CH0_TEXT	"CH0"
#define SIGNLEPOINT_CH1_TEXT	"CH1"
#define SIGNLEPOINT_CH2_TEXT	"CH2"
#define SIGNLEPOINT_CH3_TEXT	"CH3"
#define SIGNLEPOINT_CH4_TEXT	"CH4"
#define SIGNLEPOINT_CH5_TEXT	"CH5"
#define SIGNLEPOINT_CH6_TEXT	"CH6"
#define SIGNLEPOINT_CH7_TEXT	"CH7"

class CData232M100 : public CObject
{
public:
	CData232M100();
	virtual ~CData232M100();

	virtual void Serialize(CArchive& ar);

	void EEReset(void);

public:
	const static long PROTOCOL		= 1;		// Communication Protocoll
	const static long EE_PORT2DIR	= 0x03;		// Port Direction EE address
	const static long EE_ASYNCUMC_H = 0x04;		// Asynchronous Update Mode Config High Byte
	const static long EE_ASYNCUMC_L = 0x05;		// Asynchronous Update Mode Config Low Byte
	const static long EE_P1PDO		= 0x06;		// Port 2 Power Default Output
	const static long EE_P2PDO		= 0x07;		// Port 2 Power Default Output
	const static long EE_EBF		= 0x08;		// Expander Board Flag
	const static long EE_CSA		= 0x10;		// Continuous stream analog configuration count
	const static long EE_CSA_0		= 0x11;		// Analog Query 1
	const static long EE_CSA_1		= 0x12;		// Analog Query 2
	const static long EE_CSA_2		= 0x13;		// Analog Query 3
	const static long EE_CSA_3		= 0x14;		// Analog Query 4
	const static long EE_CSA_4		= 0x15;		// Analog Query 5
	const static long EE_CSA_5		= 0x16;		// Analog Query 6
	const static long EE_CSA_6		= 0x17;		// Analog Query 7
	const static long EE_CSA_7		= 0x18;		// Analog Query 8
	const static long EE_CSDI		= 0x19;		// Coninous stream digital input
	const static long EE_CSPC		= 0x1A;		// Continous stream PUlse Counter

	const static int MAX_STREAM_ITEMS = 8;		// Maximum number of stream channels

	const static DWORD SINGLEPOINT_CH0=	0x80;	// Single Point Channel 0
	const static DWORD SINGLEPOINT_CH1= 0x81;	// Single Point Channel 1
	const static DWORD SINGLEPOINT_CH2= 0x82;	// Single Point Channel 2
	const static DWORD SINGLEPOINT_CH3= 0x83;	// Single Point Channel 3
	const static DWORD SINGLEPOINT_CH4= 0x84;	// Single Point Channel 4
	const static DWORD SINGLEPOINT_CH5= 0x85;	// Single Point Channel 5
	const static DWORD SINGLEPOINT_CH6= 0x86;	// Single Point Channel 6
	const static DWORD SINGLEPOINT_CH7= 0x87;	// Single Point Channel 7

	const static int DS_ON		= 0xFF;			// Continuous digital stream on
	const static int DS_OFF		= 0x00;			// Continuous digital stream off

	const static int DP_ON		= 0xFF;			// Continuous stream pulse on
	const static int DP_OFF		= 0x00;			// Continuous stream pulse off
public:
	CByteArray m_cbaEEData;
	CString m_csCommPort;
	int m_nBaudRate;
	CIntegrityioctrl1* m_cIICtrl;

public:
	int GetDIO2Direction(void);
	void SetDIO2Direction(int dir);
	int GetAsyncUDMode(void);
	void SetAsyncUDMode(int udMode);
	int GetP1PwrOnDefault(void);
	void SetP1PwrOnDefault(int pdo);
	int GetP2PwrOnDefault(void);
	void SetP2PwrOnDefault(int pdo);
	int GetContStrAC(void);
	void SetContStrAC(int ac);
	int GetAnalogQryCtrl(int index);
	void SetAnalogQryCtrl(int index, int qryctrl);
	int GetContDIctrl(void);
	void SetContDIctrl(int dictrl);
	int GetContPCctrl(void);
	void SetContPCctrl(int pcctrl);
	void SetAnalogQueryCount(int count);
	int GetAnalogQueryCount(void);


};


