// ConsoleData.h: interface for the CConsoleData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSOLEDATA_H__F7B157C9_0441_4253_A337_7762288ED97D__INCLUDED_)
#define AFX_CONSOLEDATA_H__F7B157C9_0441_4253_A337_7762288ED97D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "defines.h"

class CConsoleData : public CObject  
{
public:
	int dataLen();
	void Serialize(CArchive &ar);
	BOOL Create(COLORREF color, int nDisplayType = DISPLAY_ASCII);
	void insertChar(UINT character);
	CConsoleData();
	virtual ~CConsoleData();

protected:
	int m_nDisplayType;
	
	COLORREF textColor;
	int nBufPtr;
	unsigned char buffer[81];
	CString PrintableCharacters;

public:
	DWORD m_dwStrLen;
	DWORD m_dwCharCount;
	void drawData(CDC* pDC, int x, int y);
	CString strConsoleData();
	DECLARE_SERIAL(CConsoleData)
};

#endif // !defined(AFX_CONSOLEDATA_H__F7B157C9_0441_4253_A337_7762288ED97D__INCLUDED_)
