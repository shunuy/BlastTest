// TestSpData.cpp : implementation file
//

#include "stdafx.h"
#include "BlastTest.h"
#include "TestSpData.h"
#include "EditSpData.h"
#include "_recordset.h"
#include "ADO/ADO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestSpData dialog


CTestSpData::CTestSpData(CWnd* pParent /*=NULL*/)
	: CDialog(CTestSpData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestSpData)
	//}}AFX_DATA_INIT
}


void CTestSpData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestSpData)
	//DDX_Control(pDX, IDC_DATAGRID1, m_SpDataDGrid);
	//DDX_Control(pDX, IDC_ADODC1, m_AdoSpData);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_EDIT2, m_edit);
}


BEGIN_MESSAGE_MAP(CTestSpData, CDialog)
	//{{AFX_MSG_MAP(CTestSpData)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CTestSpData::OnNMDblclkList1)
	ON_WM_LBUTTONDBLCLK()
	ON_EN_KILLFOCUS(IDC_EDIT2, &CTestSpData::OnEnKillfocusEdit2)
	ON_BN_CLICKED(IDC_ADDNEW, &CTestSpData::OnBnClickedAddnew)
	ON_BN_CLICKED(IDC_DELONERECORD, &CTestSpData::OnBnClickedDelonerecord)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CTestSpData::OnNMClickList1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestSpData message handlers

BOOL CTestSpData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ListDis.InitList(m_List);
	m_Set.SetConnection(::GetConnection());
	UpdataListDis();

	SetTimer(1,500,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTestSpData::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//C_Recordset RSet=m_AdoSpData.GetRecordset();
	//RSet.Requery(adConnectUnspecified);
	//m_SpDataDGrid.ReBind();                        //ʵ����datagrid�еı��� 
	//m_SpDataDGrid.Refresh(); 
	
	KillTimer(1);

	CDialog::OnTimer(nIDEvent);
}

void CTestSpData::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	SetTimer(1,500,NULL);	
	// Do not call CDialog::OnPaint() for painting messages
}
//����Ϣ�ڴ���ʧȥ����ʱ����--û�����á�
void CTestSpData::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	//SetTimer(1,100,NULL);
	// TODO: Add your message handler code here

}



void CTestSpData::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ListDis.DisDBClickEdit(pNMHDR, m_List, m_edit);

	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//CRect rc;
	//int m_Row = pNMListView->iItem;//���ѡ�е���
	//int m_Col = pNMListView->iSubItem;//���ѡ����


	//if (pNMListView->iSubItem >-1) //���ѡ���������;
	//{
	//	m_List.GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);//��������RECT��
	//	m_edit.SetParent(&m_List);//ת������Ϊ�б���е�����
	//	m_edit.MoveWindow(rc);//�ƶ�Edit��RECT���ڵ�λ��;
	//	m_edit.SetWindowText(m_List.GetItemText(m_Row, m_Col));//���������е�ֵ����Edit�ؼ��У�
	//	m_edit.ShowWindow(SW_SHOW);//��ʾEdit�ؼ���
	//	m_edit.SetFocus();//����Edit����
	//	m_edit.ShowCaret();//��ʾ���
	//	m_edit.SetSel(-1);//������ƶ������
	//	ListDis.m_Row = m_Row;
	//	ListDis.m_Col = m_Col;
	//}

	*pResult = 0;
}


void CTestSpData::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CRect rcCtrl;

	//LVHITTESTINFO lvhti;

	//lvhti.pt = point;

	//int nItem = m_List.SubItemHitTest(&lvhti);// ������굱ǰѡ�е�ITEM��SUBITEM

	//if (nItem == -1)

	//	return;

	//int nSubItem = lvhti.iSubItem;

	//m_List.GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rcCtrl);//��ȡѡ��λ�õ����� 
	////��ʾ�༭��
	//ShowEdit(TRUE, nItem, nSubItem, rcCtrl);

	CDialog::OnLButtonDblClk(nFlags, point);
}


void CTestSpData::ShowEdit(BOOL bShow, int nItem, int nIndex, CRect rcCtrl)
{
	//ֻ��Ҫ����һ��EDIT�ؼ�������ؼ�û�д������ʹ���EDIT.����ؼ��Ѿ������ˣ��Ϳ�������ʾ��������

	//if (m_edit.m_hWnd == NULL)

	//{

	//	m_edit.Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN | WS_BORDER, CRect(0, 0, 0, 0), this, IDC_EDIT);

	//	m_edit.ShowWindow(SW_HIDE);

	//	CFont tpFont;

	//	tpFont.CreateStockObject(DEFAULT_GUI_FONT);

	//	m_edit.SetFont(&tpFont);

	//	tpFont.DeleteObject();

	//}

	//if (bShow == TRUE)

	//{

	//	CString strItem = m_List.GetItemText(nItem, nIndex);

	//	m_edit.MoveWindow(rcCtrl);

	//	m_edit.ShowWindow(SW_SHOW);

	//	m_edit.SetWindowText(strItem);

	//	::SetFocus(m_edit.GetSafeHwnd());

	//	m_edit.SetSel(-1);

	//	//m_edit.SetCtrlData(MAKEWPARAM(nIndex, nItem));

	//}
	//else
	//	m_edit.ShowWindow(SW_HIDE);


}


void CTestSpData::OnEnKillfocusEdit2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ListDis.WriteEditToList(m_List, m_edit);
	ListDis.WriteToDataSet(m_Set, ar, m_edit);
	//CString tem;
	//m_edit.GetWindowText(tem);    //�õ��û�������µ�����
	//m_List.SetItemText(ListDis.m_Row, ListDis.m_Col, tem);   //���ñ༭���������
	//m_edit.ShowWindow(SW_HIDE);                //Ӧ�ر༭��
}

//����һ����¼
void CTestSpData::OnBnClickedAddnew()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CEditSpData EditDlg;
	CStringArray SpAr;
	for (int i = 0; i < 8; i++)
		SpAr.Add(_T(""));
	EditDlg.m_SpNo = &SpAr;
	if (IDOK == EditDlg.DoModal())
	{
		m_Set.Open(_T("select * from SampleTable"));
		m_Set.moveFirst();
		m_Set.AddNew();
		CString FieldName, Value;
		for (int i = 0; i < 8; i++)
		{
			FieldName = ar.GetAt(i);
			Value = SpAr.GetAt(i);
			m_Set.SetFieldValue(FieldName, (_variant_t)Value);
		}
		m_Set.Save();
		UpdataListDis();
	}
	
}

//ɾ��һ����¼
void CTestSpData::OnBnClickedDelonerecord()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_Set.move(ListDis.m_Row);
	m_Set.Delete();
	m_Set.Save();

	UpdataListDis();
}


void CTestSpData::UpdataListDis()
{
	m_Set.Open(_T("select * from SampleTable"));

	CString Tab;
	Tab = _T("SampleTable");
	ListDis.GetField(Tab, ar);

	ListDis.ListDataDis(m_Set, ar, m_List, 0);/*�Զ������п�*/
}


void CTestSpData::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	ListDis.m_Row = pNMListView->iItem;//���ѡ�е���
	ListDis.m_Col = pNMListView->iSubItem;//���ѡ����

	*pResult = 0;
}
