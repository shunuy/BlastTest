// EditSpData.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BlastTest.h"
#include "EditSpData.h"
#include "afxdialogex.h"


// CEditSpData �Ի���

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


// CEditSpData ��Ϣ�������


BOOL CEditSpData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CEditSpData::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	for (int i = 0; i < 8; i++)
		m_SpNo->SetAt(i, m_EditStr[i]);

	CDialogEx::OnOK();
}


BOOL CEditSpData::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ����ר�ô����/����û���
	

	return CDialogEx::PreCreateWindow(cs);
}
