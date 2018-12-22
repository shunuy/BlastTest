#pragma once
#include "afxwin.h"
#include "ADO.h"

// CMyDisCtrl
#define DISPLAY_SERIAL_MODE 70
#define DISPLAY_NO_SERIAL_MODE 0
#define DISPLAY_NOT_AUTO_WIDTH 71

/****************************双击修改List中的类容示例*****************************************************
准备工作：1、在资源视图窗口中添加一个编辑框；
          2、添加List的双击响应函数；
		  3、添加编辑框的EnKillfocus()响应消息
示例：
void CTestSpData::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
// TODO:  在此添加控件通知处理程序代码

//ListDis.DisDBClickEdit(pNMHDR, m_List, m_edit);

NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
CRect rc;
int m_Row = pNMListView->iItem;//获得选中的行
int m_Col = pNMListView->iSubItem;//获得选中列

if (pNMListView->iSubItem >-1) //如果选择的是子项;
{
	m_List.GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);//获得子项的RECT；
	m_edit.SetParent(&m_List);//转换坐标为列表框中的坐标
	m_edit.MoveWindow(rc);//移动Edit到RECT坐在的位置;
	m_edit.SetWindowText(m_List.GetItemText(m_Row, m_Col));//将该子项中的值放在Edit控件中；
	m_edit.ShowWindow(SW_SHOW);//显示Edit控件；
	m_edit.SetFocus();//设置Edit焦点
	m_edit.ShowCaret();//显示光标
	m_edit.SetSel(-1);//将光标移动到最后
	ListDis.m_Row = m_Row;
	ListDis.m_Col = m_Col;
}
*pResult = 0;
}

void CTestSpData::OnEnKillfocusEdit2()
{
	// TODO:  在此添加控件通知处理程序代码
	//ListDis.WriteEditToList(m_List, m_edit);
	CString tem;
	m_edit.GetWindowText(tem);    //得到用户输入的新的内容
	m_List.SetItemText(ListDis.m_Row, ListDis.m_Col, tem);   //设置编辑框的新内容
	m_edit.ShowWindow(SW_HIDE);                //应藏编辑框
}
***************************************************************************************************************************/

class CMyDisCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyDisCtrl)

public:
	CMyDisCtrl();
	virtual ~CMyDisCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	int m_Row;//当前编辑的行列
	int m_Col;
public:
	//如果参数大于70，则不调整列宽,等于70自动添加序号
	bool ListDataDis(CADODataSet &m_data, CStringArray &Ar,CListCtrl &List,int Param);//数据库内容显示到List中来显示
	void InitList(CListCtrl &List);
	void GetField(CString &Tab,CStringArray &FieldAr);
	CString BootOpenDialog(bool IsOpen=TRUE);   //返回选择的文件名称
	void WriteListToExcel(CListCtrl & listCtrl,CString  SheetName);
	void ReadExcelToList(CListCtrl & listCtrl,CString SheetName);
	void WriteDatabaseToExcel(LPTSTR Table, LPTSTR SheetName=_T("MySheet"));
	bool DisDBClickEdit(NMHDR *pNMHDR,  CListCtrl &List, CEdit &Edit);//打开双击响应的对话框
	void WriteEditToList(CListCtrl &List, CEdit &Edit);//将编辑框中的内容写入list
	void WriteToDataSet(CADODataSet &m_data, CStringArray & FieldAr, CEdit& Edit);//将编辑框的类容写入数据库

};


