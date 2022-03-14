// ADC_LogDlg.h : header file
//

#if !defined(AFX_ADC_LOGDLG_H__25E7FB87_B61E_11D2_9134_444553540000__INCLUDED_)
#define AFX_ADC_LOGDLG_H__25E7FB87_B61E_11D2_9134_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CADC_LogDlg dialog

class CADC_LogDlg : public CDialog
{
// Construction
public:
	int m_RcvSleepTime;
	int OffsetCalibration;
	int RS232_Get_Offset(void);
	int RS485_Get_Offset(void);
	int FilterArrayPosition[8];
	double FilterArray[8][100];
	int FilterCount;
	CString SampleStr;
	CString FileStr;
    CString Command_Str;
	HANDLE hLogFile;
	int Config_Sample1;
    int Config_Sample2;
    int Config_Sample3;
    int Config_Sample4;
    int Config_Sample5;
    int Config_Sample6;
    int Config_Sample7;
    int Config_Sample8;

    int Unipolar_Sample1;
    int Unipolar_Sample2;
    int Unipolar_Sample3;
    int Unipolar_Sample4;
    int Unipolar_Sample5;
    int Unipolar_Sample6;
    int Unipolar_Sample7;
    int Unipolar_Sample8;

    int ModuleAddress;

    int Log_Interval;

    double Vref;

	int bRS485_Check;
	void RS485_Sample(void);
	void RS232_Sample(void);
	int bTimerRunning;
    int bAskCancel;
	HANDLE hCom;
	CADC_LogDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CADC_LogDlg)
	enum { IDD = IDD_ADC_LOG_DIALOG };
	CButton	m_Browse;
	CButton	m_Config;
	CButton	m_CommSetup;
	CButton	m_Halt;
	CButton	m_Begin;
	CString	m_FileName;
	CString	m_LastCommand;
	CString	m_LastResponse;
	int		m_SampleCount;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADC_LogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CADC_LogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	afx_msg void OnBrowse();
	afx_msg void OnBegin();
	afx_msg void OnCommSetup();
	afx_msg void OnConfig();
	afx_msg void OnHalt();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADC_LOGDLG_H__25E7FB87_B61E_11D2_9134_444553540000__INCLUDED_)
