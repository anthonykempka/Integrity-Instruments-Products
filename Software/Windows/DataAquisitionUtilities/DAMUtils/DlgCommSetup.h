#pragma once


// CDlgCommSetup dialog

class CDlgCommSetup : public CDialog
{
	DECLARE_DYNAMIC(CDlgCommSetup)

public:
	CDlgCommSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCommSetup();

// Dialog Data
	enum { IDD = IDD_DLG_COMMSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_csCommPort;
	CString m_csBaudRate;
};
