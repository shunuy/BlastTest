#pragma once
#include "afxwin.h"
#include "ADO.h"

// CMyDisCtrl
#define DISPLAY_SERIAL_MODE 70
#define DISPLAY_NO_SERIAL_MODE 0
#define DISPLAY_NOT_AUTO_WIDTH 71

/****************************˫���޸�List�е�����ʾ��*****************************************************
׼��������1������Դ��ͼ���������һ���༭��
          2�����List��˫����Ӧ������
		  3����ӱ༭���EnKillfocus()��Ӧ��Ϣ
ʾ����
void CTestSpData::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
// TODO:  �ڴ���ӿؼ�֪ͨ����������

//ListDis.DisDBClickEdit(pNMHDR, m_List, m_edit);

NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
CRect rc;
int m_Row = pNMListView->iItem;//���ѡ�е���
int m_Col = pNMListView->iSubItem;//���ѡ����

if (pNMListView->iSubItem >-1) //���ѡ���������;
{
	m_List.GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);//��������RECT��
	m_edit.SetParent(&m_List);//ת������Ϊ�б���е�����
	m_edit.MoveWindow(rc);//�ƶ�Edit��RECT���ڵ�λ��;
	m_edit.SetWindowText(m_List.GetItemText(m_Row, m_Col));//���������е�ֵ����Edit�ؼ��У�
	m_edit.ShowWindow(SW_SHOW);//��ʾEdit�ؼ���
	m_edit.SetFocus();//����Edit����
	m_edit.ShowCaret();//��ʾ���
	m_edit.SetSel(-1);//������ƶ������
	ListDis.m_Row = m_Row;
	ListDis.m_Col = m_Col;
}
*pResult = 0;
}

void CTestSpData::OnEnKillfocusEdit2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//ListDis.WriteEditToList(m_List, m_edit);
	CString tem;
	m_edit.GetWindowText(tem);    //�õ��û�������µ�����
	m_List.SetItemText(ListDis.m_Row, ListDis.m_Col, tem);   //���ñ༭���������
	m_edit.ShowWindow(SW_HIDE);                //Ӧ�ر༭��
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
	int m_Row;//��ǰ�༭������
	int m_Col;
public:
	//�����������70���򲻵����п�,����70�Զ�������
	bool ListDataDis(CADODataSet &m_data, CStringArray &Ar,CListCtrl &List,int Param);//���ݿ�������ʾ��List������ʾ
	void InitList(CListCtrl &List);
	void GetField(CString &Tab,CStringArray &FieldAr);
	CString BootOpenDialog(bool IsOpen=TRUE);   //����ѡ����ļ�����
	void WriteListToExcel(CListCtrl & listCtrl,CString  SheetName);
	void ReadExcelToList(CListCtrl & listCtrl,CString SheetName);
	void WriteDatabaseToExcel(LPTSTR Table, LPTSTR SheetName=_T("MySheet"));
	bool DisDBClickEdit(NMHDR *pNMHDR,  CListCtrl &List, CEdit &Edit);//��˫����Ӧ�ĶԻ���
	void WriteEditToList(CListCtrl &List, CEdit &Edit);//���༭���е�����д��list
	void WriteToDataSet(CADODataSet &m_data, CStringArray & FieldAr, CEdit& Edit);//���༭�������д�����ݿ�

};


