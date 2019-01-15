// TestRecordView.cpp : implementation file
//

/*修改记录
修改日期       修改人        修改内容
20150707        伍丰          修改删除记录，从修改最后一条记录改为修改最前一条记录，因为记录的顺序发送了变化。


*/
#include "stdafx.h"
#include "BlastTest.h"
#include "TestRecordView.h"

#include "CApplication.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CRange.h"





#include "_recordset.h"
#include "ADO/ADO.h"
#include "selbookmarks.h"
#include "PrintReportDlg.h"
#include "ReportBillPRint.h"
#include "common.h"

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
		oleVar=_T("select 自动编号,药量,实验结果,烟雾状态,声级计峰值,实验日期 from TestData where 试样编号='")+m_SpNo+_T("'") +_T("order by 实验日期  desc");	
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
		if (EquipmentType == TESTTYPE_ZHUJI) m_HunKg = DataSet.ReadFloatValue(_T("重锤质量"));
		else m_HunKg = DataSet.ReadFloatValue(_T("加压压力"));
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
	DataSet.Open(_T("select * from TestConditionSetup"));
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
		oleVar=_T("select 自动编号,药量,实验结果,烟雾状态,声级计峰值,实验日期 from TestData where 试样编号='")+m_SpNo+_T("'") +_T("order by 实验日期  desc");
	RSet.Close();
	RSet.Open(oleVar,COleVariant(strConnect),lCursorType, lLockType, adConnectUnspecified );
	RSet.Requery(adConnectUnspecified );
	m_DGridRecord.Refresh();
	SetTimer(1,500,NULL);
	
	if(DataSet.GetRecordCount()>0)
	{
		m_Type=DataSet.ReadStringValue(_T("实验类型"));
		if (EquipmentType == TESTTYPE_ZHUJI) m_HunKg = DataSet.ReadFloatValue(_T("重锤质量"));
		else	 m_HunKg = DataSet.ReadFloatValue(_T("压力设定"));
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

	CApplication exApp;//应用程序对象
	CWorkbook exBook;//工作簿对象
	CWorkbooks exBooks;//工作簿集合对象
	CWorksheets exSheets;//工作表集合
	CWorksheet exSheet;//工作表对象
	CRange exRange;
	LPDISPATCH lpDisp = NULL;

	if (!exApp.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("Excel failed to start!"));
		return;
	}
	//CString aa = exApp.get_Version();

	//AfxMessageBox(aa);
	exApp.put_Visible(false);
	exBooks.AttachDispatch(exApp.get_Workbooks());
	CString strbookPath = "c:\\BlastTest\\zhuangji.xls";
	if (EquipmentType == TESTTYPE_MOCHA) strbookPath = "c:\\BlastTest\\mocha.xls";

	try
	{

		
		exBook.AttachDispatch(exBooks.Add(_variant_t(strbookPath)));
	//	lpDisp = exBooks.Open(strbookPath, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing);
	//	exBook.AttachDispatch(lpDisp);
	}
	catch (...)
	{
		lpDisp = exBooks.Add(vtMissing);
		exBook.AttachDispatch(lpDisp);
	}


	/*得到工作簿中的Sheet的容器*/
	exSheets.AttachDispatch(exBook.get_Sheets());

	/*打开一个Sheet，如不存在，就新增一个Sheet*/
	CString strSheetName = _T("Sheet1");
	try
	{
		/*打开一个已有的Sheet*/
		lpDisp = exSheets.get_Item(_variant_t(strSheetName));
		exSheet.AttachDispatch(lpDisp);
	}
	catch (...)
	{
		/*创建一个新的Sheet*/
		lpDisp = exSheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
		exSheet.AttachDispatch(lpDisp);
		exSheet.put_Name(strSheetName);
	}



	/*向Sheet中写入多个单元格,规模为10*10 */
	lpDisp = exSheet.get_Range(_variant_t("B5"), _variant_t("Z6"));
	exRange.AttachDispatch(lpDisp);

	VARTYPE vt = VT_I4; /*数组元素的类型，long*/
	SAFEARRAYBOUND sabWrite[2]; /*用于定义数组的维数和下标的起始值*/
	sabWrite[0].cElements =2;
	sabWrite[0].lLbound = 0;
	sabWrite[1].cElements = 25;
	sabWrite[1].lLbound = 0;

	COleSafeArray olesaWrite;
	olesaWrite.Create(vt, sizeof(sabWrite) / sizeof(SAFEARRAYBOUND), sabWrite);

	/*通过指向数组的指针来对二维数组的元素进行间接赋值*/
	long(*pArray)[2] = NULL;
	olesaWrite.AccessData((void **)&pArray);
	memset(pArray, 0, sabWrite[0].cElements * sabWrite[1].cElements * sizeof(long));

	/*释放指向数组的指针*/
	olesaWrite.UnaccessData();
	pArray = NULL;

	/*对二维数组的元素进行逐个赋值*/
	long index[2] = { 0, 0 };
	long lFirstLBound = 0;
	long lFirstUBound = 0;
	long lSecondLBound = 0;
	long lSecondUBound = 0;
	olesaWrite.GetLBound(1, &lFirstLBound);
	olesaWrite.GetUBound(1, &lFirstUBound);
	olesaWrite.GetLBound(2, &lSecondLBound);
	olesaWrite.GetUBound(2, &lSecondUBound);


	float * PRate = new float[2];//用于存放每组的爆炸率
	float Rate, LRate, LRateTmp, HRate, HRateTmp;//总的概率，置信下限，置信上限
	char buf[20];
	CString str;
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	float yaoliang=0.03f;
	_variant_t aa;
	for (long i = lFirstLBound; i <= lFirstUBound; i++)
	{
	    
		memset(buf, 0x00, 20);
		sprintf(buf, _T("A%2d__"), i + 1);
		str = buf;
		str += m_SpNo;
		str.Replace(' ', '0');
		str = "select  * from TestData where  自动编号 like'" + str + "' order by 实验日期 DESC";
		DataSet.Open(_T(str));
	
		index[0] = i;
		for (long j = lSecondLBound; j <= lSecondUBound; j++)
		{

			long lElement = 0;
			if (j<DataSet.GetRecordCount())
			{

				lElement = DataSet.ReadIntValue(_T("爆炸状态"));
				yaoliang = DataSet.ReadFloatValue(_T("药量"));
				 aa = DataSet.ReadValue(_T("实验日期"));
				DataSet.Next();
			}
			index[1] = j;
			olesaWrite.PutElement(index, &lElement);


		}
	}

	

	/*把ColesaWritefeArray变量转换为VARIANT,并写入到Excel表格中*/
	VARIANT varWrite = (VARIANT)olesaWrite;
	exRange.put_Value2(varWrite);


	
	exRange.AttachDispatch(exSheet.get_Cells(), true);
	//DataSet.CloseDataSet();
	//DataSet.SetConnection(::GetConnection());
	str = _T("select * from SampleTable where 试样编号='") + m_SpNo + _T("'");
	DataSet.Open(_T(str));
	str = "";
	//int aaqz = DataSet.GetRecordCount();
	if (DataSet.GetRecordCount()>0) str=DataSet.ReadStringValue(_T("样品名称"));
	exRange.put_Item(_variant_t((long)(3)), _variant_t((long)(3)),
		_variant_t(str));

	CString stri = "", strf = "",yaoliangT = "";
	stri.Format("%0.0f", m_HunHight);
	if (EquipmentType == TESTTYPE_MOCHA) strf.Format("%.2f", m_HunKg);
	else  strf.Format("%0.0f", m_HunKg);
	yaoliang = yaoliang * 1000;
	yaoliangT.Format("%0.0f", yaoliang);
	if (EquipmentType == TESTTYPE_MOCHA)	str = _T("加压压力:") + strf + _T("MPa; 落高：") + stri + _T("mm； 药量：") + yaoliangT + _T("mg");
	else str = _T("锤重:") + strf + _T("MPa; 落高：") + stri + _T("mm； 药量：") + yaoliangT + _T("mg");
	exRange.put_Item(_variant_t((long)(14)), _variant_t((long)(7)),
		_variant_t(str));

	exRange.put_Item(_variant_t((long)(3)), _variant_t((long)(16)),
		aa);

	exRange.put_Item(_variant_t((long)(3)), _variant_t((long)(25)),
		_variant_t(m_TestHumi));

	exRange.put_Item(_variant_t((long)(3)), _variant_t((long)(22)),
		_variant_t(m_TestTemp));


	exRange.put_Item(_variant_t((long)(3)), _variant_t((long)(10)),
		_variant_t(m_SpNo));

	exRange.put_Item(_variant_t((long)(19)), _variant_t((long)(3)),
		_variant_t(m_Person));

	exApp.put_Visible(true);
	//exApp.SetVisible(true);
	//exBook.PrintOut(_variant_t(false));
	exBook.PrintPreview(_variant_t(true));

	/*根据文件的后缀名选择保存文件的格式*/
	//CString strSaveAsName = _T("C:\\new.xlsx");
	//CString strSuffix = strSaveAsName.Mid(strSaveAsName.ReverseFind(_T('.')));
	//XlFileFormat NewFileFormat = xlOpenXMLWorkbook;
	//if (0 == strSuffix.CompareNoCase(_T(".xls")))
	//{
	//	NewFileFormat = xlExcel8;
	//}
	//exBook.SaveAs(_variant_t(strSaveAsName),vtMissing, vtMissing, vtMissing, vtMissing,
	//	vtMissing, 0, vtMissing, vtMissing, vtMissing,
	//vtMissing, vtMissing);

	exRange.ReleaseDispatch();
	exSheet.ReleaseDispatch();
	exSheets.ReleaseDispatch();
	exBook.ReleaseDispatch();
	exBooks.ReleaseDispatch();
	exApp.ReleaseDispatch();




	//// TODO: Add extra initialization here
	//// TODO: Add your control notification handler code here
	//CPrintReportDlg dlg;
	//dlg.m_SpNo=m_SpNo;
	//dlg.m_Person=m_Person;
	//dlg.m_Type=m_Type;
	//dlg.m_HunKg=m_HunKg;
	//dlg.m_HunHight=m_HunHight;
	//dlg.m_TestHumi=m_TestHumi;
	//dlg.m_TestTemp=m_TestTemp;
	//dlg.DoModal();
	//DataRefresh(m_SpNo);

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
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from TestData where 试样编号='")+m_SpNo+_T("'"));
	if(DataSet.GetRecordCount()>=1)
		DataSet.move(DataSet.GetRecordCount()-1);
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
