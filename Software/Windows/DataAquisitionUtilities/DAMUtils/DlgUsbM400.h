#pragma once
#include "DataUsbM400.h"
//#include "Integrityioctrl1.h"
#include "afxwin.h"
//#include "afxcoll.h"


// CDlgUsbM400 dialog

class CDlgUsbM400 : public CDialog
{
	DECLARE_DYNAMIC(CDlgUsbM400)

public:
	CDlgUsbM400(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUsbM400();

// Dialog Data
	enum { IDD = IDD_DLG_USBM400 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//CListBox m_lbAdcTypes;
	CDataUsbM400 *m_cData;
	void DataToCtrl(void);
	CString m_csDataP1Dir;
	CString m_csDataP2Dir;
	BOOL m_bEXPFlag;
	CString m_csP1PowerOn;
	CString m_csP2PowerOn;
	CString m_csDAC0;
	CString m_csDAC1;
	BOOL m_bADCFs;
	//CListBox m_lbAdcCfg;
	void CtrlToData(void);
	bool WriteCurrent(void);
	bool ReadCurrent(void);
	void SetCurrent(void);
	void SetDefaults(void);
	CString m_csModuleAddress;
	CComboBox m_cbADCRefrence;
	CListBox m_lbAdcTypes;
	CListBox m_lbAdcCfg;
	CListBox m_lbAdcCfgGain;
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnLbnSelchangeList3();
	afx_msg void OnBnClickedBtnAddusbstream();
	afx_msg void OnBnClickedBtnRemoveusbstream();
	afx_msg void OnBnClickedBtnUsbup();
	afx_msg void OnBnClickedBtnusbdown();
	void TypeToConfig(void);
	afx_msg void OnLbnDblclkList1();
	afx_msg void OnLbnDblclkList2();
	afx_msg void OnEnChangeEbmodaddru4();
	afx_msg void OnEnChangeEbdiodir1u4();
	afx_msg void OnEnChangeEbdiodir2u4();
	afx_msg void OnEnChangeEbpup1u4();
	afx_msg void OnEnChangeEbpup2u4();
	afx_msg void OnBnClickedChkexpXu4();
	afx_msg void OnEnChangeEbpuda0u4();
	afx_msg void OnBnClickedChkhighzu4();
	afx_msg void OnEnChangeEbpuda1u4();
	afx_msg void OnCbnSelchangeCbAdcrefu4();
};
