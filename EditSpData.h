#pragma once


// CEditSpData 对话框

class CEditSpData : public CDialogEx
{
	DECLARE_DYNAMIC(CEditSpData)

public:
	CEditSpData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditSpData();

// 对话框数据
	enum { IDD = IDD_EDITSPDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStringArray * m_SpNo;
	CString m_EditStr[8];
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};
