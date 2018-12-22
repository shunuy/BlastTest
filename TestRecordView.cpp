// TestRecordView.cpp : implementation file
//

/*�޸ļ�¼
�޸�����       �޸���        �޸�����
20150707        ���          �޸�ɾ����¼�����޸����һ����¼��Ϊ�޸���ǰһ����¼����Ϊ��¼��˳�����˱仯��


*/
#include "stdafx.h"
#include "BlastTest.h"
#include "TestRecordView.h"

#include "_recordset.h"
#include "ADO/ADO.h"
#include "selbookmarks.h"
#include "PrintReportDlg.h"
#include "ReportBillPRint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestRecordView dialog
/*
����           �޸���          ����
20150624       ���           ��ʵ���¼�����ڵĽ�������
20150625       ���           ���Ӵ�ӡ���淵�غ������ˢ��
20150626       ���           ����1sˢ��һ�Ρ����ӱ��������Ʒ�ֵ
  */

CTestRecordView::CTestRecordView(CWnd* pParent /*=NULL*/)
	: CDialog(CTestRecordView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestRecordView)
	m_Type = _T("");
	m_HunKg = 0.0f;
	m_Person = _T("");
	m_HunHight = 0.0f;
	m_CombSpNo = _T("");
	m_TestHumi = 0.0f;
	m_TestTemp = 0.0f;
	//}}AFX_DATA_INIT
}


void CTestRecordView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestRecordView)
	DDX_Control(pDX, IDC_ADODC1, m_AdoRecord);
	DDX_Control(pDX, IDC_DATAGRID1, m_DGridRecord);
	DDX_Text(pDX, IDC_TYPE, m_Type);
	DDX_Text(pDX, IDC_HUNKG, m_HunKg);
	DDX_Text(pDX, IDC_PERSON, m_Person);
	DDX_Text(pDX, IDC_HUNHIGHT, m_HunHight);
	DDX_CBString(pDX, IDC_COMBSPNO, m_CombSpNo);
	DDX_Text(pDX, IDC_TESTHUMI, m_TestHumi);
	DDX_Text(pDX, IDC_TESTTEMP, m_TestTemp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestRecordView, CDialog)
	//{{AFX_MSG_MAP(CTestRecordView)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBSPNO, OnSelchangeCombspno)
	ON_BN_CLICKED(IDC_FIND, OnFind)
	ON_BN_CLICKED(IDC_DELSEL, OnDelsel)
	ON_BN_CLICKED(IDC_PRINTREPORT, OnPrintreport)
	ON_BN_CLICKED(IDC_REPORTBILLBTN, OnReportbillbtn)
	ON_BN_CLICKED(IDC_CHANGEBLAST, OnChangeblast)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestRecordView message handlers

BOOL CTestRecordView::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CFont* ptf=GetDlgItem(IDC_TYPE)->GetFont();//m_editPlace.GetFont(); // �õ�ԭ�������� 
	LOGFONT lf;   
	ptf->GetLogFont(&lf);   
	lf.lfHeight = 25; // �ı�����߶�   
	strcpy (lf.lfFaceName, "����"); // �ı���������   
	m_editFont.CreateFontIndirect(&lf);   
	GetDlgItem(IDC_TYPE)->SetFont(&m_editFont); // ���������� 
	GetDlgItem(IDC_HUNKG)->SetFont(&m_editFont); // ����������
	GetDlgItem(IDC_HUNHIGHT)->SetFont(&m_editFont); // ����������
	GetDlgItem(IDC_PERSON)->SetFont(&m_editFont); // ����������
	GetDlgItem(IDC_COMBSPNO)->SetFont(&m_editFont); // ����������
	GetDlgItem(IDC_TESTTEMP)->SetFont(&m_editFont); // ����������
	GetDlgItem(IDC_TESTHUMI)->SetFont(&m_editFont); // ����������


	// TODO: Add extra initialization here
	C_Recordset RSet=m_AdoRecord.GetRecordset();
	//long lCount = oRecord.GetRecordCount();
	//VARIANT oSource = oRecord.GetSource();
	CString strConnect = m_AdoRecord.GetConnectionString();
	long lCursorType = RSet.GetCursorType();
	long lLockType = RSet.GetLockType();
	COleVariant oleVar;
	if(m_Type==_T("������߷�"))
		oleVar=_T("select �Զ����,ҩ��,����״̬,�����Ʒ�ֵ,�ش��߶�,ʵ����,ʵ������ from TestData where �������='")+m_SpNo+_T("'") +_T("order by ʵ������  desc");	
	else 
		oleVar=_T("select �Զ����,ʵ������,ҩ��,����״̬,�����Ʒ�ֵ,ʵ���� from TestData where �������='")+m_SpNo+_T("'") +_T("order by ʵ������  desc");	
	//m_AdoRecord.SetRecordSource(_T(""));
	RSet.Requery(adConnectUnspecified);
	RSet.Close();
	
	RSet.Open(oleVar,COleVariant(strConnect),lCursorType, lLockType, adConnectUnspecified );
	RSet.Requery(adConnectUnspecified );
	m_DGridRecord.Refresh();
	
	CADODataSet DataSpNo;
	DataSpNo.SetConnection(::GetConnection());
	DataSpNo.Open(_T("select top 20 * from SampleTable order by ID DESC"));
	for(int i=0;i<DataSpNo.GetRecordCount();i++)
	{
		((CComboBox *)GetDlgItem(IDC_COMBSPNO))->AddString(DataSpNo.ReadStringValue(_T("�������")));
		DataSpNo.Next();
	}
	((CComboBox *)GetDlgItem(IDC_COMBSPNO))->SetCurSel(0);

	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from TestData where �������='")+m_SpNo+_T("'"));
	if(DataSet.GetRecordCount()>0)
	{
		m_Type=DataSet.ReadStringValue(_T("ʵ������"));
		//m_HunKg=DataSet.ReadFloatValue(_T("�ش�����"));
		m_HunKg = DataSet.ReadFloatValue(_T("��ѹѹ��"));
		m_HunHight=DataSet.ReadFloatValue(_T("�ش��߶�"));
		m_Person=DataSet.ReadStringValue(_T("ʵ��Ա"));
		m_TestHumi=DataSet.ReadFloatValue(_T("ʵ��ʪ��"));
		m_TestTemp=DataSet.ReadFloatValue(_T("ʵ���¶�"));
		m_CombSpNo=m_SpNo;
		UpdateData(FALSE);
	}
	
	SetTimer(1,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTestRecordView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1)
	{
		C_Recordset RSet=m_AdoRecord.GetRecordset();
		RSet.Requery(adConnectUnspecified);
		m_DGridRecord.ReBind();                        //ʵ����datagrid�еı��� 
		m_DGridRecord.Refresh(); 
		KillTimer(1);
	}
	CDialog::OnTimer(nIDEvent);
}
//ˢ��������ʾ
void CTestRecordView::DataRefresh(CString Str)
{
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from TestData where �������='")+Str+_T("'"));
	m_Type=DataSet.ReadStringValue(_T("ʵ������"));
	
	C_Recordset RSet=m_AdoRecord.GetRecordset();
	//long lCount = oRecord.GetRecordCount();
	//VARIANT oSource = oRecord.GetSource();
	CString strConnect = m_AdoRecord.GetConnectionString();
	long lCursorType = RSet.GetCursorType();
	long lLockType = RSet.GetLockType();
	COleVariant oleVar;
	if(m_Type==_T("������߷�"))
		oleVar=_T("select �Զ����,ҩ��,����״̬,�����Ʒ�ֵ,�ش��߶�,ʵ����,ʵ������ from TestData where �������='")+m_SpNo+_T("'") +_T("order by ʵ������  desc");	
	else 
		oleVar=_T("select �Զ����,ʵ������,ҩ��,����״̬,�����Ʒ�ֵ,ʵ���� from TestData where �������='")+m_SpNo+_T("'") +_T("order by ʵ������  desc");
	RSet.Close();
	RSet.Open(oleVar,COleVariant(strConnect),lCursorType, lLockType, adConnectUnspecified );
	RSet.Requery(adConnectUnspecified );
	m_DGridRecord.Refresh();
	SetTimer(1,500,NULL);
	
	if(DataSet.GetRecordCount()>0)
	{
		m_Type=DataSet.ReadStringValue(_T("ʵ������"));
		//m_HunKg=DataSet.ReadFloatValue(_T("�ش�����"));
		m_HunKg = DataSet.ReadFloatValue(_T("��ѹѹ��"));
		m_HunHight=DataSet.ReadFloatValue(_T("�ش��߶�"));
		m_Person=DataSet.ReadStringValue(_T("ʵ��Ա"));
		m_TestHumi=DataSet.ReadFloatValue(_T("ʵ��ʪ��"));
		m_TestTemp=DataSet.ReadFloatValue(_T("ʵ���¶�"));
		UpdateData(FALSE);
	}
}
//�������ѡ�����ı�
void CTestRecordView::OnSelchangeCombspno() 
{
	// TODO: Add your control notification handler code here
	int nSel;   	
    // ��ȡ��Ͽ�ؼ����б����ѡ���������   
    nSel = ((CComboBox *)GetDlgItem(IDC_COMBSPNO))->GetCurSel();   
    // ����ѡ����������ȡ�����ַ���   
    ((CComboBox *)GetDlgItem(IDC_COMBSPNO))->GetLBText(nSel, m_CombSpNo);  
	m_SpNo=m_CombSpNo;
	DataRefresh(m_SpNo);
	// TODO: Add extra initialization here

}

void CTestRecordView::OnFind() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_SpNo=m_CombSpNo;
	DataRefresh(m_SpNo);
	// TODO: Add extra initialization here
}

void CTestRecordView::OnDelsel() 
{
	// TODO: Add your control notification handler code here
	C_Recordset Rst=m_AdoRecord.GetRecordset();
//	Rst.MoveLast();//ɾ�����һ����¼
	Rst.MoveFirst();//�ƶ�����һ����¼
	Rst.Delete(1);

}

void CTestRecordView::OnPrintreport() 
{
	// TODO: Add your control notification handler code here
	CPrintReportDlg dlg;
	dlg.m_SpNo=m_SpNo;
	dlg.m_Person=m_Person;
	dlg.m_Type=m_Type;
	dlg.m_HunKg=m_HunKg;
	dlg.m_HunHight=m_HunHight;
	dlg.m_TestHumi=m_TestHumi;
	dlg.m_TestTemp=m_TestTemp;
	dlg.DoModal();
	DataRefresh(m_SpNo);

}

void CTestRecordView::OnReportbillbtn() 
{
	// TODO: Add your control notification handler code here
	CReportBillPrint dlg;
	dlg.m_SpNo=m_SpNo;
	dlg.DoModal();
	DataRefresh(m_SpNo);

}
//�����б�
void CTestRecordView::OnChangeblast() 
{
	// TODO: Add your control notification handler code here
	CADODataSet DataSet;
	int aa = 0;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from TestData where �������='")+m_SpNo+_T("'"));
	aa = DataSet.GetRecordCount();
	//zhj modify V1.05
	//if(DataSet.GetRecordCount()>=1)
	//	DataSet.move(DataSet.GetRecordCount()-1);
	//else return;

	if (aa >= 1)
		while (!DataSet.IsadoEOF())
			DataSet.Next();
	else return;

	short BlastIndex=DataSet.ReadIntValue(_T("��ը״̬"));
	if(BlastIndex)
	{
		BlastIndex=0;
		DataSet.SetFieldValue(_T("��ը״̬"),BlastIndex);
		DataSet.SetFieldValue(_T("ʵ����"),_T("�˹���Ϊ����"));

	}
	else
	{
		BlastIndex=1;
		DataSet.SetFieldValue(_T("��ը״̬"),BlastIndex);
		DataSet.SetFieldValue(_T("ʵ����"),_T("�˹���Ϊ��ը"));
	}
	DataSet.Save();
	DataSet.Open(_T("select * from TestData where �������='")+m_SpNo+_T("'"));//����һ�β�ѯָ�Ϊ�˸���
	DataRefresh(m_SpNo);
}

void CTestRecordView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//DataRefresh(m_SpNo);//The current row is not available����������Ϊ��δ��ʼ�����
	SetTimer(1,500,NULL);
	// Do not call CDialog::OnPaint() for painting messages

	CBitmap bmp;
	bmp.LoadBitmap(IDB_TESTRECORDBK);
	BITMAP bitmap;
	bmp.GetBitmap(&bitmap);
	CDC bmpdc;
	bmpdc.CreateCompatibleDC(&dc);
	bmpdc.SelectObject(&bmp);
	CRect rect;
	//��ʾ��������������
	this->GetClientRect(&rect);
	////��ʾ��ͼƬ�ؼ���
	//GetDlgItem(IDC_LOGINIMG)->GetWindowRect(&rect);
	//ScreenToClient(&rect);//��ʾ����������������Ҫ�����
	//��ʾ��������������
	dc.StretchBlt(rect.left, rect.top, rect.right, rect.bottom, &bmpdc,
		rect.left, rect.top, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	SetTimer(2, 500, NULL);//���ڸ���Data����ʵ,������һ��֮���Զ��رն�ʱ��


}
