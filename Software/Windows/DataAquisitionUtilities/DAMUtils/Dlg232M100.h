#pragma once
#include "Data232M100.h"
#include "Integrityioctrl1.h"
#include "afxwin.h"


// CDlg232M100 dialog

class CDlg232M100 : public CDialog
{
	DECLARE_DYNAMIC(CDlg232M100)

public:
	CDlg232M100(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg232M100();

// Dialog Data
	enum { IDD = IDD_DLG_232M100 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CData232M100 *m_cData;
	void SetDefaults(void);
	void DataToCtrl(void);
	void CtrlToData(void);

	bool ReadCurrent(void);
	void SetCurrent(void);
	bool WriteCurrent(void);

	void TypeToConfig(void);
	void RemoveConfigItem(void);


	CString m_csDataP2Dir;
	CString m_csP2PowerOn;
	CString m_csAsyncUpdate;
	CListBox m_lbAdcCfg;
	CString m_csAdcCount;
	BOOL m_bEnDigitalStatus;
	BOOL m_bEnPulseStatus;
	virtual BOOL OnInitDialog();
	CListBox m_lbAdcTypes;
	afx_msg void OnLbnDblclkLstAdctypesm1();
	afx_msg void OnBnClickedBtnAddm1();
	afx_msg void OnBnClickedBtnRemovem1();
	afx_msg void OnLbnDblclkLstAdccfgm1();
	afx_msg void OnBnClickedBtnUpm1();
	afx_msg void OnBnClickedBtnDownm1();
	afx_msg void OnEnChangeEbdiodirm1();
	afx_msg void OnEnChangeEbpupm1();
	afx_msg void OnEnChangeEbdigconfigm1();
	afx_msg void OnBnClickedChkendigstatm1();
	afx_msg void OnBnClickedChkenpulsectrstatm1();
	afx_msg void OnEnChangeEbadccountm1();
};
