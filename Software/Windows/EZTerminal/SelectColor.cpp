// SelectColor.cpp : implementation file
//

#include "stdafx.h"
#include "EZTerm.h"
#include "SelectColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectColor

IMPLEMENT_DYNAMIC(CSelectColor, CColorDialog)

CSelectColor::CSelectColor(COLORREF clrInit, DWORD dwFlags, CWnd* pParentWnd) :
	CColorDialog(clrInit, dwFlags, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(CSelectColor, CColorDialog)
	//{{AFX_MSG_MAP(CSelectColor)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

