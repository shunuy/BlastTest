#pragma once


// CEditSpData �Ի���

class CEditSpData : public CDialogEx
{
	DECLARE_DYNAMIC(CEditSpData)

public:
	CEditSpData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditSpData();

// �Ի�������
	enum { IDD = IDD_EDITSPDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStringArray * m_SpNo;
	CString m_EditStr[8];
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};
