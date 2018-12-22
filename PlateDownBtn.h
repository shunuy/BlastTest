#if !defined(AFX_PLATEDOWNBTN_H__F8120266_53E2_4690_B0AF_12702F6A6993__INCLUDED_)
#define AFX_PLATEDOWNBTN_H__F8120266_53E2_4690_B0AF_12702F6A6993__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlateDownBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlateDownBtn window

class CPlateDownBtn : public CButton
{
// Construction
public:
	CPlateDownBtn();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlateDownBtn)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPlateDownBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPlateDownBtn)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLATEDOWNBTN_H__F8120266_53E2_4690_B0AF_12702F6A6993__INCLUDED_)
