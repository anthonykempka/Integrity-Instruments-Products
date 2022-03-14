// Dlg485M300.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "Dlg485M300.h"
#include ".\dlg485m300.h"


// CDlg485M300 dialog

IMPLEMENT_DYNAMIC(CDlg485M300, CDialog)
CDlg485M300::CDlg485M300(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg485M300::IDD, pParent)
	, m_csModuleAddress(_T(""))
	, m_csDataP1Dir(_T(""))
	, m_csDataP2Dir(_T(""))
	, m_csP1PowerOn(_T(""))
	, m_csP2PowerOn(_T(""))
	, m_bEXPFlag(FALSE)
	, m_csDAC0(_T(""))
	, m_bADCFs(FALSE)
	, m_csDAC1(_T(""))
{
}

CDlg485M300::~CDlg485M300()
{
}

void CDlg485M300::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EBMODADDR_485, m_csModuleAddress);
	DDX_Text(pDX, IDC_EBDIODIR1M3_485, m_csDataP1Dir);
	DDX_Text(pDX, IDC_EBDIODIR2M3_485, m_csDataP2Dir);
	DDX_Text(pDX, IDC_EBPUP1M3_485, m_csP1PowerOn);
	DDX_Text(pDX, IDC_EBPUP2M4_485, m_csP2PowerOn);
	DDX_Check(pDX, IDC_CHKEXP_XM3_485, m_bEXPFlag);
	DDX_Text(pDX, IDC_EBPUDA0M3_485, m_csDAC0);
	DDX_Check(pDX, IDC_CHKHIGHZM3_485, m_bADCFs);
	DDX_Text(pDX, IDC_EBPUDA1M3_485, m_csDAC1);
}


BEGIN_MESSAGE_MAP(CDlg485M300, CDialog)
	ON_EN_CHANGE(IDC_EBMODADDR_485, OnEnChangeEbmodaddr485)
	ON_EN_CHANGE(IDC_EBDIODIR1M3_485, OnEnChangeEbdiodir1m3485)
	ON_EN_CHANGE(IDC_EBDIODIR2M3_485, OnEnChangeEbdiodir2m3485)
	ON_EN_CHANGE(IDC_EBPUP1M3_485, OnEnChangeEbpup1m3485)
	ON_EN_CHANGE(IDC_EBPUP2M4_485, OnEnChangeEbpup2m4485)
	ON_BN_CLICKED(IDC_CHKEXP_XM3_485, OnBnClickedChkexpXm3485)
	ON_EN_CHANGE(IDC_EBPUDA0M3_485, OnEnChangeEbpuda0m3485)
	ON_BN_CLICKED(IDC_CHKHIGHZM3_485, OnBnClickedChkhighzm3485)
	ON_EN_CHANGE(IDC_EBPUDA1M3_485, OnEnChangeEbpuda1m3485)
END_MESSAGE_MAP()


// CDlg485M300 message handlers

BOOL CDlg485M300::OnInitDialog()
{
	CDialog::OnInitDialog();

	DataToCtrl(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg485M300::DataToCtrl(void)
{

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

	UpdateData(FALSE);

}
void CDlg485M300::CtrlToData(void)
{
	UpdateData(TRUE);
	char *stringEnd;

	m_cData->SetModuleAddress(strtol((LPCTSTR)m_csModuleAddress, &stringEnd, 16));
	m_cData->SetDIO1Direction(strtol((LPCTSTR)m_csDataP1Dir, &stringEnd, 16));
	m_cData->SetDIO2Direction(strtol((LPCTSTR)m_csDataP2Dir, &stringEnd, 16));
	m_cData->SetP1PwrOnDefault(strtol((LPCTSTR)m_csP1PowerOn, &stringEnd, 16));
	m_cData->SetP2PwrOnDefault(strtol((LPCTSTR)m_csP2PowerOn, &stringEnd, 16));
	m_cData->SetDAC0Pod(strtol((LPCTSTR)m_csDAC0, &stringEnd, 16));
	m_cData->SetDAC1Pod(strtol((LPCTSTR)m_csDAC1, &stringEnd, 16));
	m_cData->SetExpanderBdFlag(m_bEXPFlag ? m_cData->EXP_YES : m_cData->EXP_NO);
	m_cData->SetADCFs(m_bADCFs ? m_cData->EBF_SLOW : m_cData->EBF_NORMAL);


}

void CDlg485M300::SetDefaults(void)
{
	DataToCtrl();
}
bool CDlg485M300::WriteCurrent(void)
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
    
	// Set Communicatio Protocol
	m_cData->m_cIICtrl->SetlnProtocol(m_cData->PROTOCOL);
	m_cData->m_cIICtrl->SetlnModuleAddress(0xFF);

	
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

bool CDlg485M300::ReadCurrent(void)
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

void CDlg485M300::SetCurrent(void)
{
	DataToCtrl();
}

void CDlg485M300::OnEnChangeEbmodaddr485()
{
	CtrlToData();
}

void CDlg485M300::OnEnChangeEbdiodir1m3485()
{
	CtrlToData();
}

void CDlg485M300::OnEnChangeEbdiodir2m3485()
{
	CtrlToData();
}

void CDlg485M300::OnEnChangeEbpup1m3485()
{
	CtrlToData();
}

void CDlg485M300::OnEnChangeEbpup2m4485()
{
	CtrlToData();
}

void CDlg485M300::OnBnClickedChkexpXm3485()
{
	CtrlToData();
}

void CDlg485M300::OnEnChangeEbpuda0m3485()
{
	CtrlToData();
}

void CDlg485M300::OnBnClickedChkhighzm3485()
{
	CtrlToData();
}

void CDlg485M300::OnEnChangeEbpuda1m3485()
{
	CtrlToData();
}
