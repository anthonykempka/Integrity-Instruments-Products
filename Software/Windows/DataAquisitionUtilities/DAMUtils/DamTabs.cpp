// DamTabs.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "DamTabs.h"
#include ".\damtabs.h"
#include "Dlg232M300.h"
#include "Dlg232M200.h"
#include "Dlg485M300.h"
#include "DlgUSBM400.h"
#include "Dlg232M100.h"


// CDamTabs

IMPLEMENT_DYNAMIC(CDamTabs, CTabCtrl)
CDamTabs::CDamTabs()
: m_pDoc(NULL)
, m_nPrevSel(-1)
, m_p232M2(NULL)
, m_p232M3(NULL)
, m_p485M3(NULL)
, m_pUsbM4(NULL)
, m_p232M1(NULL)
{
	m_DialogID[MOD_RS232M1] = IDD_DLG_232M100;
	m_DialogID[MOD_RS232] = IDD_DLG_232M300;
    m_DialogID[MOD_RS232M2] = IDD_DLG_232M200;
    m_DialogID[MOD_RS485] = IDD_DLG_485M300;
    m_DialogID[MOD_USB] = IDD_DLG_USBM400;
}

CDamTabs::~CDamTabs()
{
	if(m_p232M1) delete m_p232M1;
	if(m_p232M2) delete m_p232M2;
	if(m_p232M3) delete m_p232M3;
	if(m_p485M3) delete m_p485M3;
	if(m_pUsbM4) delete m_pUsbM4;
}


BEGIN_MESSAGE_MAP(CDamTabs, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnTcnSelchange)
END_MESSAGE_MAP()



// CDamTabs message handlers


void CDamTabs::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{

	// Activate the proper dialog procedure based on 
	// tab selection
	ActivateTabDialogs();

	// Inform the parent window of the change in tab selection.
	GetParent()->PostMessage(WM_TABSELCHANGE, 0, 0);

	*pResult = 0;


}

void CDamTabs::ActivateTabDialogs()
{
	int nSel = GetCurSel();
	CWnd* pWnd = GetParent();
	
	
	
	switch(m_nPrevSel)
	{
	case MOD_RS232M1:
		if(m_p232M1->m_hWnd)
			m_p232M1->DestroyWindow();
		delete m_p232M1;
		m_p232M1 = NULL;
		break;
	case MOD_RS232M2:
		if(m_p232M2->m_hWnd)
			m_p232M2->DestroyWindow();
		delete m_p232M2;
		m_p232M2 = NULL;
		break;
	case MOD_RS232:
		if(m_p232M3->m_hWnd)
			m_p232M3->DestroyWindow();
		delete m_p232M3;
		m_p232M3 = NULL;
		break;
	case MOD_RS485:
		if(m_p485M3->m_hWnd)
			m_p485M3->DestroyWindow();
		delete m_p485M3;
		m_p485M3 = NULL;
		break;
	case MOD_USB:
		if(m_pUsbM4->m_hWnd)
			m_pUsbM4->DestroyWindow();
		delete m_pUsbM4;
		m_pUsbM4 = NULL;
		break;
	default:
		break;
	}


   CRect l_rectClient;
   CRect l_rectWnd;

   GetClientRect(l_rectClient);
   AdjustRect(FALSE,l_rectClient);
   GetWindowRect(l_rectWnd);
   GetParent()->ScreenToClient(l_rectWnd);
   l_rectClient.OffsetRect(l_rectWnd.left,l_rectWnd.top);



	switch(nSel)
	{
	case MOD_RS232M1:
		m_p232M1 = new CDlg232M100();
		m_p232M1->m_cData = &m_pDoc->m_c232M100;
		m_p232M1->Create(IDD_DLG_232M100, pWnd);
		m_p232M1->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_SHOWWINDOW);
		m_nPrevSel = MOD_RS232M1;
		break;
	case MOD_RS232M2:
		m_p232M2 = new CDlg232M200();
		m_p232M2->m_cData = &m_pDoc->m_c232M200;
		m_p232M2->Create(IDD_DLG_232M200, pWnd);
		m_p232M2->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_SHOWWINDOW);
		m_nPrevSel = MOD_RS232M2;
		//m_p232M2->m_cData = &m_pDoc->m_c232M200;
		break;
	case MOD_RS232:
		m_p232M3 = new CDlg232M300(pWnd);
		m_p232M3->m_cData = &m_pDoc->m_c232M300;
		m_p232M3->Create(IDD_DLG_232M300, pWnd);
		m_p232M3->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_SHOWWINDOW);
		m_nPrevSel = MOD_RS232;
		break;
	case MOD_RS485:
		m_p485M3 = new CDlg485M300(pWnd);
		m_p485M3->m_cData = &m_pDoc->m_c485M300;
		m_p485M3->Create(IDD_DLG_485M300, pWnd);
		m_p485M3->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_SHOWWINDOW);
		m_nPrevSel = MOD_RS485;
		break;
	case MOD_USB:
		m_pUsbM4 = new CDlgUsbM400(pWnd);
		m_pUsbM4->m_cData = &m_pDoc->m_cUSBM400;
		m_pUsbM4->Create(IDD_DLG_USBM400, pWnd);
		m_pUsbM4->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_SHOWWINDOW);
		m_nPrevSel = MOD_USB;
		break;
	}
   

}

void CDamTabs::SetDocument(CDAMUtilsDoc* pDoc)
{
	m_pDoc = pDoc;
}

int CDamTabs::GetBaudRate(void)
{
	int rVal;
	ASSERT_VALID(m_pDoc);
	switch(GetCurSel())
	{
	case MOD_RS232M1:
		rVal = m_pDoc->m_c232M100.m_nBaudRate;
		break;
	case MOD_RS232M2:
		rVal = m_pDoc->m_c232M200.m_nBaudRate;
		break;
	case MOD_RS232:
		rVal = m_pDoc->m_c232M300.m_nBaudRate;
		break;
	case MOD_RS485:
		rVal = m_pDoc->m_c485M300.m_nBaudRate;
		break;
	case MOD_USB:
		rVal = m_pDoc->m_cUSBM400.m_nBaudRate;
		break;
	}
	return rVal;
}

CString CDamTabs::GetCommPort(void)
{
	ASSERT_VALID(m_pDoc);
	CString rVal;
	switch(GetCurSel())
	{
	case MOD_RS232M1:
		rVal = m_pDoc->m_c232M100.m_csCommPort;
		break;
	case MOD_RS232M2:
		rVal = m_pDoc->m_c232M200.m_csCommPort;
		break;
	case MOD_RS232:
		rVal = m_pDoc->m_c232M300.m_csCommPort;
		break;
	case MOD_RS485:
		rVal = m_pDoc->m_c485M300.m_csCommPort;
		break;
	case MOD_USB:
		rVal = m_pDoc->m_cUSBM400.m_csCommPort;
		break;
	}

	return rVal;
}

void CDamTabs::SetBaudRate(int baudRate)
{
	ASSERT_VALID(m_pDoc);

	switch(GetCurSel())
	{
	case MOD_RS232M1:
		m_pDoc->m_c232M100.m_nBaudRate = baudRate;
		m_pDoc->SetModifiedFlag(TRUE);
		break;
	case MOD_RS232M2:
		m_pDoc->m_c232M200.m_nBaudRate = baudRate;
		m_pDoc->SetModifiedFlag(TRUE);
		break;
	case MOD_RS232:
		m_pDoc->m_c232M300.m_nBaudRate = baudRate;
		m_pDoc->SetModifiedFlag(TRUE);
		break;
	case MOD_RS485:
		m_pDoc->m_c485M300.m_nBaudRate = baudRate;
		m_pDoc->SetModifiedFlag(TRUE);
		break;
	case MOD_USB:
		m_pDoc->m_cUSBM400.m_nBaudRate = baudRate;
		m_pDoc->SetModifiedFlag(TRUE);
		break;
	}
}

void CDamTabs::SetCommPort(CString commPort)
{
	ASSERT_VALID(m_pDoc);

	switch(GetCurSel())
	{
	case MOD_RS232M1:
		m_pDoc->m_c232M100.m_csCommPort = commPort;
		m_pDoc->SetModifiedFlag(TRUE);
		break;
	case MOD_RS232M2:
		m_pDoc->m_c232M200.m_csCommPort = commPort;
		m_pDoc->SetModifiedFlag(TRUE);
		break;
	case MOD_RS232:
		m_pDoc->m_c232M300.m_csCommPort = commPort;
		m_pDoc->SetModifiedFlag(TRUE);
		break;
	case MOD_RS485:
		m_pDoc->m_c485M300.m_csCommPort = commPort;
		m_pDoc->SetModifiedFlag(TRUE);
		break;
	case MOD_USB:
		m_pDoc->m_cUSBM400.m_csCommPort = commPort;
		m_pDoc->SetModifiedFlag(TRUE);
		break;
	}
}

bool CDamTabs::ReadCurrent(void)
{
	ASSERT_VALID(m_pDoc);
	

	switch(GetCurSel())
	{
	case MOD_RS232M1:
		m_p232M1->m_cData = &m_pDoc->m_c232M100;
		if(!m_p232M1->ReadCurrent())
		{
			MessageBox("Error Reading Current Values", "IOControl Error", MB_ICONERROR | MB_OK);
		}
		else
		{
			m_p232M1->SetCurrent();
		}
		break;
	case MOD_RS232M2:
		m_p232M2->m_cData = &m_pDoc->m_c232M200;
		if(!m_p232M2->ReadCurrent())
		{
			MessageBox("Error Reading Current Values", "IOControl Error", MB_ICONERROR | MB_OK);
		}
		else
		{
			m_p232M2->SetCurrent();
		}
		break;
	case MOD_RS232:
		m_p232M3->m_cData = &m_pDoc->m_c232M300;
		if(!m_p232M3->ReadCurrent())
		{
			MessageBox("Error Reading Current Values", "IOControl Error", MB_ICONERROR | MB_OK);
		}
		else
		{
			m_p232M3->SetCurrent();
		}
		break;
	case MOD_RS485:
		m_p485M3->m_cData = &m_pDoc->m_c485M300;
		if(!m_p485M3->ReadCurrent())
		{
			MessageBox("Error Reading Current Values", "IOControl Error", MB_ICONERROR | MB_OK);
		}
		else
		{
			m_p485M3->SetCurrent();
		}
		break;
	case MOD_USB:
		m_pUsbM4->m_cData = &m_pDoc->m_cUSBM400;
		if(!m_pUsbM4->ReadCurrent())
		{
			MessageBox("Error Reading Current Values", "IOControl Error", MB_ICONERROR | MB_OK);
		}
		else
		{
			m_pUsbM4->SetCurrent();
		}

		break;
	}

	return false;
}


void CDamTabs::SetDefaults(void)
{
	switch(GetCurSel())
	{
	case MOD_RS232M1:
		m_p232M1->m_cData = &m_pDoc->m_c232M100;
		m_p232M1->m_cData->EEReset();
		m_p232M1->SetDefaults();
		break;
	case MOD_RS232M2:
		m_p232M2->m_cData = &m_pDoc->m_c232M200;
		m_p232M2->m_cData->EEReset();
		m_p232M2->SetDefaults();
		break;
	case MOD_RS232:
		m_p232M3->m_cData = &m_pDoc->m_c232M300;
		m_p232M3->m_cData->EEReset();
		m_p232M3->SetDefaults();
		break;
	case MOD_RS485:
		m_p485M3->m_cData = &m_pDoc->m_c485M300;
		m_p485M3->m_cData->EEReset();
		m_p485M3->SetDefaults();
		break;
	case MOD_USB:
		m_pUsbM4->m_cData = &m_pDoc->m_cUSBM400;
		m_pUsbM4->m_cData->EEReset();
		m_pUsbM4->SetDefaults();
		break;
	}
}

void CDamTabs::SetCurrent(void)
{
	switch(GetCurSel())
	{
	case MOD_RS232M1:
		m_p232M1->m_cData = &m_pDoc->m_c232M100;
		if(!m_p232M1->ReadCurrent())
		{
			MessageBox("Error Reading Current Values", "IOControl Error", MB_ICONERROR | MB_OK);
		}
		else
		{
			m_p232M1->SetCurrent();
		}
		break;
	case MOD_RS232M2:
		m_p232M2->m_cData = &m_pDoc->m_c232M200;
		if(!m_p232M2->ReadCurrent())
		{
			MessageBox("Error Reading Current Values", "IOControl Error", MB_ICONERROR | MB_OK);
		}
		else
		{
			m_p232M2->SetCurrent();
		}
		break;
	case MOD_RS232:
		m_p232M3->m_cData = &m_pDoc->m_c232M300;
		if(!m_p232M3->ReadCurrent())
		{
			MessageBox("Error Reading Current Values", "IOControl Error", MB_ICONERROR | MB_OK);
		}
		else
		{
			m_p232M2->SetCurrent();
		}
		break;
	case MOD_RS485:
		m_p485M3->m_cData = &m_pDoc->m_c485M300;
		if(!m_p485M3->ReadCurrent())
		{
			MessageBox("Error Reading Current Values", "IOControl Error", MB_ICONERROR | MB_OK);
		}
		else
		{
			m_p485M3->SetCurrent();
		}
		break;
	case MOD_USB:
		m_pUsbM4->m_cData = &m_pDoc->m_cUSBM400;
		if(!m_pUsbM4->ReadCurrent())
		{
			MessageBox("Error Reading Current Values", "IOControl Error", MB_ICONERROR | MB_OK);
		}
		else
		{
			m_pUsbM4->SetCurrent();
		}
		break;
	}

}

void CDamTabs::WriteCurrentValues(void)
{
	switch(GetCurSel())
	{
	case MOD_RS232M1:
		m_p232M1->m_cData = &m_pDoc->m_c232M100;
		m_p232M1->WriteCurrent();
		break;
	case MOD_RS232M2:
		m_p232M2->m_cData = &m_pDoc->m_c232M200;
		m_p232M2->WriteCurrent();
		break;
	case MOD_RS232:
		m_p232M3->m_cData = &m_pDoc->m_c232M300;
		m_p232M3->WriteCurrent();
		break;
	case MOD_RS485:
		m_p485M3->m_cData = &m_pDoc->m_c485M300;
		m_p485M3->WriteCurrent();
		break;
	case MOD_USB:
		m_pUsbM4->m_cData = &m_pDoc->m_cUSBM400;
		m_pUsbM4->WriteCurrent();
		break;
	}
}

