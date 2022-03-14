// EZTermView.cpp : implementation of the CEZTermView class
//

#include "stdafx.h"
#include "EZTerm.h"

#include "EZTermDoc.h"
#include "EZTermView.h"
#include "MainFrm.h"

#include "defines.h"

#include "RtsControlDlg.h"
#include "comdlg.h"
#include "terminaldlg.h"
#include "Comm32.h"
#include "dlgkeylog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEZTermView

IMPLEMENT_DYNCREATE(CEZTermView, CScrollView)

BEGIN_MESSAGE_MAP(CEZTermView, CScrollView)
	//{{AFX_MSG_MAP(CEZTermView)
	ON_COMMAND(ID_SETTINGS_COMMPORT, OnSettingsCommport)
	ON_COMMAND(ID_SETTINGS_RTSDTR, OnSettingsRtsdtr)
	ON_COMMAND(ID_SETTINGS_TERMINALEMULATION, OnSettingsTerminalemulation)
	ON_WM_CHAR()
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_SETTINGS_COLORS_RECIEVE, OnSettingsColorsRecieve)
	ON_COMMAND(ID_SETTINGS_COLORS_TRANSMIT, OnSettingsColorsTransmit)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_SETTINGS_KEYBOARDLOG, OnSettingsKeyboardlog)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_KEYBOARDLOG, OnUpdateSettingsKeyboardlog)
	ON_COMMAND(ID_SETTINGS_HEXMODE, OnSettingsHexmode)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_HEXMODE, OnUpdateSettingsHexmode)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEZTermView construction/destruction

CEZTermView::CEZTermView()
{
	m_dwTimerCount = 0;
	m_pDlgKeyLog = new CDlgKeyLog(this);
	PrintableCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`~!@#$%^&*()_+=-[]{}|;:',<.>/? \"\?\'\\";
}

CEZTermView::~CEZTermView()
{
	delete m_pDlgKeyLog;	
}

BOOL CEZTermView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CEZTermView drawing

void CEZTermView::OnDraw(CDC* pDC)
{
	CEZTermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	GetDocument()->SetTextSize(pDC);

	// Create a pointer to the current font object
	CFont *pOldFont;

	// Select our new font
	pOldFont = (CFont*)pDC->SelectObject(&pDoc->m_cFont);

	// Draw the document
	pDoc->drawDocument(pDC);

	// If neccessary, scroll down.
	if(m_bRedraw)
	{
		m_bRedraw = FALSE;
		OnVScroll(SB_BOTTOM, 0, NULL);
		OnHScroll(SB_LEFT, 0, NULL);
	}
	
	// Restore the previous font.
	pDC->SelectObject(pOldFont);
	
}

void CEZTermView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	UINT nID, nStyle;
	int cxWidth;
	
	GetDocument()->SetTextSize(GetDC());
	sizeTotal = GetDocument()->getSize();
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_cLastSize = sizeTotal;

	// Tell the document it's ok to open the comm port.
	GetDocument()->StartMonitor(GetDC());
	SetTimer(1, 1, NULL);

    CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
    pStatus = &pFrame->m_wndStatusBar;

	pStatus->GetPaneInfo(1, nID, nStyle, cxWidth);
	pStatus->SetPaneInfo(1, nID, nStyle, cxWidth + 25);
	
}


/////////////////////////////////////////////////////////////////////////////
// CEZTermView diagnostics

#ifdef _DEBUG
void CEZTermView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CEZTermView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CEZTermDoc* CEZTermView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEZTermDoc)));
	return (CEZTermDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEZTermView message handlers

// 
// Save and restore ALL settings for the 
// application in the documetn object CConnection.
//
void CEZTermView::OnSettingsCommport() 
{
	comdlg dlg;
	
	CEZTermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// Load up the Dialog procedure
	dlg.m_nBaudIndex = pDoc->m_cConnection.m_nBaudRate;
	dlg.m_nComportIndex = pDoc->m_cConnection.m_nCommport;
	dlg.m_nDataBitIndex = pDoc->m_cConnection.m_nDataBits;
	dlg.m_nParityIndex = pDoc->m_cConnection.m_nParity;
	dlg.m_nStopBitIndex = pDoc->m_cConnection.m_nStopBits;

	// Get user input
	if(dlg.DoModal() == IDOK)
	{
		// Save the new settings.
		pDoc->m_cConnection.m_nCommport = dlg.m_nComportIndex;
		pDoc->m_cConnection.m_nBaudRate = dlg.m_nBaudIndex;
		pDoc->m_cConnection.m_nDataBits = dlg.m_nDataBitIndex;
		pDoc->m_cConnection.m_nParity = dlg.m_nParityIndex;
		pDoc->m_cConnection.m_nStopBits = dlg.m_nStopBitIndex;

		pDoc->StartMonitor(GetDC());
	}
}

// 
// Save and restore ALL settings for the 
// application in the documetn object CConnection.
//
void CEZTermView::OnSettingsRtsdtr() 
{
	RtsControlDlg dlg;
	
	CEZTermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// Init. the dialog object
	dlg.m_DTR_ON = pDoc->m_cConnection.m_bDTROn;
	dlg.m_RTS_ON = pDoc->m_cConnection.m_bRTSOn;

	if(dlg.DoModal() == IDOK)
	{
		// Get the new settings from the dialog proc.
		pDoc->m_cConnection.m_bDTROn = dlg.m_DTR_ON;
		pDoc->m_cConnection.m_bRTSOn = dlg.m_RTS_ON;

		pDoc->m_cComm.Set_Comport_Signals(dlg.m_RTS_ON, dlg.m_DTR_ON);

		
	}
	
	
}

// 
// Save and restore ALL settings for the 
// application in the documetn object CConnection.
//
void CEZTermView::OnSettingsTerminalemulation() 
{
	terminaldlg dlg;
	
	CEZTermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	

	dlg.m_Append_LF = pDoc->m_cConnection.m_bAppendLF;
	dlg.m_bHex_Mode = pDoc->m_cConnection.m_bHexMode;
	dlg.m_Local_Echo = pDoc->m_cConnection.m_bLocalEcho;

	
	if(dlg.DoModal() == IDOK)
	{
		pDoc->m_cConnection.m_bAppendLF = dlg.m_Append_LF;
		pDoc->m_cConnection.m_bHexMode = dlg.m_bHex_Mode;
		pDoc->m_cConnection.m_bLocalEcho = dlg.m_Local_Echo;
	}
}

//
// Keyboard message handler
//
void CEZTermView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int index;
	CEZTermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// If the keyboard log is active, buffer up the keystrokes until
	// the user presses the carriage return.

	// Let the document decide what to do with the
	// keypress. Don't forget to tell the documetn
	// where the data is comming from.
	if(m_pDlgKeyLog->GetSafeHwnd() != 0)
	{
		switch(nChar)
		{
		case CARRIAGERETURN:
			{
				// Output the buffered string
				for(index=0; index<m_pDlgKeyLog->m_strKeyLog.GetLength(); index++)
				{
					pDoc->insertChar(m_pDlgKeyLog->m_strKeyLog.GetAt(index), DATASOURCE_KEYBOARD);
				}
				// Output the CR
				pDoc->insertChar(nChar, DATASOURCE_KEYBOARD);	

				// Clear Log buffer.
				m_pDlgKeyLog->m_strKeyLog = "";
			}
			break;
		case BACKSPACE:
			{
				m_pDlgKeyLog->m_strKeyLog = m_pDlgKeyLog->m_strKeyLog.Left(m_pDlgKeyLog->m_strKeyLog.GetLength() - 1);
			}
			break;
		default:
			{
				m_pDlgKeyLog->m_strKeyLog += nChar;
			}
			break;
		}
		m_pDlgKeyLog->UpdateData(FALSE);
	}
	else
	{
		pDoc->insertChar(nChar, DATASOURCE_KEYBOARD);
	}

	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}


// 
// When the document changes, it will cause the view to update.
// If the entire view needs to be updated, it will pass a BOOL value
// int the lHint parameter indicating the type of invalidation to be
// used.
//
void CEZTermView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CSize sizeTotal;
	CRect cRect;

	CEZTermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// Get the type for the call to Invalidate
	m_bRedraw = (BOOL)lHint;

	// Force a redraw
	Invalidate(FALSE);

	// Resize the view only if neccessary.
	sizeTotal = GetDocument()->getSize();
	if(sizeTotal != m_cLastSize)
	{
		m_cLastSize = sizeTotal;
		SetScrollSizes(MM_TEXT, sizeTotal);
		// Somtimes a partial scroll will cause
		// the GDI to clear the screen. Have
		// the documetn redraw all to be sure.

	}
	GetDocument()->m_bRedraw = TRUE;
	m_bRedraw = TRUE;
}

void CEZTermView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);

	// When the document scolls, have the document redraw all the lines, 
	// just to be safe, sometimes the scroll view doesn't work quite right in 
	// hex mode.
	
	GetDocument()->m_bRedraw = TRUE;
}


void CEZTermView::OnTimer(UINT nIDEvent) 
{

    DWORD CTS_On, DSR_On, RI_On, CD_On;
    CString sEntireStatus;

	CEZTermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	
	if(nIDEvent == 1 && m_dwTimerCount > 20)
	{
		m_dwTimerCount = 0;
		pDoc->m_cComm.Get_Comport_Signals(&CTS_On, &DSR_On, &CD_On, &RI_On);

		
		sEntireStatus.Format("%s %ld", pDoc->m_cComm.Com_Name.Right(pDoc->m_cComm.Com_Name.GetLength()-4), pDoc->m_cComm.DCB_Baud_Rate);
        
		if (pDoc->m_cConnection.m_bRTSOn)
            sEntireStatus += "  RTS:1";
        else 
            sEntireStatus += "  RTS:0";

        if (pDoc->m_cConnection.m_bDTROn)
            sEntireStatus += "  DTR:1";
        else 
            sEntireStatus += "  DTR:0";

        if (CTS_On)
            sEntireStatus += "  CTS:1";
        else 
            sEntireStatus += "  CTS:0";

        if (DSR_On)
            sEntireStatus += "  DSR:1";
        else 
            sEntireStatus += "  DSR:0";

        if (CD_On)
            sEntireStatus += "  CD:1";
        else 
            sEntireStatus += "  CD:0";

        if (RI_On)
            sEntireStatus += "  RI:1";
        else 
            sEntireStatus += "  RI:0";

        //
        // Display the new status bar
        //
        pStatus->SetPaneText (1, sEntireStatus, TRUE);
		//GetDocument()->SetTextSize(GetDC());
		
	}
	
	m_dwTimerCount++;

	GetDocument()->insertRxChars();	
	CScrollView::OnTimer(nIDEvent);
}

void CEZTermView::OnDestroy() 
{
	CScrollView::OnDestroy();
	
	KillTimer(1);	
}

void CEZTermView::OnSettingsColorsRecieve() 
{
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT | CC_SOLIDCOLOR;
	dlg.m_cc.rgbResult = GetDocument()->m_cConnection.m_crRx;
	
	if(IDOK == dlg.DoModal())
	{
		GetDocument()->m_cConnection.m_crRx = dlg.GetColor();
	}
}

void CEZTermView::OnSettingsColorsTransmit() 
{
	

	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT | CC_SOLIDCOLOR;
	dlg.m_cc.rgbResult = GetDocument()->m_cConnection.m_crTx;
	
	
	if(IDOK == dlg.DoModal())
	{
		GetDocument()->m_cConnection.m_crTx = dlg.GetColor();
	}
	
}

void CEZTermView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   switch (nChar) {
    case VK_HOME:
        OnVScroll (SB_TOP, 0, NULL);
        OnHScroll (SB_LEFT, 0, NULL);
        break;
    case  VK_END:
        OnVScroll (SB_BOTTOM, 0, NULL);
        OnHScroll (SB_RIGHT, 0 , NULL);
        break;
    case VK_PRIOR:
        OnVScroll (SB_PAGEUP, 0 , NULL);
        break;
    case VK_NEXT:
        OnVScroll (SB_PAGEDOWN, 0 , NULL);
        break;
    case VK_UP:
        OnVScroll (SB_LINEUP, 0, NULL);
        break;
    case VK_DOWN:
        OnVScroll (SB_LINEDOWN, 0, NULL);
        break;
    case VK_LEFT:
        OnHScroll (SB_LINELEFT, 0, NULL);
        break;
    case VK_RIGHT:
        OnHScroll (SB_LINERIGHT, 0, NULL);
        break;
    default:
        break;
    }	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}



void CEZTermView::OnSettingsKeyboardlog() 
{
	if(m_pDlgKeyLog->GetSafeHwnd() == 0)
	{
		// Create and show the dialog
		m_pDlgKeyLog->Create();
	}
	else
	{
		// Hide the dialog
		m_pDlgKeyLog->DestroyWindow();
	}
}

void CEZTermView::OnUpdateSettingsKeyboardlog(CCmdUI* pCmdUI) 
{
	if(m_pDlgKeyLog->GetSafeHwnd() == 0)
	{
		// Uncheck the keyboard dialog menu
		pCmdUI->SetCheck(0);
	}
	else
	{
		// Check the keyboard dialog menu
		pCmdUI->SetCheck();
		
	}
	
}

//
// Give the user an easy interface to the Hex mode display
//
void CEZTermView::OnSettingsHexmode() 
{
	CEZTermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
		
	if(pDoc->m_cConnection.m_bHexMode)
	{
		pDoc->m_cConnection.m_bHexMode = FALSE;
	}
	else
	{
		pDoc->m_cConnection.m_bHexMode = TRUE;
	}

}

void CEZTermView::OnUpdateSettingsHexmode(CCmdUI* pCmdUI) 
{
	CEZTermDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
		
	if(pDoc->m_cConnection.m_bHexMode)
	{
		pCmdUI->SetCheck();
	}
	else
	{
		pCmdUI->SetCheck(0);
	}

}

