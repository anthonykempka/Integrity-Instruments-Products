#pragma once
#include "integrityioctrl1.h"
#include "integritylinegraph.h"


// C232M200Log dialog

class C232M200Log : public CDialog
{
	DECLARE_DYNAMIC(C232M200Log)

public:
	C232M200Log(CWnd* pParent = NULL);   // standard constructor
	virtual ~C232M200Log();

// Dialog Data
	enum { IDD = IDD_DLG_232M200LOGGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CString m_cstrLogFilePath;
	BOOL m_bChannel0;
	BOOL m_bChannel1;
	double m_dInterval;
	CString m_strGraphMin;
	CString m_strGraphMax;
	double m_dGraphMax;
	double m_dGraphMin;
	BOOL m_bAXRedraw;

	CWinThread *m_pLogThread;
	CEvent m_ceHaltThread;

	bool m_bLogFileIsOpen;
	DWORD m_dwStartMilliseconds;


public:
	CIntegrityioctrl1* m_cIICtrl;

	afx_msg void OnBnClickedBtnbrowsem200log();
	afx_msg void OnBnClickedBtnM200logstart();
	afx_msg void OnBnClickedBtnM200logstop();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void HaltThread(void);

private:
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
protected:
	static UINT LogThreadProc(LPVOID pParam);


public:
	CIilg_m100log m_cIILG;
	afx_msg void OnEnChangeEbm200graphmin();
	afx_msg void OnEnChangeEbm200graphmax();
	afx_msg void OnBnClickedRbdiffadcoff();
	afx_msg void OnBnClickedRbdiffadcpos();
	afx_msg void OnBnClickedRbdiffadcneg();
protected:
	void SetChannelBtnStates(bool enabled);
public:
	int m_nSampleType;
	virtual BOOL OnInitDialog();
};
