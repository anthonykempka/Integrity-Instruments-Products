// 485M300Log.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "485M300Log.h"


// C485M300Log dialog

IMPLEMENT_DYNAMIC(C485M300Log, CDialog)
C485M300Log::C485M300Log(CWnd* pParent /*=NULL*/)
	: CDialog(C485M300Log::IDD, pParent)
	, m_cstrLogFilePath(_T(""))
	, m_strInterval(_T("1.0"))
	, m_strGraphMin(_T("0"))
	, m_strGraphMax(_T("5"))
	, m_bCh0(FALSE)
	, m_bCh1(FALSE)
	, m_bCh2(FALSE)
	, m_bCh3(FALSE)
	, m_bCh4(FALSE)
	, m_bCh5(FALSE)
	, m_bCh6(FALSE)
	, m_bCh7(FALSE)
	, m_pLogThread(NULL)
	, m_bAXRedraw(FALSE)
	, m_dInterval(0)
	, m_dGraphMax(5)
	, m_dGraphMin(0)
	, m_nSampleType0(0)
	, m_nSampleType2(0)
	, m_nSampleType4(0)
	, m_nSampleType6(0)
	, m_dwStartMilliseconds(0)
	, m_cstrModuleAddress(_T("1"))
	, m_lnModuleAddress(0)
{
}

C485M300Log::~C485M300Log()
{
	HaltThread();
}

void C485M300Log::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EB_M300LOGFILE, m_cstrLogFilePath);
	DDX_Text(pDX, IDC_EB_M300LOGINTERVAL, m_strInterval);
	DDX_Text(pDX, IDC_EBM300GRAPHMIN, m_strGraphMin);
	DDX_Text(pDX, IDC_EBM300GRAPHMAX, m_strGraphMax);
	DDX_Check(pDX, IDC_CHKM300CH0, m_bCh0);
	DDX_Check(pDX, IDC_CHKM300CH1, m_bCh1);
	DDX_Check(pDX, IDC_CHKM300CH2, m_bCh2);
	DDX_Check(pDX, IDC_CHKM300CH3, m_bCh3);
	DDX_Check(pDX, IDC_CHKM300CH4, m_bCh4);
	DDX_Check(pDX, IDC_CHKM300CH5, m_bCh5);
	DDX_Check(pDX, IDC_CHKM300CH6, m_bCh6);
	DDX_Check(pDX, IDC_CHKM300CH7, m_bCh7);
	DDX_Text(pDX, IDC_EBMODULEADDRESS, m_cstrModuleAddress);
	DDX_Control(pDX, IDC_INTEGRITYLINEGRACTRL1, m_cIILG);

}


BEGIN_MESSAGE_MAP(C485M300Log, CDialog)
	ON_BN_CLICKED(IDC_BTNBROWSEM300LOG, OnBnClickedBtnbrowsem300log)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_M300LOGSTART, OnBnClickedBtnM300logstart)
	ON_BN_CLICKED(IDC_BTN_M300LOGSTOP, OnBnClickedBtnM300logstop)
	ON_BN_CLICKED(IDC_RBDIFFADCOFF, OnBnClickedRbdiffadcoff)
	ON_BN_CLICKED(IDC_RBDIFFADCPOS, OnBnClickedRbdiffadcpos)
	ON_BN_CLICKED(IDC_RBDIFFADCNEG, OnBnClickedRbdiffadcneg)
	ON_BN_CLICKED(IDC_RBDIFFADCOFF2, OnBnClickedRbdiffadcoff2)
	ON_BN_CLICKED(IDC_RBDIFFADCPOS2, OnBnClickedRbdiffadcpos2)
	ON_BN_CLICKED(IDC_RBDIFFADCNEG2, OnBnClickedRbdiffadcneg2)
	ON_BN_CLICKED(IDC_RBDIFFADCOFF3, OnBnClickedRbdiffadcoff3)
	ON_BN_CLICKED(IDC_RBDIFFADCPOS3, OnBnClickedRbdiffadcpos3)
	ON_BN_CLICKED(IDC_RBDIFFADCNEG3, OnBnClickedRbdiffadcneg3)
	ON_BN_CLICKED(IDC_RBDIFFADCOFF4, OnBnClickedRbdiffadcoff4)
	ON_BN_CLICKED(IDC_RBDIFFADCPOS4, OnBnClickedRbdiffadcpos4)
	ON_BN_CLICKED(IDC_RBDIFFADCNEG4, OnBnClickedRbdiffadcneg4)
	ON_EN_CHANGE(IDC_EBM300GRAPHMIN, OnEnChangeEbm300graphmin)
	ON_EN_CHANGE(IDC_EBM300GRAPHMAX, OnEnChangeEbm300graphmax)
END_MESSAGE_MAP()


// C485M300Log message handlers
void C485M300Log::OnBnClickedBtnbrowsem300log()
{
	/*
	 * Event Handler for Browse Button, 
	 * Retrieves the name of the file for logging to.
	 */
	static char BASED_CODE szFilter[] = "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||";
	CFileDialog cDlg(FALSE, "csv", NULL, OFN_HIDEREADONLY , szFilter);
	UpdateData();
	if(IDOK == cDlg.DoModal())
	{
		m_cstrLogFilePath = cDlg.GetPathName();
		UpdateData(FALSE);
	}
}

void C485M300Log::OnBnClickedOk()
{
	HaltThread();
	m_cIICtrl->SetbPortOpen(FALSE);
	OnOK();
}

void C485M300Log::OnBnClickedCancel()
{
	HaltThread();
	m_cIICtrl->SetbPortOpen(FALSE);
	OnCancel();
}

void C485M300Log::OnBnClickedBtnM300logstart()
{
	UpdateData();

	// Check to see if the interval is long enough
	// Request Length: 30 bits
	// Respnse Length: 56 bits
	// Total bits: 86

	double requestTime;
	double channelCount = 0;
	requestTime = (1/(double)m_cIICtrl->GetlnBaudRate()) * 86;





	// Count up the request time
	switch(m_nSampleType0)
	{
	case 1:
	case 2:
		channelCount += 1;
		break;
	default:
		channelCount += m_bCh0 ? 1 : 0;
		channelCount += m_bCh1 ? 1 : 0;
		break;
	}


	switch(m_nSampleType2)
	{
	case 1:
	case 2:
		channelCount += 1;
		break;
	default:
		channelCount += m_bCh2 ? 1 : 0;
		channelCount += m_bCh3 ? 1 : 0;
		break;
	}

	switch(m_nSampleType4)
	{
	case 1:
	case 2:
		channelCount += 1;
		break;
	default:
		channelCount += m_bCh4 ? 1 : 0;
		channelCount += m_bCh5 ? 1 : 0;
		break;
	}

	switch(m_nSampleType6)
	{
	case 1:
	case 2:
		channelCount += 1;
		break;
	default:
		channelCount += m_bCh6 ? 1 : 0;
		channelCount += m_bCh7 ? 1 : 0;
		break;
	}




	requestTime *= channelCount;


	m_dInterval = atof((LPCTSTR)m_strInterval);
	if((requestTime > this->m_dInterval) || (this->m_dInterval < .055))
	{
		AfxMessageBox("Please specifiy a longer interval.\rThe requested inteval is to short");
		return;
	}


	m_dwInterval = (DWORD)(m_dInterval * 1000.0);

	if(m_cstrLogFilePath.GetLength() <= 0)
	{
		AfxMessageBox("You must supply a valid log file name");
		return;
	}

	
	// Verify the module address is within bounds
	m_lnModuleAddress = atol((char*)(LPCTSTR)m_cstrModuleAddress);

	if(m_lnModuleAddress > 254 || m_lnModuleAddress < 1)
	{
		AfxMessageBox("Invalid Module Address.\nValid range is from 1 to 254");
		return;
	}


	m_cIICtrl->SetlnModuleAddress(m_lnModuleAddress);
	m_cIICtrl->SetbPortOpen(TRUE);

	if(!m_cIICtrl->GetbPortOpen())
	{
		return;
	}
	


	// Set button states
	CButton *pStart = (CButton*)GetDlgItem(IDC_BTN_M300LOGSTART);
	CButton *pHalt = (CButton*)GetDlgItem(IDC_BTN_M300LOGSTOP);
	CButton *pBrowse = (CButton*)GetDlgItem(IDC_BTNBROWSEM300LOG);
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EB_M300LOGFILE);

	pEdit->EnableWindow(FALSE);
	pBrowse->EnableWindow(FALSE);

	pHalt->EnableWindow(TRUE);
	pStart->EnableWindow(FALSE);


	// Everything is good to go, now configure the Graphing Control.
	
	m_cIILG.SetdblMaxValue(m_dGraphMax);
	m_cIILG.SetdblMinValue(m_dGraphMin);

	if(channelCount)
	{
		/*
		 * Count up the number of elements for display
		 */
		m_cIILG.RemoveAll();
		m_cIILG.SetlnNumElements(8L);
		m_cIILG.put_ulElementColor(0,C485M300Log::GraphColors::ch0);
		m_cIILG.put_ulElementColor(1,C485M300Log::GraphColors::ch1);
		m_cIILG.put_ulElementColor(2,C485M300Log::GraphColors::ch2);
		m_cIILG.put_ulElementColor(3,C485M300Log::GraphColors::ch3);
		m_cIILG.put_ulElementColor(4,C485M300Log::GraphColors::ch4);
		m_cIILG.put_ulElementColor(5,C485M300Log::GraphColors::ch5);
		m_cIILG.put_ulElementColor(6,C485M300Log::GraphColors::ch6);
		m_cIILG.put_ulElementColor(7,C485M300Log::GraphColors::ch7);
	}
	

	if(!m_pLogThread)
	{
		// Start up the thread
		m_ceHaltThread.ResetEvent();
		m_pLogThread = AfxBeginThread(LogThreadProc, this,THREAD_PRIORITY_LOWEST);

		// Get the redraw status of the IOControl
		m_bAXRedraw = this->m_cIICtrl->GetbInvalidate();

		// Turn it off to save time.
		m_cIICtrl->SetbInvalidate(FALSE);
	}


}

void C485M300Log::OnBnClickedBtnM300logstop()
{
	HaltThread();
	// TODO: Add your control notification handler code here
	CButton *pStart = (CButton*)GetDlgItem(IDC_BTN_M300LOGSTART);
	CButton *pHalt = (CButton*)GetDlgItem(IDC_BTN_M300LOGSTOP);
	CButton *pBrowse = (CButton*)GetDlgItem(IDC_BTNBROWSEM300LOG);
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EB_M300LOGFILE);

	pHalt->EnableWindow(FALSE);
	pEdit->EnableWindow(TRUE);
	pBrowse->EnableWindow(TRUE);
	pStart->EnableWindow(TRUE);



	m_cIICtrl->SetbPortOpen(FALSE);
}
void C485M300Log::SetChCheckStates(void)
{
	CButton* pCh0 = (CButton*)GetDlgItem(IDC_CHKM300CH0);
	CButton* pCh1 = (CButton*)GetDlgItem(IDC_CHKM300CH1);
	CButton* pCh2 = (CButton*)GetDlgItem(IDC_CHKM300CH2);
	CButton* pCh3 = (CButton*)GetDlgItem(IDC_CHKM300CH3);
	CButton* pCh4 = (CButton*)GetDlgItem(IDC_CHKM300CH4);
	CButton* pCh5 = (CButton*)GetDlgItem(IDC_CHKM300CH5);
	CButton* pCh6 = (CButton*)GetDlgItem(IDC_CHKM300CH6);
	CButton* pCh7 = (CButton*)GetDlgItem(IDC_CHKM300CH7);


	if(m_nSampleType0)
	{
		pCh0->EnableWindow(FALSE);
		pCh1->EnableWindow(FALSE);
	}
	else
	{
		pCh0->EnableWindow(TRUE);
		pCh1->EnableWindow(TRUE);
	}

	if(m_nSampleType2)
	{
		pCh2->EnableWindow(FALSE);
		pCh3->EnableWindow(FALSE);
	}
	else
	{
		pCh2->EnableWindow(TRUE);
		pCh3->EnableWindow(TRUE);
	}


	if(m_nSampleType4)
	{
		pCh4->EnableWindow(FALSE);
		pCh5->EnableWindow(FALSE);
	}
	else
	{
		pCh4->EnableWindow(TRUE);
		pCh5->EnableWindow(TRUE);
	}

	if(m_nSampleType6)
	{
		pCh6->EnableWindow(FALSE);
		pCh7->EnableWindow(FALSE);
	}
	else
	{
		pCh6->EnableWindow(TRUE);
		pCh7->EnableWindow(TRUE);
	}
}

/*
 * Bipolar CH0/CH1 Event Handlers
 */
void C485M300Log::OnBnClickedRbdiffadcoff()
{
	m_nSampleType0 = 0;
	UpdateData();
	SetChCheckStates();
}

void C485M300Log::OnBnClickedRbdiffadcpos()
{
	m_nSampleType0 = 1;
	UpdateData();
	SetChCheckStates();
}

void C485M300Log::OnBnClickedRbdiffadcneg()
{
	m_nSampleType0 = 2;
	UpdateData();
	SetChCheckStates();
}

/*
 * Bipolar CH2/CH3 Event Handlers
 */

void C485M300Log::OnBnClickedRbdiffadcoff2()
{
	m_nSampleType2 = 0;
	UpdateData();
	SetChCheckStates();
}

void C485M300Log::OnBnClickedRbdiffadcpos2()
{
	m_nSampleType2 = 1;
	UpdateData();
	SetChCheckStates();
}

void C485M300Log::OnBnClickedRbdiffadcneg2()
{
	m_nSampleType2 = 2;
	UpdateData();
	SetChCheckStates();
}

/*
 * Bipolar CH4/CH5 Event Handlers
 */


void C485M300Log::OnBnClickedRbdiffadcoff3()
{
	m_nSampleType4 = 0;
	UpdateData();
	SetChCheckStates();
}

void C485M300Log::OnBnClickedRbdiffadcpos3()
{
	m_nSampleType4 = 1;
	UpdateData();
	SetChCheckStates();
}

void C485M300Log::OnBnClickedRbdiffadcneg3()
{
	m_nSampleType4 = 2;
	UpdateData();
	SetChCheckStates();
}

/*
 * Bipolar CH6/CH7 Event Handlers
 */


void C485M300Log::OnBnClickedRbdiffadcoff4()
{
	m_nSampleType6 = 0;
	UpdateData();
	SetChCheckStates();
}

void C485M300Log::OnBnClickedRbdiffadcpos4()
{
	m_nSampleType6 = 1;
	UpdateData();
	SetChCheckStates();
}

void C485M300Log::OnBnClickedRbdiffadcneg4()
{
	m_nSampleType6 = 2;
	UpdateData();
	SetChCheckStates();
}

BOOL C485M300Log::OnInitDialog()
{
	CDialog::OnInitDialog();

	CButton* pRbC0 = (CButton*)GetDlgItem(IDC_RBDIFFADCOFF);
	CButton* pRbC2 = (CButton*)GetDlgItem(IDC_RBDIFFADCOFF2);
	CButton* pRbC4 = (CButton*)GetDlgItem(IDC_RBDIFFADCOFF3);
	CButton* pRbC6 = (CButton*)GetDlgItem(IDC_RBDIFFADCOFF4);


	pRbC0->SetCheck(1);
	pRbC2->SetCheck(1);
	pRbC4->SetCheck(1);
	pRbC6->SetCheck(1);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void C485M300Log::HaltThread(void)
{
			// thread appears to be running ... stop it
		BeginWaitCursor();


		if(m_pLogThread)
		{
			// Signal thread that it's time to exit
			m_ceHaltThread.SetEvent();

			// Wait for the thread to exit
			while(WAIT_OBJECT_0 != WaitForSingleObject(m_pLogThread->m_hThread, INFINITE))
			{
				// Context Switch :D
				Sleep(55);			
			}

			// clear our event
			m_ceHaltThread.ResetEvent();

			// Set thread pointer to null so next start is valid.
			m_pLogThread = NULL;

			m_cIICtrl->SetbInvalidate(this->m_bAXRedraw);
		}

}

UINT C485M300Log::LogThreadProc(LPVOID pParam)
{
	C485M300Log *pThis = (C485M300Log*)pParam;
	CString str;
	CString strTemp;
	DWORD lastMsCount, currentMsCount;
	CFile cfLogFile;
	bool bLogFileIsOpen = false;
	double chanVal[8] = {-11,-11,-11,-11,-11,-11,-11,-11};
	int i; 

	pThis->m_dwStartMilliseconds = GetTickCount();
	lastMsCount = pThis->m_dwStartMilliseconds;

	// Open the file
	if(!cfLogFile.Open(pThis->m_cstrLogFilePath,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		AfxMessageBox("Error opening file!");
	}
	else
	{
		cfLogFile.SeekToEnd();
		bLogFileIsOpen = true;
		
		str.Format("\r\nIntegrity Instruments Data Acquisition Module Utility\r\nModule Address,%ld\r\n", pThis->m_cIICtrl->GetlnModuleAddress());
		COleDateTime dt;
		dt = COleDateTime::GetCurrentTime();
		strTemp.Format("%sStart Time,%04d/%02d/%02d %02d:%02d:%02d\r\n",str,
			dt.GetYear(), dt.GetMonth(), dt.GetDay(),
			dt.GetHour(), dt.GetMinute(), dt.GetSecond());

		str = strTemp;


		// Create the CSV file header
		strTemp = "mS";

		switch(pThis->m_nSampleType0)
		{
		case 0:
			if(pThis->m_bCh0) strTemp += ",CH0";
			if(pThis->m_bCh1) strTemp += ",CH1";
			break;
		case 1:
			strTemp += ",CH0+ / CH1-";
			break;
		case 2:
			strTemp += ",CH0- / CH1+";
			break;
		}

		switch(pThis->m_nSampleType2)
		{
		case 0:
			if(pThis->m_bCh2) strTemp += ",CH2";
			if(pThis->m_bCh3) strTemp += ",CH3";
			break;
		case 1:
			strTemp += ",CH2+ / CH3-";
			break;
		case 2:
			strTemp += ",CH2- / CH3+";
			break;
		}
		
		switch(pThis->m_nSampleType4)
		{
		case 0:
			if(pThis->m_bCh4) strTemp += ",CH4";
			if(pThis->m_bCh5) strTemp += ",CH5";
			break;
		case 1:
			strTemp += ",CH4+ / CH5-";
			break;
		case 2:
			strTemp += ",CH4- / CH5+";
			break;
		}
		
		
		switch(pThis->m_nSampleType6)
		{
		case 0:
			if(pThis->m_bCh6) strTemp += ",CH6";
			if(pThis->m_bCh7) strTemp += ",CH7";
			break;
		case 1:
			strTemp += ",CH6+ / CH7-";
			break;
		case 2:
			strTemp += ",CH6- / CH7+";
			break;
		}
		
		
		
		str += strTemp;
		str += "\r\n";
		if(str.GetLength())
		{
			cfLogFile.Write(str, str.GetLength());
		}
		
	}



	while(WAIT_OBJECT_0 != ::WaitForSingleObject(pThis->m_ceHaltThread, 0))
	{
		// Run logging process every N mS
		currentMsCount = GetTickCount();
		if(bLogFileIsOpen)
		{

			// Keep track of mS from start for log file
			lastMsCount = currentMsCount;

			str.Format("%ld", currentMsCount - pThis->m_dwStartMilliseconds);

			switch(pThis->m_nSampleType0)
			{
			case 0:						// Unipolar Samples
				if(pThis->m_bCh0)
				{
					pThis->m_cIICtrl->SetlnADCControl(0x08);
					chanVal[0] = pThis->m_cIICtrl->GetdADCUnipolar();
					strTemp.Format("%s,%f", str, chanVal[0]);
					str = strTemp;
				}

				if(pThis->m_bCh1)
				{
					pThis->m_cIICtrl->SetlnADCControl(0x0c);
					chanVal[1] = pThis->m_cIICtrl->GetdADCUnipolar();
					strTemp.Format("%s,%f", str, chanVal[1]);
					str = strTemp;
				}

				break;
			case 1:					// BiPolar Type 1
				pThis->m_cIICtrl->SetlnADCControl(0x00);
				chanVal[0] = pThis->m_cIICtrl->GetdADCBipolar();
				chanVal[1] = -11;
				strTemp.Format("%s,%f", str, chanVal[0]);
				str = strTemp;
				break;
			case 2:					// Bipolar type 2
				pThis->m_cIICtrl->SetlnADCControl(0x04);
				chanVal[0] = pThis->m_cIICtrl->GetdADCBipolar();
				chanVal[1] = -11;
				strTemp.Format("%s,%f", str, chanVal[0]);
				str = strTemp;
				break;
			}


			switch(pThis->m_nSampleType2)
			{
			case 0:						// Unipolar samples
				if(pThis->m_bCh2)
				{
					pThis->m_cIICtrl->SetlnADCControl(0x09);
					chanVal[2] = pThis->m_cIICtrl->GetdADCUnipolar();
					strTemp.Format("%s,%f", str, chanVal[2]);
					str = strTemp;
				}

				if(pThis->m_bCh3)
				{
					pThis->m_cIICtrl->SetlnADCControl(0x0d);
					chanVal[3] = pThis->m_cIICtrl->GetdADCUnipolar();
					strTemp.Format("%s,%f", str, chanVal[3]);
					str = strTemp;
				}

				break;
			case 1:						// Bipolar type 1
				pThis->m_cIICtrl->SetlnADCControl(0x01);
				chanVal[2] = pThis->m_cIICtrl->GetdADCBipolar();
				chanVal[3] = -11;
				strTemp.Format("%s,%f", str, chanVal[2]);
				str = strTemp;
				break;
			case 2:						// Bipolar type 2
				pThis->m_cIICtrl->SetlnADCControl(0x05);
				chanVal[2] = pThis->m_cIICtrl->GetdADCBipolar();
				chanVal[3] = -11;
				strTemp.Format("%s,%f", str, chanVal[2]);
				str = strTemp;
				break;
			}




			switch(pThis->m_nSampleType4)
			{
			case 0:						// Unipolar samples
				if(pThis->m_bCh4)
				{
					pThis->m_cIICtrl->SetlnADCControl(0x0A);
					chanVal[4] = pThis->m_cIICtrl->GetdADCUnipolar();
					strTemp.Format("%s,%f", str, chanVal[4]);
					str = strTemp;
				}

				if(pThis->m_bCh5)
				{
					pThis->m_cIICtrl->SetlnADCControl(0x0E);
					chanVal[5] = pThis->m_cIICtrl->GetdADCUnipolar();
					strTemp.Format("%s,%f", str, chanVal[5]);
					str = strTemp;
				}

				break;
			case 1:						// Bipolar type 1
				pThis->m_cIICtrl->SetlnADCControl(0x02);
				chanVal[4] = pThis->m_cIICtrl->GetdADCBipolar();
				chanVal[5] = -11;
				strTemp.Format("%s,%f", str, chanVal[4]);
				str = strTemp;
				break;
			case 2:						// Bipolar type 2
				pThis->m_cIICtrl->SetlnADCControl(0x06);
				chanVal[4] = pThis->m_cIICtrl->GetdADCBipolar();
				chanVal[5] = -11;
				strTemp.Format("%s,%f", str, chanVal[4]);
				str = strTemp;
				break;
			}


			switch(pThis->m_nSampleType6)
			{
			case 0:						// Unipolar samples
				if(pThis->m_bCh6)
				{
					pThis->m_cIICtrl->SetlnADCControl(0x0B);
					chanVal[6] = pThis->m_cIICtrl->GetdADCUnipolar();
					strTemp.Format("%s,%f", str, chanVal[6]);
					str = strTemp;
				}

				if(pThis->m_bCh7)
				{
					pThis->m_cIICtrl->SetlnADCControl(0x0F);
					chanVal[7] = pThis->m_cIICtrl->GetdADCUnipolar();
					strTemp.Format("%s,%f", str, chanVal[7]);
					str = strTemp;
				}

				break;
			case 1:						// Bipolar type 1
				pThis->m_cIICtrl->SetlnADCControl(0x03);
				chanVal[6] = pThis->m_cIICtrl->GetdADCBipolar();
				chanVal[7] = -11;
				strTemp.Format("%s,%f", str, chanVal[6]);
				str = strTemp;
				break;
			case 2:						// Bipolar type 2
				pThis->m_cIICtrl->SetlnADCControl(0x07);
				chanVal[6] = pThis->m_cIICtrl->GetdADCBipolar();
				chanVal[7] = -11;
				strTemp.Format("%s,%f", str, chanVal[6]);
				str = strTemp;
				break;
			}




			if(str.GetLength())
			{
				strTemp.Format("%s\r\n", str);
				cfLogFile.Write(strTemp, strTemp.GetLength());
			}

			// Put the values in the graph.
			for(i=0; i<8; i++)
			{
				// Use the initilaizd values to determin which
				// data to add to the graph
				if(chanVal[i] > -11)	
					pThis->m_cIILG.put_dElement(i, chanVal[i]);
			}
		}
		
		// Allow a context switch
		Sleep(pThis->m_dwInterval);
		
	}

	if(bLogFileIsOpen)
	{
		cfLogFile.Close();
	}
	return 0;
}


void C485M300Log::OnEnChangeEbm300graphmin()
{
	UpdateData();
	m_dGraphMin = atof((LPCTSTR)m_strGraphMin);
	m_cIILG.SetdblMinValue(m_dGraphMin);
}

void C485M300Log::OnEnChangeEbm300graphmax()
{
	UpdateData();
	m_dGraphMax = atof((LPCTSTR)m_strGraphMax);
	m_cIILG.SetdblMaxValue(m_dGraphMax);
}
