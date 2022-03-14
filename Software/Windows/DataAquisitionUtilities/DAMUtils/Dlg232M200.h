#pragma once
#include "Data232M200.h"
#include "Integrityioctrl1.h"
#include "afxwin.h"

// CDlg232M200 dialog

class CDlg232M200 : public CDialog
{
	DECLARE_DYNAMIC(CDlg232M200)

public:
	CDlg232M200(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg232M200();

// Dialog Data
	enum { IDD = IDD_DLG_232M200 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	bool ReadCurrent(void);
	CData232M200 *m_cData;
protected:
	void DataToCtrl(void);
public:
	CString m_csDataP2Dir;
	CString m_csAsyncUpdate;
	CString m_csP2PowerOn;
	CString m_csDAC0;
	CString m_csDAC1;
	BOOL m_bADCFs;
	CListBox m_lbAdcTypes;
	CListBox m_lbAdcCfg;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnRemove();
	afx_msg void OnLbnDblclkLstAdctypes();
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
protected:
	void TypeToConfig(void);
public:
	afx_msg void OnLbnDblclkLstAdccfg();
protected:
	void RemoveConfigItem(void);
public:
	void SetDefaults(void);
	void SetCurrent(void);
	BOOL m_bEXPFlag;
	CString m_csAdcCount;
	BOOL m_bEnDigitalStatus;
	BOOL m_bEnPulseStatus;
	bool WriteCurrent(void);
	void CtrlToData(void);
	afx_msg void OnEnChangeEbdiodir();
	afx_msg void OnEnChangeEbpup2();
	afx_msg void OnBnClickedChkexpX();
	afx_msg void OnEnChangeEbpuda0();
	afx_msg void OnBnClickedChkhighz();
	afx_msg void OnEnChangeEbpuda1();
	afx_msg void OnEnChangeEbdigconfig();
	afx_msg void OnBnClickedChkendigstat();
	afx_msg void OnBnClickedChkenpulsectrstat();
	afx_msg void OnEnChangeEbadccount();
};
