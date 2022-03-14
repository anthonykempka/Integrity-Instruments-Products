// 232M200Log.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "232M200Log.h"
#include ".\232m200log.h"


// C232M200Log dialog

IMPLEMENT_DYNAMIC(C232M200Log, CDialog)
C232M200Log::C232M200Log(CWnd* pParent /*=NULL*/)
	: CDialog(C232M200Log::IDD, pParent)
	, m_cstrLogFilePath(_T(""))
	, m_bChannel0(FALSE)
	, m_bChannel1(FALSE)
	, m_dInterval(1.0)
	, m_pLogThread(NULL)
	, m_bLogFileIsOpen(false)
	, m_dwInterval(0)
	, m_bAXRedraw(FALSE)
	, m_strGraphMin(_T("0"))
	, m_strGraphMax(_T("5"))
	, m_dGraphMin(0)
	, m_dGraphMax(5)
	, m_nSampleType(0)
{
}

C232M200Log::~C232M200Log()
{
}

void C232M200Log::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EB_M200LOGFILE, m_cstrLogFilePath);
	DDX_Check(pDX, IDC_CHKM200CH0, m_bChannel0);
	DDX_Check(pDX, IDC_CHKM200CH1, m_bChannel1);
	DDX_Text(pDX, IDC_EB_M200LOGINTERVAL, m_dInterval);
	DDX_Text(pDX, IDC_EBM200GRAPHMIN, m_strGraphMin);
	DDX_Text(pDX, IDC_EBM200GRAPHMAX, m_strGraphMax);

	DDX_Control(pDX, IDC_INTEGRITYLINEGRACTRL1, m_cIILG);
}


BEGIN_MESSAGE_MAP(C232M200Log, CDialog)
	ON_BN_CLICKED(IDC_BTNBROWSEM200LOG, OnBnClickedBtnbrowsem200log)
	ON_BN_CLICKED(IDC_BTN_M200LOGSTART, OnBnClickedBtnM200logstart)
	ON_BN_CLICKED(IDC_BTN_M200LOGSTOP, OnBnClickedBtnM200logstop)
	ON_EN_CHANGE(IDC_EBM200GRAPHMIN, OnEnChangeEbm200graphmin)
	ON_EN_CHANGE(IDC_EBM200GRAPHMAX, OnEnChangeEbm200graphmax)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RBDIFFADCOFF, OnBnClickedRbdiffadcoff)
	ON_BN_CLICKED(IDC_RBDIFFADCPOS, OnBnClickedRbdiffadcpos)
	ON_BN_CLICKED(IDC_RBDIFFADCNEG, OnBnClickedRbdiffadcneg)
END_MESSAGE_MAP()


// C232M200Log message handlers
void C232M200Log::OnBnClickedBtnbrowsem200log()
{
	static char BASED_CODE szFilter[] = "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||";
	CFileDialog cDlg(FALSE, "csv", NULL, OFN_HIDEREADONLY , szFilter);
	UpdateData();
	if(IDOK == cDlg.DoModal())
	{
		m_cstrLogFilePath = cDlg.GetPathName();
		UpdateData(FALSE);
	}

}

void C232M200Log::OnBnClickedBtnM200logstart()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	// Check to see if the interval is long enough
	// Request Length: 30 bits
	// Respnse Length: 56 bits
	// Total bits: 86

	double requestTime;
	double channelCount = 0;
	requestTime = (1/(double)m_cIICtrl->GetlnBaudRate()) * 86;

	channelCount += m_bChannel0 ? 1 : 0;
	channelCount += m_bChannel1 ? 1 : 0;
	if(m_nSampleType) channelCount = 1;

	requestTime *= channelCount;



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

	m_cIICtrl->SetbPortOpen(TRUE);

	if(!m_cIICtrl->GetbPortOpen())
	{
		return;
	}
	// Open the file


	// Set button states
	CButton *pStart = (CButton*)GetDlgItem(IDC_BTN_M200LOGSTART);
	CButton *pHalt = (CButton*)GetDlgItem(IDC_BTN_M200LOGSTOP);
	CButton *pBrowse = (CButton*)GetDlgItem(IDC_BTNBROWSEM200LOG);
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EB_M200LOGFILE);

	pEdit->EnableWindow(FALSE);
	pBrowse->EnableWindow(FALSE);

	pHalt->EnableWindow(TRUE);
	pStart->EnableWindow(FALSE);


	// Everything is good to go, now configure the Graphing Control.
	
	m_cIILG.SetdblMaxValue(m_dGraphMax);
	m_cIILG.SetdblMinValue(m_dGraphMin);

	if(channelCount)
	{
		if(m_bChannel0) channelCount = 1;
		if(m_bChannel1) channelCount = 2;
		if(m_nSampleType) channelCount = 1;


		m_cIILG.RemoveAll();
		m_cIILG.SetlnNumElements((long)channelCount);

		if(m_bChannel0)
		{
			m_cIILG.put_ulElementColor(0,C232M200Log::GraphColors::ch0);
		}

		if(m_bChannel1)
		{
			m_cIILG.put_ulElementColor(1,C232M200Log::GraphColors::ch1);
		}

		if(m_nSampleType)
		{
			m_cIILG.put_ulElementColor(0,C232M200Log::GraphColors::ch0);
		}
	}
	

	if(!m_pLogThread)
	{
		// Start up the thread
		m_ceHaltThread.ResetEvent();
		m_pLogThread = AfxBeginThread(LogThreadProc, this,THREAD_PRIORITY_LOWEST);
		m_bAXRedraw = this->m_cIICtrl->GetbInvalidate();
		m_cIICtrl->SetbInvalidate(FALSE);
	}


}

void C232M200Log::OnBnClickedBtnM200logstop()
{
	HaltThread();
	// TODO: Add your control notification handler code here
	CButton *pStart = (CButton*)GetDlgItem(IDC_BTN_M200LOGSTART);
	CButton *pHalt = (CButton*)GetDlgItem(IDC_BTN_M200LOGSTOP);
	CButton *pBrowse = (CButton*)GetDlgItem(IDC_BTNBROWSEM200LOG);
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EB_M200LOGFILE);

	pHalt->EnableWindow(FALSE);
	pEdit->EnableWindow(TRUE);
	pBrowse->EnableWindow(TRUE);
	pStart->EnableWindow(TRUE);



	m_cIICtrl->SetbPortOpen(FALSE);
	
}




void C232M200Log::HaltThread(void)
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

UINT C232M200Log::LogThreadProc(LPVOID pParam)
{
	C232M200Log *pThis = (C232M200Log*)pParam;
	CString str;
	CString strTemp;
	DWORD lastMsCount, currentMsCount;
	CFile cfLogFile;
	bool bLogFileIsOpen = false;
	double chanVal[8] = {-11,-11};
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
		
		str.Format("\r\nIntegrity Instruments Data Acquisition Module Utility\r\n");
		COleDateTime dt;
		dt = COleDateTime::GetCurrentTime();
		strTemp.Format("%sStart Time,%04d/%02d/%02d %02d:%02d:%02d\r\n",str,
			dt.GetYear(), dt.GetMonth(), dt.GetDay(),
			dt.GetHour(), dt.GetMinute(), dt.GetSecond());

		str = strTemp;
		if(pThis->m_bChannel0 || pThis->m_bChannel1 || pThis->m_nSampleType)
		{

			strTemp = "mS";
	
			switch(pThis->m_nSampleType)
			{
			case 0:
				if(pThis->m_bChannel0)
				{
					strTemp+=",CH0";
				}
		
				if(pThis->m_bChannel1)
				{
					strTemp+=",CH1";
				}
				break;
			case 1:
				strTemp += ",CH0+ CH1-";
				break;
			case 2:
				strTemp += ",CH0- CH1+";
				break;
			}
	

			strTemp += "\r\n";
			str += strTemp;
		}

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

			if(pThis->m_bChannel0 | pThis->m_bChannel1 | pThis->m_nSampleType)
			{
				str.Format("%ld", currentMsCount - pThis->m_dwStartMilliseconds);
			}

			switch(pThis->m_nSampleType)
			{
			case 0:
				if(pThis->m_bChannel0)
				{
					pThis->m_cIICtrl->SetlnADCControl(2);
					chanVal[0] = pThis->m_cIICtrl->GetdADCUnipolar();
					strTemp.Format("%s,%f", str, chanVal[0]);
					str = strTemp;
				}
				if(pThis->m_bChannel1)
				{
					pThis->m_cIICtrl->SetlnADCControl(3);
					chanVal[1] = pThis->m_cIICtrl->GetdADCUnipolar();
					strTemp.Format("%s,%f", str, chanVal[1]);
					str = strTemp;
				}
				break;
			case 1:
				pThis->m_cIICtrl->SetlnADCControl(0);
				chanVal[0] = pThis->m_cIICtrl->GetdADCUnipolar();
				chanVal[1] = -11;
				strTemp.Format("%s,%f", str, chanVal[0]);
				str = strTemp;
				break;
			case 2:
				pThis->m_cIICtrl->SetlnADCControl(1);
				chanVal[0] = pThis->m_cIICtrl->GetdADCUnipolar();
				chanVal[1] = -11;
				strTemp.Format("%s,%f", str, chanVal[0]);
				str = strTemp;
				break;
			}


			if(str.GetLength())
			{
				strTemp.Format("%s\r\n", str);
				cfLogFile.Write(strTemp, strTemp.GetLength());
			}

			// Put the values in the graph.
			for(i=0; i<2; i++)
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

void C232M200Log::OnEnChangeEbm200graphmin()
{
	UpdateData();
	m_dGraphMin = atof((LPCTSTR)m_strGraphMin);
	m_cIILG.SetdblMinValue(m_dGraphMin);
}

void C232M200Log::OnEnChangeEbm200graphmax()
{
	UpdateData();
	m_dGraphMax = atof((LPCTSTR)m_strGraphMax);
	m_cIILG.SetdblMaxValue(m_dGraphMax);
}
void C232M200Log::OnBnClickedOk()
{
	HaltThread();
	m_cIICtrl->SetbPortOpen(FALSE);
	OnOK();
}

void C232M200Log::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	HaltThread();
	m_cIICtrl->SetbPortOpen(FALSE);
	OnCancel();
}


void C232M200Log::OnBnClickedRbdiffadcoff()
{
	SetChannelBtnStates(true);
	m_nSampleType = 0;
}

void C232M200Log::OnBnClickedRbdiffadcpos()
{
	SetChannelBtnStates(false);
	m_nSampleType = 1;
}

void C232M200Log::OnBnClickedRbdiffadcneg()
{
	SetChannelBtnStates(false);
	m_nSampleType = 2;
}

void C232M200Log::SetChannelBtnStates(bool enabled)
{
	CButton* pCh0 = (CButton*)GetDlgItem(IDC_CHKM200CH0);
	CButton* pCh1 = (CButton*)GetDlgItem(IDC_CHKM200CH1);

	if(!enabled)
	{
		pCh0->SetCheck(0);
		pCh1->SetCheck(0);
	}
	pCh0->EnableWindow(enabled ? TRUE : FALSE);
	pCh1->EnableWindow(enabled ? TRUE : FALSE);

}

BOOL C232M200Log::OnInitDialog()
{
	CDialog::OnInitDialog();

	CButton* pChBipolar = (CButton*)GetDlgItem(IDC_RBDIFFADCOFF);

	pChBipolar->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
