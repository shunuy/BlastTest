//{{AFX_INCLUDES()
#include "datagrid.h"
#include "adodc.h"
#include "afxcmn.h"
#include "MyDisCtrl.h"
#include "afxwin.h"
#include "ADO\ADO.h"

//}}AFX_INCLUDES
#if !defined(AFX_TESTSPDATA_H__EE66B085_39D0_42D5_8D74_CB8F72DEA80E__INCLUDED_)
#define AFX_TESTSPDATA_H__EE66B085_39D0_42D5_8D74_CB8F72DEA80E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestSpData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestSpData dialog

class CTestSpData : public CDialog
{
// Construction
public:
	CTestSpData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestSpData)
	enum { IDD = IDD_TESTSPDATA };
	//CDataGrid	m_SpDataDGrid;
	//CAdodc	m_AdoSpData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestSpData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestSpData)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	CMyDisCtrl ListDis;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void ShowEdit(BOOL bShow, int nItem, int nIndex, CRect rcCtrl);
	CEdit m_edit;
	CADODataSet m_Set;
	CStringArray ar;//¶ÎÃû
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnBnClickedAddnew();
	afx_msg void OnBnClickedDelonerecord();
	void UpdataListDis();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTSPDATA_H__EE66B085_39D0_42D5_8D74_CB8F72DEA80E__INCLUDED_)
