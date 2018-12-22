// ReportBillPrint.cpp : implementation file
//

#include "stdafx.h"
#include "BlastTest.h"
#include "ReportBillPrint.h"

//wf
#include "Utility\GetPath.h"
//#include "Utility\GridppReportEventImpl.c"
//wf end

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportBillPrint dialog


CReportBillPrint::CReportBillPrint(CWnd* pParent /*=NULL*/)
	: CDialog(CReportBillPrint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportBillPrint)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CReportBillPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportBillPrint)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportBillPrint, CDialog)
	//{{AFX_MSG_MAP(CReportBillPrint)
	ON_BN_CLICKED(IDC_PRINT, OnPrint)
	ON_BN_CLICKED(IDC_PRINTVIEW, OnPrintview)
	ON_BN_CLICKED(IDC_EXPORTFILE, OnExportfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportBillPrint message handlers

BOOL CReportBillPrint::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
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
	if(m_TestType==_T("特性落高法"))
	    FileName = GetReportTemplatePath() + _T("My Report 02CRT.grf");
	else
		FileName = GetReportTemplatePath() + _T("My Report 02.grf");

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
	CString SQL=_T("select * from SampleTable where 试样编号='")+m_SpNo+_T("'");
	m_pGridppReport->DetailGrid->Recordset->QuerySQL = (LPCTSTR)SQL;
	

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

void CReportBillPrint::OnPrint() 
{
	// TODO: Add your control notification handler code here
	//m_pGridppReport->PrintPreview(TRUE);
	//打印功能
	m_pGridppReport->Print(TRUE);
}

void CReportBillPrint::OnPrintview() 
{
	// TODO: Add your control notification handler code here
	m_pGridppReport->PrintPreview(TRUE);
	//打印功能
	//m_pGridppReport->Print(TRUE);
}
//导出PDF
void CReportBillPrint::OnExportfile() 
{
	// TODO: Add your control notification handler code here
	m_pGridppReport->ExportDirect(gretPDF,_T("D:\\爆破项目\\BlastTest\\实验报告单.pdf"),TRUE,TRUE);

}
