// EditSpData.cpp : 实现文件
//

#include "stdafx.h"
#include "BlastTest.h"
#include "EditSpData.h"
#include "afxdialogex.h"


// CEditSpData 对话框

IMPLEMENT_DYNAMIC(CEditSpData, CDialogEx)

CEditSpData::CEditSpData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditSpData::IDD, pParent)
	/*, m_SpNo(_T(""))*/
{
	
}

CEditSpData::~CEditSpData()
{
}

void CEditSpData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_EditStr[0]);
	DDX_Text(pDX, IDC_EDIT19, m_EditStr[1]);
	DDX_Text(pDX, IDC_EDIT20, m_EditStr[2]);
	DDX_Text(pDX, IDC_EDIT21, m_EditStr[3]);
	DDX_Text(pDX, IDC_EDIT22, m_EditStr[4]);
	DDX_Text(pDX, IDC_EDIT23, m_EditStr[5]);
	DDX_Text(pDX, IDC_EDIT24, m_EditStr[6]);
	DDX_Text(pDX, IDC_EDIT25, m_EditStr[7]);
}


BEGIN_MESSAGE_MAP(CEditSpData, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEditSpData::OnBnClickedOk)
END_MESSAGE_MAP()


// CEditSpData 消息处理程序


BOOL CEditSpData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int No = m_SpNo->GetCount();
	if (No < 8)
	{
		for (int i = No; i < 8; i++)
			m_SpNo->Add(_T(""));
	}
	for (int i = 0; i < 8; i++)
		m_EditStr[i] = m_SpNo->GetAt(i);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CEditSpData::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	for (int i = 0; i < 8; i++)
		m_SpNo->SetAt(i, m_EditStr[i]);

	CDialogEx::OnOK();
}


BOOL CEditSpData::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此添加专用代码和/或调用基类
	

	return CDialogEx::PreCreateWindow(cs);
}
