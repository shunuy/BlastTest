#if !defined(AFX_USERSETUPDLG_H__F15BC272_47C5_42AE_BD9A_3A3F89FA46F8__INCLUDED_)
#define AFX_USERSETUPDLG_H__F15BC272_47C5_42AE_BD9A_3A3F89FA46F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserSetupDlg dialog

class CUserSetupDlg : public CDialog
{
// Construction
public:
	CUserSetupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserSetupDlg)
	enum { IDD = IDD_USERSETUP };
	float	m_SLThreshold;
	float   m_HunCurrentH;
	float   m_HunRealH;
	float   m_HunKg;//重锤质量
	float	m_ZeroSetValue;//零点的设置值
	float	m_HunHight;    //当前编码器的实时值
	float	m_CorSetValue;
	float	m_SmogSheld;
	float	m_CRHHunHight;
	int		m_Step;
	float   m_PressOffset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserSetupDlg)
	afx_msg void OnStartkeyboard();
	afx_msg void OnUserSetupSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnDebugdlg();
	afx_msg void OnBackup();
	afx_msg void OnBackuprecorve();
	afx_msg void OnSysreset();
	afx_msg void OnAdjusthight();
	afx_msg void OnCleardatabase();
	afx_msg void OnZerosetbtn();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCorsetbtn();
	afx_msg void OnBtnfileexport();
	afx_msg void OnSavevalue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERSETUPDLG_H__F15BC272_47C5_42AE_BD9A_3A3F89FA46F8__INCLUDED_)
