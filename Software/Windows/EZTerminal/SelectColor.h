#if !defined(AFX_SELECTCOLOR_H__55A4C1E7_4FA4_49FF_8F49_1A7EEC7F4FCF__INCLUDED_)
#define AFX_SELECTCOLOR_H__55A4C1E7_4FA4_49FF_8F49_1A7EEC7F4FCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectColor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectColor dialog

class CSelectColor : public CColorDialog
{
	DECLARE_DYNAMIC(CSelectColor)

public:
	CSelectColor(COLORREF clrInit = 0, DWORD dwFlags = 0,
			CWnd* pParentWnd = NULL);

protected:
	//{{AFX_MSG(CSelectColor)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTCOLOR_H__55A4C1E7_4FA4_49FF_8F49_1A7EEC7F4FCF__INCLUDED_)
