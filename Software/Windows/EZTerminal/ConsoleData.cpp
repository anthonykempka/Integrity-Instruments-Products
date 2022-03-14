// ConsoleData.cpp: implementation of the CConsoleData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EZTerm.h"
#include "ConsoleData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// This object is serializable.
IMPLEMENT_SERIAL( CConsoleData, CObject, VERSIONABLE_SCHEMA | 2 )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConsoleData::CConsoleData()
{
	// Leagal characters
    PrintableCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`~!@#$%^&*()_+=-[]{}|;:',<.>/? \"\?\'\\";

	// make sure the buffer is empty
	nBufPtr = 0;

	//Fill the buffer with whitespace
	memset(buffer, 0x0020, sizeof(buffer));

	//Make the string 0 len
	buffer[0] = '\0';
	
	m_dwCharCount = 0;
}

CConsoleData::~CConsoleData()
{

}

// 
// Only insert a charater into the buffer if
// it is a printable character.
//
void CConsoleData::insertChar(UINT character)
{
	//
	// Handle the diffarent display types.
	//
	switch (m_nDisplayType)
	{
	default:
	case DISPLAY_ASCII:
		// Handle the backspace characters
		if (character == BACKSPACE  && nBufPtr > 0)
		{
			nBufPtr--;
			buffer[nBufPtr] = '\0';
		}
		if(PrintableCharacters.Find(character) != -1)
		{
			buffer[nBufPtr] = (unsigned char)character;
			buffer[nBufPtr+1] = '\0';
			if(nBufPtr < 80) nBufPtr++;
		}
		if(character == CARRIAGERETURN) 
		{
			nBufPtr = 0;
		}
		break;
	case DISPLAY_ASCIIHEX:
		buffer[nBufPtr] = (unsigned char)character;
		buffer[nBufPtr+1] = '\0';
		if(nBufPtr < 80) nBufPtr++;
		break;
	}
	m_dwCharCount++;
}

BOOL CConsoleData::Create(COLORREF color, int nDisplayType)
{
	textColor = color;
	m_nDisplayType = nDisplayType;

	return TRUE;
}

void CConsoleData::Serialize(CArchive &ar)
{
	TRACE("Entering CConnection::Serialize\n");


	if(ar.IsStoring())
	{
		// Store object to file.
	}
	else
	{
		// Retreive object from file.
	}
	
}

int CConsoleData::dataLen()
{
	return nBufPtr;
}

//
// Return a CString from the data buffer
CString CConsoleData::strConsoleData()
{
	CString str(buffer);

	return str;
}

void CConsoleData::drawData(CDC *pDC, int x, int y)
{
	COLORREF oldColor;
	CString str, strAscii, sTemp;
	int index;
	// make the new brush current and save the old brush
	oldColor = pDC->SetTextColor(textColor);
	
	switch (m_nDisplayType)
	{
	default:
	case DISPLAY_ASCII:
		str.Format("%s", buffer);
		pDC->TextOut(x, y, buffer);
		break;
	case DISPLAY_ASCIIHEX:
		str = "";
		strAscii = "";
		for (index=0; index < nBufPtr; index++)
		{
			sTemp.Format("%s%02X ", str, buffer[index]);
			if(index==7) sTemp += "- ";
			str = sTemp;

			sTemp.Format("%s%c", strAscii, buffer[index]);
			strAscii = sTemp;
		}
		pDC->TextOut(x, y, str);
		pDC->TextOut(ASCII_POSITION, y, strAscii);
		
		break;
	}
	m_dwStrLen = str.GetLength();
	pDC->SetTextColor(oldColor);
}


