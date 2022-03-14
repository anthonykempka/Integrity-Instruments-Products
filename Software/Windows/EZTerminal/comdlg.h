#if !defined(AFX_COMDLG_H__3E503D15_83B9_11D2_9134_444553540000__INCLUDED_)
#define AFX_COMDLG_H__3E503D15_83B9_11D2_9134_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// comdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// comdlg dialog

class comdlg : public CDialog
{
// Construction
public:
	int m_nBaudIndex;
	int m_nParityIndex;
	int m_nDataBitIndex;
	int m_nComportIndex;
	int m_nStopBitIndex;
	comdlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(comdlg)
	enum { IDD = IDD_COMMSETTINGS };
	CComboBox	m_StopBits;
	CComboBox	m_Parity;
	CComboBox	m_DataBits;
	CComboBox	m_Comport;
	CComboBox	m_BaudRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(comdlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(comdlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMDLG_H__3E503D15_83B9_11D2_9134_444553540000__INCLUDED_)
