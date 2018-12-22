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
	
	//��������������
	m_pGridppReport.CreateInstance( __uuidof(GridppReport) );
	ATLASSERT(m_pGridppReport != NULL);
	
	//ȡ�ò�ѯ��ʾ���ؼ��Ľӿ�ָ��
	CWnd *pDispalyViewerWnd = GetDlgItem(IDC_GRDISPLAYVIEWER1);
	LPUNKNOWN spUnk = pDispalyViewerWnd->GetControlUnknown();
	spUnk->QueryInterface(__uuidof(IGRDisplayViewer), (void**)(&m_pDisplayViewer));
	ATLASSERT(m_pDisplayViewer != NULL);
	
	//���ļ������뱨��ģ�����ݵ�����������
	//CString FileName = GetReportTemplatePath() + _T("Sale summary by product.grf");

	CString FileName;
	if(m_TestType==_T("������߷�"))
	    FileName = GetReportTemplatePath() + _T("My Report 02CRT.grf");
	else
		FileName = GetReportTemplatePath() + _T("My Report 02.grf");

	m_pGridppReport->LoadFromFile( (LPCTSTR)FileName );
	
    //����������Դ�����Ӵ�����Ϊ�����ʱָ�������ݿ�·���Ǿ���·����
    m_pGridppReport->DetailGrid->Recordset->ConnectionString = (LPCTSTR)GetDatabaseConnectionString();
	
	//�����¼���Ӧ����
	/*CComObject<CDetailReportEvent> *pEvent;
	CComObject<CDetailReportEvent>::CreateInstance( &pEvent );
	pEvent->AddRef();
	pEvent->m_pDetailDlg = this;
	m_pReportEvents = pEvent;
	HRESULT hr = m_pReportEvents->DispEventAdvise(m_pGridppReport, &__uuidof(_IGridppReportEvents)) ;
	hr;
	ATLASSERT( SUCCEEDED(hr) );*/

	//��ѯ��ʾ���ؼ���������������
	m_pDisplayViewer->Report = m_pGridppReport;
	
	//ApplyDateRangeParam();
	CString SQL=_T("select * from SampleTable where �������='")+m_SpNo+_T("'");
	m_pGridppReport->DetailGrid->Recordset->QuerySQL = (LPCTSTR)SQL;
	

	//������ѯ��ʾ��������
	m_pDisplayViewer->Start();
	
	//AdjustViewerSize();
	//��ӡԤ������
	//m_pGridppReport->PrintPreview(TRUE);
	//��ӡ����
	//m_pGridppReport->Print(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportBillPrint::OnPrint() 
{
	// TODO: Add your control notification handler code here
	//m_pGridppReport->PrintPreview(TRUE);
	//��ӡ����
	m_pGridppReport->Print(TRUE);
}

void CReportBillPrint::OnPrintview() 
{
	// TODO: Add your control notification handler code here
	m_pGridppReport->PrintPreview(TRUE);
	//��ӡ����
	//m_pGridppReport->Print(TRUE);
}
//����PDF
void CReportBillPrint::OnExportfile() 
{
	// TODO: Add your control notification handler code here
	m_pGridppReport->ExportDirect(gretPDF,_T("D:\\������Ŀ\\BlastTest\\ʵ�鱨�浥.pdf"),TRUE,TRUE);

}
