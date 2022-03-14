// DlgUsbM400.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "DlgUsbM400.h"
#include ".\dlgusbm400.h"
#include "usbstreamdata.h"

// CDlgUsbM400 dialog

IMPLEMENT_DYNAMIC(CDlgUsbM400, CDialog)
CDlgUsbM400::CDlgUsbM400(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUsbM400::IDD, pParent)
	, m_cData(NULL)
	, m_csDataP1Dir(_T(""))
	, m_csDataP2Dir(_T(""))
	, m_bEXPFlag(FALSE)
	, m_csP1PowerOn(_T(""))
	, m_csP2PowerOn(_T(""))
	, m_csDAC0(_T(""))
	, m_csDAC1(_T(""))
	, m_bADCFs(FALSE)
	, m_csModuleAddress(_T(""))
{
}

CDlgUsbM400::~CDlgUsbM400()
{
}

void CDlgUsbM400::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EBDIODIR1U4, m_csDataP1Dir);
	DDX_Text(pDX, IDC_EBDIODIR2U4, m_csDataP2Dir);
	DDX_Check(pDX, IDC_CHKEXP_XU4, m_bEXPFlag);
	DDX_Text(pDX, IDC_EBPUP1U4, m_csP1PowerOn);
	DDX_Text(pDX, IDC_EBPUP2U4, m_csP2PowerOn);
	DDX_Text(pDX, IDC_EBPUDA0U4, m_csDAC0);
	DDX_Text(pDX, IDC_EBPUDA1U4, m_csDAC1);
	DDX_Check(pDX, IDC_CHKHIGHZU4, m_bADCFs);
	DDX_Text(pDX, IDC_EBMODADDRU4, m_csModuleAddress);
	DDX_Control(pDX, IDC_CB_ADCREFU4, m_cbADCRefrence);
	DDX_Control(pDX, IDC_LIST1, m_lbAdcTypes);
	DDX_Control(pDX, IDC_LIST2, m_lbAdcCfg);
	DDX_Control(pDX, IDC_LIST3, m_lbAdcCfgGain);
}


BEGIN_MESSAGE_MAP(CDlgUsbM400, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST2, OnLbnSelchangeList2)
	ON_LBN_SELCHANGE(IDC_LIST3, OnLbnSelchangeList3)
	ON_BN_CLICKED(IDC_BTN_ADDUSBSTREAM, OnBnClickedBtnAddusbstream)
	ON_BN_CLICKED(IDC_BTN_REMOVEUSBSTREAM, OnBnClickedBtnRemoveusbstream)
	ON_BN_CLICKED(IDC_BTN_USBUP, OnBnClickedBtnUsbup)
	ON_BN_CLICKED(IDC_BTNUSBDOWN, OnBnClickedBtnusbdown)
	ON_LBN_DBLCLK(IDC_LIST1, OnLbnDblclkList1)
	ON_LBN_DBLCLK(IDC_LIST2, OnLbnDblclkList2)
	ON_EN_CHANGE(IDC_EBMODADDRU4, OnEnChangeEbmodaddru4)
	ON_EN_CHANGE(IDC_EBDIODIR1U4, OnEnChangeEbdiodir1u4)
	ON_EN_CHANGE(IDC_EBDIODIR2U4, OnEnChangeEbdiodir2u4)
	ON_EN_CHANGE(IDC_EBPUP1U4, OnEnChangeEbpup1u4)
	ON_EN_CHANGE(IDC_EBPUP2U4, OnEnChangeEbpup2u4)
	ON_BN_CLICKED(IDC_CHKEXP_XU4, OnBnClickedChkexpXu4)
	ON_EN_CHANGE(IDC_EBPUDA0U4, OnEnChangeEbpuda0u4)
	ON_BN_CLICKED(IDC_CHKHIGHZU4, OnBnClickedChkhighzu4)
	ON_EN_CHANGE(IDC_EBPUDA1U4, OnEnChangeEbpuda1u4)
	ON_CBN_SELCHANGE(IDC_CB_ADCREFU4, OnCbnSelchangeCbAdcrefu4)
END_MESSAGE_MAP()


// CDlgUsbM400 message handlers

BOOL CDlgUsbM400::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH0_TEXT);			
	m_lbAdcTypes.SetItemData(0, CDataUsbM400::SINGLEPOINT_CH0);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH1_TEXT);
	m_lbAdcTypes.SetItemData(1, CDataUsbM400::SINGLEPOINT_CH1);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH2_TEXT);
	m_lbAdcTypes.SetItemData(2, CDataUsbM400::SINGLEPOINT_CH2);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH3_TEXT);
	m_lbAdcTypes.SetItemData(3, CDataUsbM400::SINGLEPOINT_CH3);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH4_TEXT);
	m_lbAdcTypes.SetItemData(4, CDataUsbM400::SINGLEPOINT_CH4);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH5_TEXT);
	m_lbAdcTypes.SetItemData(5, CDataUsbM400::SINGLEPOINT_CH5);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH6_TEXT);
	m_lbAdcTypes.SetItemData(6, CDataUsbM400::SINGLEPOINT_CH6);

	m_lbAdcTypes.AddString(SIGNLEPOINT_CH7_TEXT);
	m_lbAdcTypes.SetItemData(7, CDataUsbM400::SINGLEPOINT_CH7);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH0P_TEXT);
	m_lbAdcTypes.SetItemData(8, CDataUsbM400::DIFFERENTIAL_CH0P);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH2P_TEXT);
	m_lbAdcTypes.SetItemData(9, CDataUsbM400::DIFFERENTIAL_CH2P);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH4P_TEXT);
	m_lbAdcTypes.SetItemData(10, CDataUsbM400::DIFFERENTIAL_CH4P);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH6P_TEXT);
	m_lbAdcTypes.SetItemData(11, CDataUsbM400::DIFFERENTIAL_CH6P);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH0N_TEXT);
	m_lbAdcTypes.SetItemData(12, CDataUsbM400::DIFFERENTIAL_CH0N);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH2N_TEXT);
	m_lbAdcTypes.SetItemData(13, CDataUsbM400::DIFFERENTIAL_CH2N);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH4N_TEXT);
	m_lbAdcTypes.SetItemData(14, CDataUsbM400::DIFFERENTIAL_CH4N);

	m_lbAdcTypes.AddString(DIFFERENTIAL_CH6N_TEXT);
	m_lbAdcTypes.SetItemData(15, CDataUsbM400::DIFFERENTIAL_CH6N);

	m_lbAdcCfgGain.AddString(ADC_GAIN_TEXT_1);
	m_lbAdcCfgGain.SetItemData(0, CDataUsbM400::ADCGAIN_1);
	m_lbAdcCfgGain.AddString(ADC_GAIN_TEXT_2);
	m_lbAdcCfgGain.SetItemData(1, CDataUsbM400::ADCGAIN_2);
	m_lbAdcCfgGain.AddString(ADC_GAIN_TEXT_4);
	m_lbAdcCfgGain.SetItemData(2, CDataUsbM400::ADCGAIN_4);
	m_lbAdcCfgGain.AddString(ADC_GAIN_TEXT_5);
	m_lbAdcCfgGain.SetItemData(3, CDataUsbM400::ADCGAIN_5);
	m_lbAdcCfgGain.AddString(ADC_GAIN_TEXT_8);
	m_lbAdcCfgGain.SetItemData(4, CDataUsbM400::ADCGAIN_8);
	m_lbAdcCfgGain.AddString(ADC_GAIN_TEXT_10);
	m_lbAdcCfgGain.SetItemData(5, CDataUsbM400::ADCGAIN_10);
	m_lbAdcCfgGain.AddString(ADC_GAIN_TEXT_16);
	m_lbAdcCfgGain.SetItemData(6, CDataUsbM400::ADCGAIN_16);
	m_lbAdcCfgGain.AddString(ADC_GAIN_TEXT_20);
	m_lbAdcCfgGain.SetItemData(7, CDataUsbM400::ADCGAIN_20);


	m_cbADCRefrence.AddString("1.150 (ADC Internal)");
	m_cbADCRefrence.SetItemData(0, m_cData->REF_115);
	m_cbADCRefrence.AddString("2.048 (ADC Internal)");
	m_cbADCRefrence.SetItemData(1, m_cData->REF_2048);
	m_cbADCRefrence.AddString("2.500 (ADC Internal)");
	m_cbADCRefrence.SetItemData(2, m_cData->REF_25);
	m_cbADCRefrence.AddString("ADC External");
	m_cbADCRefrence.SetItemData(3, m_cData->REF_EXT);
	m_cbADCRefrence.SetCurSel(0);
	DataToCtrl(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgUsbM400::DataToCtrl(void)
{
	int i;

	//if(!m_cData) return;
	m_csModuleAddress.Format("%02X", m_cData->GetModuleAddress());

	// Data Directrion Port
	m_csDataP1Dir.Format("%02X", m_cData->GetDIO1Direction());
	m_csDataP2Dir.Format("%02X", m_cData->GetDIO2Direction());
	
	// EXP Invert
	m_bEXPFlag = (m_cData->GetExpanderBdFlag()) ? TRUE : FALSE;


	// Port 1..2 power on default
	m_csP1PowerOn.Format("%02X", m_cData->GetP1PwrOnDefault()); 
	m_csP2PowerOn.Format("%02X", m_cData->GetP2PwrOnDefault()); 


	// DAC0 Power On default
	m_csDAC0.Format("%04X", m_cData->GetDAC0Pod());

	// DAC1 Power On default
	m_csDAC1.Format("%04X", m_cData->GetDAC1Pod());

	// A/D SAmple Rate Clock
	m_bADCFs = m_cData->GetADCFs() ? TRUE : FALSE;

	// A/D Refrence
	switch(m_cData->GetAnalogRefrence())
	{
	case m_cData->REF_115:
		m_cbADCRefrence.SetCurSel(0);
		break;
	case m_cData->REF_2048:
		m_cbADCRefrence.SetCurSel(1);
		break;
	case m_cData->REF_25:
		m_cbADCRefrence.SetCurSel(2);
		break;
	case m_cData->REF_EXT:
		m_cbADCRefrence.SetCurSel(3);
		break;
	}
	// continuous stram analog configuration count
	m_lbAdcCfg.ResetContent();
	
	//for(i=0; (i<m_cData->MAX_STREAM_ITEMS) && (i < m_cData->GetContStrAC()); i++)
	for(i=0; (i<m_cData->MAX_STREAM_ITEMS); i++)
	{
		switch(m_cData->GetAnalogQryCtrl(i) & 0x0F)
		{
		case m_cData->DIFFERENTIAL_CH0P:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH0P_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue );
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->DIFFERENTIAL_CH2P:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH2P_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->DIFFERENTIAL_CH4P:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH4P_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->DIFFERENTIAL_CH6P:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH6P_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->DIFFERENTIAL_CH0N:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH0N_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->DIFFERENTIAL_CH2N:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH2N_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->DIFFERENTIAL_CH4N:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH4N_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->DIFFERENTIAL_CH6N:
			m_lbAdcCfg.AddString(DIFFERENTIAL_CH6N_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->SINGLEPOINT_CH0:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH0_TEXT);			
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->SINGLEPOINT_CH1:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH1_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->SINGLEPOINT_CH2:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH2_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->SINGLEPOINT_CH3:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH3_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->SINGLEPOINT_CH4:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH4_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->SINGLEPOINT_CH5:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH5_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->SINGLEPOINT_CH6:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH6_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		case m_cData->SINGLEPOINT_CH7:
			m_lbAdcCfg.AddString(SIGNLEPOINT_CH7_TEXT);
			//m_lbAdcCfg.SetItemData(i, ((CUsbStreamData*)m_cData->m_cObaStreamData[i])->m_dwValue);
			m_lbAdcCfg.SetItemData(i, m_cData->GetAnalogQryCtrl(i) );
			break;
		}
	}

	// Enable Digital Status
	//m_bEnDigitalStatus = m_cData->GetContDIctrl() ? TRUE : FALSE;

	// Enable Pulse Status
	//m_bEnPulseStatus = m_cData->GetContPCctrl() ? TRUE : FALSE;

	
	
	UpdateData(FALSE);

}

void CDlgUsbM400::CtrlToData(void)
{
	UpdateData(TRUE);
	char *stringEnd;
	int i;
	m_cData->SetModuleAddress(strtol((LPCTSTR)m_csModuleAddress, &stringEnd, 16));
	m_cData->SetDIO1Direction(strtol((LPCTSTR)m_csDataP1Dir, &stringEnd, 16));
	m_cData->SetDIO2Direction(strtol((LPCTSTR)m_csDataP2Dir, &stringEnd, 16));
	m_cData->SetP1PwrOnDefault(strtol((LPCTSTR)m_csP1PowerOn, &stringEnd, 16));
	m_cData->SetP2PwrOnDefault(strtol((LPCTSTR)m_csP2PowerOn, &stringEnd, 16));
	m_cData->SetDAC0Pod(strtol((LPCTSTR)m_csDAC0, &stringEnd, 16));
	m_cData->SetDAC1Pod(strtol((LPCTSTR)m_csDAC1, &stringEnd, 16));
	m_cData->SetExpanderBdFlag(m_bEXPFlag ? m_cData->EXP_YES : m_cData->EXP_NO);
	m_cData->SetADCFs(m_bADCFs ? m_cData->EBF_SLOW : m_cData->EBF_NORMAL);

	m_cData->SetAnalogRefrence(	(int)m_cbADCRefrence.GetItemData(m_cbADCRefrence.GetCurSel()));

//	m_cData->SetAsyncUDMode(strtol((LPCTSTR)m_csAsyncUpdate, &stringEnd, 16));
//	m_cData->SetContDIctrl( m_bEnDigitalStatus ? m_cData->DS_ON : m_cData->DS_OFF);
//	m_cData->SetContPCctrl( m_bEnPulseStatus ? m_cData->DP_ON : m_cData->DP_OFF);

//	m_cData->SetAnalogQueryCount(strtol((LPCTSTR)m_csAdcCount, &stringEnd, 16));

	for(i=0; i < m_lbAdcCfg.GetCount(); i++)
	{
		m_cData->SetAnalogQryCtrl(i, (int)m_lbAdcCfg.GetItemData(i));
	}

}

void CDlgUsbM400::SetCurrent(void)
{
	DataToCtrl();
}
void CDlgUsbM400::SetDefaults(void)
{
	DataToCtrl();
}
bool CDlgUsbM400::WriteCurrent(void)
{
	int eeCount, eeLoc, eeData;
	bool rVal = true;
	CString csErrors, csTemp;

	// Update data object
	CtrlToData();
	
	// Set the comm port
	//m_cData->m_cIICtrl->SetlnComPort(atol(m_cData->m_csCommPort.MakeReverse()));
	//m_cData->m_csCommPort.MakeReverse();		// Fix the string reversal for the atoi

	// Set the baud rate
	//m_cData->m_cIICtrl->SetlnBaudRate(m_cData->m_nBaudRate);
   	m_cData->m_cIICtrl->SetlnProtocol(m_cData->PROTOCOL);
	m_cData->m_cIICtrl->SetlnModuleAddress(m_cData->m_cIICtrl->GetUSBModuleAddress(0));

	// Open communications
	m_cData->m_cIICtrl->SetbPortOpen(TRUE);

	if(m_cData->m_cIICtrl->GetbPortOpen())
	{
		eeCount = (int)m_cData->m_cbaEEData.GetCount();

		// Set address to read from EEProm
		// Configure the unit module address, then reset the ActiveX control Unit
		// Address to deal with the change
		m_cData->m_cIICtrl->SetlnEEPROMAddress(m_cData->EE_UNITADDR);
		m_cData->m_cIICtrl->SetlnEEPROMData(m_cData->m_cbaEEData[m_cData->EE_UNITADDR]);

		m_cData->m_cIICtrl->SetlnModuleAddress(m_cData->m_cIICtrl->GetUSBModuleAddress(0));

		for(eeLoc = 1; eeLoc < eeCount; eeLoc++)
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

bool CDlgUsbM400::ReadCurrent(void)
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
	m_cData->m_cIICtrl->SetlnModuleAddress(m_cData->m_cIICtrl->GetUSBModuleAddress(0));
	m_cData->m_cIICtrl->SetlnModuleAddress(m_cData->m_cIICtrl->GetUSBModuleAddress(0));

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

void CDlgUsbM400::OnLbnSelchangeList2()
{
	int curSel;

	curSel = m_lbAdcCfg.GetCurSel();

	if(curSel != LB_ERR)
	{
		switch(m_lbAdcCfg.GetItemData(curSel) & 0x000000F0)
		{
		case CDataUsbM400::ADCGAIN_1:
			m_lbAdcCfgGain.SetCurSel(0);
			break;
		case CDataUsbM400::ADCGAIN_2:
			m_lbAdcCfgGain.SetCurSel(1);
			break;
		case CDataUsbM400::ADCGAIN_4:
			m_lbAdcCfgGain.SetCurSel(2);
			break;
		case CDataUsbM400::ADCGAIN_5:
			m_lbAdcCfgGain.SetCurSel(3);
			break;
		case CDataUsbM400::ADCGAIN_8:
			m_lbAdcCfgGain.SetCurSel(4);
			break;
		case CDataUsbM400::ADCGAIN_10:
			m_lbAdcCfgGain.SetCurSel(5);
			break;
		case CDataUsbM400::ADCGAIN_16:
			m_lbAdcCfgGain.SetCurSel(6);
			break;
		case CDataUsbM400::ADCGAIN_20:
			m_lbAdcCfgGain.SetCurSel(7);
			break;
		}
	}
	CtrlToData();
}

void CDlgUsbM400::OnLbnSelchangeList3()
{
	int configSel;
	int curSel;
	int cfgItemData;

	configSel = m_lbAdcCfg.GetCurSel();
	curSel = m_lbAdcCfgGain.GetCurSel();

	if(configSel != LB_ERR)
	{
		cfgItemData = (int)m_lbAdcCfg.GetItemData(configSel) & 0x0000000F;
		cfgItemData |= m_lbAdcCfgGain.GetItemData(curSel);
		m_lbAdcCfg.SetItemData(configSel, cfgItemData);
	}
}

void CDlgUsbM400::OnBnClickedBtnAddusbstream()
{
	TypeToConfig();
	CtrlToData();
}

void CDlgUsbM400::OnLbnDblclkList1()
{
	TypeToConfig();
	CtrlToData();
}

void CDlgUsbM400::OnBnClickedBtnRemoveusbstream()
{
	int curSel;
	curSel = m_lbAdcCfg.GetCurSel();
	if(curSel != LB_ERR)
	{
		m_lbAdcCfg.DeleteString(curSel);
		m_lbAdcCfg.SetCurSel(curSel);
	}
	CtrlToData();
}
void CDlgUsbM400::OnLbnDblclkList2()
{
	OnBnClickedBtnRemoveusbstream();
	CtrlToData();
}

void CDlgUsbM400::OnBnClickedBtnUsbup()
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

void CDlgUsbM400::OnBnClickedBtnusbdown()
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

void CDlgUsbM400::TypeToConfig(void)
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
		if(m_lbAdcCfg.GetCount() < CDataUsbM400::MAX_STREAM_ITEMS)
		{
			// We havn't reached the maximum number of config
			// items, so it's save to add the text and item data.
			curSel = m_lbAdcCfg.AddString(cs);
			m_lbAdcCfg.SetItemData(curSel, itemData); 

		}
	}
}



void CDlgUsbM400::OnEnChangeEbmodaddru4()
{
	CtrlToData();
}

void CDlgUsbM400::OnEnChangeEbdiodir1u4()
{
	CtrlToData();
}

void CDlgUsbM400::OnEnChangeEbdiodir2u4()
{
	CtrlToData();
}

void CDlgUsbM400::OnEnChangeEbpup1u4()
{
	CtrlToData();
}

void CDlgUsbM400::OnEnChangeEbpup2u4()
{
	CtrlToData();
}

void CDlgUsbM400::OnBnClickedChkexpXu4()
{
	CtrlToData();
}

void CDlgUsbM400::OnEnChangeEbpuda0u4()
{
	CtrlToData();
}

void CDlgUsbM400::OnBnClickedChkhighzu4()
{
	CtrlToData();
}

void CDlgUsbM400::OnEnChangeEbpuda1u4()
{
	CtrlToData();
}

void CDlgUsbM400::OnCbnSelchangeCbAdcrefu4()
{
	CtrlToData();
}
