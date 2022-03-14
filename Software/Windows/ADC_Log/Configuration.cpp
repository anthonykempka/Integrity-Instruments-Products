// Configuration.cpp : implementation file
//

#include "stdafx.h"
#include "ADC_Log.h"
#include "Configuration.h"
#include "Comm32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfiguration dialog


CConfiguration::CConfiguration(CWnd* pParent /*=NULL*/)
	: CDialog(CConfiguration::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfiguration)
	m_Log_Interval = 0;
	m_Filter = 0;
	//}}AFX_DATA_INIT
}


void CConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfiguration)
	DDX_Control(pDX, IDC_SAMPLE8, m_Sample8);
	DDX_Control(pDX, IDC_SAMPLE7, m_Sample7);
	DDX_Control(pDX, IDC_SAMPLE6, m_Sample6);
	DDX_Control(pDX, IDC_SAMPLE5, m_Sample5);
	DDX_Control(pDX, IDC_SAMPLE4, m_Sample4);
	DDX_Control(pDX, IDC_SAMPLE3, m_Sample3);
	DDX_Control(pDX, IDC_SAMPLE2, m_Sample2);
	DDX_Control(pDX, IDC_SAMPLE1, m_Sample1);
	DDX_Control(pDX, IDC_VREF, m_Vref);
	DDX_Control(pDX, IDC_UNI_SAMPLE8, m_Uni_Sample8);
	DDX_Control(pDX, IDC_UNI_SAMPLE7, m_Uni_Sample7);
	DDX_Control(pDX, IDC_UNI_SAMPLE6, m_Uni_Sample6);
	DDX_Control(pDX, IDC_UNI_SAMPLE5, m_Uni_Sample5);
	DDX_Control(pDX, IDC_UNI_SAMPLE4, m_Uni_Sample4);
	DDX_Control(pDX, IDC_UNI_SAMPLE3, m_Uni_Sample3);
	DDX_Control(pDX, IDC_UNI_SAMPLE2, m_Uni_Sample2);
	DDX_Control(pDX, IDC_UNI_SAMPLE1, m_Uni_Sample1);
	DDX_Text(pDX, IDC_LOG_INTERVAL, m_Log_Interval);
	DDV_MinMaxDWord(pDX, m_Log_Interval, 1, 100000);
	DDX_Text(pDX, IDC_FILTER, m_Filter);
	DDV_MinMaxInt(pDX, m_Filter, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfiguration, CDialog)
	//{{AFX_MSG_MAP(CConfiguration)
	ON_BN_CLICKED(IDC_OFFSET, OnOffset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfiguration message handlers

BOOL CConfiguration::OnInitDialog() 
{
    int Sel;

	CDialog::OnInitDialog();
	
	//Read the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);


    // Default 0 (None) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Sample1", 0);
    m_Sample1.SetCurSel(Sel);

    // Default 0 (None) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Sample2", 0);
    m_Sample2.SetCurSel(Sel);

    // Default 0 (None) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Sample3", 0);
    m_Sample3.SetCurSel(Sel);

    // Default 0 (None) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Sample4", 0);
    m_Sample4.SetCurSel(Sel);

    // Default 0 (None) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Sample5", 0);
    m_Sample5.SetCurSel(Sel);

    // Default 0 (None) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Sample6", 0);
    m_Sample6.SetCurSel(Sel);

    // Default 0 (None) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Sample7", 0);
    m_Sample7.SetCurSel(Sel);

    // Default 0 (None) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Sample8", 0);
    m_Sample8.SetCurSel(Sel);


    // Default 0 (Not checked) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Unipolar1", 0);
    m_Uni_Sample1.SetCheck(Sel);

    // Default 0 (Not checked) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Unipolar2", 0);
    m_Uni_Sample2.SetCheck(Sel);

    // Default 0 (Not checked) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Unipolar3", 0);
    m_Uni_Sample3.SetCheck(Sel);

    // Default 0 (Not checked) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Unipolar4", 0);
    m_Uni_Sample4.SetCheck(Sel);

    // Default 0 (Not checked) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Unipolar5", 0);
    m_Uni_Sample5.SetCheck(Sel);

    // Default 0 (Not checked) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Unipolar6", 0);
    m_Uni_Sample6.SetCheck(Sel);

    // Default 0 (Not checked) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Unipolar7", 0);
    m_Uni_Sample7.SetCheck(Sel);

    // Default 0 (Not checked) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Unipolar8", 0);
    m_Uni_Sample8.SetCheck(Sel);


    // Default 3 (5.000V) if no setting found
	Sel = pApp->GetProfileInt("Settings", "Vref", 3);
    m_Vref.SetCurSel(Sel);

    // Default 10 seconds - 100 counts
    m_Log_Interval = pApp->GetProfileInt("Settings", "Interval", 100);

    // Default = 0 (no filter)
    m_Filter = pApp->GetProfileInt("Settings", "Filter", 0);

    UpdateData (FALSE); // Update after setting (write)    
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfiguration::OnOK() 
{
	UpdateData (TRUE); // Update before reading values

	//Write the new registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);

	pApp->WriteProfileInt("Settings", "Sample1", m_Sample1.GetCurSel());
	pApp->WriteProfileInt("Settings", "Sample2", m_Sample2.GetCurSel());
	pApp->WriteProfileInt("Settings", "Sample3", m_Sample3.GetCurSel());
	pApp->WriteProfileInt("Settings", "Sample4", m_Sample4.GetCurSel());
	pApp->WriteProfileInt("Settings", "Sample5", m_Sample5.GetCurSel());
	pApp->WriteProfileInt("Settings", "Sample6", m_Sample6.GetCurSel());
	pApp->WriteProfileInt("Settings", "Sample7", m_Sample7.GetCurSel());
	pApp->WriteProfileInt("Settings", "Sample8", m_Sample8.GetCurSel());

	pApp->WriteProfileInt("Settings", "Unipolar1", m_Uni_Sample1.GetCheck());
	pApp->WriteProfileInt("Settings", "Unipolar2", m_Uni_Sample2.GetCheck());
	pApp->WriteProfileInt("Settings", "Unipolar3", m_Uni_Sample3.GetCheck());
	pApp->WriteProfileInt("Settings", "Unipolar4", m_Uni_Sample4.GetCheck());
	pApp->WriteProfileInt("Settings", "Unipolar5", m_Uni_Sample5.GetCheck());
	pApp->WriteProfileInt("Settings", "Unipolar6", m_Uni_Sample6.GetCheck());
	pApp->WriteProfileInt("Settings", "Unipolar7", m_Uni_Sample7.GetCheck());
	pApp->WriteProfileInt("Settings", "Unipolar8", m_Uni_Sample8.GetCheck());

	pApp->WriteProfileInt("Settings", "Vref", m_Vref.GetCurSel());
    pApp->WriteProfileInt("Settings", "Interval", m_Log_Interval);
    pApp->WriteProfileInt("Settings", "Filter", m_Filter);
    
	CDialog::OnOK();
}

void CConfiguration::OnOffset() 
{
    int m_ComPort, m_BaudRate, m_RS485_Check;


    CWaitCursor Cur;        // Displays the Wait cursor

	//Read the registry entrys
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);


    // Default 1 (COM2) if no setting found
	m_ComPort = pApp->GetProfileInt("Settings", "ComPort", 1);

    // Default 0 (115200) if not setting found
	m_BaudRate = pApp->GetProfileInt("Settings", "BaudRate", 0);

    //
    m_RS485_Check = pApp->GetProfileInt("Settings", "RS485", 0);

    //
    // Adjust zero based offset to COMx number
    m_ComPort += 1;
        
    //
    // Convert from zero based offset to baud rate
    switch (m_BaudRate) {
        case 0:
            m_BaudRate = 115200;
            break;
        case 1:
            m_BaudRate = 57600;
            break;
        case 2:
            m_BaudRate = 19200;
            break;
        case 3:
        default:
            m_BaudRate = 9600;
            break;
    }

    //
    // Attempt to open a Comport
    //

    hCom = ::Open_Comport (m_ComPort, m_BaudRate);

    if (hCom != INVALID_HANDLE_VALUE) {

        //
        // Perform test battery here
        //

        if (m_RS485_Check) {

            //
            // RS485 Offset calibration
            //
            RS485_Offset();

        } else {

            //
            // RS232 Offset Calibration
            //
            RS232_Offset();

        }

        ::Close_Comport (hCom);
    } else {
       AfxMessageBox("Cannot open CommPort!");
    }
}

void CConfiguration::RS485_Offset()
{
    DWORD   BytesRead;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);
    int     Sample_Count, Offset, i;

    //
    // Warn the user to Short CH0 to GND
    if (AfxMessageBox("ADC-x Offset Calibration\r\rPlease attach CH0 to GND", MB_OKCANCEL) != IDOK) {
        return;
    }

    //
    // Take a bipolar reading - 10 times for averaging

    Offset = 0;

    for (i=0; i<10; i++) {

        m_Command_Str = "FF00Q8\r";
        ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));
        Sleep (10);
        ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);

        //
        // Null terminate for printing
        Buffer[BytesRead] = 0;
        m_Response_Str = Buffer;

        if (BytesRead != 10) {
            MessageBox ("Analog response not received from Module!", "Error", MB_ICONERROR);
            return;
        }

        ::sscanf(&Buffer[6], "%X", &Sample_Count);

        if (Sample_Count >= 2048) {
            Offset += -(Sample_Count-4097);
        } else {
            Offset += -Sample_Count;
        }
    }

    //
    // Average the offset value
    Offset = Offset / 10;

    //
    // Save the offset calibration to EEPROM location 0x0F
    m_Command_Str.Format("FF00W0F%02X\r", Offset);
    ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));
    Sleep (10);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    //
    // Null terminate for printing
    Buffer[BytesRead] = 0;
    m_Response_Str = Buffer;

    if (BytesRead != 6) {
        MessageBox ("EEPROM Write response not received from Module!", "Error", MB_ICONERROR);
        return;
    } else {
        m_Command_Str.Format ("Calibration value = %02X stored in EEPROM location 0x0F", Offset);
        AfxMessageBox(m_Command_Str, MB_OK);
        return;
    }
}

void CConfiguration::RS232_Offset()
{
    DWORD   BytesRead;
    char    Buffer[100];
    DWORD   BufferSize = sizeof (Buffer);
    int     Sample_Count, Offset, i;

    //
    // Warn the user to Short CH0 to GND
    if (AfxMessageBox("ADC-x Offset Calibration\r\rPlease attach CH0 to GND", MB_OKCANCEL) != IDOK) {
        return;
    }

    //
    // Take a bipolar reading - 10 times for averaging

    Offset = 0;

    for (i=0; i<10; i++) {

        m_Command_Str = "Q8\r";
        ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));
        Sleep (20);
        ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);

        //
        // Null terminate for printing
        Buffer[BytesRead] = 0;
        m_Response_Str = Buffer;

        if (BytesRead != 6) {
            MessageBox ("Analog response not received from Module!", "Error", MB_ICONERROR);
            return;
        }

        ::sscanf(&Buffer[2], "%X", &Sample_Count);

        if (Sample_Count >= 2048) {
            Offset += -(Sample_Count-4097);
        } else {
            Offset += -Sample_Count;
        }
    }

    //
    // Average the offset value
    Offset = Offset / 10;

    //
    // Save the offset calibration to EEPROM location 0x0F
    m_Command_Str.Format("W0F%02X\r", Offset);
    ::Write_Comport (hCom, m_Command_Str.GetLength(), m_Command_Str.GetBuffer(1));
    Sleep (20);
    ::Read_Comport (hCom, &BytesRead, BufferSize, Buffer);
    //
    // Null terminate for printing
    Buffer[BytesRead] = 0;
    m_Response_Str = Buffer;

    if (BytesRead != 2) {
        MessageBox ("EEPROM Write response not received from Module!", "Error", MB_ICONERROR);
        return;
    } else {
        m_Command_Str.Format ("Calibration value = %02X stored in EEPROM location 0x0F", Offset);
        AfxMessageBox(m_Command_Str, MB_OK);
        return;
    }
}
