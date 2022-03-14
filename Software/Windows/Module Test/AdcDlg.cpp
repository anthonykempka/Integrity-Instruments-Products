// AdcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test_IO.h"
#include "AdcDlg.h"
#include "Comm32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdcDlg dialog


CAdcDlg::CAdcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdcDlg)
	m_Samp1 = _T("");
	m_Samp2 = _T("");
	m_Samp3 = _T("");
	m_Samp4 = _T("");
	m_Samp5 = _T("");
	m_Samp6 = _T("");
	m_Samp7 = _T("");
	m_Samp8 = _T("");
	//}}AFX_DATA_INIT
}


void CAdcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdcDlg)
	DDX_Text(pDX, IDC_SAMPLE1, m_Samp1);
	DDX_Text(pDX, IDC_SAMPLE2, m_Samp2);
	DDX_Text(pDX, IDC_SAMPLE3, m_Samp3);
	DDX_Text(pDX, IDC_SAMPLE4, m_Samp4);
	DDX_Text(pDX, IDC_SAMPLE5, m_Samp5);
	DDX_Text(pDX, IDC_SAMPLE6, m_Samp6);
	DDX_Text(pDX, IDC_SAMPLE7, m_Samp7);
	DDX_Text(pDX, IDC_SAMPLE8, m_Samp8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdcDlg, CDialog)
	//{{AFX_MSG_MAP(CAdcDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdcDlg message handlers

void CAdcDlg::OnOK() 
{
    int BaudRate, ComPort;

    if (bTimerRunning) {        // Don't do anything if it's already running.
        AfxMessageBox("Sampling already started Bozo!");
        return;
    }

	//Read the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);


    // Default 1 (COM2) if no setting found
	ComPort = pApp->GetProfileInt("Settings", "ComPort", 1);

    // Default 0 (115200) if not setting found
	BaudRate = pApp->GetProfileInt("Settings", "BaudRate", 0);

    //
    // Adjust zero based offset to COMx number
    ComPort += 1;
    
    // Default 1 (RS485) if no setting found
	bRS485_Check = pApp->GetProfileInt("Settings", "RS485", 1);

    //
    // Convert from zero based offset to baud rate
    switch (BaudRate) {
        case 0:
            BaudRate = 115200;
            break;
        case 1:
            BaudRate = 57600;
            break;
        case 2:
            BaudRate = 19200;
            break;
        case 3:
        default:
            BaudRate = 9600;
            break;
    }

    //
    // Attempt to open a Comport
    //

    hCom = ::Open_Comport (ComPort, BaudRate);

    if (hCom != INVALID_HANDLE_VALUE) {

        //
        // Setup the window timer
        //

        if (bRS485_Check) {
            RS485_Test_Adc();
            SetTimer (1, 1000, NULL);       // RS485 Test
        } else {
            RS232_Test_Adc();
            SetTimer (2, 1000, NULL);       // RS232 Test
        }
        
        bTimerRunning = TRUE;
    } else {
        AfxMessageBox("Cannot open CommPort!");
        bTimerRunning = FALSE;
    }


//	CDialog::OnOK();
}

void CAdcDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

    if (bTimerRunning) {        // Only kill timer if one was made

        if (bRS485_Check) {
            KillTimer(1);
        } else {
            KillTimer(2);
        }
        ::Close_Comport (hCom);
    }
	
	CDialog::OnCancel();
}

void CAdcDlg::RS232_Test_Adc()
{
    DWORD   BytesRead;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);
    CString Command_Str;
    CString Response_Str;


    Command_Str = "U8\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp1 = Response_Str;

    Command_Str = "UC\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp2 = Response_Str;

    Command_Str = "U9\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp3 = Response_Str;

    Command_Str = "UD\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp4 = Response_Str;

    Command_Str = "UA\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp5 = Response_Str;

    Command_Str = "UE\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp6 = Response_Str;

    Command_Str = "UB\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp7 = Response_Str;

    Command_Str = "UF\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp8 = Response_Str;

    
    UpdateData (FALSE);
}

void CAdcDlg::RS485_Test_Adc()
{
    DWORD   BytesRead;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);
    CString Command_Str;
    CString Response_Str;


    Command_Str = "0100U8\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp1 = Response_Str;

    Command_Str = "0100UC\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp2 = Response_Str;

    Command_Str = "0100U9\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp3 = Response_Str;

    Command_Str = "0100UD\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp4 = Response_Str;

    Command_Str = "0100UA\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp5 = Response_Str;

    Command_Str = "0100UE\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp6 = Response_Str;

    Command_Str = "0100UB\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp7 = Response_Str;

    Command_Str = "0100UF\r";
    ::Write_Comport (hCom, Command_Str.GetLength(), Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    Buffer[BytesRead] = 0;          // Null terminate for printing
    Response_Str = Buffer;
    m_Samp8 = Response_Str;

    
    UpdateData (FALSE);
}

void CAdcDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

    if (nIDEvent == 1) {
            RS485_Test_Adc();
    }

    if (nIDEvent == 2) {
            RS232_Test_Adc();
    }

	
	CDialog::OnTimer(nIDEvent);
}

BOOL CAdcDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    bTimerRunning = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
