#pragma once
#include "Data485M300.h"
#include "Integrityioctrl1.h"
#include "afxwin.h"


// CDlg485M300 dialog

class CDlg485M300 : public CDialog
{
	DECLARE_DYNAMIC(CDlg485M300)

public:
	CDlg485M300(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg485M300();

// Dialog Data
	enum { IDD = IDD_DLG_485M300 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_csModuleAddress;
	CString m_csDataP1Dir;
	CString m_csDataP2Dir;
	CString m_csP1PowerOn;
	CString m_csP2PowerOn;
	BOOL m_bEXPFlag;
	CString m_csDAC0;
	BOOL m_bADCFs;
	CString m_csDAC1;


	CData485M300 *m_cData;
	void DataToCtrl(void);
	void CtrlToData(void);
	void SetDefaults(void);
	bool WriteCurrent(void);
	bool ReadCurrent(void);
	void SetCurrent(void);


	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEbmodaddr485();
	afx_msg void OnEnChangeEbdiodir1m3485();
	afx_msg void OnEnChangeEbdiodir2m3485();
	afx_msg void OnEnChangeEbpup1m3485();
	afx_msg void OnEnChangeEbpup2m4485();
	afx_msg void OnBnClickedChkexpXm3485();
	afx_msg void OnEnChangeEbpuda0m3485();
	afx_msg void OnBnClickedChkhighzm3485();
	afx_msg void OnEnChangeEbpuda1m3485();
};
