// Dlg232M200.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "Dlg232M200.h"
#include ".\dlg232m200.h"


// CDlg232M200 dialog

IMPLEMENT_DYNAMIC(CDlg232M200, CDialog)
CDlg232M200::CDlg232M200(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg232M200::IDD, pParent)
	, m_cData(NULL)
	, m_csDataP2Dir(_T(""))
	, m_csAsyncUpdate(_T(""))
	, m_csP2PowerOn(_T(""))
	, m_csDAC0(_T(""))
	, m_csDAC1(_T(""))
	, m_bADCFs(FALSE)
	, m_bEXPFlag(FALSE)
	, m_csAdcCount(_T(""))
	, m_bEnDigitalStatus(FALSE)
	, m_bEnPulseStatus(FALSE)
{
}

CDlg232M200::~CDlg232M200()
{
}

void CDlg232M200::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EBDIODIR, m_csDataP2Dir);
	DDX_Text(pDX, IDC_EBDIGCONFIG, m_csAsyncUpdate);
	DDX_Text(pDX, IDC_EBPUP2, m_csP2PowerOn);
	DDX_Text(pDX, IDC_EBPUDA0, m_csDAC0);
	DDX_Text(pDX, IDC_EBPUDA1, m_csDAC1);
	DDX_Check(pDX, IDC_CHKHIGHZ, m_bADCFs);
	DDX_Control(pDX, IDC_LST_ADCTYPES, m_lbAdcTypes);
	DDX_Control(pDX, IDC_LST_ADCCFG, m_lbAdcCfg);
	DDX_Check(pDX, IDC_CHKEXP_X, m_bEXPFlag);
	DDX_Text(pDX, IDC_EBADCCOUNT, m_csAdcCount);
	DDX_Check(pDX, IDC_CHKENDIGSTAT, m_bEnDigitalStatus);
	DDX_Check(pDX, IDC_CHKENPULSECTRSTAT, m_bEnPulseStatus);
}

BEGIN_MESSAGE_MAP(CDlg232M200, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_REMOVE, OnBnClickedBtnRemove)
	ON_LBN_DBLCLK(IDC_LST_ADCTYPES, OnLbnDblclkLstAdctypes)
	ON_BN_CLICKED(IDC_BTN_UP, OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBnClickedBtnDown)
	ON_LBN_DBLCLK(IDC_LST_ADCCFG, OnLbnDblclkLstAdccfg)
	ON_EN_CHANGE(IDC_EBDIODIR, OnEnChangeEbdiodir)
	ON_EN_CHANGE(IDC_EBPUP2, OnEnChangeEbpup2)
	ON_BN_CLICKED(IDC_CHKEXP_X, OnBnClickedChkexpX)
	ON_EN_CHANGE(IDC_EBPUDA0, OnEnChangeEbpuda0)
	ON_BN_CLICKED(IDC_CHKHIGHZ, OnBnClickedChkhighz)
	ON_EN_CHANGE(IDC_EBPUDA1, OnEnChangeEbpuda1)
	ON_EN_CHANGE(IDC_EBDIGCONFIG, OnEnChangeEbdigconfig)
	ON_BN_CLICKED(IDC_CHKENDIGSTAT, OnBnClickedChkendigstat)
	ON_BN_CLICKED(IDC_CHKENPULSECTRSTAT, OnBnClickedChkenpulsectrstat)
	ON_EN_CHANGE(IDC_EBADCCOUNT, OnEnChangeEbadccount)
END_MESSAGE_MAP()

// CDlg232M200 message handlers


bool CDlg232M200::ReadCurrent(void)
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
bool CDlg232M200::WriteCurrent(void)
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

/*
 * Translate the EEProm Image data to UI Controls
 */
void CDlg232M200::DataToCtrl(void)
{
	int i;

	//if(!m_cData) return;

	// Data Directrion Port
	m_csDataP2Dir.Format("%02X", m_cData->GetDIODirection()); 
	
	// EXP Invert
	m_bEXPFlag = (m_cData->GetExpanderBdFlag()) ? TRUE : FALSE;

	// Async Update Mode
	m_csAsyncUpdate.Format("%04X", m_cData->GetAsyncUDMode()); 

	// Port 2 power on default
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
			m_lbAdcCfg.SetItemData(i, CData232M200::DIFFERENTIAL_CH0P);
			break;
		case m_cData->DIFFERENTIAL_CH1P:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH0N_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M200::DIFFERENTIAL_CH1P);
			break;
		case m_cData->SINGLEPOINT_CH0:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH0_TEXT);			
			m_lbAdcCfg.SetItemData(i, CData232M200::SINGLEPOINT_CH0);
			break;
		case m_cData->SINGLEPOINT_CH1:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH1_TEXT);
			m_lbAdcCfg.SetItemData(i, CData232M200::SINGLEPOINT_CH1);
			break;
		}
	}

	// Enable Digital Status
	m_bEnDigitalStatus = m_cData->GetContDIctrl() ? TRUE : FALSE;

	// Enable Pulse Status
	m_bEnPulseStatus = m_cData->GetContPCctrl() ? TRUE : FALSE;

	
	
	UpdateData(FALSE);
}

void CDlg232M200::CtrlToData(void)
{
	UpdateData(TRUE);
	char *stringEnd;
	int i;

	m_cData->SetDIODirection(strtol((LPCTSTR)m_csDataP2Dir, &stringEnd, 16));
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


BOOL CDlg232M200::OnInitDialog()
{
	CDialog::OnInitDialog();

	// --
	// -- Add choices to the list. 
	// -- The item data contains the actual 
	// -- Data Point that will be sent to the unit
	// --
	m_lbAdcTypes.AddString(SIGNLEPOINT_CH0_TEXT);			
	m_lbAdcTypes.SetItemData(0, CData232M200::SINGLEPOINT_CH0);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH1_TEXT);
	m_lbAdcTypes.SetItemData(1, CData232M200::SINGLEPOINT_CH1);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH0P_TEXT);
	m_lbAdcTypes.SetItemData(2, CData232M200::DIFFERENTIAL_CH0P);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH0N_TEXT);
	m_lbAdcTypes.SetItemData(3, CData232M200::DIFFERENTIAL_CH1P);

	DataToCtrl();



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg232M200::OnBnClickedBtnAdd()
{
	TypeToConfig();
	CtrlToData();
}

void CDlg232M200::OnBnClickedBtnRemove()
{
	RemoveConfigItem();
	CtrlToData();
}

void CDlg232M200::OnLbnDblclkLstAdctypes()
{
	TypeToConfig();
	CtrlToData();
}

void CDlg232M200::OnLbnDblclkLstAdccfg()
{
	RemoveConfigItem();
	CtrlToData();
}

void CDlg232M200::OnBnClickedBtnUp()
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

void CDlg232M200::OnBnClickedBtnDown()
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

void CDlg232M200::TypeToConfig(void)
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
		if(m_lbAdcCfg.GetCount() < CData232M200::MAX_STREAM_ITEMS)
		{
			// We havn't reached the maximum number of config
			// items, so it's save to add the text and item data.
			curSel = m_lbAdcCfg.AddString(cs);
			m_lbAdcCfg.SetItemData(curSel, itemData); 

		}
	}
}


void CDlg232M200::RemoveConfigItem(void)
{
	int curSel;
	curSel = m_lbAdcCfg.GetCurSel();
	if(curSel != LB_ERR)
	{
		m_lbAdcCfg.DeleteString(curSel);
		m_lbAdcCfg.SetCurSel(curSel);
	}
}

void CDlg232M200::SetDefaults(void)
{
	DataToCtrl();
}

void CDlg232M200::SetCurrent(void)
{
	DataToCtrl();
}



void CDlg232M200::OnEnChangeEbdiodir()
{
	CtrlToData();
}

void CDlg232M200::OnEnChangeEbpup2()
{
	CtrlToData();
}

void CDlg232M200::OnBnClickedChkexpX()
{
	CtrlToData();
}

void CDlg232M200::OnEnChangeEbpuda0()
{
	CtrlToData();
}

void CDlg232M200::OnBnClickedChkhighz()
{
	CtrlToData();
}

void CDlg232M200::OnEnChangeEbpuda1()
{
	CtrlToData();
}

void CDlg232M200::OnEnChangeEbdigconfig()
{
	CtrlToData();
}

void CDlg232M200::OnBnClickedChkendigstat()
{
	CtrlToData();
}

void CDlg232M200::OnBnClickedChkenpulsectrstat()
{
	CtrlToData();
}

void CDlg232M200::OnEnChangeEbadccount()
{
	CtrlToData();
}
