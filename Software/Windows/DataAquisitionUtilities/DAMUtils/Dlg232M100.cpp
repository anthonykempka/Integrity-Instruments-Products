// Dlg232M100.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "Dlg232M100.h"
#include ".\dlg232m100.h"


// CDlg232M100 dialog

IMPLEMENT_DYNAMIC(CDlg232M100, CDialog)
CDlg232M100::CDlg232M100(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg232M100::IDD, pParent)
	, m_csDataP2Dir(_T(""))
	, m_csP2PowerOn(_T(""))
	, m_csAsyncUpdate(_T(""))
	, m_csAdcCount(_T(""))
	, m_bEnDigitalStatus(FALSE)
	, m_bEnPulseStatus(FALSE)
{
}

CDlg232M100::~CDlg232M100()
{
}

void CDlg232M100::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EBDIODIRM1, m_csDataP2Dir);
	DDX_Text(pDX, IDC_EBPUPM1, m_csP2PowerOn);
	DDX_Text(pDX, IDC_EBDIGCONFIGM1, m_csAsyncUpdate);
	DDX_Control(pDX, IDC_LST_ADCCFGM1, m_lbAdcCfg);
	DDX_Text(pDX, IDC_EBADCCOUNTM1, m_csAdcCount);
	DDX_Check(pDX, IDC_CHKENDIGSTATM1, m_bEnDigitalStatus);
	DDX_Check(pDX, IDC_CHKENPULSECTRSTATM1, m_bEnPulseStatus);
	DDX_Control(pDX, IDC_LST_ADCTYPESM1, m_lbAdcTypes);
}


BEGIN_MESSAGE_MAP(CDlg232M100, CDialog)
	ON_LBN_DBLCLK(IDC_LST_ADCTYPESM1, OnLbnDblclkLstAdctypesm1)
	ON_BN_CLICKED(IDC_BTN_ADDM1, OnBnClickedBtnAddm1)
	ON_BN_CLICKED(IDC_BTN_REMOVEM1, OnBnClickedBtnRemovem1)
	ON_LBN_DBLCLK(IDC_LST_ADCCFGM1, OnLbnDblclkLstAdccfgm1)
	ON_BN_CLICKED(IDC_BTN_UPM1, OnBnClickedBtnUpm1)
	ON_BN_CLICKED(IDC_BTN_DOWNM1, OnBnClickedBtnDownm1)
	ON_EN_CHANGE(IDC_EBDIODIRM1, OnEnChangeEbdiodirm1)
	ON_EN_CHANGE(IDC_EBPUPM1, OnEnChangeEbpupm1)
	ON_EN_CHANGE(IDC_EBDIGCONFIGM1, OnEnChangeEbdigconfigm1)
	ON_BN_CLICKED(IDC_CHKENDIGSTATM1, OnBnClickedChkendigstatm1)
	ON_BN_CLICKED(IDC_CHKENPULSECTRSTATM1, OnBnClickedChkenpulsectrstatm1)
	ON_EN_CHANGE(IDC_EBADCCOUNTM1, OnEnChangeEbadccountm1)
END_MESSAGE_MAP()


// CDlg232M100 message handlers



void CDlg232M100::SetDefaults(void)
{
	DataToCtrl();
	CtrlToData();
}

void CDlg232M100::DataToCtrl(void)
{
	int i;

	//if(!m_cData) return;

	// Data Directrion Port
	m_csDataP2Dir.Format("%02X", m_cData->GetDIO2Direction());
	
	// Async Update Mode
	m_csAsyncUpdate.Format("%04X", m_cData->GetAsyncUDMode()); 

	// Port 1..2 power on default
	m_csP2PowerOn.Format("%02X", m_cData->GetP2PwrOnDefault()); 



	// continuous stram analog configuration count
	m_lbAdcCfg.ResetContent();
	m_csAdcCount.Format("%02X", m_cData->GetContStrAC());
	//for(i=0; (i<m_cData->MAX_STREAM_ITEMS) && (i < m_cData->GetContStrAC()); i++)
	for(i=0; (i<m_cData->MAX_STREAM_ITEMS); i++)
	{
		switch(m_cData->GetAnalogQryCtrl(i))
		{
		case m_cData->SINGLEPOINT_CH0:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH0_TEXT);			
			m_lbAdcCfg.SetItemData(i, CData232M100::SINGLEPOINT_CH0);
			break;
		case m_cData->SINGLEPOINT_CH1:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH1_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M100::SINGLEPOINT_CH1);
			break;
		case m_cData->SINGLEPOINT_CH2:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH2_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M100::SINGLEPOINT_CH2);
			break;
		case m_cData->SINGLEPOINT_CH3:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH3_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M100::SINGLEPOINT_CH3);
			break;
		case m_cData->SINGLEPOINT_CH4:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH4_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M100::SINGLEPOINT_CH4);
			break;
		case m_cData->SINGLEPOINT_CH5:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH5_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M100::SINGLEPOINT_CH5);
			break;
		case m_cData->SINGLEPOINT_CH6:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH6_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M100::SINGLEPOINT_CH6);
			break;
		case m_cData->SINGLEPOINT_CH7:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH7_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M100::SINGLEPOINT_CH7);
			break;
		}
	}

	// Enable Digital Status
	m_bEnDigitalStatus = m_cData->GetContDIctrl() ? TRUE : FALSE;

	// Enable Pulse Status
	m_bEnPulseStatus = m_cData->GetContPCctrl() ? TRUE : FALSE;

	
	
	UpdateData(FALSE);

}
void CDlg232M100::CtrlToData(void)
{
	UpdateData(TRUE);
	char *stringEnd;
	int i;

	m_cData->SetDIO2Direction(strtol((LPCTSTR)m_csDataP2Dir, &stringEnd, 16));
	m_cData->SetP2PwrOnDefault(strtol((LPCTSTR)m_csP2PowerOn, &stringEnd, 16));
	m_cData->SetAsyncUDMode(strtol((LPCTSTR)m_csAsyncUpdate, &stringEnd, 16));
	m_cData->SetContDIctrl( m_bEnDigitalStatus ? m_cData->DS_ON : m_cData->DS_OFF);
	m_cData->SetContPCctrl( m_bEnPulseStatus ? m_cData->DP_ON : m_cData->DP_OFF);

	m_cData->SetAnalogQueryCount(strtol((LPCTSTR)m_csAdcCount, &stringEnd, 16));

	for(i=0; i < m_lbAdcCfg.GetCount(); i++)
	{
		m_cData->SetAnalogQryCtrl(i, (int)m_lbAdcCfg.GetItemData(i));
	}
}


BOOL CDlg232M100::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lbAdcTypes.ResetContent();

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH0_TEXT);			
	m_lbAdcTypes.SetItemData(0, CData232M100::SINGLEPOINT_CH0);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH1_TEXT);
	m_lbAdcTypes.SetItemData(1, CData232M100::SINGLEPOINT_CH1);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH2_TEXT);
	m_lbAdcTypes.SetItemData(2, CData232M100::SINGLEPOINT_CH2);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH3_TEXT);
	m_lbAdcTypes.SetItemData(3, CData232M100::SINGLEPOINT_CH3);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH4_TEXT);
	m_lbAdcTypes.SetItemData(4, CData232M100::SINGLEPOINT_CH4);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH5_TEXT);
	m_lbAdcTypes.SetItemData(5, CData232M100::SINGLEPOINT_CH5);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH6_TEXT);
	m_lbAdcTypes.SetItemData(6, CData232M100::SINGLEPOINT_CH6);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH7_TEXT);
	m_lbAdcTypes.SetItemData(7, CData232M100::SINGLEPOINT_CH7);

	DataToCtrl(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
bool CDlg232M100::ReadCurrent(void)
{
	int eeCount, eeLoc, eeData;
	bool rVal = true;

	// Set the comm port
	m_cData->m_cIICtrl->SetlnComPort(atol(m_cData->m_csCommPort.MakeReverse()));
	m_cData->m_csCommPort.MakeReverse();		// Fix the string reversal for the atoi

	// Set the baud rate
	m_cData->m_cIICtrl->SetlnBaudRate(m_cData->m_nBaudRate);
    
	// Set Communicatio Protocol
	m_cData->m_cIICtrl->SetlnProtocol(m_cData->PROTOCOL);
	
	// Open communications
	m_cData->m_cIICtrl->SetbPortOpen(TRUE);

	// Read back the EE data
	eeCount = (int)m_cData->m_cbaEEData.GetCount();
	for(eeLoc = 0; eeLoc < eeCount; eeLoc++)
	{
		// Set address to read from EEProm
		m_cData->m_cIICtrl->SetlnEEPROMAddress(eeLoc);
		// Read the EEPROM data and store in local array
		eeData = m_cData->m_cIICtrl->GetlnEEPROMData();
		if(eeData >=0)
		{
			m_cData->m_cbaEEData.SetAt(eeLoc, eeData);
		}
		else
		{
			rVal = false;
			break;
		}
	}
	
	m_cData->m_cIICtrl->SetbPortOpen(FALSE);
	
	return rVal;
}

bool CDlg232M100::WriteCurrent(void)
{
	int eeCount, eeLoc, eeData;
	bool rVal = true;
	CString csErrors, csTemp;

	// Update data object
	CtrlToData();
	
	// Set the comm port
	m_cData->m_cIICtrl->SetlnComPort(atol(m_cData->m_csCommPort.MakeReverse()));
	m_cData->m_csCommPort.MakeReverse();		// Fix the string reversal for the atoi

	// Set the baud rate
	m_cData->m_cIICtrl->SetlnBaudRate(m_cData->m_nBaudRate);
   	m_cData->m_cIICtrl->SetlnProtocol(m_cData->PROTOCOL);

	// Open communications
	m_cData->m_cIICtrl->SetbPortOpen(TRUE);

	if(m_cData->m_cIICtrl->GetbPortOpen())
	{
		eeCount = (int)m_cData->m_cbaEEData.GetCount();
		for(eeLoc = 0; eeLoc < eeCount; eeLoc++)
		{
			// Set address to read from EEProm
			m_cData->m_cIICtrl->SetlnEEPROMAddress(eeLoc);
			
			m_cData->m_cIICtrl->SetlnEEPROMData(m_cData->m_cbaEEData[eeLoc]);
			eeData = (int)m_cData->m_cIICtrl->GetlnEEPROMData();
			if(eeData != (int)m_cData->m_cbaEEData[eeLoc])
			{
				csTemp.Format("   EE Validation Error at: 0x%02X\n", eeLoc);
				csErrors += csTemp;
				rVal = false;
				break;
			}
		}

		/*
		csErrors = "";

		// Verify EE Data
		for(eeLoc = 0; eeLoc < eeCount; eeLoc++)
		{
			m_cData->m_cIICtrl->SetlnEEPROMAddress(eeLoc);
			eeData = (int)m_cData->m_cIICtrl->GetlnEEPROMData();
			if(eeData != (int)m_cData->m_cbaEEData[eeLoc])
			{
				csTemp.Format("   EE Validation Error at: 0x%02X\n", eeLoc);
				csErrors += csTemp;
				rVal = false;
			}
		}
		*/

		m_cData->m_cIICtrl->SetbPortOpen(FALSE);
		
		if(!rVal)
		{
			AfxMessageBox("Error configuring unit!\n\n" + csErrors);
		}
		else
		{
			AfxMessageBox("Unit configuration complete!");
		}

	}

	return rVal;
}

void CDlg232M100::SetCurrent(void)
{
	DataToCtrl();
}

void CDlg232M100::TypeToConfig(void)
{
	int curSel;
	DWORD itemData;
	CString cs;
	curSel = m_lbAdcTypes.GetCurSel();
	if(curSel != LB_ERR)
	{
		// Get the text and item data from the source object
		m_lbAdcTypes.GetText(curSel, cs);
		itemData = (DWORD)m_lbAdcTypes.GetItemData(curSel);

		// Check for maximum configuration items
		if(m_lbAdcCfg.GetCount() < CData232M100::MAX_STREAM_ITEMS)
		{
			// We havn't reached the maximum number of config
			// items, so it's save to add the text and item data.
			curSel = m_lbAdcCfg.AddString(cs);
			m_lbAdcCfg.SetItemData(curSel, itemData); 

		}
	}

}
void CDlg232M100::RemoveConfigItem(void)
{
	int curSel;
	curSel = m_lbAdcCfg.GetCurSel();
	if(curSel != LB_ERR)
	{
		m_lbAdcCfg.DeleteString(curSel);
		m_lbAdcCfg.SetCurSel(curSel);
	}

}

void CDlg232M100::OnLbnDblclkLstAdctypesm1()
{
	TypeToConfig();
	CtrlToData();
}
void CDlg232M100::OnLbnDblclkLstAdccfgm1()
{
	RemoveConfigItem();
	CtrlToData();
}

void CDlg232M100::OnBnClickedBtnAddm1()
{
	TypeToConfig();
	CtrlToData();
}

void CDlg232M100::OnBnClickedBtnRemovem1()
{
	RemoveConfigItem();
	CtrlToData();
}


void CDlg232M100::OnBnClickedBtnUpm1()
{
	int curSel;
	DWORD itemData;
	CString cs;

	curSel = m_lbAdcCfg.GetCurSel();
	if(curSel != LB_ERR)
	{
		// An item is selected ...
		if(curSel > 0)
		{
			// and it's not at the top of the list
			m_lbAdcCfg.GetText(curSel, cs);
			itemData = (DWORD)m_lbAdcCfg.GetItemData(curSel);
			
			// Remove this item from the list box
			m_lbAdcCfg.DeleteString(curSel);

			// Move to a lower selection
			curSel--;
			
			// Now insert the item
			m_lbAdcCfg.InsertString(curSel, cs);
			m_lbAdcCfg.SetItemData(curSel, itemData);

			// Keep Selection
			m_lbAdcCfg.SetCurSel(curSel);
			
		}
	}
	CtrlToData();
}

void CDlg232M100::OnBnClickedBtnDownm1()
{
	int curSel;
	DWORD itemData;
	CString cs;

	curSel = m_lbAdcCfg.GetCurSel();
	if(curSel != LB_ERR)
	{
		// An item is selected ...
		if(curSel < m_lbAdcCfg.GetCount() - 1)
		{
			// and it's not at the end of the list
			m_lbAdcCfg.GetText(curSel, cs);
			itemData = (DWORD)m_lbAdcCfg.GetItemData(curSel);
			
			// Remove this item from the list box
			m_lbAdcCfg.DeleteString(curSel);

			// Move to a lower selection
			curSel++;
			
			// Now insert the item
			m_lbAdcCfg.InsertString(curSel, cs);
			m_lbAdcCfg.SetItemData(curSel, itemData);
			
			// Keep Selection
			m_lbAdcCfg.SetCurSel(curSel);
		}
	}
	CtrlToData();
}

void CDlg232M100::OnEnChangeEbdiodirm1()
{
	CtrlToData();
}

void CDlg232M100::OnEnChangeEbpupm1()
{
	CtrlToData();
}

void CDlg232M100::OnEnChangeEbdigconfigm1()
{
	CtrlToData();
}

void CDlg232M100::OnBnClickedChkendigstatm1()
{
	CtrlToData();
}

void CDlg232M100::OnBnClickedChkenpulsectrstatm1()
{
	CtrlToData();
}

void CDlg232M100::OnEnChangeEbadccountm1()
{
	CtrlToData();
}
