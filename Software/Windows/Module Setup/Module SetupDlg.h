// Module SetupDlg.h : header file
//

#if !defined(AFX_MODULESETUPDLG_H__A2D3ED27_B155_11D2_9134_444553540000__INCLUDED_)
#define AFX_MODULESETUPDLG_H__A2D3ED27_B155_11D2_9134_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CModuleSetupDlg dialog

class CModuleSetupDlg : public CDialog
{
// Construction
public:
	CFont font;
    CStatic *m_hStatic;
	CString RtnSampleConfigurationStr(int Sel, BOOL Uni, BOOL b232M200);
	int RtnSampleConfigurationSel(CString Response_Str, BOOL b232M200);
	void Configure_Module(BOOL bRS485);
    void Read_Current_Settings(BOOL bRS485);
	int Check_Hex (CString Str, int LowVal, int HighVal, int MinChars);
	int String_to_Hex(CString str);
	int Open_Comport();
	HANDLE hCom;
	
    BOOL Send_Command(CString sCommand, CString *sResponse, BOOL bRS485 = FALSE);

    void Create_Sample_Combo_Box(CComboBox *pComboBox, BOOL b232M200);
    void Config_Sample_Combo_Box(BOOL b232M200);

	CModuleSetupDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CModuleSetupDlg)
	enum { IDD = IDD_MODULESETUP_DIALOG };
	CButton	m_232M200_Check;
	CEdit	m_Stream_Analog;
	CEdit	m_Async_Config;
	CButton	m_Uni_Sample8;
	CButton	m_Uni_Sample7;
	CButton	m_Uni_Sample6;
	CButton	m_Uni_Sample5;
	CButton	m_Uni_Sample4;
	CButton	m_Uni_Sample3;
	CButton	m_Uni_Sample2;
	CButton	m_Uni_Sample1;
	CButton	m_Slow_ADC;
	CButton	m_Expander;
	CComboBox	m_Sample8;
	CComboBox	m_Sample7;
	CComboBox	m_Sample6;
	CComboBox	m_Sample5;
	CComboBox	m_Sample4;
	CComboBox	m_Sample3;
	CComboBox	m_Sample2;
	CComboBox	m_Sample1;
	CButton	m_Stream_Digital;
	CButton	m_Stream_Pulse;
	CButton	m_RS485_Check;
	CButton	m_RS232_Check;
    CButton m_USB_Check;
	CString	m_Data_Direction;
	CString	m_Module_Address;
	CString	m_DA_CH0_Default;
	CString	m_DA_CH1_Default;
	CString	m_Port1_Default;
	CString	m_Port2_Default;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModuleSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CModuleSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRS232Radio();
	afx_msg void OnRS485Radio();
	afx_msg void OnComSetup();
	afx_msg void OnReadCurrent();
	afx_msg void OnDefault();
	afx_msg void OnConfigure();
	afx_msg void OnUSBRadio();
	afx_msg void OnAboutUs();
	afx_msg void On232m200Check();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODULESETUPDLG_H__A2D3ED27_B155_11D2_9134_444553540000__INCLUDED_)
