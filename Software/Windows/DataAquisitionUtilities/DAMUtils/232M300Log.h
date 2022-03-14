#pragma once
#include "integritylinegraph.h"
#include "integrityioctrl1.h"


// C232M300Log dialog

class C232M300Log : public CDialog
{
	DECLARE_DYNAMIC(C232M300Log)

public:
	C232M300Log(CWnd* pParent = NULL);   // standard constructor
	virtual ~C232M300Log();

// Dialog Data
	enum { IDD = IDD_DLG_232M300LOGGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Full path to the log file
	CString m_cstrLogFilePath;
	afx_msg void OnBnClickedBtnbrowsem300log();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnM300logstart();
	afx_msg void OnBnClickedBtnM300logstop();
	afx_msg void OnBnClickedRbdiffadcoff();
	afx_msg void OnBnClickedRbdiffadcpos();
	afx_msg void OnBnClickedRbdiffadcneg();
	afx_msg void OnBnClickedRbdiffadcoff2();
	afx_msg void OnBnClickedRbdiffadcpos2();
	afx_msg void OnBnClickedRbdiffadcneg2();
	afx_msg void OnBnClickedRbdiffadcoff3();
	afx_msg void OnBnClickedRbdiffadcpos3();
	afx_msg void OnBnClickedRbdiffadcneg3();
	afx_msg void OnBnClickedRbdiffadcoff4();
	afx_msg void OnBnClickedRbdiffadcpos4();
	afx_msg void OnBnClickedRbdiffadcneg4();
	CString m_strInterval;
	CString m_strGraphMin;
	CString m_strGraphMax;
	BOOL m_bCh0;
	BOOL m_bCh1;
	BOOL m_bCh2;
	BOOL m_bCh3;
	BOOL m_bCh4;
	BOOL m_bCh5;
	BOOL m_bCh6;
	BOOL m_bCh7;
	CIilg_m100log m_cIILG;
	CIntegrityioctrl1* m_cIICtrl;

	virtual BOOL OnInitDialog();

public:
	void HaltThread(void);

protected:
	CWinThread *m_pLogThread;
	CEvent m_ceHaltThread;


	BOOL m_bAXRedraw;
	double m_dInterval;

	enum GraphColors
	{
		ch7 = 0x7f7f7f,
		ch6 = 0x7f7fff,
		ch5 = 0x7fff7f,
		ch4 = 0x7fffff,
		ch3 = 0xff7f7f,
		ch2 = 0xff7fff,
		ch1 = 0xffff7f,
		ch0 = 0xffffff
	};

	DWORD m_dwInterval;
	static UINT LogThreadProc(LPVOID pParam);


public:
	double m_dGraphMax;
	double m_dGraphMin;
	int m_nSampleType0;
	int m_nSampleType2;
	int m_nSampleType4;
	int m_nSampleType6;
	DWORD m_dwStartMilliseconds;
protected:
	void SetChCheckStates(void);
public:
	afx_msg void OnEnChangeEbm300graphmin();
	afx_msg void OnEnChangeEbm300graphmax();
};
