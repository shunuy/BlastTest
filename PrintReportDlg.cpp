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
	//�趨���ڷ�Χ�ĳ�ʼֵ��1997/1/1 - 1997/12/31
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
	if(m_Type==_T("������߷�"))
		 FileName = GetReportTemplatePath() + _T("My Report CRT.grf");
	else
		 FileName = GetReportTemplatePath() + _T("My Report 01.grf");

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
	CString SQL=_T("select * from TestData where �������='")+m_SpNo+_T("' order by �Զ���� ASC");
	m_pGridppReport->DetailGrid->Recordset->QuerySQL = (LPCTSTR)SQL;
	
	m_pGridppReport->Parameters->Item["ʵ��Ա"]->AsString=(_bstr_t)m_Person;
	m_pGridppReport->Parameters->Item["�������"]->AsString=(_bstr_t)m_SpNo;
	m_pGridppReport->Parameters->Item["ʵ������"]->AsString=(_bstr_t)m_Type;
	m_pGridppReport->Parameters->Item["�ش�����"]->AsFloat=m_HunKg;
	m_pGridppReport->Parameters->Item["�ش��߶�"]->AsFloat=m_HunHight;
	m_pGridppReport->Parameters->Item["ʵ���¶�"]->AsFloat=m_TestTemp;
	m_pGridppReport->Parameters->Item["ʵ��ʪ��"]->AsFloat=m_TestHumi;

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

BEGIN_EVENTSINK_MAP(CPrintReportDlg, CDialog)
//{{AFX_EVENTSINK_MAP(CMainDlg)
ON_EVENT(CPrintReportDlg, IDC_GRDISPLAYVIEWER1, DISPID_CONTENTCELLDBLCLICK, OnContentCellDblClickDisplayViewer, VTS_DISPATCH)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPrintReportDlg::OnContentCellDblClickDisplayViewer(LPDISPATCH pSender)
{
	//OnDetail();
	/*int line=m_pGridppReport->FieldByName("���")->AsInteger;
	CString str;
	str.Format("������:%d",line);
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
	//��ӡԤ������
	//m_pGridppReport->PrintPreview(TRUE);
	//��ӡ����
	m_pGridppReport->Print(TRUE);
}

void CPrintReportDlg::OnPrintview() 
{
	// TODO: Add your control notification handler code here
	//��ӡԤ������
	m_pGridppReport->PrintPreview(TRUE);
	//��ӡ����
	//m_pGridppReport->Print(TRUE);
}
//����PDF�ļ�
void CPrintReportDlg::OnExportfile() 
{
	// TODO: Add your control notification handler code here
	m_pGridppReport->ExportDirect(gretPDF,_T("D:\\������Ŀ\\BlastTest\\ʵ���¼��.pdf"),TRUE,TRUE);

}
