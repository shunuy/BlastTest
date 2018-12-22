// TestConditon.cpp : implementation file
//

#include "stdafx.h"
#include "BlastTest.h"
#include "TestConditon.h"
#include "common.h"

#include "ADO/ADO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestConditon dialog
/******
����           �޸���         �޸�����
20150626       ���           ��������ʱ��������
20151023        ���          �޸ĳ�ʼ��IDC_PERSON��Ͽ�ķ�ʽ��ȡ�����ظ���ʵ��Ա
******/

CTestConditon::CTestConditon(CWnd* pParent /*=NULL*/)
	: CDialog(CTestConditon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestConditon)
	m_GroupNo = 0;
	m_HunHight = 0;
	m_HunKg = 0;
	m_OneGroupNo = 0;
	m_Person = _T("");
	m_SpKg = 0;
	m_Type = _T("");
	m_TestTemp = 0.0f;
	m_Humidity = 0.0f;
	m_StepLength = 0.0f;
	//}}AFX_DATA_INIT
}


void CTestConditon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestConditon)
	DDX_Text(pDX, IDC_GROUPNO, m_GroupNo);
	DDV_MinMaxUInt(pDX, m_GroupNo, 0, 50);
	DDX_Text(pDX, IDC_HUNHIGHT, m_HunHight);
	DDX_Text(pDX, IDC_HUNKG, m_HunKg);
	DDX_Text(pDX, IDC_ONEGROUPNO, m_OneGroupNo);
	DDX_CBString(pDX, IDC_PERSON, m_Person);
	DDX_Text(pDX, IDC_SPKG, m_SpKg);
	DDX_CBString(pDX, IDC_TYPE, m_Type);
	DDX_Text(pDX, IDC_EDIT1, m_TestTemp);
	DDX_Text(pDX, IDC_EDIT2, m_Humidity);
	DDX_Text(pDX, IDC_PRESSSET, m_PressSet);
	DDX_Text(pDX, IDC_STEPLENGNT, m_StepLength);
	DDV_MinMaxFloat(pDX, m_StepLength, 5.e-002f, 0.1f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestConditon, CDialog)
	//{{AFX_MSG_MAP(CTestConditon)
	ON_BN_CLICKED(IDC_Save, OnSave)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestConditon message handlers

void CTestConditon::OnSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if((m_HunKg!=2) & (m_HunKg!=10) & (m_HunKg!=5))
	{
		MessageBox(_T("Ŀǰֻ֧��2Kg��10Kg��5Kg���ش���"));
		return;
	}

	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from TestConditionSetup"));

	DataSet.SetFieldValue(_T("ʵ������"),m_GroupNo);
	DataSet.SetFieldValue(_T("ÿ�����"),(_variant_t)m_OneGroupNo);
	DataSet.SetFieldValue(_T("��������"),(_variant_t)m_SpKg);
	DataSet.SetFieldValue(_T("�ش��߶�"),(_variant_t)m_HunHight);
	DataSet.SetFieldValue(_T("�ش�����"),(_variant_t)m_HunKg);
	DataSet.SetFieldValue(_T("ʵ������"),(_variant_t)m_Type);
	DataSet.SetFieldValue(_T("ʵ��Ա"),(_variant_t)m_Person);
	DataSet.SetFieldValue(_T("ʵ���¶�"),(_variant_t)m_TestTemp);
	DataSet.SetFieldValue(_T("ʵ��ʪ��"),(_variant_t)m_Humidity);
	m_StepLength*=10;
	DataSet.SetFieldValue(_T("��������"),(_variant_t)m_StepLength);
	DataSet.SetFieldValue(_T("ѹ���趨"), (_variant_t)m_PressSet);

	DataSet.Save();

	DataSet.AddToTable(_T("UserTable"),_T("�û�����"),m_Person);

	CDialog::OnCancel();
}

BOOL CTestConditon::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CComboBox*)GetDlgItem(IDC_TYPE))->AddString("��ը���ʷ�");
	if (EquipmentType == TESTTYPE_ZHUJI) ((CComboBox*)GetDlgItem(IDC_TYPE))->AddString("������߷�");
	//((CComboBox*)GetDlgItem(IDC_TYPE))->AddString("������߷�");
	//((CComboBox*)GetDlgItem(IDC_TYPE))->AddString("Ħ���жȷ�");
	((CComboBox*)GetDlgItem(IDC_TYPE))->SetCurSel(0);
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from TestConditionSetup"));
	m_GroupNo=DataSet.ReadIntValue(_T("ʵ������"));
	m_OneGroupNo=DataSet.ReadIntValue(_T("ÿ�����"));
	m_SpKg=DataSet.ReadFloatValue(_T("��������"));
	m_HunHight=DataSet.ReadFloatValue(_T("�ش��߶�"));
	m_HunKg=DataSet.ReadFloatValue(_T("�ش�����"));
	m_Type=DataSet.ReadStringValue(_T("ʵ������"));
	m_Person=DataSet.ReadStringValue(_T("ʵ��Ա"));
	m_TestTemp=DataSet.ReadFloatValue(_T("ʵ���¶�"));
	m_Humidity=DataSet.ReadFloatValue(_T("ʵ��ʪ��"));
	m_StepLength=(float)(DataSet.ReadFloatValue(_T("��������"))/10);
	m_PressSet = DataSet.ReadFloatValue(_T("ѹ���趨"));

	//���û�����������ӵ��û���Ͽ���
	//((CComboBox *)GetDlgItem(IDC_PERSON))->AddString(DataSet.ReadStringValue(_T("ʵ��Ա")));//20151023ɾ��
	m_Person=DataSet.ReadStringValue(_T("ʵ��Ա"));//20151023���
	DataSet.AddToCommb(this->m_hWnd,IDC_PERSON,_T("UserTable"),_T("�û�����"));
	//((CComboBox *)GetDlgItem(IDC_PERSON))->SetCurSel(0);//20151023ɾ��

   // ((CComboBox *)GetDlgItem(IDC_PERSON))->
	//WinExec(_T("C:\\Program Files\\HotVirtualkeyboard\\hvk.exe"),SW_NORMAL);//��������

	SetTimer(1,500,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTestConditon::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	/*GetDlgItem(IDC_SPKG)->SetFocus();
	GetDlgItem(IDC_HUNKG)->SetFocus();
	GetDlgItem(IDC_ONEGROUPNO)->SetFocus();
	GetDlgItem(IDC_HUNHIGHT)->SetFocus();
	GetDlgItem(IDC_GROUPNO)->SetFocus();

	KillTimer(1);*/
	UpdateData(FALSE);
	KillTimer(1);
	//WinExec(_T("C:\\Program Files\\HotVirtualkeyboard\\hvk.exe"),SW_NORMAL);//��������
    WinExec(_T("C:\\Program Files\\ComfortKeyboard\\CKeyboard.exe"),SW_NORMAL);

	CDialog::OnTimer(nIDEvent);
}
