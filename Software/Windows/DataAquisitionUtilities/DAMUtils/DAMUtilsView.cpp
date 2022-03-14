// DAMUtilsView.cpp : implementation of the CDAMUtilsView class
//

#include "stdafx.h"
#include "DAMUtils.h"

#include "DAMUtilsDoc.h"
#include "DAMUtilsView.h"
#include ".\damutilsview.h"

#include ".\ModuleTypes.h"
#include ".\DlgCommSetup.h"
#include ".\DlgUsbStreamManager.h"
#include ".\232M100Log.h"
#include ".\232M200Log.h"
#include ".\232M300Log.h"
#include ".\485M300Log.h"
#include ".\USBM400Log.h"
#include <htmlhelp.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDAMUtilsView

IMPLEMENT_DYNCREATE(CDAMUtilsView, CFormView)

BEGIN_MESSAGE_MAP(CDAMUtilsView, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_READCURRENT, OnBnClickedBtnReadcurrent)
	ON_BN_CLICKED(IDC_BTN_CONFIGURE, OnBnClickedBtnConfigure)
	ON_BN_CLICKED(IDC_BTN_DEFAULTS, OnBnClickedBtnDefaults)
	ON_BN_CLICKED(IDC_BTN_COMMSETUP, OnBnClickedBtnCommsetup)
	ON_MESSAGE(WM_TABSELCHANGE, OnTabSelChange)
	ON_COMMAND(ID_TOOLS_STREAMMODE, OnToolsStreammode)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_STREAMMODE, OnUpdateToolsStreammode)
	ON_COMMAND(ID_TOOLS_DATALOGGER, OnToolsDatalogger)
	ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)
	ON_COMMAND(ID_HELP_INDEX32778, OnHelpIndex32778)
END_MESSAGE_MAP()

// CDAMUtilsView construction/destruction

CDAMUtilsView::CDAMUtilsView()
	: CFormView(CDAMUtilsView::IDD)
	, m_binDirectory(_T(""))
{
	// TODO: add construction code here
	char cwd[512];

	getcwd(cwd, sizeof(cwd));

	m_binDirectory.Format("%s", cwd);

}

CDAMUtilsView::~CDAMUtilsView()
{
}

void CDAMUtilsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_cDamTabs);

	DDX_Control(pDX, IDC_INTEGRITYIOCTRL1, m_axIICtrl);
}

BOOL CDAMUtilsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CDAMUtilsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	// Add tabs to our Tab Control
	m_cDamTabs.InsertItem(MOD_RS232M1, "232M100");
	m_cDamTabs.InsertItem(MOD_RS232M2, "232M200");
	m_cDamTabs.InsertItem(MOD_RS232, "232M300");
	m_cDamTabs.InsertItem(MOD_RS485, "485M300");
	m_cDamTabs.InsertItem(MOD_USB, "USBM400");

	
	// Make sure the the current objects document pointer is current
	CDAMUtilsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	m_cDamTabs.SetDocument(pDoc);
	
	// Set the Active X control pointer(s) 
	pDoc->m_c232M100.m_cIICtrl = &m_axIICtrl;
	pDoc->m_c232M200.m_cIICtrl = &m_axIICtrl;
	pDoc->m_c232M300.m_cIICtrl = &m_axIICtrl;
	pDoc->m_c485M300.m_cIICtrl = &m_axIICtrl;
	pDoc->m_cUSBM400.m_cIICtrl = &m_axIICtrl;

	m_cDamTabs.ActivateTabDialogs();

}
/*
 *	Message handler / Posted by the tab control object
 */
LRESULT CDAMUtilsView::OnTabSelChange(WPARAM wParam, LPARAM lParam)
{
	CButton *pBtn;
	pBtn = (CButton*)GetDlgItem(IDC_BTN_COMMSETUP);
	switch(m_cDamTabs.GetCurSel())
	{
	case MOD_RS232M1:
		pBtn->EnableWindow(TRUE);
		break;
	case MOD_RS232M2:
		pBtn->EnableWindow(TRUE);
		break;
	case MOD_RS232:
		pBtn->EnableWindow(TRUE);
		break;
	case MOD_RS485:
		pBtn->EnableWindow(TRUE);
		break;
	case MOD_USB:
		pBtn->EnableWindow(FALSE);
		break;
	}

	return 0L;
}
// CDAMUtilsView diagnostics

#ifdef _DEBUG
void CDAMUtilsView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDAMUtilsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDAMUtilsDoc* CDAMUtilsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDAMUtilsDoc)));
	return (CDAMUtilsDoc*)m_pDocument;
}
#endif //_DEBUG


// CDAMUtilsView message handlers

void CDAMUtilsView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}



void CDAMUtilsView::OnBnClickedBtnReadcurrent()
{
	m_cDamTabs.ReadCurrent();
}

void CDAMUtilsView::OnBnClickedBtnConfigure()
{
	// TODO: Add your control notification handler code here
	m_cDamTabs.WriteCurrentValues();

}

void CDAMUtilsView::OnBnClickedBtnDefaults()
{
	// TODO: Add your control notification handler code here
	m_cDamTabs.SetDefaults();

}

/*
 * Get communication parameters from the user.
 */
void CDAMUtilsView::OnBnClickedBtnCommsetup()
{
	CDlgCommSetup cDlg;
	CDAMUtilsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	cDlg.m_csBaudRate.Format("%d", m_cDamTabs.GetBaudRate());		
	cDlg.m_csCommPort = m_cDamTabs.GetCommPort();

	if(IDOK == cDlg.DoModal())
	{
		m_cDamTabs.SetBaudRate(atoi(cDlg.m_csBaudRate));
		m_cDamTabs.SetCommPort(cDlg.m_csCommPort);
	}
}

/*******************************************************
 *
 * When Stream mode is enabled, a modal dialog will
 * be used for configuration, as well as denying access
 * to any other module functions until the stream
 * mode is turned off.
 ********************************************************/
void CDAMUtilsView::OnToolsStreammode()
{
	CDAMUtilsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	BOOL modeStream;

	// Toggle Stream Modes
	modeStream = m_axIICtrl.GetbModeStream();

	if(!modeStream)
	{
		switch(m_cDamTabs.GetCurSel())
		{
		case MOD_RS232M1:
			break;
		case MOD_RS232M2:
			break;
		case MOD_RS232:
			break;
		case MOD_RS485:
			break;
		case MOD_USB:
			{
				CDlgUsbStreamManager cDlg;			// Construct our dialog procedure
				cDlg.m_cIICtrl = &m_axIICtrl;		// Set ActiveX control pointer.

				m_axIICtrl.SetlnProtocol(3);
				m_axIICtrl.SetnADCBits(14);
				m_axIICtrl.SetdADCScale(1.0);

				switch(pDoc->m_cUSBM400.GetAnalogRefrence())
				{
				case pDoc->m_cUSBM400.REF_115:
					m_axIICtrl.SetdReferenceVoltage(1.15);
					cDlg.m_dGraphMax = 1.15;
					break;
				case pDoc->m_cUSBM400.REF_2048:
					m_axIICtrl.SetdReferenceVoltage(2.048);
					cDlg.m_dGraphMax = 1.048;
					break;
				case pDoc->m_cUSBM400.REF_25:
					m_axIICtrl.SetdReferenceVoltage(2.5);
					cDlg.m_dGraphMax = 2.5;
					break;
				case pDoc->m_cUSBM400.REF_EXT:
					m_axIICtrl.SetdReferenceVoltage(5.0);
					cDlg.m_dGraphMax = 5.0;
					break;
				}


				cDlg.m_nUnitAddress = GetDocument()->m_cUSBM400.GetModuleAddress();
				cDlg.DoModal();						// Start the procedure
			}
			break;
		}	
	}
	else
	{
		m_axIICtrl.SetbModeStream(FALSE);
	}
	
}

void CDAMUtilsView::OnUpdateToolsStreammode(CCmdUI *pCmdUI)
{
	// Set the enabled state based on Tab Setting
	switch(m_cDamTabs.GetCurSel())
	{
	case MOD_RS232M1:
		pCmdUI->Enable(FALSE);
		break;
	case MOD_RS232M2:
		pCmdUI->Enable(FALSE);
		break;
	case MOD_RS232:
		pCmdUI->Enable(FALSE);
		break;
	case MOD_RS485:
		pCmdUI->Enable(FALSE);
		break;
	case MOD_USB:
		pCmdUI->Enable(TRUE);
		break;
	}	

	/*
	// Check with the activeX control to see if the 
	// stream mode is active.
	if(m_axIICtrl.GetbModeStream())
	{
		pCmdUI->SetCheck(TRUE);
		
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
		
	}
	*/
}

void CDAMUtilsView::OnToolsDatalogger()
{
	C232M100Log cDlgM100;
	C232M200Log cDlgM200;
	C232M300Log cDlgM300;
	C485M300Log cDlgM300_485;
	CUSBM400Log cDlgM400;

	CDAMUtilsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	switch(m_cDamTabs.GetCurSel())
	{
	case MOD_RS232M1:
		cDlgM100.m_cIICtrl = &m_axIICtrl;		// Set ActiveX control pointer.
		m_axIICtrl.SetlnComPort(atol((char*)(LPCTSTR)m_cDamTabs.GetCommPort().MakeReverse()));
		m_axIICtrl.SetlnBaudRate(m_cDamTabs.GetBaudRate());
		m_axIICtrl.SetlnProtocol(1);
		m_axIICtrl.SetnADCBits(10);
		m_axIICtrl.SetdADCScale(2.0);
		m_axIICtrl.SetdReferenceVoltage(5.0);
		cDlgM100.DoModal();
		break;
	case MOD_RS232M2:
		cDlgM200.m_cIICtrl = &m_axIICtrl;
		m_axIICtrl.SetlnComPort(atol((char*)(LPCTSTR)m_cDamTabs.GetCommPort().MakeReverse()));
		m_axIICtrl.SetlnBaudRate(m_cDamTabs.GetBaudRate());
		m_axIICtrl.SetlnProtocol(1);
		m_axIICtrl.SetnADCBits(12);
		m_axIICtrl.SetdADCScale(1.0);
		m_axIICtrl.SetdReferenceVoltage(5.0);
		cDlgM200.DoModal();
		break;
	case MOD_RS232:
		cDlgM300.m_cIICtrl = &m_axIICtrl;
		m_axIICtrl.SetlnComPort(atol((char*)(LPCTSTR)m_cDamTabs.GetCommPort().MakeReverse()));
		m_axIICtrl.SetlnBaudRate(m_cDamTabs.GetBaudRate());
		m_axIICtrl.SetlnProtocol(1);
		m_axIICtrl.SetnADCBits(12);
		m_axIICtrl.SetdADCScale(1.0);
		m_axIICtrl.SetdReferenceVoltage(5.0);
		cDlgM300.DoModal();
		break;

	case MOD_RS485:
		cDlgM300_485.m_cIICtrl = &m_axIICtrl;
		m_axIICtrl.SetlnComPort(atol((char*)(LPCTSTR)m_cDamTabs.GetCommPort().MakeReverse()));
		m_axIICtrl.SetlnBaudRate(m_cDamTabs.GetBaudRate());
		m_axIICtrl.SetlnProtocol(2);
		m_axIICtrl.SetnADCBits(12);
		m_axIICtrl.SetdADCScale(1.0);
		m_axIICtrl.SetdReferenceVoltage(5.0);
		cDlgM300_485.DoModal();
		break;
	case MOD_USB:
		cDlgM400.m_cIICtrl = &m_axIICtrl;
		m_axIICtrl.SetlnProtocol(3);
		m_axIICtrl.SetnADCBits(14);
		m_axIICtrl.SetdADCScale(1.0);
		switch(pDoc->m_cUSBM400.GetAnalogRefrence())
		{
		case pDoc->m_cUSBM400.REF_115:
			m_axIICtrl.SetdReferenceVoltage(1.15);
			break;
		case pDoc->m_cUSBM400.REF_2048:
			m_axIICtrl.SetdReferenceVoltage(2.048);
			break;
		case pDoc->m_cUSBM400.REF_25:
			m_axIICtrl.SetdReferenceVoltage(2.5);
			break;
		case pDoc->m_cUSBM400.REF_EXT:
			m_axIICtrl.SetdReferenceVoltage(5.0);
			break;
		}

		cDlgM400.DoModal();
		break;


	}
}

/*
 * No online help was initially created, so hack it in manually
 */
void CDAMUtilsView::OnHelpContents()
{
	CString helpPath;
	helpPath.Format("%s\\damhelp.chm", m_binDirectory);
	::HtmlHelp(GetSafeHwnd(), helpPath, HH_DISPLAY_TOC, 0);	
}

void CDAMUtilsView::OnHelpIndex32778()
{
	CString helpPath;
	helpPath.Format("%s\\damhelp.chm", m_binDirectory);
	::HtmlHelp(GetSafeHwnd(),helpPath, HH_DISPLAY_INDEX, 0);	
}

