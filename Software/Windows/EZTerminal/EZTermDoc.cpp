//
// EZTermDoc.cpp : implementation of the CEZTermDoc class
//

#include "stdafx.h"
#include "EZTerm.h"

#include "EZTermDoc.h"
#include "EZTermView.h"

#include "defines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEZTermDoc

IMPLEMENT_DYNCREATE(CEZTermDoc, CDocument)

BEGIN_MESSAGE_MAP(CEZTermDoc, CDocument)
	//{{AFX_MSG_MAP(CEZTermDoc)
	ON_COMMAND(ID_LOGGING_SETFILENAME, OnLoggingSetfilename)
	ON_COMMAND(ID_LOGGING_STARTLOGGING, OnLoggingStartlogging)
	ON_UPDATE_COMMAND_UI(ID_LOGGING_STARTLOGGING, OnUpdateLoggingStartlogging)
	ON_COMMAND(ID_LOGGING_STOPLOGGING, OnLoggingStoplogging)
	ON_UPDATE_COMMAND_UI(ID_LOGGING_STOPLOGGING, OnUpdateLoggingStoplogging)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEZTermDoc construction/destruction

CEZTermDoc::CEZTermDoc()
{
	// Create a fixed witdh font.
	// Create the font 
	VERIFY(m_cFont.CreateFont(
		16,                        // nHeight
		10,                        // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Courier"));               // lpszFacename


	// Set the maximum number of lines
	m_nMaxLines = 100;
	m_bMonitorActive=FALSE;	
	m_nBufHead = m_nBufTail = 0;

	m_bLoggingToFile = FALSE;


}

CEZTermDoc::~CEZTermDoc()
{
	// Make sure the thread is not active.
	StopMonitor();

	CConsoleData* cData;
	while(!m_cDataObjects.IsEmpty())
	{
		cData = (CConsoleData*)m_cDataObjects.RemoveHead();
		delete cData;
	}

	if(m_bLoggingToFile)
	{
		m_cfLogFile.Close();
	}
}

BOOL CEZTermDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEZTermDoc serialization

void CEZTermDoc::Serialize(CArchive& ar)
{

	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Save the current settings.
	m_cConnection.Serialize(ar);

	
}

/////////////////////////////////////////////////////////////////////////////
// CEZTermDoc diagnostics

#ifdef _DEBUG
void CEZTermDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEZTermDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEZTermDoc commands


//
// Insert the next character into the document.
// If the source of the character changes, the size
// of the current line reaches 80 chars, or a newline
// is received, create a new line in the document.
//
void CEZTermDoc::insertChar(UINT nChar, UINT source)
{
	CConsoleData *cConsole;
	
	if(source == DATASOURCE_KEYBOARD)
	{
		m_cComm.Write_Comport(1, (char*)&nChar); 
	}
	if(source == DATASOURCE_SERIAL)
	{
		// This is a serial port character. So, if
		// the log file is turned on save the character.
		if(m_bLoggingToFile)
		{
			m_cfLogFile.Write(&nChar, sizeof(unsigned char));
		}
	}
	
	// If the display mode has changed,
	// and the current object has received
	// characters, then add a new object.
	if(m_bDisplayMode != m_cConnection.m_bHexMode)
	{
		cConsole = addNewObject(source);
	}

	// Don't insert keyboard characters if local echo is off.
	if(source == DATASOURCE_KEYBOARD && !m_cConnection.m_bLocalEcho)
	{
		return;
	}
	
	// If the document is empty, or the data source changes
	// add a new data object.
	if(m_cDataObjects.GetCount() == 0)
	{
		cConsole = addNewObject(source);
		m_nLastSource = source;
		m_bRedraw = TRUE;
	}

	if(source != m_nLastSource)
	{
		cConsole = (CConsoleData*)m_cDataObjects.GetTail();
		// Only add a new object if the current one has
		// processed any data.
		if(cConsole->m_dwCharCount > 0)
		{
			cConsole = addNewObject(source);
		}
		else
		{
			// Change ownership of the object to get the color
			// correct and doctype correct.
			cConsole->Create((source==DATASOURCE_KEYBOARD) ? m_cConnection.m_crTx : m_cConnection.m_crRx,
				m_cConnection.m_bHexMode?DISPLAY_ASCIIHEX:DISPLAY_ASCII);
		}
		m_bRedraw = TRUE;
		m_nLastSource = source;
	}
	else
	{
		// Add to the current object
		cConsole = (CConsoleData*)m_cDataObjects.GetTail();
		m_bRedraw = FALSE;
	}

	// If there are 80 characters in the buffer, start a new line
	if (!m_cConnection.m_bHexMode && cConsole->dataLen() >= 80)
	{
		cConsole = addNewObject(source);
		m_nLastSource = source;
		m_bRedraw = TRUE;
	}

	if (m_cConnection.m_bHexMode && cConsole->dataLen() >= 16)
	{
		cConsole = addNewObject(source);
		m_nLastSource = source;
		m_bRedraw = TRUE;
	}

	// handle the character accordingly.	
	switch (nChar)
	{
	case CARRIAGERETURN:
		cConsole->insertChar(nChar);
		if (m_cConnection.m_bAppendLF && m_cConnection.m_bHexMode == FALSE)
		{
			cConsole = addNewObject(source);
			m_bRedraw = TRUE;
		}
		break;
	case NEWLINE:
		cConsole = addNewObject(source);
		m_bRedraw = TRUE;
		break;
	default:
		cConsole->insertChar(nChar);
		break;
	}

	
	// If this char came from a keyboard then
	// update the views, otherwise let the serial
	// handler decide when to update so that
	// the system can handle a large volume of data.
	if(source == DATASOURCE_KEYBOARD)
	{
		UpdateAllViews(NULL, (LPARAM) m_bRedraw );
	}
}





//
// Create and insert a new data object into this
// document. If there are more objects than there
// is supposed to be then remove the oldest object.
// and delete it from the heap.
//
CConsoleData* CEZTermDoc::addNewObject(UINT source)
{
	CConsoleData *cConsole;
	POSITION pos;

	m_bDisplayMode = m_cConnection.m_bHexMode;

	// Create the new object
	cConsole = new CConsoleData();
	cConsole->Create((source==DATASOURCE_KEYBOARD) ? m_cConnection.m_crTx : m_cConnection.m_crRx,
		m_cConnection.m_bHexMode?DISPLAY_ASCIIHEX:DISPLAY_ASCII);

	// Insert it into the document.
	m_cDataObjects.AddTail(cConsole);

	// Check for objects to dump
	if (m_cDataObjects.GetCount() > (int)m_nMaxLines)
	{
		pos = m_cDataObjects.GetHeadPosition();
		CConsoleData *cTemp = (CConsoleData*)m_cDataObjects.RemoveHead();
		delete cTemp;
	}
	m_bRedraw = TRUE;
	return cConsole;
}

//
// update the view with information, 
// Only redraw the entire view if it is neccessary.
//
void CEZTermDoc::drawDocument(CDC *pDC)
{
	TEXTMETRIC textMetric;
	POSITION pos;
	int currentLine;
	
	CRect fillRect;

	//
	// If there is nothing to draw return.
	// It's embarrasing to throw exceptions.
	//
	if(m_cDataObjects.IsEmpty()) return;
	
	// Get a pointer to the current data object
	CConsoleData* pData = (CConsoleData*)m_cDataObjects.GetTail();

	// Fill the textMetrics structure
	//
	pDC->GetTextMetrics(&textMetric);

	// Create a whit brush for clearing screen regions.
	CBrush bg(RGB(255,255,255));
	CBrush fg(RGB(0,0,0));


	if(m_bRedraw)
	{
	// If we need to redraw the entire document,
	// start at line 0
		currentLine = 0;
	}
	else
	{
	// ... Otherwise just draw the current line
		currentLine = m_cDataObjects.GetCount() - 1;
	}

	// Always start at the left
	fillRect.left = 0;

	//
	// Find the position of the first line of the
	// document to redraw, Note: this could be the
	// tail.
	//
	pos = m_cDataObjects.FindIndex(currentLine);
	while(pos != NULL)
	{
		// Iterate throught the list untill all invalid objects
		// are redrawn. POSITION gets updated.
		pData = (CConsoleData*)m_cDataObjects.GetNext(pos);
		
		// Set the rectangle to only draw over the 
		// current document object
		fillRect.top = currentLine * textMetric.tmHeight;
		fillRect.right = (80 * textMetric.tmMaxCharWidth) + 10;
		fillRect.bottom = fillRect.top + textMetric.tmHeight;

		// Clear the rectangle.
		pDC->FillRect(&fillRect, &bg);
		
		// Draw the new text over the top.
		// The object knows which color it's supposed to 
		// be.
		pData->drawData(pDC, fillRect.left, fillRect.top);

		// do the next line.
		currentLine++;
	}

//	caretPos.x = pData->m_dwStrLen * textMetric.tmAveCharWidth + 2;
//	caretPos.y = fillRect.top;

	if(pData->m_dwStrLen < 80)	
	{
		fillRect.top += 1;
		fillRect.bottom -= 1;
		fillRect.left = pData->m_dwStrLen * textMetric.tmAveCharWidth + 1;
		fillRect.right = (pData->m_dwStrLen * textMetric.tmAveCharWidth) + 3;
		pDC->FillRect(&fillRect, &fg);
	}

}

//
// Return the view size based on the 
// number of items in the document
//
CSize CEZTermDoc::getSize()
{
	return m_cSizeTotal;
}

/////////////////////////////////////////////////////////////////////////////
// Communcations procedures.
//


// Communciaton monitor thread
// This thread will run until
//
void CEZTermDoc::StartMonitor(CDC* pDC)
{
    DWORD Comport, BaudRate, DataBits, StopBits, Parity;

	TRACE("Closing Communications\n");
	m_cComm.Close_Comport();

	if(m_bMonitorActive)
	{
		TRACE("Stopping Monitor Thread\n");
		StopMonitor();
	}
	

	Comport = m_cConnection.m_nCommport + 1;

    switch (m_cConnection.m_nBaudRate) {
    case 0:
    default:
        BaudRate = CBR_300;
        break;
    case 1:
        BaudRate = CBR_600;
        break;
    case 2:
        BaudRate = CBR_1200;
        break;
    case 3:
        BaudRate = CBR_2400;
        break;
    case 4:
        BaudRate = CBR_4800;
        break;
    case 5:
        BaudRate = CBR_9600;
        break;
    case 6:
        BaudRate = CBR_19200;
        break;
    case 7:
        BaudRate = CBR_38400;
        break;
    case 8:
        BaudRate = CBR_57600;
        break;
    case 9:
        BaudRate = CBR_115200;
        break;
    }

    switch (m_cConnection.m_nDataBits) {
    case 0:
    default:
        DataBits = 8;
        break;
    case 1:
        DataBits = 7;
        break;
    case 2:
        DataBits = 6;
        break;
    case 3:
        DataBits = 5;
        break;
    }

    switch (m_cConnection.m_nParity) {
    case 0:
    default:
        Parity = NOPARITY;
        break;
    case 1:
        Parity = EVENPARITY;
        break;
    case 2:
        Parity = ODDPARITY;
        break;
    case 3:
        Parity = SPACEPARITY;
        break;
    case 4:
        Parity = MARKPARITY;
        break;
    }

    switch (m_cConnection.m_nStopBits) {
    case 0:
    default:
        StopBits = ONESTOPBIT;
        break;
    case 1:
        StopBits = ONE5STOPBITS;
        break;
    case 2:
        StopBits = TWOSTOPBITS;
        break;
    }

	if(m_cComm.Open_Comport(Comport,BaudRate, DataBits,StopBits, Parity))
	{
		TRACE("Communications Open\n");
	}
	else
	{
		AfxMessageBox("Communication Port failed to  open",MB_OK | MB_ICONSTOP);
	}


}
void CEZTermDoc::StopMonitor()
{
	UINT breakTimer;
	
	
	if(m_bMonitorActive)
	{
		KillTimer(NULL, m_upTimer);

		// signal a thread exit
		m_ceStopMonitor.SetEvent();
		m_cComm.Close_Comport();


		breakTimer = 0;
		while(m_bMonitorActive)
		{
			Sleep(1);
			breakTimer++;

			// Timeout in 3 seconds, and let the memory leak.
			if(breakTimer > 3000) 
			{
				TRACE("Timeout waiting for monitor thread to exit\n");
				break;
			}
		}
	}
}

UINT CEZTermDoc::commMonitor(LPVOID pParam)
{
	CEZTermDoc *pThis = (CEZTermDoc*)pParam;
	DWORD bytesRead;
	char buffer[16];
	BOOL chRx;
	// Get a view pointer to post a message to.
	POSITION pos = pThis->GetFirstViewPosition();
	CView* pFirstView = pThis->GetNextView( pos );

	pThis->m_bMonitorActive = TRUE;
	TRACE("Entering Comm Monitor Thread\n");
	TRACE("Thread ID = 0x%X\n", AfxGetThread()->m_nThreadID);

	chRx = FALSE;
	while(WAIT_OBJECT_0 != WaitForSingleObject(pThis->m_ceStopMonitor, 0))
	{
		while(pThis->m_cComm.Read_Comport(&bytesRead, 1, buffer))
		{
			//pThis->bufferChar(buffer[0]);
		}
	}


	TRACE("Exiting Comm Monitor Thread\n");
	pThis->m_bMonitorActive = FALSE;
	return 1;	
}

void CEZTermDoc::insertRxChars()
{
	char buffer[4096];
	DWORD dwBytesRead, index;
	
	if(m_cComm.Read_Comport(&dwBytesRead, 4096, buffer))
	{
		for(index=0; index<dwBytesRead; index++)
		{
			insertChar((UINT)buffer[index], DATASOURCE_SERIAL);
		}

		// Only update the view when the receive buffer is empty.
		UpdateAllViews(NULL, (LPARAM) m_bRedraw );
		TRACE("Rx Chars: %ld\n", dwBytesRead);
	}
}


void CEZTermDoc::OnLoggingSetfilename() 
{
	char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|Data Files (*.dat)|*.dat|All Files (*.*)|*.*||";
	CFileDialog dlg( FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );
	
	// Get a filename from the user
	if(IDOK == dlg.DoModal())
	{
		// Get the log file name.
		if(dlg.GetFileName().GetLength() > 0)
		{
			m_strLogFileName = dlg.GetPathName();
		}
		else
		{
			AfxMessageBox("Invalid File Name", MB_OK | MB_ICONQUESTION);
		}
	}
}

void CEZTermDoc::OnLoggingStartlogging() 
{
	
	char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|Data Files (*.dat)|*.dat|All Files (*.*)|*.*||";
	
	// Give the user a chance to change the filename.
	CFileDialog dlg( FALSE, "*.dat", m_strLogFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL );
	
	if(dlg.DoModal() == IDOK)
	{
		

		// Open the file for write, if the file exists, seek to end
		// for file append.
		if(m_cfLogFile.Open(dlg.GetPathName(), CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate, NULL))
		{
			m_cfLogFile.SeekToEnd();
			m_bLoggingToFile = TRUE;

		}
		else
		{
			AfxMessageBox("Error opening file.", MB_OK | MB_ICONQUESTION);
		}
	}

	

}

void CEZTermDoc::OnUpdateLoggingStartlogging(CCmdUI* pCmdUI) 
{
	if(m_bLoggingToFile == TRUE || m_strLogFileName.GetLength() == 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CEZTermDoc::OnLoggingStoplogging() 
{
	

	// Close the open file
	if(m_bLoggingToFile)
	{
		m_cfLogFile.Close();
		m_bLoggingToFile = FALSE;		
	}
}

void CEZTermDoc::OnUpdateLoggingStoplogging(CCmdUI* pCmdUI) 
{
	if(m_bLoggingToFile)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CEZTermDoc::SetTextSize(CDC *pDC)
{
	TEXTMETRIC textMetric;
	CFont *pOldFont = pDC->SelectObject(&m_cFont);

	pDC->GetTextMetrics(&textMetric);

	m_cSizeTotal.cx = 80 * textMetric.tmAveCharWidth;
	m_cSizeTotal.cy = (m_cDataObjects.GetCount() * textMetric.tmHeight) < 100 ? 
		100 : ( (m_cDataObjects.GetCount()+1) * textMetric.tmHeight);

	pDC->SelectObject(pOldFont);	

}
