// TestRecordView.cpp : implementation file
//

/*修改记录
修改日期       修改人        修改内容
20150707        伍丰          修改删除记录，从修改最后一条记录改为修改最前一条记录，因为记录的顺序发送了变化。


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
日期           修改人          内容
20150624       伍丰           将实验记录按日期的降序排列
20150625       伍丰           增加打印界面返回后的数据刷新
20150626       伍丰           增加1s刷新一次。增加保存声级计峰值
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
	
	CFont* ptf=GetDlgItem(IDC_TYPE)->GetFont();//m_editPlace.GetFont(); // 得到原来的字体 
	LOGFONT lf;   
	ptf->GetLogFont(&lf);   
	lf.lfHeight = 25; // 改变字体高度   
	strcpy (lf.lfFaceName, "宋体"); // 改变字体名称   
	m_editFont.CreateFontIndirect(&lf);   
	GetDlgItem(IDC_TYPE)->SetFont(&m_editFont); // 设置新字体 
	GetDlgItem(IDC_HUNKG)->SetFont(&m_editFont); // 设置新字体
	GetDlgItem(IDC_HUNHIGHT)->SetFont(&m_editFont); // 设置新字体
	GetDlgItem(IDC_PERSON)->SetFont(&m_editFont); // 设置新字体
	GetDlgItem(IDC_COMBSPNO)->SetFont(&m_editFont); // 设置新字体
	GetDlgItem(IDC_TESTTEMP)->SetFont(&m_editFont); // 设置新字体
	GetDlgItem(IDC_TESTHUMI)->SetFont(&m_editFont); // 设置新字体


	// TODO: Add extra initialization here
	C_Recordset RSet=m_AdoRecord.GetRecordset();
	//long lCount = oRecord.GetRecordCount();
	//VARIANT oSource = oRecord.GetSource();
	CString strConnect = m_AdoRecord.GetConnectionString();
	long lCursorType = RSet.GetCursorType();
	long lLockType = RSet.GetLockType();
	COleVariant oleVar;
	if(m_Type==_T("特性落高法"))
		oleVar=_T("select 自动编号,药量,烟雾状态,声级计峰值,重锤高度,实验结果,实验日期 from TestData where 试样编号='")+m_SpNo+_T("'") +_T("order by 实验日期  desc");	
	else 
		oleVar=_T("select 自动编号,实验日期,药量,烟雾状态,声级计峰值,实验结果 from TestData where 试样编号='")+m_SpNo+_T("'") +_T("order by 实验日期  desc");	
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
		((CComboBox *)GetDlgItem(IDC_COMBSPNO))->AddString(DataSpNo.ReadStringValue(_T("试样编号")));
		DataSpNo.Next();
	}
	((CComboBox *)GetDlgItem(IDC_COMBSPNO))->SetCurSel(0);

	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from TestData where 试样编号='")+m_SpNo+_T("'"));
	if(DataSet.GetRecordCount()>0)
	{
		m_Type=DataSet.ReadStringValue(_T("实验类型"));
		//m_HunKg=DataSet.ReadFloatValue(_T("重锤质量"));
		m_HunKg = DataSet.ReadFloatValue(_T("加压压力"));
		m_HunHight=DataSet.ReadFloatValue(_T("重锤高度"));
		m_Person=DataSet.ReadStringValue(_T("实验员"));
		m_TestHumi=DataSet.ReadFloatValue(_T("实验湿度"));
		m_TestTemp=DataSet.ReadFloatValue(_T("实验温度"));
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
		m_DGridRecord.ReBind();                        //实现与datagrid中的表格绑定 
		m_DGridRecord.Refresh(); 
		KillTimer(1);
	}
	CDialog::OnTimer(nIDEvent);
}
//刷新数据显示
void CTestRecordView::DataRefresh(CString Str)
{
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from TestData where 试样编号='")+Str+_T("'"));
	m_Type=DataSet.ReadStringValue(_T("实验类型"));
	
	C_Recordset RSet=m_AdoRecord.GetRecordset();
	//long lCount = oRecord.GetRecordCount();
	//VARIANT oSource = oRecord.GetSource();
	CString strConnect = m_AdoRecord.GetConnectionString();
	long lCursorType = RSet.GetCursorType();
	long lLockType = RSet.GetLockType();
	COleVariant oleVar;
	if(m_Type==_T("特性落高法"))
		oleVar=_T("select 自动编号,药量,烟雾状态,声级计峰值,重锤高度,实验结果,实验日期 from TestData where 试样编号='")+m_SpNo+_T("'") +_T("order by 实验日期  desc");	
	else 
		oleVar=_T("select 自动编号,实验日期,药量,烟雾状态,声级计峰值,实验结果 from TestData where 试样编号='")+m_SpNo+_T("'") +_T("order by 实验日期  desc");
	RSet.Close();
	RSet.Open(oleVar,COleVariant(strConnect),lCursorType, lLockType, adConnectUnspecified );
	RSet.Requery(adConnectUnspecified );
	m_DGridRecord.Refresh();
	SetTimer(1,500,NULL);
	
	if(DataSet.GetRecordCount()>0)
	{
		m_Type=DataSet.ReadStringValue(_T("实验类型"));
		//m_HunKg=DataSet.ReadFloatValue(_T("重锤质量"));
		m_HunKg = DataSet.ReadFloatValue(_T("加压压力"));
		m_HunHight=DataSet.ReadFloatValue(_T("重锤高度"));
		m_Person=DataSet.ReadStringValue(_T("实验员"));
		m_TestHumi=DataSet.ReadFloatValue(_T("实验湿度"));
		m_TestTemp=DataSet.ReadFloatValue(_T("实验温度"));
		UpdateData(FALSE);
	}
}
//试样编号选择发生改变
void CTestRecordView::OnSelchangeCombspno() 
{
	// TODO: Add your control notification handler code here
	int nSel;   	
    // 获取组合框控件的列表框中选中项的索引   
    nSel = ((CComboBox *)GetDlgItem(IDC_COMBSPNO))->GetCurSel();   
    // 根据选中项索引获取该项字符串   
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
//	Rst.MoveLast();//删除最后一条记录
	Rst.MoveFirst();//移动到第一条记录
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
//修正判爆
void CTestRecordView::OnChangeblast() 
{
	// TODO: Add your control notification handler code here
	CADODataSet DataSet;
	int aa = 0;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from TestData where 试样编号='")+m_SpNo+_T("'"));
	aa = DataSet.GetRecordCount();
	//zhj modify V1.05
	//if(DataSet.GetRecordCount()>=1)
	//	DataSet.move(DataSet.GetRecordCount()-1);
	//else return;

	if (aa >= 1)
		while (!DataSet.IsadoEOF())
			DataSet.Next();
	else return;

	short BlastIndex=DataSet.ReadIntValue(_T("爆炸状态"));
	if(BlastIndex)
	{
		BlastIndex=0;
		DataSet.SetFieldValue(_T("爆炸状态"),BlastIndex);
		DataSet.SetFieldValue(_T("实验结果"),_T("人工判为不爆"));

	}
	else
	{
		BlastIndex=1;
		DataSet.SetFieldValue(_T("爆炸状态"),BlastIndex);
		DataSet.SetFieldValue(_T("实验结果"),_T("人工判为爆炸"));
	}
	DataSet.Save();
	DataSet.Open(_T("select * from TestData where 试样编号='")+m_SpNo+_T("'"));//增加一次查询指令，为了更新
	DataRefresh(m_SpNo);
}

void CTestRecordView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//DataRefresh(m_SpNo);//The current row is not available，可能是因为尚未初始化完成
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
	//显示在整个窗体区域
	this->GetClientRect(&rect);
	////显示在图片控件中
	//GetDlgItem(IDC_LOGINIMG)->GetWindowRect(&rect);
	//ScreenToClient(&rect);//显示在整个窗体区域不需要该语句
	//显示在整个窗体区域
	dc.StretchBlt(rect.left, rect.top, rect.right, rect.bottom, &bmpdc,
		rect.left, rect.top, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	SetTimer(2, 500, NULL);//用于更新Data的现实,更新完一次之后自动关闭定时器


}
