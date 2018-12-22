#if !defined(AFX_TESTCONDITON_H__4846D7B1_7CE7_459E_9EAC_8A2CCC4E3647__INCLUDED_)
#define AFX_TESTCONDITON_H__4846D7B1_7CE7_459E_9EAC_8A2CCC4E3647__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestConditon.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestConditon dialog

class CTestConditon : public CDialog
{
// Construction
public:
	CTestConditon(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestConditon)
	enum { IDD = IDD_TESTCONDSETUP };
	short	m_GroupNo;
	float	m_HunHight;
	float	m_HunKg;
	short	m_OneGroupNo;
	CString	m_Person;
	float	m_SpKg;
	CString	m_Type;
	float	m_TestTemp;//实验温度与湿度
	float	m_Humidity;
	float	m_StepLength;
	float   m_PressSet;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestConditon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestConditon)
	afx_msg void OnSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCONDITON_H__4846D7B1_7CE7_459E_9EAC_8A2CCC4E3647__INCLUDED_)
