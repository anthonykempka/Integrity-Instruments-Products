#pragma once
#include "Data232M300.h"
#include "Integrityioctrl1.h"
#include "afxwin.h"


// CDlg232M300 dialog

class CDlg232M300 : public CDialog
{
	DECLARE_DYNAMIC(CDlg232M300)

public:
	CDlg232M300(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg232M300();

// Dialog Data
	enum { IDD = IDD_DLG_232M300 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CData232M300 *m_cData;
	void DataToCtrl(void);
	CString m_csDataP1Dir;
	CString m_csDataP2Dir;
	BOOL m_bEXPFlag;
	CString m_csAsyncUpdate;
	CString m_csP1PowerOn;
	CString m_csP2PowerOn;
	CString m_csDAC0;
	CString m_csDAC1;
	BOOL m_bADCFs;
	CString m_csAdcCount;
	CListBox m_lbAdcCfg;
	BOOL m_bEnDigitalStatus;
	BOOL m_bEnPulseStatus;
	virtual BOOL OnInitDialog();
	CListBox m_lbAdcTypes;
	void SetDefaults(void);
	afx_msg void OnBnClickedBtnAddm3();
	afx_msg void OnBnClickedBtnRemovem3();
	afx_msg void OnBnClickedBtnUpm3();
	afx_msg void OnBnClickedBtnDownm3();
	void TypeToConfig(void);
	void RemoveConfigItem(void);
	afx_msg void OnLbnDblclkLstAdctypesm3();
	afx_msg void OnLbnDblclkLstAdccfgm3();
	void CtrlToData(void);
	bool WriteCurrent(void);
	bool ReadCurrent(void);
	void SetCurrent(void);
	afx_msg void OnEnChangeEbdiodir1m3();
	afx_msg void OnEnChangeEbdiodir2m3();
	afx_msg void OnEnChangeEbpup1m3();
	afx_msg void OnEnChangeEbpup2m4();
	afx_msg void OnBnClickedChkexpXm3();
	afx_msg void OnEnChangeEbpuda0m3();
	afx_msg void OnBnClickedChkhighzm3();
	afx_msg void OnEnChangeEbpuda1m3();
	afx_msg void OnEnChangeEbdigconfigm3();
	afx_msg void OnBnClickedChkendigstatm3();
	afx_msg void OnBnClickedChkenpulsectrstatm3();
	afx_msg void OnEnChangeEbadccountm3();
};
