// PlateDownBtn.cpp : implementation file
//

#include "stdafx.h"
#include "BlastTest.h"
#include "PlateDownBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlateDownBtn

CPlateDownBtn::CPlateDownBtn()
{
}

CPlateDownBtn::~CPlateDownBtn()
{
}


BEGIN_MESSAGE_MAP(CPlateDownBtn, CButton)
	//{{AFX_MSG_MAP(CPlateDownBtn)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlateDownBtn message handlers

BOOL CPlateDownBtn::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message)  
    {  
    case WM_LBUTTONDOWN:  
        ::SendMessage(this->GetParent()->m_hWnd, 9999, 1, 0); 
        break;  
    case WM_LBUTTONUP:  
        ::SendMessage(this->GetParent()->m_hWnd, 9999, 0, 0);  
        break;  
    default:  
        break;  
    }  
	return CButton::PreTranslateMessage(pMsg);
}
