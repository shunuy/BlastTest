#if !defined(AFX_DEBUGDLG_H__1B661E15_500C_44AE_A53B_B1C35233B16B__INCLUDED_)
#define AFX_DEBUGDLG_H__1B661E15_500C_44AE_A53B_B1C35233B16B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DebugDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDebugDlg dialog

class CDebugDlg : public CDialog
{
// Construction
public:
	CDebugDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDebugDlg)
	enum { IDD = IDD_DEBUGDLG };
	int		m_IrADSwitch;
	int	m_HunColliValue;
	int	m_UpLimite;
	int	m_HunReleaseValue;
	CString	m_ByteAdd;
	CString	m_ByteValue;
	CString	m_MenAdd;
	CString	m_MenValue;
	float	m_SoundLevelPeak;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDebugDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDebugDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUGDLG_H__1B661E15_500C_44AE_A53B_B1C35233B16B__INCLUDED_)
