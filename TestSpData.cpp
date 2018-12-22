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
	//m_SpDataDGrid.ReBind();                        //实现与datagrid中的表格绑定 
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
//该消息在窗口失去焦点时调用--没有作用。
void CTestSpData::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	//SetTimer(1,100,NULL);
	// TODO: Add your message handler code here

}



void CTestSpData::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	ListDis.DisDBClickEdit(pNMHDR, m_List, m_edit);

	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//CRect rc;
	//int m_Row = pNMListView->iItem;//获得选中的行
	//int m_Col = pNMListView->iSubItem;//获得选中列


	//if (pNMListView->iSubItem >-1) //如果选择的是子项;
	//{
	//	m_List.GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);//获得子项的RECT；
	//	m_edit.SetParent(&m_List);//转换坐标为列表框中的坐标
	//	m_edit.MoveWindow(rc);//移动Edit到RECT坐在的位置;
	//	m_edit.SetWindowText(m_List.GetItemText(m_Row, m_Col));//将该子项中的值放在Edit控件中；
	//	m_edit.ShowWindow(SW_SHOW);//显示Edit控件；
	//	m_edit.SetFocus();//设置Edit焦点
	//	m_edit.ShowCaret();//显示光标
	//	m_edit.SetSel(-1);//将光标移动到最后
	//	ListDis.m_Row = m_Row;
	//	ListDis.m_Col = m_Col;
	//}

	*pResult = 0;
}


void CTestSpData::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//CRect rcCtrl;

	//LVHITTESTINFO lvhti;

	//lvhti.pt = point;

	//int nItem = m_List.SubItemHitTest(&lvhti);// 测试鼠标当前选中的ITEM和SUBITEM

	//if (nItem == -1)

	//	return;

	//int nSubItem = lvhti.iSubItem;

	//m_List.GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rcCtrl);//获取选中位置的区域 
	////显示编辑框
	//ShowEdit(TRUE, nItem, nSubItem, rcCtrl);

	CDialog::OnLButtonDblClk(nFlags, point);
}


void CTestSpData::ShowEdit(BOOL bShow, int nItem, int nIndex, CRect rcCtrl)
{
	//只需要创建一次EDIT控件，如果控件没有创建，就创建EDIT.如果控件已经创建了，就控制是显示还是隐藏

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
	// TODO:  在此添加控件通知处理程序代码
	ListDis.WriteEditToList(m_List, m_edit);
	ListDis.WriteToDataSet(m_Set, ar, m_edit);
	//CString tem;
	//m_edit.GetWindowText(tem);    //得到用户输入的新的内容
	//m_List.SetItemText(ListDis.m_Row, ListDis.m_Col, tem);   //设置编辑框的新内容
	//m_edit.ShowWindow(SW_HIDE);                //应藏编辑框
}

//新增一条记录
void CTestSpData::OnBnClickedAddnew()
{
	// TODO:  在此添加控件通知处理程序代码
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

//删除一条记录
void CTestSpData::OnBnClickedDelonerecord()
{
	// TODO:  在此添加控件通知处理程序代码
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

	ListDis.ListDataDis(m_Set, ar, m_List, 0);/*自动调整列宽*/
}


void CTestSpData::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	ListDis.m_Row = pNMListView->iItem;//获得选中的行
	ListDis.m_Col = pNMListView->iSubItem;//获得选中列

	*pResult = 0;
}
