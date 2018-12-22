// MyDisCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "MyDisCtrl.h"
#include "ADO.h"

//excel头文件
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



// 在list控件中显示数据库中的内容

void CMyDisCtrl::InitList(CListCtrl &List)
{
	List.SetBkColor(RGB(200, 200, 200)); // 设置背景颜色
	List.SetTextBkColor(RGB(200, 200, 200)); // 设置文本背景
	List.SetTextColor(RGB(0, 0, 0)); // 设置文本颜色
	DWORD dwStyle = List.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |=LVS_SHOWSELALWAYS;  //一直选中item
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	//dwStyle |=LVS_EX_HEADERINALLVIEWS; //Windows Vista. Show column headers in all view modes.
	List.SetExtendedStyle(dwStyle); //设置扩展风格

}
void CMyDisCtrl::GetField(CString &Tab,CStringArray &FieldAr)
{
	CADODataSet m_DataSet;
	m_DataSet.SetConnection(::GetConnection());

	CString cName=_T("SELECT * from ")+Tab;
	m_DataSet.Open(cName);//读取全部字段名
	long i,cou;
	cou=m_DataSet.GetDataSet()->Fields->Count;
	CString str;
	FieldAr.RemoveAll();
	for(i=0;i<cou;i++)
	{
		str=LPCSTR(m_DataSet.GetDataSet()->Fields->GetItem(i)->GetName());
		if(str!=_T("序号"))
			FieldAr.Add(str);
	}	

}
bool CMyDisCtrl::DisDBClickEdit(NMHDR *pNMHDR, CListCtrl &List,CEdit &Edit)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;
	m_Row = pNMListView->iItem;//获得选中的行
	m_Col = pNMListView->iSubItem;//获得选中列

	if (pNMListView->iSubItem >-1) //如果选择的是子项;
	{
		List.GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);//获得子项的RECT；
		Edit.SetParent(&List);//转换坐标为列表框中的坐标
		Edit.MoveWindow(rc);//移动Edit到RECT坐在的位置;
		Edit.SetWindowText(List.GetItemText(m_Row, m_Col));//将该子项中的值放在Edit控件中；
		Edit.ShowWindow(SW_SHOW);//显示Edit控件；
		Edit.SetFocus();//设置Edit焦点
		Edit.ShowCaret();//显示光标
		Edit.SetSel(-1);//将光标移动到最后
	
	}
	return true;
}
void CMyDisCtrl::WriteEditToList(CListCtrl &List, CEdit &Edit)
{
	CString tem;
	Edit.GetWindowText(tem);    //得到用户输入的新的内容
	List.SetItemText(m_Row, m_Col, tem);   //设置编辑框的新内容
	Edit.ShowWindow(SW_HIDE);                //应藏编辑框
}
void CMyDisCtrl::WriteToDataSet(CADODataSet &m_data, CStringArray & FieldAr, CEdit& Edit)//将编辑框的类容写入数据库
{
	CString Field = FieldAr.GetAt(m_Col);
	CString Value;
	Edit.GetWindowText(Value);    //得到用户输入的新的内容
	m_data.moveFirst();
	m_data.SetFieldValue(Field, m_Row, Value);
}
bool CMyDisCtrl::ListDataDis(CADODataSet &m_Data, CStringArray &Ar,CListCtrl &List,int Param)//如果参数大于70，则不调整列宽,等于70自动添加序号
{
	int nCount;
	int FieldCount=0;
	if(!m_Data.IsadoEOF())  //判断记录结果是否为空
		m_Data.moveFirst();
	else
	{
		AfxMessageBox(_T("无可显示内容"));
		List.DeleteAllItems(); //清空所有类容
		return FALSE;
	}

	List.DeleteAllItems(); //清空所有类容
	nCount=List.GetHeaderCtrl()->GetItemCount();
	for(int i_add=0;i_add<nCount;i_add++)  //删除所有列
		List.DeleteColumn(0);

	FieldCount=Ar.GetCount();
	if(FieldCount<=0) return FALSE;  //没有可显示的字段
	for(int i_add=0;i_add<FieldCount;i_add++)
	{
		List.InsertColumn( 0, Ar.GetAt(FieldCount-i_add-1), LVCFMT_LEFT, 200 );//插入列//LVCFMT_LEFT, LVCFMT_RIGHT, or LVCFMT_CENTER.

	}
	if(Param==DISPLAY_SERIAL_MODE)//显示序号
		List.InsertColumn( 0, _T("序号"), LVCFMT_CENTER, 200 );//插入列//LVCFMT_LEFT, LVCFMT_RIGHT, or LVCFMT_CENTER.

	LVCOLUMN  m_name; // 存储列信息的结构体
	m_name.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for(int i_test=FieldCount;i_test>0;i_test--)//测试是否所有字段均存在
	{
		try{
			m_Data.ReadValue(Ar.GetAt(FieldCount-i_test));
		}
		catch(...)
		{
			MessageBox(_T("目标数据库中不存在字段："+Ar.GetAt(FieldCount-i_test)));	
			//i_add++;
		}
	}

	int i=0;
	_variant_t  var;
	int nRow;
	while(!m_Data.IsadoEOF())
	{
		if(Param!=DISPLAY_SERIAL_MODE)//不显示序号
		{
			try{
				var=m_Data.ReadValue( Ar.GetAt(0));
				if(var.vt!=VT_NULL)
					nRow = List.InsertItem(i+1,(_bstr_t)var);//插入行,第一个参数为行号
				else
					nRow= List.InsertItem(i+1,(_bstr_t)"");//插入行,第一个参数为行号//nRow++;
			}
			catch(...)
			{
				//MessageBox("不存在字段："+MyData.m_FieldsArray.GetAt(FieldCount-i_add-1));	
				//i_add++;
				nRow= List.InsertItem(i+1,(_bstr_t)"");
			}
		}
		else //显示序号
			nRow = List.InsertItem(i+1,(_bstr_t)(i+1));//插入行,第一个参数为行号

		for(int i_add=FieldCount;i_add>0;i_add--)
		{
			try{
				var=m_Data.ReadValue(Ar.GetAt(FieldCount-i_add));
				if(var.vt!=VT_NULL)
				{
					if(Param!=DISPLAY_SERIAL_MODE)//不显示序号
						List.SetItemText(nRow, FieldCount-i_add,(_bstr_t)var);//设置数据第一个参数为行，第二个参数为列
					else //显示序号
						List.SetItemText(nRow, FieldCount-i_add+1,(_bstr_t)var);//设置数据第一个参数为行，第二个参数为列

				}
			}
			catch(...)
			{
				//MessageBox("不存在字段："+MyData.m_FieldsArray.GetAt(FieldCount-i_add-1));	
				//i_add++;
			}
		}

		if(m_Data.Next())
			i++;
		else
			break;
	}

	//listCtrl.SetColumnWidth(0,40);
	//自动调整列宽
	//ShowWindow(SW_HIDE);//避免闪烁
	List.ShowWindow(SW_HIDE);
	for(int i_add=0;i_add<FieldCount;i_add++)
	{
		List.SetColumnWidth(i_add,LVSCW_AUTOSIZE);//自动调整列宽
		if(Param>=DISPLAY_NOT_AUTO_WIDTH)//如果参数大于70，则不调整列宽
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
	List.ShowWindow(SW_SHOW);//避免闪烁


	return TRUE;
}

CString CMyDisCtrl::BootOpenDialog(bool IsOpen)   //返回选择的文件名称
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
		MessageBox(_T("创建Excel服务失败...."), _T("信息提示"), MB_OK);
		return;
	}

	//books=app.get_Workbooks();
	CString selstrpath = BootOpenDialog(FALSE);//弹出选择文件对话框
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
	books.AttachDispatch(app.get_Workbooks());/*得到工作簿容器*/
	try{
		lpDis = books.Open(strpath, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);

		//book=books.Add(covOptional);
		book.AttachDispatch(lpDis);
		IsFileExist = TRUE;
	}
	catch (...){
		/*无法打开，则增加一个新的工作簿*/
		lpDis = books.Add(vtMissing);
		book.AttachDispatch(lpDis);
		IsFileExist = FALSE;

	}
	//sheets=book.get_Sheets();
	//sheet.AttachDispatch(book.get_Worksheets());
	//sheet=sheets.get_Item(_variant_t((short)1));
	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Sheets());
	/*打开一个Sheet，如不存在，就新增一个Sheet*/
	CString strSheetName = SheetName;// _T("ExportSheet");
	try
	{
		/*打开一个已有的Sheet*/
		lpDis = sheets.get_Item(_variant_t(strSheetName));
		sheet.AttachDispatch(lpDis);
	}
	catch (...)
	{
		/*创建一个新的Sheet*/
		lpDis = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
		sheet.AttachDispatch(lpDis);
		sheet.put_Name(strSheetName);
	}

	//system("pause");

	//获得ListCtrl
	//CListCtrl& listCtrl =m_List;	
	CStringArray SHead;
	CString ReadTable;
	ReadTable = Table;
	GetField(ReadTable, SHead);

	CADODataSet ReadData;
	ReadData.SetConnection(::GetConnection());
	ReadData.Open(_T("select * from ") + ReadTable);
	int total_row;
	total_row = ReadData.GetRecordCount();//得到总行数
	if (total_row <= 0)
		MessageBox(_T("无可导出的内容"));
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
		for (colCount = 0; colCount<TColCount; colCount++)//得到列数和列标题
		{
			range.put_Item(COleVariant((long)1), COleVariant((long)(colCount + 1)), COleVariant(SHead.GetAt(colCount)));
		}
		variant_t RValue;
		for (int i = 0; i<total_row; i++)//行
		{
			for (int j = 0; j<colCount; j++)//列
			{
				RValue=ReadData.ReadValue(SHead.GetAt(j));
				range.put_Item(COleVariant((long)(i + 2)), COleVariant((long)(j + 1)), COleVariant(RValue));
			}
			ReadData.Next();

		}
		cols = range.get_EntireColumn();
		cols.AutoFit();

		//app.put_Visible(TRUE); //添加该语句则会自动弹出Excel表格
		app.put_UserControl(TRUE);

		//另存为excel表，文件之前已经存在，用save,不存在用saveas
		if (FALSE == IsFileExist)
			book.SaveAs(COleVariant(strpath), covOptional,
			covOptional, covOptional,
			covOptional, covOptional, (long)1,
			covOptional, covOptional,
			covOptional, covOptional,
			covOptional);
		else
			book.Save();//
		app.put_DisplayAlerts(false); //隐藏弹出的对话框,如果不删除，则会弹出保存对话框
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
		MessageBox(_T("创建Excel服务失败...."),_T("信息提示"), MB_OK);
		return;
	}

	//books=app.get_Workbooks();
	CString selstrpath=BootOpenDialog();//弹出选择文件对话框
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
	books.AttachDispatch(app.get_Workbooks());/*得到工作簿容器*/
	try{
		lpDis=books.Open(strpath,covOptional,covOptional,covOptional,covOptional,
			covOptional,covOptional,covOptional,covOptional,
			covOptional,covOptional,covOptional,covOptional,covOptional,covOptional);

		//book=books.Add(covOptional);
		book.AttachDispatch(lpDis);
		IsFileExist=TRUE;
	}
	catch(...){ 
		/*无法打开，则增加一个新的工作簿*/
		lpDis = books.Add(vtMissing);
			book.AttachDispatch(lpDis);
		IsFileExist=FALSE;

	}
	//sheets=book.get_Sheets();
	//sheet.AttachDispatch(book.get_Worksheets());
	//sheet=sheets.get_Item(_variant_t((short)1));
	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Sheets());
	/*打开一个Sheet，如不存在，就新增一个Sheet*/
	CString strSheetName = SheetName;// _T("ExportSheet");
		try
		{
			/*打开一个已有的Sheet*/
			lpDis = sheets.get_Item(_variant_t(strSheetName));
			sheet.AttachDispatch(lpDis);
		}
		catch(...)
		{
			/*创建一个新的Sheet*/
			lpDis = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
			sheet.AttachDispatch(lpDis);
			sheet.put_Name(strSheetName);
		}

		//system("pause");

	//获得ListCtrl
	//CListCtrl& listCtrl =m_List;	
	LVCOLUMN lvcol;
	char str[40];
	CString SHead;
	CString CellText;
	int total_row;
	total_row=listCtrl.GetItemCount();//得到总行数
	if(total_row<=0)
		MessageBox(_T("无可导出的内容"));
	else
	{
		CString strrow;
		//char Cstrrow[20];
		//itoa(total_row,Cstrrow,10);
		strrow.Format(_T("Z%d"),total_row);
		//strrow=_T("Z")+(CString)Cstrrow;
		range=sheet.get_Range(_variant_t(_T("A1")),_variant_t(strrow));
		long colCount=0;
		for(colCount=0;colCount<100;colCount++)//得到列数和列标题
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
		for(int i=0;i<total_row;i++)//行
		{
			for(int j=0;j<colCount;j++)//列
			{
				CellText=listCtrl.GetItemText(i,j);
				range.put_Item(COleVariant((long)(i+2)),COleVariant((long)(j+1)),COleVariant(CellText));

			}

		}
		cols=range.get_EntireColumn();
		cols.AutoFit();

		app.put_Visible(TRUE);
		app.put_UserControl(TRUE);

		//另存为excel表，文件之前已经存在，用save,不存在用saveas
		if(FALSE==IsFileExist)
			book.SaveAs(COleVariant(strpath),covOptional,
			covOptional,covOptional,
			covOptional,covOptional,(long)1,
			covOptional,covOptional,
			covOptional,covOptional,
			covOptional); 
		else
			book.Save();//
		app.put_DisplayAlerts(false); //隐藏弹出的对话框,如果不删除，则会弹出保存对话框
	}
	//COleVariant vResult;
	//range.AttachDispatch(sheet.get_Cells());//range为整个数据表元素的集合
	/*range=sheet.get_Range(_variant_t(_T("A1")),_variant_t(_T("A1")));
	//range.put_Value2(_variant_t("HELLO EXCEL!"));
	range.pu t_Value2(COleVariant((float)14.234));
	range.put_NumberFormat(_variant_t("0.0000"));//设定小数点位数。
	range=sheet.get_Range(_variant_t("A2"),_variant_t("A2"));
	range.put_Formula(_variant_t("=RAND()*100000"));
	range.put_NumberFormat(_variant_t("$0.00"));*/

	//如果以下两行语句不删除，则Excel打开后会马上自动关闭
	//books.Close();     
	//app.Quit();             // 退出    

	//释放对象      
	//iCell.ReleaseDispatch(); 

	//以下语句向excel多个单元格中写入数据。
	/*
	//向Sheet中写入多个单元格,规模为10*10 
	lpDisp = sheet.get_Range(_variant_t("A1"), _variant_t("J10"));
	range.AttachDispatch(lpDisp);

	VARTYPE vt = VT_I4; //数组元素的类型，long
	SAFEARRAYBOUND sabWrite[2]; //用于定义数组的维数和下标的起始值
	sabWrite[0].cElements = 10;
	sabWrite[0].lLbound = 0;
	sabWrite[1].cElements = 10;
	sabWrite[1].lLbound = 0;

	COleSafeArray olesaWrite;
	olesaWrite.Create(vt, sizeof(sabWrite)/sizeof(SAFEARRAYBOUND), sabWrite);

	//通过指向数组的指针来对二维数组的元素进行间接赋值
	long (*pArray)[2] = NULL;
	olesaWrite.AccessData((void **)&pArray);
	memset(pArray, 0, sabWrite[0].cElements * sabWrite[1].cElements * sizeof(long));

	//释放指向数组的指针
	olesaWrite.UnaccessData();
	pArray = NULL;

	//对二维数组的元素进行逐个赋值
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

	//把ColesaWritefeArray变量转换为VARIANT,并写入到Excel表格中
	VARIANT varWrite = (VARIANT)olesaWrite;
	range.put_Value2(varWrite);

	system("pause");
	*/

	//以下语句用于读取excel多行单元格的内容
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
		MessageBox(_T("创建Excel服务失败...."),_T("信息提示"), MB_OK);
		return;
	}

	//books=app.get_Workbooks();
	CString selstrpath=BootOpenDialog();//弹出选择文件对话框
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
	books.AttachDispatch(app.get_Workbooks());/*得到工作簿容器*/
	try{
		lpDis=books.Open(strpath,covOptional,covOptional,covOptional,covOptional,
			covOptional,covOptional,covOptional,covOptional,
			covOptional,covOptional,covOptional,covOptional,covOptional,covOptional);

		//book=books.Add(covOptional);
		book.AttachDispatch(lpDis);
		IsFileExist=TRUE;
	}
	catch(...){ 
		/*无法打开，则增加一个新的工作簿*/
		lpDis = books.Add(vtMissing);
			book.AttachDispatch(lpDis);
		IsFileExist=FALSE;

	}
	//sheets=book.get_Sheets();
	//sheet.AttachDispatch(book.get_Worksheets());
	//sheet=sheets.get_Item(_variant_t((short)1));
	/*得到工作簿中的Sheet的容器*/
	sheets.AttachDispatch(book.get_Sheets());
	/*打开一个Sheet，如不存在，就新增一个Sheet*/
	CString strSheetName = SheetName;// _T("ExportSheet");
	try
	{
		/*打开一个已有的Sheet*/
		lpDis = sheets.get_Item(_variant_t(strSheetName));
		sheet.AttachDispatch(lpDis);
	}
	catch(...)
	{
		/*创建一个新的Sheet*/
		//lpDis = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
		//sheet.AttachDispatch(lpDis);
		//sheet.put_Name(strSheetName);
		MessageBox(_T("不存在表格：")+strSheetName);
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

	//以下语句用于读取excel多行单元格的内容
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
	
	 listCtrl.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 100 );//插入列//LVCFMT_LEFT, LVCFMT_RIGHT, or LVCFMT_CENTER.
	 for (long i = lFirstLBound; i <= lFirstUBound; i++)
	 {
		 index[0] = i;

		 CString strRowName = _T("");
		 strRowName.Format(_T("%d"), i-1);
		 if((1!=i) & (lFirstUBound-i<WEIGHT_DIS_LEN))//显示最近的1000条
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
					/* //获取行名称
					 long index_time[2] = {0, 0};
					 index_time[0]=1;
					 index_time[1]=j;
					 VARIANT varItem_time;
					 CString strtemp;
					 olesaRead.GetElement(index_time, &varItem_time);//获取行名称
					 strtemp=varItem_time.bstrVal;
					 if(_T("日期")==strtemp)//日期
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
			 if(i==1)//第一行为列名
				 listCtrl.InsertColumn( j, strItem, LVCFMT_LEFT, 100 );//插入列//LVCFMT_LEFT, LVCFMT_RIGHT, or LVCFMT_CENTER.
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

