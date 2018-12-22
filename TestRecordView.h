//{{AFX_INCLUDES()
#include "adodc.h"
#include "datagrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TESTRECORDVIEW_H__703F39A2_C705_4B0E_A7A2_1E7CEF37992C__INCLUDED_)
#define AFX_TESTRECORDVIEW_H__703F39A2_C705_4B0E_A7A2_1E7CEF37992C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestRecordView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestRecordView dialog

class CTestRecordView : public CDialog
{
// Construction
public:
	CTestRecordView(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestRecordView)
	enum { IDD = IDD_TESTREPORT };
	CFont m_editFont; 
	CAdodc	m_AdoRecord;
	CDataGrid	m_DGridRecord;
	CString   m_SpNo;//试样编号
	CString	m_Type;
	float	m_HunKg;
	CString	m_Person;
	float	m_HunHight;
	CString	m_CombSpNo;//关联的组合框样品编号变量
	float	m_TestHumi;
	float	m_TestTemp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestRecordView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	void DataRefresh(CString Str);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestRecordView)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeCombspno();
	afx_msg void OnFind();
	afx_msg void OnDelsel();
	afx_msg void OnPrintreport();
	afx_msg void OnReportbillbtn();
	afx_msg void OnChangeblast();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTRECORDVIEW_H__703F39A2_C705_4B0E_A7A2_1E7CEF37992C__INCLUDED_)
