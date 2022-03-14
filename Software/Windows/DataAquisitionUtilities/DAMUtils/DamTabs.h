#pragma once
#include ".\ModuleTypes.h"
#include "DAMUtilsDoc.h"
#include "Dlg232M200.h"
#include "Dlg232M300.h"
#include "Dlg485M300.h"
#include "DlgUsbM400.h"
#include "Dlg232M100.h"

#define WM_TABSELCHANGE WM_USER+100


// CDamTabs

class CDamTabs : public CTabCtrl
{
	DECLARE_DYNAMIC(CDamTabs)

public:
	CDamTabs();
	virtual ~CDamTabs();

protected:
	DECLARE_MESSAGE_MAP()
	int m_DialogID[MOD_NUMTYPES];
	CDialog m_DialogFocus;

	CDlg232M100* m_p232M1;
	CDlg232M200* m_p232M2;
	CDlg232M300* m_p232M3;
	CDlg485M300* m_p485M3;
	CDlgUsbM400* m_pUsbM4;


public:
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	void ActivateTabDialogs();
	void SetDocument(CDAMUtilsDoc* pDoc);
protected:
	CDAMUtilsDoc* m_pDoc;
public:
	int GetBaudRate(void);
	CString GetCommPort(void);
	void SetBaudRate(int baudRate);
	void SetCommPort(CString commPort);
	bool ReadCurrent(void);
	int m_nPrevSel;
	void SetDefaults(void);
	void SetCurrent(void);
	void WriteCurrentValues(void);
	void SetTabEnabled(bool value);
protected:

};


