#if !defined(AFX_REPORTBILLPRINT_H__7699D4E0_D988_40A6_9719_19E341CC93B5__INCLUDED_)
#define AFX_REPORTBILLPRINT_H__7699D4E0_D988_40A6_9719_19E341CC93B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportBillPrint.h : header file
//
#include "Utility\GridppReportEventImpl.h"
/////////////////////////////////////////////////////////////////////////////
// CReportBillPrint dialog

class CReportBillPrint : public CDialog
{
// Construction
public:
	CReportBillPrint(CWnd* pParent = NULL);   // standard constructor
	CString m_SpNo;
	CString m_TestType;
// Dialog Data
	//{{AFX_DATA(CReportBillPrint)
	enum { IDD = IDD_REPORTBILL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportBillPrint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	IGridppReportPtr m_pGridppReport; 
	IGRDisplayViewerPtr m_pDisplayViewer;
	// Generated message map functions
	//{{AFX_MSG(CReportBillPrint)
	virtual BOOL OnInitDialog();
	afx_msg void OnPrint();
	afx_msg void OnPrintview();
	afx_msg void OnExportfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTBILLPRINT_H__7699D4E0_D988_40A6_9719_19E341CC93B5__INCLUDED_)
