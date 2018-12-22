#if !defined(AFX_PRINTREPORTDLG_H__2E318D81_C8F7_4AF0_A295_0D9DD0B02633__INCLUDED_)
#define AFX_PRINTREPORTDLG_H__2E318D81_C8F7_4AF0_A295_0D9DD0B02633__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintReportDlg.h : header file
//
//wf
#include "Utility\GridppReportEventImpl.h"
//wf end
/////////////////////////////////////////////////////////////////////////////
// CPrintReportDlg dialog


class CPrintReportDlg : public CDialog
{
// Construction
public:
	CString m_SpNo;
	CString m_Person;
	CString m_Spkg;
	CString m_Type;
	float   m_HunKg;
	float   m_HunHight;
	float   m_TestHumi;
	float   m_TestTemp;//ÎÂ¶ÈÓëÊª¶È

	CPrintReportDlg(CWnd* pParent = NULL);   // standard constructor

	//CGridppReportEventImpl* m_pReportEvents;
// Dialog Data
	//{{AFX_DATA(CPrintReportDlg)
	enum { IDD = IDD_PRINTREPORT1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrintReportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPrint();
	afx_msg void OnPrintview();
	afx_msg void OnExportfile();
	//}}AFX_MSG
	IGridppReportPtr m_pGridppReport; 
	IGRDisplayViewerPtr m_pDisplayViewer;

	afx_msg void OnContentCellDblClickDisplayViewer(LPDISPATCH pSender);
	DECLARE_EVENTSINK_MAP()

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTREPORTDLG_H__2E318D81_C8F7_4AF0_A295_0D9DD0B02633__INCLUDED_)
