// DlgUsbStreamManager.cpp : implementation file
//

#include "stdafx.h"
#include "DAMUtils.h"
#include "DlgUsbStreamManager.h"
#include ".\dlgusbstreammanager.h"

#define UPDATE_UIINFO	WM_USER + 200

// CDlgUsbStreamManager dialog

IMPLEMENT_DYNAMIC(CDlgUsbStreamManager, CDialog)
CDlgUsbStreamManager::CDlgUsbStreamManager(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUsbStreamManager::IDD, pParent)
	, m_nUnitAddress(0)
	, m_cstrUnitAddress(_T(""))
	, m_cstrStatus(_T(""))
	, m_cstrDataBlocks(_T(""))
	, m_cstrAvailable(_T(""))
	, m_cstrDisplay(_T(""))
	, m_bLogToFile(FALSE)
	, m_cstrLogFilePath(_T(""))
	, m_dwStartTicks(0)
	, m_dwRxByteCount(0)
	, m_cstrBPS(_T(""))
	, m_cstrRunTime(_T(""))
	, m_lnMessageLock(0)
	, m_dGraphMax(2.5)
{
	m_pStreamThread = NULL;
}

CDlgUsbStreamManager::~CDlgUsbStreamManager()
{
}

void CDlgUsbStreamManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STUNITADDRESS, m_cstrUnitAddress);
	DDX_Text(pDX, IDC_STSTATUS, m_cstrStatus);
	DDX_Text(pDX, IDC_STDATABLOCKS, m_cstrDataBlocks);
	DDX_Text(pDX, IDC_STAVAILABLE, m_cstrAvailable);
	DDX_Text(pDX, IDC_STDISPLAY, m_cstrDisplay);
	DDX_Check(pDX, IDC_CHECK1, m_bLogToFile);
	DDX_Text(pDX, IDC_EBLOGFILE, m_cstrLogFilePath);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
	DDX_Control(pDX, IDC_BTNSTOP, m_btnStop);
	DDX_Text(pDX, IDC_STBPS, m_cstrBPS);
	DDX_Text(pDX, IDC_STTIME, m_cstrRunTime);
	DDX_Control(pDX, IDC_INTEGRITYBARGRAPCTRL2, m_cBarGraph);
}


BEGIN_MESSAGE_MAP(CDlgUsbStreamManager, CDialog)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_START, OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTNSTOP, OnBnClickedBtnstop)
//	ON_WM_TIMER()
	ON_MESSAGE(UPDATE_UIINFO, OnUpdateUIData)
//	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
END_MESSAGE_MAP()


// CDlgUsbStreamManager message handlers

void CDlgUsbStreamManager::OnBnClickedBtnBrowse()
{
	static char BASED_CODE szFilter[] = "CSV (*.csv)|*.csv|All Files (*.*)|*.*||";
	CFileDialog cDlg(FALSE, "*.csv", "*.csv", OFN_HIDEREADONLY, szFilter);
	
	UpdateData();

	if(IDOK == cDlg.DoModal())
	{
		m_cstrLogFilePath = cDlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDlgUsbStreamManager::OnBnClickedBtnStart()
{
	
	// fire up a thread to handle the stream mode.
	UpdateData();
	if(!m_pStreamThread)
	{
		// Start up the thread
		m_ceHaltThread.ResetEvent();
		m_pStreamThread = AfxBeginThread(StreamThreadProc, this,THREAD_PRIORITY_LOWEST);

		// force user to stop stream before any other action
		// can be taken
		m_btnOK.EnableWindow(FALSE);
		m_btnCancel.EnableWindow(FALSE);
		m_btnStart.EnableWindow(FALSE);
		m_btnStop.EnableWindow(TRUE);
	}

	
}
void CDlgUsbStreamManager::OnBnClickedBtnstop()
{
	if(m_pStreamThread)
	{
		// thread appears to be running ... stop it
		BeginWaitCursor();

		// Signal thread that it's time to exit
		m_ceHaltThread.SetEvent();

		// Wait for the thread to exit
		while(WAIT_OBJECT_0 != WaitForSingleObject(m_pStreamThread->m_hThread, INFINITE))
		{
			// Context Switch :D
			Sleep(55);			
		}

		// clear our event
		m_ceHaltThread.ResetEvent();

		// disable the stop button
		m_btnStop.EnableWindow(FALSE);
		m_btnOK.EnableWindow(TRUE);
		m_btnCancel.EnableWindow(TRUE);
		m_btnStart.EnableWindow(TRUE);

		// Invalidate the CWinThread pointer.
		m_pStreamThread = NULL;
		EndWaitCursor();
	}
	UpdateData(FALSE);

}

BOOL CDlgUsbStreamManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	// add the unit address to the statistics group
	m_cstrUnitAddress.Format("Unit Address: %d", m_nUnitAddress);
	m_cstrStatus.Format("Status: Idle");
	m_cstrDataBlocks.Format("Data Blocks: 0");
	m_cstrAvailable.Format("Que Size: 0");
	m_cstrBPS.Format("Bytes / Sec.: 0");
	m_cstrRunTime.Format("Run Time: 0");

	UpdateData(FALSE);

	
	// disable the stop button
	m_btnStop.EnableWindow(FALSE);

	m_cBarGraph.SetlnNumElements(32);
	m_cBarGraph.SetdblMaxValue(m_dGraphMax);
	m_cBarGraph.SetdblMinValue(0);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



LRESULT CDlgUsbStreamManager::OnUpdateUIData(WPARAM wParam, LPARAM lParam)
{
	int index;
	double *dblVal = (double*)wParam;
	for(index=0; index < lParam; (int)index++)
	{
		m_cBarGraph.put_dElement(index, dblVal[index]);
	}


	// Thread Safe decrement message lock.
	// This message handler Must only be asserted by 
	// our StreamThreadProc procedure
	::InterlockedDecrement(&m_lnMessageLock);			



	UpdateData(FALSE);


	return 0L;
}

/*
 * USB Stream Thread Proc
 */
UINT CDlgUsbStreamManager::StreamThreadProc(LPVOID pParam)
{
	CDlgUsbStreamManager *pThis = (CDlgUsbStreamManager*)pParam;
	CFile cfLogFile;
	CString str;
	BOOL bFileOpenFlag;
	DWORD rxByteCount;
	DWORD startTime;
	DWORD currentTime;
	DWORD elapsedTime;
	DWORD loopCount;
	double data[32];
	int blockCount;
	int index;
	int blocksAvailable;
	int queSize;
	long lockTemp;

	TRACE("StreamThreadProc Started\n");

	
	// Setup the activeX control

	// 1. USB Protocol
	pThis->m_cIICtrl->SetlnProtocol(3);

	// 2. Unit Address
	pThis->m_cIICtrl->SetlnModuleAddress(pThis->m_nUnitAddress);

	// 3. Open comm channle
	pThis->m_cIICtrl->SetbPortOpen(TRUE);
	if(!pThis->m_cIICtrl->GetbPortOpen())
	{
		// TODO: Error Opening Communicatoin
	}

	// 4. Start Streaming
	pThis->m_cIICtrl->SetbModeStream(TRUE);

	
	// 5. Check for File Open Flag
	bFileOpenFlag = FALSE;
	if(pThis->m_bLogToFile)
	{
		if(!cfLogFile.Open(pThis->m_cstrLogFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
		{
			// TODO: Error Opening File
			bFileOpenFlag = FALSE;
		}
		else
		{
			bFileOpenFlag = TRUE;
		}
	}
	
	rxByteCount = 0;				// Clear byte counter
	startTime = GetTickCount();		// Get the Current Tick Count
	blockCount = 0;
	elapsedTime = 0;
	loopCount = 0;
	queSize = 0;

	pThis->m_cstrStatus.Format("Status: Stream Active");

	while(WAIT_OBJECT_0 != ::WaitForSingleObject(pThis->m_ceHaltThread, 0))
	{
//		pThis->m_cstrAvailable.Format("Que Size: %d", pThis->m_cIICtrl->GetlnUSBStreamBlockCount());
		
		blocksAvailable = pThis->m_cIICtrl->GetUSBDataBlock(data, 0, 32);
		queSize = queSize < blocksAvailable ? blocksAvailable : queSize;
		
		while( blocksAvailable > 0)
		{
			// USB Data Block Received
			if(bFileOpenFlag)
			{
				for(index=0; index < 32; index++)
				{
					// Comma Delimited, <CRLF> terminated string
					if(index < 31)
					{
						str.Format("%f,", data[index]);
					}
					else
					{
						str.Format("%f\r\n", data[index]);
					}
					cfLogFile.Write((LPCTSTR)str, str.GetLength());	
				}
				
				
			}

			rxByteCount += 64;			// A block of data has been extracted
			blockCount++;
			
			blocksAvailable = pThis->m_cIICtrl->GetUSBDataBlock(data, 0, 32);
			queSize = queSize < blocksAvailable ? blocksAvailable : queSize;

		}



		currentTime = GetTickCount();
		elapsedTime = currentTime - startTime;

		// Update the UI at 100mS Rate
		pThis->m_cstrAvailable.Format("Que Size: %d", queSize);
		pThis->m_cstrDisplay.Format("%f, %f, %f, %f, %f, %f, %f, %f", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
		pThis->m_cstrBPS.Format("Data Bytes: %012d", rxByteCount );
		pThis->m_cstrRunTime.Format("Run Time: %06d", elapsedTime / 1000 );
		pThis->m_cstrDataBlocks.Format("Received Blocks: %d", blockCount);
		queSize = 0;
		
		if(pThis->m_lnMessageLock == 0)
		{
			pThis->PostMessage(UPDATE_UIINFO, (WPARAM)&data, 32L );
			InterlockedIncrement(&pThis->m_lnMessageLock);			
		}
		// Loop 5 times per second
		Sleep(100);
	}

	if(bFileOpenFlag)
	{
		cfLogFile.Close();
	}
	pThis->m_cIICtrl->SetbModeStream(FALSE);
	pThis->m_cIICtrl->SetbPortOpen(FALSE);

	TRACE("StreamThreadProc Terminated\n");


	// Context switch ... allow any pending posted messages to complete
	while((lockTemp = pThis->m_lnMessageLock) != 0)
	{
		Sleep(55);
	}
	pThis->m_cstrStatus.Format("Status: Idle");

	return 0;
}

