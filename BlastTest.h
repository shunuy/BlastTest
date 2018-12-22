// BlastTest.h : main header file for the BLASTTEST application
//

#if !defined(AFX_BLASTTEST_H__F7678F3F_A3BF_49D3_A88B_93244BEA5A65__INCLUDED_)
#define AFX_BLASTTEST_H__F7678F3F_A3BF_49D3_A88B_93244BEA5A65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBlastTestApp:
// See BlastTest.cpp for the implementation of this class
//

class CBlastTestApp : public CWinApp
{
public:
	CBlastTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlastTestApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
	int m_DataBaseType;//数据库类型,0--access,1--sql,2--mysql
// Implementation

	//{{AFX_MSG(CBlastTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CBlastTestApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLASTTEST_H__F7678F3F_A3BF_49D3_A88B_93244BEA5A65__INCLUDED_)
