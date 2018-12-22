// PrintReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BlastTest.h"
#include "PrintReportDlg.h"

//wf
#include "Utility\GetPath.h"
#include "Utility\GridppReportEventImpl.c"
//wf end

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPrintReportDlg dialog


CPrintReportDlg::CPrintReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrintReportDlg::IDD, pParent)
	,m_TestHumi(0)
	,m_TestTemp(0)
{
	//{{AFX_DATA_INIT(CPrintReportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPrintReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintReportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintReportDlg, CDialog)
	//{{AFX_MSG_MAP(CPrintReportDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PRINT, OnPrint)
	ON_BN_CLICKED(IDC_PRINTVIEW, OnPrintview)
	ON_BN_CLICKED(IDC_EXPORTFILE, OnExportfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintReportDlg message handlers

BOOL CPrintReportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//设定日期范围的初始值，1997/1/1 - 1997/12/31
	SYSTEMTIME stBegin;
	SYSTEMTIME stEnd;
	ZeroMemory(&stBegin, sizeof(SYSTEMTIME));
	ZeroMemory(&stEnd, sizeof(SYSTEMTIME));
	stBegin.wYear = 1997;
	stBegin.wMonth = 1;
	stBegin.wDay = 1;
	stEnd.wYear = 1997;
	stEnd.wMonth = 12;
	stEnd.wDay = 31;
//	DateTime_SetSystemtime(GetDlgItem(IDC_DATETIMEPICKER1)->m_hWnd, GDT_VALID, &stBegin);
//	DateTime_SetSystemtime(GetDlgItem(IDC_DATETIMEPICKER2)->m_hWnd, GDT_VALID, &stEnd);
	
	//创建报表主对象
	m_pGridppReport.CreateInstance( __uuidof(GridppReport) );
	ATLASSERT(m_pGridppReport != NULL);
	
	//取得查询显示器控件的接口指针
	CWnd *pDispalyViewerWnd = GetDlgItem(IDC_GRDISPLAYVIEWER1);
	LPUNKNOWN spUnk = pDispalyViewerWnd->GetControlUnknown();
	spUnk->QueryInterface(__uuidof(IGRDisplayViewer), (void**)(&m_pDisplayViewer));
	ATLASSERT(m_pDisplayViewer != NULL);
	
	//从文件中载入报表模板数据到报表主对象
	//CString FileName = GetReportTemplatePath() + _T("Sale summary by product.grf");
	CString FileName;
	if(m_Type==_T("特性落高法"))
		 FileName = GetReportTemplatePath() + _T("My Report CRT.grf");
	else
		 FileName = GetReportTemplatePath() + _T("My Report 01.grf");

	m_pGridppReport->LoadFromFile( (LPCTSTR)FileName );
	
    //设置与数据源的连接串，因为在设计时指定的数据库路径是绝对路径。
    m_pGridppReport->DetailGrid->Recordset->ConnectionString = (LPCTSTR)GetDatabaseConnectionString();
	
	//创建事件响应对象
	/*CComObject<CDetailReportEvent> *pEvent;
	CComObject<CDetailReportEvent>::CreateInstance( &pEvent );
	pEvent->AddRef();
	pEvent->m_pDetailDlg = this;
	m_pReportEvents = pEvent;
	HRESULT hr = m_pReportEvents->DispEventAdvise(m_pGridppReport, &__uuidof(_IGridppReportEvents)) ;
	hr;
	ATLASSERT( SUCCEEDED(hr) );*/

	//查询显示器控件关联报表主对象
	m_pDisplayViewer->Report = m_pGridppReport;
	
	//ApplyDateRangeParam();
	CString SQL=_T("select * from TestData where 试样编号='")+m_SpNo+_T("' order by 自动编号 ASC");
	m_pGridppReport->DetailGrid->Recordset->QuerySQL = (LPCTSTR)SQL;
	
	m_pGridppReport->Parameters->Item["实验员"]->AsString=(_bstr_t)m_Person;
	m_pGridppReport->Parameters->Item["试样编号"]->AsString=(_bstr_t)m_SpNo;
	m_pGridppReport->Parameters->Item["实验类型"]->AsString=(_bstr_t)m_Type;
	m_pGridppReport->Parameters->Item["重锤质量"]->AsFloat=m_HunKg;
	m_pGridppReport->Parameters->Item["重锤高度"]->AsFloat=m_HunHight;
	m_pGridppReport->Parameters->Item["实验温度"]->AsFloat=m_TestTemp;
	m_pGridppReport->Parameters->Item["实验湿度"]->AsFloat=m_TestHumi;

	//启动查询显示器的运行
	m_pDisplayViewer->Start();
	
	//AdjustViewerSize();
	//打印预览功能
	//m_pGridppReport->PrintPreview(TRUE);
	//打印功能
	//m_pGridppReport->Print(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CPrintReportDlg, CDialog)
//{{AFX_EVENTSINK_MAP(CMainDlg)
ON_EVENT(CPrintReportDlg, IDC_GRDISPLAYVIEWER1, DISPID_CONTENTCELLDBLCLICK, OnContentCellDblClickDisplayViewer, VTS_DISPATCH)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPrintReportDlg::OnContentCellDblClickDisplayViewer(LPDISPATCH pSender)
{
	//OnDetail();
	/*int line=m_pGridppReport->FieldByName("序号")->AsInteger;
	CString str;
	str.Format("单击行:%d",line);
	MessageBox(str);*/
}

void CPrintReportDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	/*if (m_pReportEvents != NULL)
	{
		HRESULT hr = m_pReportEvents->DispEventUnadvise(m_pGridppReport, &__uuidof(_IGridppReportEvents));
		m_pReportEvents->Release();
		m_pReportEvents = NULL;
		hr;
		ATLASSERT( SUCCEEDED(hr) );
	}*/
	m_pDisplayViewer.Release();
	m_pGridppReport.Release();
}

void CPrintReportDlg::OnPrint() 
{
	// TODO: Add your control notification handler code here
	//打印预览功能
	//m_pGridppReport->PrintPreview(TRUE);
	//打印功能
	m_pGridppReport->Print(TRUE);
}

void CPrintReportDlg::OnPrintview() 
{
	// TODO: Add your control notification handler code here
	//打印预览功能
	m_pGridppReport->PrintPreview(TRUE);
	//打印功能
	//m_pGridppReport->Print(TRUE);
}
//导出PDF文件
void CPrintReportDlg::OnExportfile() 
{
	// TODO: Add your control notification handler code here
	m_pGridppReport->ExportDirect(gretPDF,_T("D:\\爆破项目\\BlastTest\\实验记录单.pdf"),TRUE,TRUE);

}
