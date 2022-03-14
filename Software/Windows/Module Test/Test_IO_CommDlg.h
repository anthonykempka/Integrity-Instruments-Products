#if !defined(AFX_TEST_IO_COMMDLG_H__F83BA9AF_AF80_11D2_9134_444553540000__INCLUDED_)
#define AFX_TEST_IO_COMMDLG_H__F83BA9AF_AF80_11D2_9134_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Test_IO_CommDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTest_IO_CommDlg dialog

class CTest_IO_CommDlg : public CDialog
{
// Construction
public:
	int nComPort;
	int nBaudRate;
	CTest_IO_CommDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTest_IO_CommDlg)
	enum { IDD = IDD_COMM_DIALOG };
	CComboBox	m_BaudRate;
	CComboBox	m_Comport;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_IO_CommDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTest_IO_CommDlg)
	afx_msg void OnCommOk();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_IO_COMMDLG_H__F83BA9AF_AF80_11D2_9134_444553540000__INCLUDED_)
