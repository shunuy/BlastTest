// MyDisCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyDisCtrl.h"
#include "ADO.h"

//excelͷ�ļ�
#include "CApplication.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include <fstream>

#define WEIGHT_DIS_LEN 1000


// CMyDisCtrl

IMPLEMENT_DYNAMIC(CMyDisCtrl, CListCtrl)

CMyDisCtrl::CMyDisCtrl()
{
	m_Row = 0;
	m_Col = 0;
}

CMyDisCtrl::~CMyDisCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyDisCtrl, CListCtrl)
END_MESSAGE_MAP()



// ��list�ؼ�����ʾ���ݿ��е�����

void CMyDisCtrl::InitList(CListCtrl &List)
{
	List.SetBkColor(RGB(200, 200, 200)); // ���ñ�����ɫ
	List.SetTextBkColor(RGB(200, 200, 200)); // �����ı�����
	List.SetTextColor(RGB(0, 0, 0)); // �����ı���ɫ
	DWORD dwStyle = List.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |=LVS_SHOWSELALWAYS;  //һֱѡ��item
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	//dwStyle |=LVS_EX_HEADERINALLVIEWS; //Windows Vista. Show column headers in all view modes.
	List.SetExtendedStyle(dwStyle); //������չ���

}
void CMyDisCtrl::GetField(CString &Tab,CStringArray &FieldAr)
{
	CADODataSet m_DataSet;
	m_DataSet.SetConnection(::GetConnection());

	CString cName=_T("SELECT * from ")+Tab;
	m_DataSet.Open(cName);//��ȡȫ���ֶ���
	long i,cou;
	cou=m_DataSet.GetDataSet()->Fields->Count;
	CString str;
	FieldAr.RemoveAll();
	for(i=0;i<cou;i++)
	{
		str=LPCSTR(m_DataSet.GetDataSet()->Fields->GetItem(i)->GetName());
		if(str!=_T("���"))
			FieldAr.Add(str);
	}	

}
bool CMyDisCtrl::DisDBClickEdit(NMHDR *pNMHDR, CListCtrl &List,CEdit &Edit)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;
	m_Row = pNMListView->iItem;//���ѡ�е���
	m_Col = pNMListView->iSubItem;//���ѡ����

	if (pNMListView->iSubItem >-1) //���ѡ���������;
	{
		List.GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);//��������RECT��
		Edit.SetParent(&List);//ת������Ϊ�б���е�����
		Edit.MoveWindow(rc);//�ƶ�Edit��RECT���ڵ�λ��;
		Edit.SetWindowText(List.GetItemText(m_Row, m_Col));//���������е�ֵ����Edit�ؼ��У�
		Edit.ShowWindow(SW_SHOW);//��ʾEdit�ؼ���
		Edit.SetFocus();//����Edit����
		Edit.ShowCaret();//��ʾ���
		Edit.SetSel(-1);//������ƶ������
	
	}
	return true;
}
void CMyDisCtrl::WriteEditToList(CListCtrl &List, CEdit &Edit)
{
	CString tem;
	Edit.GetWindowText(tem);    //�õ��û�������µ�����
	List.SetItemText(m_Row, m_Col, tem);   //���ñ༭���������
	Edit.ShowWindow(SW_HIDE);                //Ӧ�ر༭��
}
void CMyDisCtrl::WriteToDataSet(CADODataSet &m_data, CStringArray & FieldAr, CEdit& Edit)//���༭�������д�����ݿ�
{
	CString Field = FieldAr.GetAt(m_Col);
	CString Value;
	Edit.GetWindowText(Value);    //�õ��û�������µ�����
	m_data.moveFirst();
	m_data.SetFieldValue(Field, m_Row, Value);
}
bool CMyDisCtrl::ListDataDis(CADODataSet &m_Data, CStringArray &Ar,CListCtrl &List,int Param)//�����������70���򲻵����п�,����70�Զ�������
{
	int nCount;
	int FieldCount=0;
	if(!m_Data.IsadoEOF())  //�жϼ�¼����Ƿ�Ϊ��
		m_Data.moveFirst();
	else
	{
		AfxMessageBox(_T("�޿���ʾ����"));
		List.DeleteAllItems(); //�����������
		return FALSE;
	}

	List.DeleteAllItems(); //�����������
	nCount=List.GetHeaderCtrl()->GetItemCount();
	for(int i_add=0;i_add<nCount;i_add++)  //ɾ��������
		List.DeleteColumn(0);

	FieldCount=Ar.GetCount();
	if(FieldCount<=0) return FALSE;  //û�п���ʾ���ֶ�
	for(int i_add=0;i_add<FieldCount;i_add++)
	{
		List.InsertColumn( 0, Ar.GetAt(FieldCount-i_add-1), LVCFMT_LEFT, 200 );//������//LVCFMT_LEFT, LVCFMT_RIGHT, or LVCFMT_CENTER.

	}
	if(Param==DISPLAY_SERIAL_MODE)//��ʾ���
		List.InsertColumn( 0, _T("���"), LVCFMT_CENTER, 200 );//������//LVCFMT_LEFT, LVCFMT_RIGHT, or LVCFMT_CENTER.

	LVCOLUMN  m_name; // �洢����Ϣ�Ľṹ��
	m_name.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for(int i_test=FieldCount;i_test>0;i_test--)//�����Ƿ������ֶξ�����
	{
		try{
			m_Data.ReadValue(Ar.GetAt(FieldCount-i_test));
		}
		catch(...)
		{
			MessageBox(_T("Ŀ�����ݿ��в������ֶΣ�"+Ar.GetAt(FieldCount-i_test)));	
			//i_add++;
		}
	}

	int i=0;
	_variant_t  var;
	int nRow;
	while(!m_Data.IsadoEOF())
	{
		if(Param!=DISPLAY_SERIAL_MODE)//����ʾ���
		{
			try{
				var=m_Data.ReadValue( Ar.GetAt(0));
				if(var.vt!=VT_NULL)
					nRow = List.InsertItem(i+1,(_bstr_t)var);//������,��һ������Ϊ�к�
				else
					nRow= List.InsertItem(i+1,(_bstr_t)"");//������,��һ������Ϊ�к�//nRow++;
			}
			catch(...)
			{
				//MessageBox("�������ֶΣ�"+MyData.m_FieldsArray.GetAt(FieldCount-i_add-1));	
				//i_add++;
				nRow= List.InsertItem(i+1,(_bstr_t)"");
			}
		}
		else //��ʾ���
			nRow = List.InsertItem(i+1,(_bstr_t)(i+1));//������,��һ������Ϊ�к�

		for(int i_add=FieldCount;i_add>0;i_add--)
		{
			try{
				var=m_Data.ReadValue(Ar.GetAt(FieldCount-i_add));
				if(var.vt!=VT_NULL)
				{
					if(Param!=DISPLAY_SERIAL_MODE)//����ʾ���
						List.SetItemText(nRow, FieldCount-i_add,(_bstr_t)var);//�������ݵ�һ������Ϊ�У��ڶ�������Ϊ��
					else //��ʾ���
						List.SetItemText(nRow, FieldCount-i_add+1,(_bstr_t)var);//�������ݵ�һ������Ϊ�У��ڶ�������Ϊ��

				}
			}
			catch(...)
			{
				//MessageBox("�������ֶΣ�"+MyData.m_FieldsArray.GetAt(FieldCount-i_add-1));	
				//i_add++;
			}
		}

		if(m_Data.Next())
			i++;
		else
			break;
	}

	//listCtrl.SetColumnWidth(0,40);
	//�Զ������п�
	//ShowWindow(SW_HIDE);//������˸
	List.ShowWindow(SW_HIDE);
	for(int i_add=0;i_add<FieldCount;i_add++)
	{
		List.SetColumnWidth(i_add,LVSCW_AUTOSIZE);//�Զ������п�
		if(Param>=DISPLAY_NOT_AUTO_WIDTH)//�����������70���򲻵����п�
		{
			List.SetColumnWidth(i_add,Param);//
			continue;
		}
		if(List.GetColumnWidth(i_add)>300)
		{
			List.SetColumnWidth(i_add,300);//
		}
		if(List.GetColumnWidth(i_add)<70)
		{
			List.SetColumnWidth(i_add,70);//
		}
	}
	List.ShowWindow(SW_SHOW);//������˸


	return TRUE;
}

CString CMyDisCtrl::BootOpenDialog(bool IsOpen)   //����ѡ����ļ�����
{
	CString strFile = _T("");

	CFileDialog   dlgFile(IsOpen, NULL, NULL, OFN_HIDEREADONLY, _T("Excel 2010 Files (*.xlsx)|*.xlsx|All Files (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal()==IDOK)
	{
		strFile = dlgFile.GetPathName();
	}

	return strFile;
}
void CMyDisCtrl::WriteDatabaseToExcel(LPTSTR Table,LPTSTR SheetName)
{
	CApplication app;
	CWorkbooks books;
	CWorkbook book;
	CWorksheets sheets;
	CWorksheet sheet;
	CRange range;
	CRange cols;
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	bool IsFileExist;

	if (!app.CreateDispatch(_T("Excel.Application")))
	{
		MessageBox(_T("����Excel����ʧ��...."), _T("��Ϣ��ʾ"), MB_OK);
		return;
	}

	//books=app.get_Workbooks();
	CString selstrpath = BootOpenDialog(FALSE);//����ѡ���ļ��Ի���
	CString strpath;
	if (selstrpath == "")
	{
		strpath = _T("D:\\1.xlsx");
		range.ReleaseDispatch();
		sheet.ReleaseDispatch();
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();
		books.ReleaseDispatch();
		app.Quit();
		app.ReleaseDispatch();

		return;
	}
	else
		strpath = selstrpath;

	//strpath=_T("E:\\5.xlsx");
	LPDISPATCH lpDis = NULL;
	books.AttachDispatch(app.get_Workbooks());/*�õ�����������*/
	try{
		lpDis = books.Open(strpath, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);

		//book=books.Add(covOptional);
		book.AttachDispatch(lpDis);
		IsFileExist = TRUE;
	}
	catch (...){
		/*�޷��򿪣�������һ���µĹ�����*/
		lpDis = books.Add(vtMissing);
		book.AttachDispatch(lpDis);
		IsFileExist = FALSE;

	}
	//sheets=book.get_Sheets();
	//sheet.AttachDispatch(book.get_Worksheets());
	//sheet=sheets.get_Item(_variant_t((short)1));
	/*�õ��������е�Sheet������*/
	sheets.AttachDispatch(book.get_Sheets());
	/*��һ��Sheet���粻���ڣ�������һ��Sheet*/
	CString strSheetName = SheetName;// _T("ExportSheet");
	try
	{
		/*��һ�����е�Sheet*/
		lpDis = sheets.get_Item(_variant_t(strSheetName));
		sheet.AttachDispatch(lpDis);
	}
	catch (...)
	{
		/*����һ���µ�Sheet*/
		lpDis = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
		sheet.AttachDispatch(lpDis);
		sheet.put_Name(strSheetName);
	}

	//system("pause");

	//���ListCtrl
	//CListCtrl& listCtrl =m_List;	
	CStringArray SHead;
	CString ReadTable;
	ReadTable = Table;
	GetField(ReadTable, SHead);

	CADODataSet ReadData;
	ReadData.SetConnection(::GetConnection());
	ReadData.Open(_T("select * from ") + ReadTable);
	int total_row;
	total_row = ReadData.GetRecordCount();//�õ�������
	if (total_row <= 0)
		MessageBox(_T("�޿ɵ���������"));
	else
	{
		CString strrow;
		//char Cstrrow[20];
		//itoa(total_row,Cstrrow,10);
		strrow.Format(_T("Z%d"), total_row);
		//strrow=_T("Z")+(CString)Cstrrow;
		range = sheet.get_Range(_variant_t(_T("A1")), _variant_t(strrow));
		long colCount = 0;
		int TColCount = SHead.GetCount();
		for (colCount = 0; colCount<TColCount; colCount++)//�õ��������б���
		{
			range.put_Item(COleVariant((long)1), COleVariant((long)(colCount + 1)), COleVariant(SHead.GetAt(colCount)));
		}
		variant_t RValue;
		for (int i = 0; i<total_row; i++)//��
		{
			for (int j = 0; j<colCount; j++)//��
			{
				RValue=ReadData.ReadValue(SHead.GetAt(j));
				range.put_Item(COleVariant((long)(i + 2)), COleVariant((long)(j + 1)), COleVariant(RValue));
			}
			ReadData.Next();

		}
		cols = range.get_EntireColumn();
		cols.AutoFit();

		//app.put_Visible(TRUE); //��Ӹ��������Զ�����Excel���
		app.put_UserControl(TRUE);

		//���Ϊexcel���ļ�֮ǰ�Ѿ����ڣ���save,��������saveas
		if (FALSE == IsFileExist)
			book.SaveAs(COleVariant(strpath), covOptional,
			covOptional, covOptional,
			covOptional, covOptional, (long)1,
			covOptional, covOptional,
			covOptional, covOptional,
			covOptional);
		else
			book.Save();//
		app.put_DisplayAlerts(false); //���ص����ĶԻ���,�����ɾ������ᵯ������Ի���
	}

	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app.Quit();
	app.ReleaseDispatch();
}
void CMyDisCtrl::WriteListToExcel(CListCtrl & listCtrl,CString SheetName)
{
	CApplication app;
	CWorkbooks books;
	CWorkbook book;
	CWorksheets sheets;
	CWorksheet sheet;
	CRange range;
	CRange cols;
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	bool IsFileExist;

	if(!app.CreateDispatch(_T("Excel.Application")))
	{
		MessageBox(_T("����Excel����ʧ��...."),_T("��Ϣ��ʾ"), MB_OK);
		return;
	}

	//books=app.get_Workbooks();
	CString selstrpath=BootOpenDialog();//����ѡ���ļ��Ի���
	CString strpath;
	if(selstrpath=="")
	{
		strpath=_T("D:\\1.xlsx");
		range.ReleaseDispatch();    
		sheet.ReleaseDispatch();    
		sheets.ReleaseDispatch();    
		book.ReleaseDispatch();    
		books.ReleaseDispatch();    
		app.Quit();
		app.ReleaseDispatch();

		return ;
	}
	else
		strpath=selstrpath;
	
	//strpath=_T("E:\\5.xlsx");
	LPDISPATCH lpDis = NULL;
	books.AttachDispatch(app.get_Workbooks());/*�õ�����������*/
	try{
		lpDis=books.Open(strpath,covOptional,covOptional,covOptional,covOptional,
			covOptional,covOptional,covOptional,covOptional,
			covOptional,covOptional,covOptional,covOptional,covOptional,covOptional);

		//book=books.Add(covOptional);
		book.AttachDispatch(lpDis);
		IsFileExist=TRUE;
	}
	catch(...){ 
		/*�޷��򿪣�������һ���µĹ�����*/
		lpDis = books.Add(vtMissing);
			book.AttachDispatch(lpDis);
		IsFileExist=FALSE;

	}
	//sheets=book.get_Sheets();
	//sheet.AttachDispatch(book.get_Worksheets());
	//sheet=sheets.get_Item(_variant_t((short)1));
	/*�õ��������е�Sheet������*/
	sheets.AttachDispatch(book.get_Sheets());
	/*��һ��Sheet���粻���ڣ�������һ��Sheet*/
	CString strSheetName = SheetName;// _T("ExportSheet");
		try
		{
			/*��һ�����е�Sheet*/
			lpDis = sheets.get_Item(_variant_t(strSheetName));
			sheet.AttachDispatch(lpDis);
		}
		catch(...)
		{
			/*����һ���µ�Sheet*/
			lpDis = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
			sheet.AttachDispatch(lpDis);
			sheet.put_Name(strSheetName);
		}

		//system("pause");

	//���ListCtrl
	//CListCtrl& listCtrl =m_List;	
	LVCOLUMN lvcol;
	char str[40];
	CString SHead;
	CString CellText;
	int total_row;
	total_row=listCtrl.GetItemCount();//�õ�������
	if(total_row<=0)
		MessageBox(_T("�޿ɵ���������"));
	else
	{
		CString strrow;
		//char Cstrrow[20];
		//itoa(total_row,Cstrrow,10);
		strrow.Format(_T("Z%d"),total_row);
		//strrow=_T("Z")+(CString)Cstrrow;
		range=sheet.get_Range(_variant_t(_T("A1")),_variant_t(strrow));
		long colCount=0;
		for(colCount=0;colCount<100;colCount++)//�õ��������б���
		{
			//str[0]='\0';
			memset(str,0,40);
			lvcol.mask = LVCF_TEXT; 
			lvcol.pszText = LPSTR(str); 
			lvcol.cchTextMax = 256; 
			listCtrl.GetColumn(colCount, &lvcol);
			SHead=lvcol.pszText;
			if(SHead==_T(""))
				break;
			//range.AttachDispatch(range.get_Item(COleVariant((long)1),COleVariant((long)(colCount+1))).pdispVal );   /*COleVariant*/ 
			//range.put_Value2(COleVariant(SHead)); 
			range.put_Item(COleVariant((long)1),COleVariant((long)(colCount+1)),COleVariant(SHead));
		}
		for(int i=0;i<total_row;i++)//��
		{
			for(int j=0;j<colCount;j++)//��
			{
				CellText=listCtrl.GetItemText(i,j);
				range.put_Item(COleVariant((long)(i+2)),COleVariant((long)(j+1)),COleVariant(CellText));

			}

		}
		cols=range.get_EntireColumn();
		cols.AutoFit();

		app.put_Visible(TRUE);
		app.put_UserControl(TRUE);

		//���Ϊexcel���ļ�֮ǰ�Ѿ����ڣ���save,��������saveas
		if(FALSE==IsFileExist)
			book.SaveAs(COleVariant(strpath),covOptional,
			covOptional,covOptional,
			covOptional,covOptional,(long)1,
			covOptional,covOptional,
			covOptional,covOptional,
			covOptional); 
		else
			book.Save();//
		app.put_DisplayAlerts(false); //���ص����ĶԻ���,�����ɾ������ᵯ������Ի���
	}
	//COleVariant vResult;
	//range.AttachDispatch(sheet.get_Cells());//rangeΪ�������ݱ�Ԫ�صļ���
	/*range=sheet.get_Range(_variant_t(_T("A1")),_variant_t(_T("A1")));
	//range.put_Value2(_variant_t("HELLO EXCEL!"));
	range.pu t_Value2(COleVariant((float)14.234));
	range.put_NumberFormat(_variant_t("0.0000"));//�趨С����λ����
	range=sheet.get_Range(_variant_t("A2"),_variant_t("A2"));
	range.put_Formula(_variant_t("=RAND()*100000"));
	range.put_NumberFormat(_variant_t("$0.00"));*/

	//�������������䲻ɾ������Excel�򿪺�������Զ��ر�
	//books.Close();     
	//app.Quit();             // �˳�    

	//�ͷŶ���      
	//iCell.ReleaseDispatch(); 

	//���������excel�����Ԫ����д�����ݡ�
	/*
	//��Sheet��д������Ԫ��,��ģΪ10*10 
	lpDisp = sheet.get_Range(_variant_t("A1"), _variant_t("J10"));
	range.AttachDispatch(lpDisp);

	VARTYPE vt = VT_I4; //����Ԫ�ص����ͣ�long
	SAFEARRAYBOUND sabWrite[2]; //���ڶ��������ά�����±����ʼֵ
	sabWrite[0].cElements = 10;
	sabWrite[0].lLbound = 0;
	sabWrite[1].cElements = 10;
	sabWrite[1].lLbound = 0;

	COleSafeArray olesaWrite;
	olesaWrite.Create(vt, sizeof(sabWrite)/sizeof(SAFEARRAYBOUND), sabWrite);

	//ͨ��ָ�������ָ�����Զ�ά�����Ԫ�ؽ��м�Ӹ�ֵ
	long (*pArray)[2] = NULL;
	olesaWrite.AccessData((void **)&pArray);
	memset(pArray, 0, sabWrite[0].cElements * sabWrite[1].cElements * sizeof(long));

	//�ͷ�ָ�������ָ��
	olesaWrite.UnaccessData();
	pArray = NULL;

	//�Զ�ά�����Ԫ�ؽ��������ֵ
	long index[2] = {0, 0};
	long lFirstLBound = 0;
	long lFirstUBound = 0;
	long lSecondLBound = 0;
	long lSecondUBound = 0;
	olesaWrite.GetLBound(1, &lFirstLBound);
	olesaWrite.GetUBound(1, &lFirstUBound);
	olesaWrite.GetLBound(2, &lSecondLBound);
	olesaWrite.GetUBound(2, &lSecondUBound);
	for (long i = lFirstLBound; i <= lFirstUBound; i++)
	{
		index[0] = i;
		for (long j = lSecondLBound; j <= lSecondUBound; j++)
		{
			index[1] = j;
			long lElement = i * sabWrite[1].cElements + j; 
			olesaWrite.PutElement(index, &lElement);
		}
	}

	//��ColesaWritefeArray����ת��ΪVARIANT,��д�뵽Excel�����
	VARIANT varWrite = (VARIANT)olesaWrite;
	range.put_Value2(varWrite);

	system("pause");
	*/

	//����������ڶ�ȡexcel���е�Ԫ�������
	/*range=sheet.get_UsedRange();
	VARIANT varRead = range.get_Value2();
	COleSafeArray olesaRead(varRead);
	VARIANT varItem;
	CString strItem;
	long index[2] = {0, 0};
	long lFirstLBound = 0;
	long lFirstUBound = 0;
	long lSecondLBound = 0;
	long lSecondUBound = 0;
	olesaRead.GetLBound(1, &lFirstLBound);
	olesaRead.GetUBound(1, &lFirstUBound);
	olesaRead.GetLBound(2, &lSecondLBound);
	olesaRead.GetUBound(2, &lSecondUBound);
	 memset(index, 0, 2 * sizeof(long));
	 //
	 for (long i = lFirstLBound; i <= lFirstUBound; i++)
	 {
		 CString strRowName = _T("");
		 strRowName.Format(_T("%d"), i);
		 //m_ListCtrl.InsertItem(i-1, strRowName);

		 index[0] = i;
		 for (long j = lSecondLBound; j <= lSecondUBound; j++)
		 {
			 index[1] = j;
			 olesaRead.GetElement(index, &varItem);

			 switch (varItem.vt)
			 {
			 case VT_R8:
				 {
					 strItem.Format(_T("%d"), (int)varItem.dblVal);
					 break;
				 }

			 case VT_BSTR:
				 {
					 strItem = varItem.bstrVal;
					 break;
				 }

			 case VT_I4:
				 {
					 strItem.Format(_T("%ld"), (int)varItem.lVal);
					 break;
				 }

			 default:
				 {
					break;
				 }
			 }
			 //m_ListCtrl.SetItemText(i-1, j, strItem);
		 }
	 }*/

	range.ReleaseDispatch();    
	sheet.ReleaseDispatch();    
	sheets.ReleaseDispatch();    
	book.ReleaseDispatch();    
	books.ReleaseDispatch();    
	app.Quit();
	app.ReleaseDispatch();
}
//******************************************************************************
//
//******************************************************************************
void CMyDisCtrl::ReadExcelToList(CListCtrl & listCtrl,CString SheetName)
{
	CApplication app;
	CWorkbooks books;
	CWorkbook book;
	CWorksheets sheets;
	CWorksheet sheet;
	CRange range;
	CRange cols;
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	bool IsFileExist;

	if(!app.CreateDispatch(_T("Excel.Application")))
	{
		MessageBox(_T("����Excel����ʧ��...."),_T("��Ϣ��ʾ"), MB_OK);
		return;
	}

	//books=app.get_Workbooks();
	CString selstrpath=BootOpenDialog();//����ѡ���ļ��Ի���
	CString strpath;
	if(selstrpath=="")
	{
		strpath=_T("D:\\1.xlsx");
		range.ReleaseDispatch();    
		sheet.ReleaseDispatch();    
		sheets.ReleaseDispatch();    
		book.ReleaseDispatch();    
		books.ReleaseDispatch();    
		app.Quit();
		app.ReleaseDispatch();

		return ;
	}
	else
		strpath=selstrpath;
	
	//strpath=_T("E:\\5.xlsx");
	LPDISPATCH lpDis = NULL;
	books.AttachDispatch(app.get_Workbooks());/*�õ�����������*/
	try{
		lpDis=books.Open(strpath,covOptional,covOptional,covOptional,covOptional,
			covOptional,covOptional,covOptional,covOptional,
			covOptional,covOptional,covOptional,covOptional,covOptional,covOptional);

		//book=books.Add(covOptional);
		book.AttachDispatch(lpDis);
		IsFileExist=TRUE;
	}
	catch(...){ 
		/*�޷��򿪣�������һ���µĹ�����*/
		lpDis = books.Add(vtMissing);
			book.AttachDispatch(lpDis);
		IsFileExist=FALSE;

	}
	//sheets=book.get_Sheets();
	//sheet.AttachDispatch(book.get_Worksheets());
	//sheet=sheets.get_Item(_variant_t((short)1));
	/*�õ��������е�Sheet������*/
	sheets.AttachDispatch(book.get_Sheets());
	/*��һ��Sheet���粻���ڣ�������һ��Sheet*/
	CString strSheetName = SheetName;// _T("ExportSheet");
	try
	{
		/*��һ�����е�Sheet*/
		lpDis = sheets.get_Item(_variant_t(strSheetName));
		sheet.AttachDispatch(lpDis);
	}
	catch(...)
	{
		/*����һ���µ�Sheet*/
		//lpDis = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
		//sheet.AttachDispatch(lpDis);
		//sheet.put_Name(strSheetName);
		MessageBox(_T("�����ڱ��")+strSheetName);
		range.ReleaseDispatch();    
		sheet.ReleaseDispatch();    
		sheets.ReleaseDispatch();    
		book.ReleaseDispatch();    
		books.ReleaseDispatch();    
		app.Quit();
		app.ReleaseDispatch();
		return;
	}

	//system("pause");

	//����������ڶ�ȡexcel���е�Ԫ�������
	range=sheet.get_UsedRange();
	VARIANT varRead = range.get_Value2();
	COleSafeArray olesaRead(varRead);
	VARIANT varItem;
	CString strItem;
	long index[2] = {0, 0};
	long lFirstLBound = 0;
	long lFirstUBound = 0;
	long lSecondLBound = 0;
	long lSecondUBound = 0;
	olesaRead.GetLBound(1, &lFirstLBound);
	olesaRead.GetUBound(1, &lFirstUBound);
	olesaRead.GetLBound(2, &lSecondLBound);
	olesaRead.GetUBound(2, &lSecondUBound);
	 memset(index, 0, 2 * sizeof(long));
	 //
	
	 listCtrl.InsertColumn(0, _T("���"), LVCFMT_LEFT, 100 );//������//LVCFMT_LEFT, LVCFMT_RIGHT, or LVCFMT_CENTER.
	 for (long i = lFirstLBound; i <= lFirstUBound; i++)
	 {
		 index[0] = i;

		 CString strRowName = _T("");
		 strRowName.Format(_T("%d"), i-1);
		 if((1!=i) & (lFirstUBound-i<WEIGHT_DIS_LEN))//��ʾ�����1000��
			 listCtrl.InsertItem(0, strRowName);

		 for (long j = lSecondLBound; j <= lSecondUBound; j++)
		 {
			 index[1] = j;
			 olesaRead.GetElement(index, &varItem);

			 switch (varItem.vt)
			 {
			 case VT_R8:
				 {
					 strItem.Format(_T("%d"), (int)varItem.dblVal);
					/* //��ȡ������
					 long index_time[2] = {0, 0};
					 index_time[0]=1;
					 index_time[1]=j;
					 VARIANT varItem_time;
					 CString strtemp;
					 olesaRead.GetElement(index_time, &varItem_time);//��ȡ������
					 strtemp=varItem_time.bstrVal;
					 if(_T("����")==strtemp)//����
					 {
						 COleDateTime  ti(varItem);
						 strItem=ti.Format(_T("%Y-%m-%d"));
					 }*/

					 break;
				 }

			 case VT_BSTR:
				 {
					 strItem = varItem.bstrVal;
					 break;
				 }

			 case VT_I4:
				 {
					 strItem.Format(_T("%ld"), (int)varItem.lVal);
					 break;
				 }

			 default:
				 {
					break;
				 }
			 }
			 if(i==1)//��һ��Ϊ����
				 listCtrl.InsertColumn( j, strItem, LVCFMT_LEFT, 100 );//������//LVCFMT_LEFT, LVCFMT_RIGHT, or LVCFMT_CENTER.
			 else if((lFirstUBound-i<WEIGHT_DIS_LEN))
			 {
				 listCtrl.SetItemText(0, j, strItem);
			 }
		 }
	 }

	range.ReleaseDispatch();    
	sheet.ReleaseDispatch();    
	sheets.ReleaseDispatch();    
	book.ReleaseDispatch();    
	books.ReleaseDispatch();    
	app.Quit();
	app.ReleaseDispatch();
}

