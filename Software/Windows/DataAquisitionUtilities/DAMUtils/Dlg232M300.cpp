// Dlg232M300.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "Dlg232M300.h"
#include ".\dlg232m300.h"


// CDlg232M300 dialog

IMPLEMENT_DYNAMIC(CDlg232M300, CDialog)
CDlg232M300::CDlg232M300(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg232M300::IDD, pParent)
	, m_cData(NULL)
	, m_csDataP1Dir(_T(""))
	, m_csDataP2Dir(_T(""))
	, m_bEXPFlag(FALSE)
	, m_csAsyncUpdate(_T(""))
	, m_csP1PowerOn(_T(""))
	, m_csP2PowerOn(_T(""))
	, m_csDAC0(_T(""))
	, m_csDAC1(_T(""))
	, m_bADCFs(FALSE)
	, m_csAdcCount(_T(""))
	, m_bEnDigitalStatus(FALSE)
	, m_bEnPulseStatus(FALSE)
{
}

CDlg232M300::~CDlg232M300()
{
}

void CDlg232M300::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EBDIODIR1M3, m_csDataP1Dir);
	DDX_Text(pDX, IDC_EBDIODIR2M3, m_csDataP2Dir);
	DDX_Check(pDX, IDC_CHKEXP_XM3, m_bEXPFlag);
	DDX_Text(pDX, IDC_EBDIGCONFIGM3, m_csAsyncUpdate);
	DDX_Text(pDX, IDC_EBPUP1M3, m_csP1PowerOn);
	DDX_Text(pDX, IDC_EBPUP2M4, m_csP2PowerOn);
	DDX_Text(pDX, IDC_EBPUDA0M3, m_csDAC0);
	DDX_Text(pDX, IDC_EBPUDA1M3, m_csDAC1);
	DDX_Check(pDX, IDC_CHKHIGHZM3, m_bADCFs);
	DDX_Text(pDX, IDC_EBADCCOUNTM3, m_csAdcCount);
	DDX_Control(pDX, IDC_LST_ADCCFGM3, m_lbAdcCfg);
	DDX_Check(pDX, IDC_CHKENDIGSTATM3, m_bEnDigitalStatus);
	DDX_Check(pDX, IDC_CHKENPULSECTRSTATM3, m_bEnPulseStatus);
	DDX_Control(pDX, IDC_LST_ADCTYPESM3, m_lbAdcTypes);
}


BEGIN_MESSAGE_MAP(CDlg232M300, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADDM3, OnBnClickedBtnAddm3)
	ON_BN_CLICKED(IDC_BTN_REMOVEM3, OnBnClickedBtnRemovem3)
	ON_BN_CLICKED(IDC_BTN_UPM3, OnBnClickedBtnUpm3)
	ON_BN_CLICKED(IDC_BTN_DOWNM3, OnBnClickedBtnDownm3)
	ON_LBN_DBLCLK(IDC_LST_ADCTYPESM3, OnLbnDblclkLstAdctypesm3)
	ON_LBN_DBLCLK(IDC_LST_ADCCFGM3, OnLbnDblclkLstAdccfgm3)
	ON_EN_CHANGE(IDC_EBDIODIR1M3, OnEnChangeEbdiodir1m3)
	ON_EN_CHANGE(IDC_EBDIODIR2M3, OnEnChangeEbdiodir2m3)
	ON_EN_CHANGE(IDC_EBPUP1M3, OnEnChangeEbpup1m3)
	ON_EN_CHANGE(IDC_EBPUP2M4, OnEnChangeEbpup2m4)
	ON_BN_CLICKED(IDC_CHKEXP_XM3, OnBnClickedChkexpXm3)
	ON_EN_CHANGE(IDC_EBPUDA0M3, OnEnChangeEbpuda0m3)
	ON_BN_CLICKED(IDC_CHKHIGHZM3, OnBnClickedChkhighzm3)
	ON_EN_CHANGE(IDC_EBPUDA1M3, OnEnChangeEbpuda1m3)
	ON_EN_CHANGE(IDC_EBDIGCONFIGM3, OnEnChangeEbdigconfigm3)
	ON_BN_CLICKED(IDC_CHKENDIGSTATM3, OnBnClickedChkendigstatm3)
	ON_BN_CLICKED(IDC_CHKENPULSECTRSTATM3, OnBnClickedChkenpulsectrstatm3)
	ON_EN_CHANGE(IDC_EBADCCOUNTM3, OnEnChangeEbadccountm3)
END_MESSAGE_MAP()


// CDlg232M300 message handlers

void CDlg232M300::DataToCtrl(void)
{
	int i;

	//if(!m_cData) return;

	// Data Directrion Port
	m_csDataP1Dir.Format("%02X", m_cData->GetDIO1Direction());
	m_csDataP2Dir.Format("%02X", m_cData->GetDIO2Direction());
	
	// EXP Invert
	m_bEXPFlag = (m_cData->GetExpanderBdFlag()) ? TRUE : FALSE;

	// Async Update Mode
	m_csAsyncUpdate.Format("%04X", m_cData->GetAsyncUDMode()); 

	// Port 1..2 power on default
	m_csP1PowerOn.Format("%02X", m_cData->GetP1PwrOnDefault()); 
	m_csP2PowerOn.Format("%02X", m_cData->GetP2PwrOnDefault()); 


	// DAC0 Power On default
	m_csDAC0.Format("%04X", m_cData->GetDAC0Pod());

	// DAC1 Power On default
	m_csDAC1.Format("%04X", m_cData->GetDAC1Pod());

	// A/D SAmple Rate Clock
	m_bADCFs = m_cData->GetADCFs() ? TRUE : FALSE;

	// continuous stram analog configuration count
	m_lbAdcCfg.ResetContent();
	m_csAdcCount.Format("%02X", m_cData->GetContStrAC());
	//for(i=0; (i<m_cData->MAX_STREAM_ITEMS) && (i < m_cData->GetContStrAC()); i++)
	for(i=0; (i<m_cData->MAX_STREAM_ITEMS); i++)
	{
		switch(m_cData->GetAnalogQryCtrl(i))
		{
		case m_cData->DIFFERENTIAL_CH0P:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH0P_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::DIFFERENTIAL_CH0P);
			break;
		case m_cData->DIFFERENTIAL_CH2P:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH2P_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::DIFFERENTIAL_CH2P);
			break;
		case m_cData->DIFFERENTIAL_CH4P:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH4P_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::DIFFERENTIAL_CH4P);
			break;
		case m_cData->DIFFERENTIAL_CH6P:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH6P_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::DIFFERENTIAL_CH6P);
			break;
		case m_cData->DIFFERENTIAL_CH0N:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH0N_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::DIFFERENTIAL_CH0N);
			break;
		case m_cData->DIFFERENTIAL_CH2N:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH2N_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::DIFFERENTIAL_CH2N);
			break;
		case m_cData->DIFFERENTIAL_CH4N:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH4N_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::DIFFERENTIAL_CH4N);
			break;
		case m_cData->DIFFERENTIAL_CH6N:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH6N_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::DIFFERENTIAL_CH6N);
			break;
		case m_cData->SINGLEPOINT_CH0:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH0_TEXT);			
			m_lbAdcCfg.SetItemData(i, CData232M300::SINGLEPOINT_CH0);
			break;
		case m_cData->SINGLEPOINT_CH1:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH1_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::SINGLEPOINT_CH1);
			break;
		case m_cData->SINGLEPOINT_CH2:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH2_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::SINGLEPOINT_CH2);
			break;
		case m_cData->SINGLEPOINT_CH3:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH3_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::SINGLEPOINT_CH3);
			break;
		case m_cData->SINGLEPOINT_CH4:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH4_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::SINGLEPOINT_CH4);
			break;
		case m_cData->SINGLEPOINT_CH5:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH5_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::SINGLEPOINT_CH5);
			break;
		case m_cData->SINGLEPOINT_CH6:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH6_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::SINGLEPOINT_CH6);
			break;
		case m_cData->SINGLEPOINT_CH7:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH7_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M300::SINGLEPOINT_CH7);
			break;
		}
	}

	// Enable Digital Status
	m_bEnDigitalStatus = m_cData->GetContDIctrl() ? TRUE : FALSE;

	// Enable Pulse Status
	m_bEnPulseStatus = m_cData->GetContPCctrl() ? TRUE : FALSE;

	
	
	UpdateData(FALSE);

}
void CDlg232M300::CtrlToData(void)
{
	UpdateData(TRUE);
	char *stringEnd;
	int i;

	m_cData->SetDIO1Direction(strtol((LPCTSTR)m_csDataP1Dir, &stringEnd, 16));
	m_cData->SetDIO2Direction(strtol((LPCTSTR)m_csDataP2Dir, &stringEnd, 16));
	m_cData->SetP1PwrOnDefault(strtol((LPCTSTR)m_csP1PowerOn, &stringEnd, 16));
	m_cData->SetP2PwrOnDefault(strtol((LPCTSTR)m_csP2PowerOn, &stringEnd, 16));
	m_cData->SetDAC0Pod(strtol((LPCTSTR)m_csDAC0, &stringEnd, 16));
	m_cData->SetDAC1Pod(strtol((LPCTSTR)m_csDAC1, &stringEnd, 16));
	m_cData->SetExpanderBdFlag(m_bEXPFlag ? m_cData->EXP_YES : m_cData->EXP_NO);
	m_cData->SetADCFs(m_bADCFs ? m_cData->EBF_SLOW : m_cData->EBF_NORMAL);
	m_cData->SetAsyncUDMode(strtol((LPCTSTR)m_csAsyncUpdate, &stringEnd, 16));
	m_cData->SetContDIctrl( m_bEnDigitalStatus ? m_cData->DS_ON : m_cData->DS_OFF);
	m_cData->SetContPCctrl( m_bEnPulseStatus ? m_cData->DP_ON : m_cData->DP_OFF);

	m_cData->SetAnalogQueryCount(strtol((LPCTSTR)m_csAdcCount, &stringEnd, 16));

	for(i=0; i < m_lbAdcCfg.GetCount(); i++)
	{
		m_cData->SetAnalogQryCtrl(i, (int)m_lbAdcCfg.GetItemData(i));
	}

}

BOOL CDlg232M300::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH0_TEXT);			
	m_lbAdcTypes.SetItemData(0, CData232M300::SINGLEPOINT_CH0);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH1_TEXT);
	m_lbAdcTypes.SetItemData(1, CData232M300::SINGLEPOINT_CH1);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH2_TEXT);
	m_lbAdcTypes.SetItemData(2, CData232M300::SINGLEPOINT_CH2);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH3_TEXT);
	m_lbAdcTypes.SetItemData(3, CData232M300::SINGLEPOINT_CH3);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH4_TEXT);
	m_lbAdcTypes.SetItemData(4, CData232M300::SINGLEPOINT_CH4);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH5_TEXT);
	m_lbAdcTypes.SetItemData(5, CData232M300::SINGLEPOINT_CH5);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH6_TEXT);
	m_lbAdcTypes.SetItemData(6, CData232M300::SINGLEPOINT_CH6);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH7_TEXT);
	m_lbAdcTypes.SetItemData(7, CData232M300::SINGLEPOINT_CH7);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH0P_TEXT);
	m_lbAdcTypes.SetItemData(8, CData232M300::DIFFERENTIAL_CH0P);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH2P_TEXT);
	m_lbAdcTypes.SetItemData(9, CData232M300::DIFFERENTIAL_CH2P);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH4P_TEXT);
	m_lbAdcTypes.SetItemData(10, CData232M300::DIFFERENTIAL_CH4P);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH6P_TEXT);
	m_lbAdcTypes.SetItemData(11, CData232M300::DIFFERENTIAL_CH6P);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH0N_TEXT);
	m_lbAdcTypes.SetItemData(12, CData232M300::DIFFERENTIAL_CH0N);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH2N_TEXT);
	m_lbAdcTypes.SetItemData(13, CData232M300::DIFFERENTIAL_CH2N);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH4N_TEXT);
	m_lbAdcTypes.SetItemData(14, CData232M300::DIFFERENTIAL_CH4N);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH6N_TEXT);
	m_lbAdcTypes.SetItemData(15, CData232M300::DIFFERENTIAL_CH6N);

	DataToCtrl(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg232M300::SetDefaults(void)
{
	DataToCtrl();
	CtrlToData();
}

void CDlg232M300::OnBnClickedBtnAddm3()
{
	TypeToConfig();
	CtrlToData();
}

void CDlg232M300::OnBnClickedBtnRemovem3()
{
	RemoveConfigItem();
	CtrlToData();
}

void CDlg232M300::OnLbnDblclkLstAdctypesm3()
{
	TypeToConfig();
	CtrlToData();
}

void CDlg232M300::OnLbnDblclkLstAdccfgm3()
{
	RemoveConfigItem();
	CtrlToData();
}

void CDlg232M300::OnBnClickedBtnUpm3()
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

void CDlg232M300::OnBnClickedBtnDownm3()
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

void CDlg232M300::TypeToConfig(void)
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
		if(m_lbAdcCfg.GetCount() < CData232M300::MAX_STREAM_ITEMS)
		{
			// We havn't reached the maximum number of config
			// items, so it's save to add the text and item data.
			curSel = m_lbAdcCfg.AddString(cs);
			m_lbAdcCfg.SetItemData(curSel, itemData); 

		}
	}

}

void CDlg232M300::RemoveConfigItem(void)
{
	int curSel;
	curSel = m_lbAdcCfg.GetCurSel();
	if(curSel != LB_ERR)
	{
		m_lbAdcCfg.DeleteString(curSel);
		m_lbAdcCfg.SetCurSel(curSel);
	}

}



bool CDlg232M300::WriteCurrent(void)
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

			// Verify EE Data
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

bool CDlg232M300::ReadCurrent(void)
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

void CDlg232M300::SetCurrent(void)
{
	DataToCtrl();
}

void CDlg232M300::OnEnChangeEbdiodir1m3()
{
	CtrlToData();
}

void CDlg232M300::OnEnChangeEbdiodir2m3()
{
	CtrlToData();
}

void CDlg232M300::OnEnChangeEbpup1m3()
{
	CtrlToData();
}

void CDlg232M300::OnEnChangeEbpup2m4()
{
	CtrlToData();
}

void CDlg232M300::OnBnClickedChkexpXm3()
{
	CtrlToData();
}

void CDlg232M300::OnEnChangeEbpuda0m3()
{
	CtrlToData();
}

void CDlg232M300::OnBnClickedChkhighzm3()
{
	CtrlToData();
}

void CDlg232M300::OnEnChangeEbpuda1m3()
{
	CtrlToData();
}

void CDlg232M300::OnEnChangeEbdigconfigm3()
{
	CtrlToData();
}

void CDlg232M300::OnBnClickedChkendigstatm3()
{
	CtrlToData();
}

void CDlg232M300::OnBnClickedChkenpulsectrstatm3()
{
	CtrlToData();
}

void CDlg232M300::OnEnChangeEbadccountm3()
{
	CtrlToData();
}
