#pragma once
#include "integritylinegraph.h"
#include "integrityioctrl1.h"




// C232M100Log dialog

class C232M100Log : public CDialog
{
	DECLARE_DYNAMIC(C232M100Log)

public:
	C232M100Log(CWnd* pParent = NULL);   // standard constructor
	virtual ~C232M100Log();

// Dialog Data
	enum { IDD = IDD_DLG_232M100LOGGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_cstrLogFilePath;
	BOOL m_bChannel0;
	BOOL m_bChannel1;
	BOOL m_bChannel2;
	BOOL m_bChannel3;
	BOOL m_bChannel4;
	BOOL m_bChannel5;
	BOOL m_bChannel6;
	BOOL m_bChannel7;
	double m_dInterval;
protected:
	//CFile m_cfLogFile;
	bool m_bLogFileIsOpen;
	DWORD m_dwStartMilliseconds;
public:
	afx_msg void OnBnClickedBtnbrowsem100log();
	afx_msg void OnBnClickedBtnM100logstart();
	afx_msg void OnBnClickedBtnM100logstop();
//	afx_msg void OnTimer(UINT nIDEvent);

public:
	CIntegrityioctrl1* m_cIICtrl;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CIilg_m100log m_cIILG;
private:
	enum GraphColors
	{
		ch0 = 0x7f7f7f,
		ch1 = 0x7f7fff,
		ch2 = 0x7fff7f,
		ch3 = 0x7fffff,
		ch4 = 0xff7f7f,
		ch5 = 0xff7fff,
		ch6 = 0xffff7f,
		ch7 = 0xffffff
	};
	CWinThread *m_pLogThread;
	CEvent m_ceHaltThread;

protected:
	static UINT LogThreadProc(LPVOID pParam);
public:
	void HaltThread(void);
private:
	DWORD m_dwInterval;
protected:
	BOOL m_bAXRedraw;
	double m_dGraphMax;
	double m_dGraphMin;
public:
	CString m_strGraphMin;
	CString m_strGraphMax;
	afx_msg void OnEnChangeEbgraphmin();
	afx_msg void OnEnChangeEbgraphmax();
};
