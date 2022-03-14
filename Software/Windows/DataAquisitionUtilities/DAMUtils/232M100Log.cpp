// 232M100Log.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "232M100Log.h"
#include ".\232m100log.h"


// C232M100Log dialog

IMPLEMENT_DYNAMIC(C232M100Log, CDialog)
C232M100Log::C232M100Log(CWnd* pParent /*=NULL*/)
	: CDialog(C232M100Log::IDD, pParent)
	, m_cstrLogFilePath(_T(""))
	, m_bChannel0(FALSE)
	, m_bChannel1(FALSE)
	, m_bChannel2(FALSE)
	, m_bChannel3(FALSE)
	, m_bChannel4(FALSE)
	, m_bChannel5(FALSE)
	, m_bChannel6(FALSE)
	, m_bChannel7(FALSE)
	, m_dInterval(1.0)
	, m_pLogThread(NULL)
	, m_bLogFileIsOpen(false)
	, m_dwInterval(0)
	, m_bAXRedraw(FALSE)
	, m_strGraphMin(_T("0"))
	, m_strGraphMax(_T("10"))
	, m_dGraphMin(0)
	, m_dGraphMax(10)
{
}

C232M100Log::~C232M100Log()
{
}

void C232M100Log::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EB_M100LOGFILE, m_cstrLogFilePath);
	DDX_Check(pDX, IDC_CHKM100CH0, m_bChannel0);
	DDX_Check(pDX, IDC_CHKM100CH1, m_bChannel1);
	DDX_Check(pDX, IDC_CHKM100CH2, m_bChannel2);
	DDX_Check(pDX, IDC_CHKM100CH3, m_bChannel3);
	DDX_Check(pDX, IDC_CHKM100CH4, m_bChannel4);
	DDX_Check(pDX, IDC_CHKM100CH5, m_bChannel5);
	DDX_Check(pDX, IDC_CHKM100CH6, m_bChannel6);
	DDX_Check(pDX, IDC_CHKM100CH7, m_bChannel7);
	DDX_Text(pDX, IDC_EB_M100LOGINTERVAL, m_dInterval);
	DDX_Control(pDX, IDC_INTEGRITYLINEGRACTRL2, m_cIILG);
	DDX_Text(pDX, IDC_EBGRAPHMIN, m_strGraphMin);
	DDX_Text(pDX, IDC_EBGRAPHMAX, m_strGraphMax);
}


BEGIN_MESSAGE_MAP(C232M100Log, CDialog)
	ON_BN_CLICKED(IDC_BTNBROWSEM100LOG, OnBnClickedBtnbrowsem100log)
	ON_BN_CLICKED(IDC_BTN_M100LOGSTART, OnBnClickedBtnM100logstart)
	ON_BN_CLICKED(IDC_BTN_M100LOGSTOP, OnBnClickedBtnM100logstop)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EBGRAPHMIN, OnEnChangeEbgraphmin)
	ON_EN_CHANGE(IDC_EBGRAPHMAX, OnEnChangeEbgraphmax)
END_MESSAGE_MAP()


// C232M100Log message handlers

void C232M100Log::OnBnClickedBtnbrowsem100log()
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

void C232M100Log::OnBnClickedBtnM100logstart()
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
	channelCount += m_bChannel2 ? 1 : 0;
	channelCount += m_bChannel3 ? 1 : 0;
	channelCount += m_bChannel4 ? 1 : 0;
	channelCount += m_bChannel5 ? 1 : 0;
	channelCount += m_bChannel6 ? 1 : 0;
	channelCount += m_bChannel7 ? 1 : 0;

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
	CButton *pStart = (CButton*)GetDlgItem(IDC_BTN_M100LOGSTART);
	CButton *pHalt = (CButton*)GetDlgItem(IDC_BTN_M100LOGSTOP);
	CButton *pBrowse = (CButton*)GetDlgItem(IDC_BTNBROWSEM100LOG);
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EB_M100LOGFILE);

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
		if(m_bChannel2) channelCount = 3;
		if(m_bChannel3) channelCount = 4;
		if(m_bChannel4) channelCount = 5;
		if(m_bChannel5) channelCount = 6;
		if(m_bChannel6) channelCount = 7;
		if(m_bChannel7) channelCount = 8;


		m_cIILG.RemoveAll();
		m_cIILG.SetlnNumElements((long)channelCount);

		if(m_bChannel0)
		{
			m_cIILG.put_ulElementColor(0,GraphColors::ch0);
		}

		if(m_bChannel1)
		{
			m_cIILG.put_ulElementColor(1,GraphColors::ch1);
		}
		if(m_bChannel2)
		{
			m_cIILG.put_ulElementColor(2,GraphColors::ch2);
		}
		if(m_bChannel3)
		{
			m_cIILG.put_ulElementColor(3,GraphColors::ch3);
		}
		if(m_bChannel4)
		{
			m_cIILG.put_ulElementColor(4,GraphColors::ch4);
		}
		if(m_bChannel5)
		{
			m_cIILG.put_ulElementColor(5,GraphColors::ch5);
		}
		if(m_bChannel6)
		{
			m_cIILG.put_ulElementColor(6,GraphColors::ch6);
		}
		if(m_bChannel7)
		{
			m_cIILG.put_ulElementColor(7,GraphColors::ch7);
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

void C232M100Log::OnBnClickedBtnM100logstop()
{
	HaltThread();
	// TODO: Add your control notification handler code here
	CButton *pStart = (CButton*)GetDlgItem(IDC_BTN_M100LOGSTART);
	CButton *pHalt = (CButton*)GetDlgItem(IDC_BTN_M100LOGSTOP);
	CButton *pBrowse = (CButton*)GetDlgItem(IDC_BTNBROWSEM100LOG);
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EB_M100LOGFILE);

	pHalt->EnableWindow(FALSE);
	pEdit->EnableWindow(TRUE);
	pBrowse->EnableWindow(TRUE);
	pStart->EnableWindow(TRUE);



	m_cIICtrl->SetbPortOpen(FALSE);
	
}



void C232M100Log::OnBnClickedOk()
{
	HaltThread();
	m_cIICtrl->SetbPortOpen(FALSE);
	OnOK();
}

void C232M100Log::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	HaltThread();
	m_cIICtrl->SetbPortOpen(FALSE);
	OnCancel();
}


UINT C232M100Log::LogThreadProc(LPVOID pParam)
{
	C232M100Log *pThis = (C232M100Log*)pParam;
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
		
		str.Format("\r\nIntegrity Instruments Data Acquisition Module Utility\r\n");
		COleDateTime dt;
		dt = COleDateTime::GetCurrentTime();
		strTemp.Format("%sStart Time,%04d/%02d/%02d %02d:%02d:%02d\r\n",str,
			dt.GetYear(), dt.GetMonth(), dt.GetDay(),
			dt.GetHour(), dt.GetMinute(), dt.GetSecond());

		str = strTemp;
		if(pThis->m_bChannel0 | pThis->m_bChannel1 | pThis->m_bChannel2 | pThis->m_bChannel3 | 
			pThis->m_bChannel4 | pThis->m_bChannel5 | pThis->m_bChannel6 | pThis->m_bChannel7)
		{

			strTemp = "mS";
	
			if(pThis->m_bChannel0)
			{
				strTemp+=",CH0";
			}
	
			if(pThis->m_bChannel1)
			{
				strTemp+=",CH1";
			}
	
			if(pThis->m_bChannel2)
			{
				strTemp+=",CH2";
			}
	
			if(pThis->m_bChannel3)
			{
				strTemp+=",CH3";
			}
	
			if(pThis->m_bChannel4)
			{
				strTemp+=",CH4";
			}
	
			if(pThis->m_bChannel5)
			{
				strTemp+=",CH5";
			}
	
			if(pThis->m_bChannel6)
			{
				strTemp+=",CH6";
			}
	
			if(pThis->m_bChannel7)
			{
				strTemp+=",CH7";
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

			if(pThis->m_bChannel0 | pThis->m_bChannel1 | pThis->m_bChannel2 | pThis->m_bChannel3 | 
				pThis->m_bChannel4 | pThis->m_bChannel5 | pThis->m_bChannel6 | pThis->m_bChannel7)
			{
				str.Format("%ld", currentMsCount - pThis->m_dwStartMilliseconds);
			}



			if(pThis->m_bChannel0)
			{
				pThis->m_cIICtrl->SetlnADCControl(0);
				chanVal[0] = pThis->m_cIICtrl->GetdADCUnipolar();
				strTemp.Format("%s,%f", str, chanVal[0]);
				str = strTemp;
			}

			if(pThis->m_bChannel1)
			{
				pThis->m_cIICtrl->SetlnADCControl(1);
				chanVal[1] = pThis->m_cIICtrl->GetdADCUnipolar();
				strTemp.Format("%s,%f", str, chanVal[1]);
				str = strTemp;

			}

			if(pThis->m_bChannel2)
			{
				pThis->m_cIICtrl->SetlnADCControl(2);
				chanVal[2] = pThis->m_cIICtrl->GetdADCUnipolar();
				strTemp.Format("%s,%f", str, chanVal[2]);
				str = strTemp;

			}
			if(pThis->m_bChannel3)
			{
				pThis->m_cIICtrl->SetlnADCControl(3);
				chanVal[3] = pThis->m_cIICtrl->GetdADCUnipolar();
				strTemp.Format("%s,%f", str, chanVal[3]);
				str = strTemp;

			}
			if(pThis->m_bChannel4)
			{
				pThis->m_cIICtrl->SetlnADCControl(4);
				chanVal[4] = pThis->m_cIICtrl->GetdADCUnipolar();
				strTemp.Format("%s,%f", str, chanVal[4]);
				str = strTemp;

			}
			if(pThis->m_bChannel5)
			{
				pThis->m_cIICtrl->SetlnADCControl(5);
				chanVal[5] = pThis->m_cIICtrl->GetdADCUnipolar();
				strTemp.Format("%s,%f", str, chanVal[5]);
				str = strTemp;
			}
			if(pThis->m_bChannel6)
			{
				pThis->m_cIICtrl->SetlnADCControl(6);
				chanVal[6] = pThis->m_cIICtrl->GetdADCUnipolar();
				strTemp.Format("%s,%f", str, chanVal[6]);
				str = strTemp;
			}

			if(pThis->m_bChannel7)
			{
				pThis->m_cIICtrl->SetlnADCControl(7);
				chanVal[7] = pThis->m_cIICtrl->GetdADCUnipolar();
				strTemp.Format("%s,%f", str, chanVal[7]);
				str = strTemp;
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

void C232M100Log::HaltThread(void)
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

void C232M100Log::OnEnChangeEbgraphmin()
{
	
	UpdateData();
	m_dGraphMin = atof((LPCTSTR)m_strGraphMin);
	m_cIILG.SetdblMinValue(m_dGraphMin);

}

void C232M100Log::OnEnChangeEbgraphmax()
{
	
	UpdateData();
	m_dGraphMax = atof((LPCTSTR)m_strGraphMax);
	m_cIILG.SetdblMaxValue(m_dGraphMax);
}
