// LineGraphTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LineGraphTest.h"
#include "LineGraphTestDlg.h"
#include ".\linegraphtestdlg.h"
#include <stdlib.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CLineGraphTestDlg dialog



CLineGraphTestDlg::CLineGraphTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineGraphTestDlg::IDD, pParent)
	, m_strMinValue(_T("0"))
	, m_strMaxValue(_T("5"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLineGraphTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INTEGRITYLINEGRACTRL1, m_cGraph);
	DDX_Control(pDX, IDC_INTEGRITYBARGRAPCTRL1, m_cBargraph);
	DDX_Control(pDX, IDC_INTEGRITYLINEGRACTRL2, m_cLineGraph2);
	DDX_Text(pDX, IDC_TXTMINVALUE, m_strMinValue);
	DDX_Text(pDX, IDC_TXTMAXVALUE, m_strMaxValue);
}

BEGIN_MESSAGE_MAP(CLineGraphTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_TXTMINVALUE, OnEnChangeTxtminvalue)
	ON_EN_CHANGE(IDC_TXTMAXVALUE, OnEnChangeTxtmaxvalue)
END_MESSAGE_MAP()


// CLineGraphTestDlg message handlers

BOOL CLineGraphTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_cGraph.SetlnNumElements(8);
	m_cGraph.put_ulElementColor(0, 0x7f7f7f);
	m_cGraph.put_ulElementColor(1, 0x7f7fff);
	m_cGraph.put_ulElementColor(2, 0x7fff7f);
	m_cGraph.put_ulElementColor(3, 0x7fffff);
	m_cGraph.put_ulElementColor(4, 0xff7f7f);
	m_cGraph.put_ulElementColor(5, 0xff7fff);
	m_cGraph.put_ulElementColor(6, 0xffff7f);
	m_cGraph.put_ulElementColor(7, 0x000000);
	



	m_cLineGraph2.SetlnNumElements(8);
	m_cLineGraph2.put_ulElementColor(0, 0x7f7f7f);
	m_cLineGraph2.put_ulElementColor(1, 0x7f7fff);
	m_cLineGraph2.put_ulElementColor(2, 0x7fff7f);
	m_cLineGraph2.put_ulElementColor(3, 0x7fffff);
	m_cLineGraph2.put_ulElementColor(4, 0xff7f7f);
	m_cLineGraph2.put_ulElementColor(5, 0xff7fff);
	m_cLineGraph2.put_ulElementColor(6, 0xffff7f);
	m_cLineGraph2.put_ulElementColor(7, 0x000000);
	

	
	m_cBargraph.SetlnNumElements(8);
	m_cBargraph.SetdblMaxValue(5);
	m_cBargraph.SetdblMinValue(0);



	SetTimer(1, 55, 0);
	srand(GetTickCount());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLineGraphTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLineGraphTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLineGraphTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLineGraphTestDlg::OnTimer(UINT nIDEvent)
{
	double i;
	double index;
	double r;

	
	for(index = 0; index < (double)m_cGraph.GetlnNumElements(); index++)
	{
		i = rand();
		r = i / 32768;
		r *= (m_cGraph.GetdblMaxValue() / 10);
		r += index * 0.6;
		m_cGraph.put_dElement(index, r);

		m_cLineGraph2.put_dElement(7-index, r);
		m_cBargraph.put_dElement(index, r);
		
	}

	CDialog::OnTimer(nIDEvent);
}

void CLineGraphTestDlg::OnClose()
{
	KillTimer(1);

	CDialog::OnClose();
}

void CLineGraphTestDlg::OnEnChangeTxtminvalue()
{
	UpdateData();
	m_cLineGraph2.SetdblMinValue(atof((LPCTSTR)m_strMinValue));
}

void CLineGraphTestDlg::OnEnChangeTxtmaxvalue()
{
	UpdateData();
	m_cLineGraph2.SetdblMaxValue(atof((LPCTSTR)m_strMaxValue));
}
