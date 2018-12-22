// ADO.cpp: implementation of the CADO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ADO.h"
#include <comdef.h>
#include "BlastTest.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int ConCount = 0;
CADOConnection g_Connection;//全局数据库连接对象



//连接故障显示函数
#ifndef ADO_FORMATMSG
#define ADO_FORMATMSG(e)		AdoFormatMsg(e, __FILE__, __FUNCTION__, __LINE__)
#endif // ADO_FORMATMSG

CADOConnection * GetConnection()
{
	return &g_Connection;
}

CADOConnection::CADOConnection()
{
	InitADO();
	m_Connection.CreateInstance(_T("ADODB.Connection"));
}

CADOConnection::~CADOConnection()
{
	if (IsOpen())
		m_Connection->Close();
	m_Connection = NULL;
	UnInitADO();
}

void CADOConnection::InitADO()
{
		if (ConCount++ == 0) 
			CoInitialize(NULL);
};
void CADOConnection::UnInitADO()
{
		if (--ConCount == 0)
			 CoUninitialize();
};
//创建制定名称的表格
/*_RecordsetPtr Connection15::Execute (  _bstr_t   CommandText, VARIANT   *RecordsAffected, long   Options );
其中CommandText通常是SQL命令字符串。
RecordsAffected是操作完成后所影响的行数，
Options表示CommandText中内容的类型，Options可以取如下值之一：  ?
adCmdText：表明CommandText是SQL文本命令 ?
adCmdTable：表明CommandText是一个表名 ?
adCmdProc：表明CommandText是一个存储过程 ?
adCmdUnknown：未知  Execute执行完后返回一个指向记录集的指针*/

//调用方法：	GetConnection()->CreateTable(_T("我的表格"));
                //或GetConnection()->CreateTable(_T("我的表格 (序号 LONG, 内容 TEXT)"));//指定字段

/*如果没有连接指定连接到哪个数据库，不能直接定义CADOConnection对象调用该函数，因为如果没有使用GetConnection的open()打开特定的数据库，
系统将不知道操作的是哪个数据库(.mdb)，操作将会报错，
因此使用GetConnection()获得全局的数据库连接，再调用该函数操作表格*/
//数据类型包括 Boolean、Integer、Long、Currency、Single、Double、Date、String 和 Variant（默认））
//如果指定的表格已经存在，将会返回空?*/
_RecordsetPtr  CADOConnection::CreateTable(CString TableName)//创建指定名称的表格
{
	_RecordsetPtr RPtr;
	TableName = _T("CREATE TABLE ") + TableName;
	_variant_t RecordsAffected;
	try
	{
		RPtr = m_Connection->Execute((_bstr_t)TableName, &RecordsAffected, adCmdText);
		return RPtr;
	}
	catch (_com_error e)
	{
		return NULL;
	}
}
_RecordsetPtr  CADOConnection::DelTable(CString TableName)//创建指定名称的表格
{
	_RecordsetPtr RPtr;
	TableName = _T("DROP TABLE ") + TableName;
	_variant_t RecordsAffected;
	try
	{
		RPtr = m_Connection->Execute((_bstr_t)TableName, &RecordsAffected, adCmdText);
		return RPtr;
	}
	catch (_com_error e)
	{
		return NULL;
	}
}
//删除指定表格

//在指定表格中插入字段,如果指定的字段存在，则会返回NULL
_RecordsetPtr  CADOConnection::AddNewField(CString Table, CString Field, CString Type)//在指定表格中增加字段
{
	Field = _T("alter table ") + Table + _T(" add COLUMN ") + Field + _T(" ") + Type;
	_variant_t RecordsAffected;
	_RecordsetPtr RPtr;
	try
	{
		RPtr=m_Connection->Execute((_bstr_t)Field, &RecordsAffected, adCmdText);
		return RPtr;
	}
	catch (_com_error e)
	{
		return NULL;
	}
}
//在指定表格中插入字段,如果指定的字段存在，则会返回NULL  
_RecordsetPtr  CADOConnection::DelField(CString Table, CString Field)//删除指定名称的表格中的字段
{
	Field = _T("alter table ") + Table + _T(" drop COLUMN ") + Field;
	_variant_t RecordsAffected;
	_RecordsetPtr RPtr;
	try
	{
		RPtr = m_Connection->Execute((_bstr_t)Field, &RecordsAffected, adCmdText);
		return RPtr;
	}
	catch (_com_error e)
	{
		return NULL;
	}
}


BOOL CADOConnection::Open(CString ConStr)
{
	if (IsOpen())
		m_Connection->Close();
	try
	{
		//SQLPROVIDE
		//Open(_bstr_t(_T("Provider=SQLNCLI10.1;Integrated Security="";Persist Security Info=False;User ID=wufeng;Initial Catalog=FieldData;Data Source=DELL-PC-11\WF;Extended Properties="Server=127.0.0.1,1434;Database=info;Trusted_Connection=yes";Initial File Name="";Server SPN=""")), _bstr_t(_T("wufeng")), _bstr_t(_T("2317000")), adOpenUnspecified)
		//ConStr = _T("Provider = Microsoft.Jet.OLEDB.4.0; Data Source = ..\\PC108T\\DataBase\\MyData.mdb; Persist Security Info = False");
		m_Connection->Open((_bstr_t)ConStr,"","",adModeUnknown);
		//ACCESSPROVIDE
		//m_Connection->ConnectionString=(_bstr_t)ConStr;    //此法可以运行
		//m_Connection->Open("","","",adConnectUnspecified); 
	}
	catch(_com_error e)///捕捉异常
	{
		//CString errormessage;
		//errormessage.Format(_T("连接数据库失败！/r/n错误信息：%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///显示错误信息
//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
		AfxMessageBox(_T("数据库连接失败：") + ConStr);
	}

	
	return IsOpen();
}

//CString CADOConnection::GetSQLConStr(CString IP, CString DBName)
//{
//	CString Str;
//	Str.Format(_T("Provider=SQLOLEDB.1;Persist Security Info=False;User ID=sa;Initial Catalog=%s;Data Source=%s"),DBName,IP);
//	return Str;
//}

CString CADOConnection::GetSQLConStr(CString IP, CString DBName)
{
	CString Str;
	//连接SQL数据库 Provider=SQLNCLI10.1;Integrated Security="";Persist Security Info=False;User ID=wufeng;Initial Catalog=FieldData;Data Source=DELL-PC-11\WF;Extended Properties="Server=127.0.0.1,1434;Database=info;Trusted_Connection=yes";Initial File Name="";Server SPN=""
	//Provider=SQLNCLI10.1;Integrated Security="";Persist Security Info=False;User ID=wufeng;Initial Catalog=FieldData;Data Source=DELL-PC-11\WF;Extended Properties="Server=127.0.0.1,1434;Database=info;Trusted_Connection=yes";Initial File Name="";Server SPN=""
	if(theApp.m_DataBaseType==SQLPROVIDE)
	{
		//Str.Format(_T("Provider=SQLOLEDB.1;Persist Security Info=False;User ID=wufeng;Password=2317000;Initial Catalog=%s;Data Source=%s"),DBName,IP);
		//Str.Format(_T("Provider=SQLNCLI10.1;Persist Security Info=False;User ID=wufeng;Password=123456;Initial Catalog=%s;Data Source=%s"),DBName,IP);//公司电脑
		//Str.Format(_T("Provider=SQLNCLI10.1;Persist Security Info=False;User ID=wufeng1;Password=2317000;Initial Catalog=%s;Data Source=%s"),DBName,IP);//自己电脑	
		//Str.Format(_T("Provider=SQLNCLI10.1;Persist Security Info=False;User ID=wufeng;Password=2317000;Initial Catalog=%s;Data Source=DELL-PC-11\\WF;Server=%s;Database=info;Trusted_Connection=yes"),DBName,IP);

	}
	//连接access数据库
	if(theApp.m_DataBaseType==ACCESSPROVIDE)
	{
		Str.Format(_T("DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s;"),DBName);//本地电脑可以正常登陆使用
		//Str.Format(_T("uid=admin;pwd=111;DRIVER={Microsoft Access Driver (*.mdb)};Data Source=\\Data\\%s;"),DBName);//暂时无法连接
		//Str.Format(_T("Provider=Microsoft.Jet.OleDb.4.0;Jet OleDb:DataBase Password=;Data Source=%s;Persist Security Info=False"),DBName);//本地版可用此链接正常登陆
		//Str.Format(_T("Provider=Microsoft.Jet.OleDb.4.0;Jet OleDb:DataBase Password=111;Data Source=\\\\120.24.222.237\\C:\\Data\\%s;Persist Security Info=False"),DBName);//网络版暂未调试成功
		//\\120.24.222.237\Data\FieldSersiveData.mdb
	}

	return Str;
}

_ConnectionPtr CADOConnection::GetConnection()
{
	return m_Connection;
}

BOOL CADOConnection::IsOpen()
{
	/*long State;
	m_Connection->get_State(&State);
	if (State == adStateOpen)
		return true;
	return false;*/
	if (m_Connection == NULL)
		return  false;
	if (m_Connection->GetState() == adStateClosed)
		return  false;

	return true;
}

////////////////////////////////////////
CADODataSet::CADODataSet()
{
	m_DataSet.CreateInstance(_T("ADODB.Recordset"));
}

CADODataSet::~CADODataSet()
{
	if (IsOpen())
		m_DataSet->Close();
	m_DataSet = NULL;
	m_Connection = NULL;
}
void CADODataSet::CloseDataSet()
{
	if (IsOpen())
		m_DataSet->Close();
	m_DataSet = NULL;
	m_Connection = NULL;
}

void CADODataSet::SetConnection(CADOConnection *pCon)
{
	m_Connection = pCon;
}

int CADODataSet::GetRecordCount()
{
	if (IsOpen())
		return m_DataSet->GetRecordCount();
	else
		return 0;
}

BOOL CADODataSet::Open(CString SQLStr)
{
	if (IsOpen())
		m_DataSet->Close();
	//*/
	try
	{
		if(theApp.m_DataBaseType==ACCESSPROVIDE)
		{
			m_DataSet->Open(_bstr_t(SQLStr),
				_variant_t((IDispatch*)g_Connection.GetConnection(), true),
				adOpenKeyset, adLockOptimistic, adCmdText);
		}
		if(theApp.m_DataBaseType==SQLPROVIDE)
		{
			//m_DataSet->Open(_bstr_t(SQLStr), (g_Connection.GetConnection()).GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);//使用此法打开无法使用GetRecordCount()直接获取记录条数
			m_DataSet->Open(_bstr_t(SQLStr),
				_variant_t((IDispatch*)g_Connection.GetConnection(), true),
				adOpenKeyset, adLockOptimistic, adCmdText);
		}

	}
	catch(_com_error e)///捕捉异常
	{
		//CString errormessage;
		//errormessage.Format(_T("打开数据库失败！/r/n错误信息：%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///显示错误信息
//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
	}

	return IsOpen();
	//*/

}

BOOL CADODataSet::IsOpen()
{
	/*long State;
	m_DataSet->get_State(&State);
	if (State == adStateOpen)
		return true;
	
	return false;*/
	if (m_DataSet == NULL)
		return  false;
	if (m_DataSet->GetState() == adStateClosed)
		return  false;

	return true;
}

FieldsPtr CADODataSet::GetFields()
{
	return m_DataSet->GetFields();
}
void CADODataSet::GetAccessField(CString &Tab, CStringArray &FieldAr)
{
	CString cName = _T("SELECT * from ") + Tab;
	Open(cName);//读取全部字段名
	long i, cou;
	cou = GetDataSet()->Fields->Count;
	CString str;
	FieldAr.RemoveAll();
	for (i = 0; i<cou; i++)
	{
		str = LPCSTR(GetDataSet()->Fields->GetItem(i)->GetName());
		FieldAr.Add(str);
	}

}
//判断是否在记录集的头或者尾部
BOOL CADODataSet::IsBOF()
{
	
	if(m_DataSet->BOF)
		return TRUE;

	return FALSE;
}
BOOL CADODataSet::IsadoEOF()
{
	if(m_DataSet->adoEOF)
		return TRUE;

	return FALSE;
}

BOOL CADODataSet::Next()
{
	if (m_DataSet->adoEOF)
		return false;
	m_DataSet->MoveNext();
	return true;
}

void CADODataSet::AddNew()
{
	m_DataSet->AddNew();
}

void CADODataSet::SetFieldValue(CString FieldName, _variant_t Value)
{
	try
	{
		m_DataSet->PutCollect((_bstr_t)FieldName,Value);
	}

	catch(_com_error e)///捕捉异常
	{
		//CString errormessage;
		//errormessage.Format(_T("打开数据库失败！/r/n错误信息：%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///显示错误信息
//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
	}
}
//20160612增加
char CADODataSet::SetFieldValue(CString FieldName, int nRow, CString& Value)//将cstring的值写入数据库指定的行列，根据数据库的转换数据类型
{
	BSTR bsName;
	//LPCTSTR LpFieldName = FieldName;
	move(nRow);//移动到指定的行
	try
	{
		//_variant_t vtFld = m_DataSet->Fields->GetItem(LpFieldName)->Value;
		_variant_t vtFld = ReadValue(FieldName);
		switch (vtFld.vt)
		{
		case VT_BSTR:
			{
				bsName = Value.AllocSysString();
				vtFld.bstrVal = bsName;
				SysFreeString(bsName);
				break;
			}
		case VT_R4:/*浮点类型*/
		case VT_R8:
			//将cstring转换为浮点类型

			break;
		default:
			return 1;
			break;
		}

		//m_DataSet->PutCollect((_bstr_t)FieldName, (_variant_t)vtFld);
		SetFieldValue(FieldName, (_variant_t)vtFld);
		Save();
	}

	catch (_com_error e)///捕捉异常
	{
		//CString errormessage;
		//errormessage.Format(_T("打开数据库失败！/r/n错误信息：%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///显示错误信息
		//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
	}
	return 0;
}
void CADODataSet::SetArrayFieldValue(CStringArray &FieldAr,int FieldStart, float Value[],int ValueStart,int len )//数组的起始位置从1开始算第一个
{
	//if((FieldStart+len)>FieldAr.GetCount())
	if((FieldStart+len)>FieldAr.GetSize())
	{
		AfxMessageBox(_T("数组长度不够！"));
		return;
	}
	for(int i=0;i<len;i++)
		SetFieldValue(FieldAr.GetAt(FieldStart+i),Value[ValueStart+i]);
}

void CADODataSet::Save()
{
	m_DataSet->Update();
}

void CADODataSet::move(int nIndex)
{
	m_DataSet->MoveFirst();
	m_DataSet->Move(nIndex);
}
void CADODataSet::moveFirst()
{
	m_DataSet->MoveFirst();
}


int CADODataSet::GetRecordNo()
{
	return m_DataSet->AbsolutePosition;
}

void CADODataSet::Delete()
{
	m_DataSet->Delete(adAffectCurrent);
}
//清除指定查询的全部记录
void CADODataSet::ClearRecord(CString sql)
{
	Open(sql);
	int RecordCount=GetRecordCount();
	for(int i=0;i<RecordCount;i++)
	{
		Delete();
		Save();
		Next();
	}
}

BOOL CADODataSet::Open(CString SQLStr, int LockType)
{
	if (IsOpen())
		m_DataSet->Close();
	//*/
	try
	{
		m_DataSet->Open(_bstr_t(SQLStr),
			_variant_t((IDispatch*)g_Connection.GetConnection(), true),
			adOpenKeyset,(LockTypeEnum) LockType, adCmdText);

	}
	catch(_com_error e)///捕捉异常
	{
		//CString errormessage;
		//errormessage.Format(_T("打开数据库失败！/r/n错误信息：%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///显示错误信息
//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
	}

	return IsOpen();
}
//???????????????????У??????????
BOOL CADODataSet::AddToTable(CString TableName,CString FieldName,CString Content)
{
	CString Sql;
	Sql=_T("select * from ")+TableName+_T(" where ")+FieldName +_T("='")+Content+_T("'");
	Open(Sql);//???????
	int RecordCount=GetRecordCount();
	if(RecordCount)
		return FALSE;
	AddNew();
	SetFieldValue(FieldName,(_variant_t)Content);
	Save();
	return TRUE;
}

_variant_t  CADODataSet::ReadValue(CString FieldStr)
{
	return m_DataSet->GetFields()->Item[(_variant_t)FieldStr]->Value;
}

float CADODataSet::ReadFloatValue(CString FieldStr)
{
	float val;
	_variant_t var;
	try
	{	
		var=m_DataSet->GetFields()->Item[(_variant_t)FieldStr]->Value;
		if(var.vt==VT_NULL)
			val=0;
		else
			val=var.fltVal;
	}
	catch(_com_error e)///捕捉异常
	{
		//CString errormessage;
		//errormessage.Format(_T("打开数据库失败！/r/n错误信息：%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///显示错误信息
		val=0;
//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
	}

	return val;
}
double  CADODataSet::ReadDbValue(CString FieldStr)
{
	double val;
	_variant_t var;
	try
	{	
		var=m_DataSet->GetFields()->Item[(_variant_t)FieldStr]->Value;
		if(var.vt==VT_NULL)
			val=0;
		else
			val=var.dblVal;
	}
	catch(_com_error e)///捕捉异常
	{
		//CString errormessage;
		//errormessage.Format(_T("打开数据库失败！/r/n错误信息：%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///显示错误信息
		val=0;
//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
	}

	return val;
}
int CADODataSet::ReadIntValue(CString FieldStr)
{
	int val;
	_variant_t var;
	try
	{	
		var=m_DataSet->GetFields()->Item[(_variant_t)FieldStr]->Value;
		if(var.vt==VT_NULL)
			val=0;
		else
			val=var.intVal;
	}
	catch(_com_error e)///捕捉异常
	{
		//CString errormessage;
		//errormessage.Format(_T("打开数据库失败！/r/n错误信息：%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///显示错误信息
		val=0;
//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
	}

	return val;
}

CString  CADODataSet::ReadStringValue(CString FieldStr)
{
	CString str;
	_variant_t var;
	try
	{	
		var=m_DataSet->GetFields()->Item[(_variant_t)FieldStr]->Value;
		if(var.vt==VT_NULL)
			str=_T("");
		else
			str=CString(var.bstrVal);
	}
	catch(_com_error e)///捕捉异常
	{
		//CString errormessage;
		//errormessage.Format(_T("打开数据库失败！/r/n错误信息：%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///显示错误信息
		str=_T("**无效字段**");
//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
	}

	return str;
}
//将表格中的某个字段加入组合框
void CADODataSet::AddToCommb(HWND Dlg, int CommboId, CString TableName, CString FieldName)
{
	TableName = _T("select * from ") + TableName;
	Open(TableName);//打开用户表
	int RecordCount = GetRecordCount();
	for (int i = 0; i<RecordCount; i++)
	{
		((CComboBox *)(CWnd::FromHandle(GetDlgItem(Dlg, CommboId))))->AddString(ReadStringValue(FieldName));
		Next();

	}
	((CComboBox *)(CWnd::FromHandle(GetDlgItem(Dlg, CommboId))))->SetCurSel(0);
}

_RecordsetPtr CADODataSet::GetDataSet()
{
	return m_DataSet;
}


//显示故障代码
inline CString AdoFormatMsg(const _com_error& e,
	LPCSTR lpszFile, 
	LPCSTR lpszFunction,
	int nLine)
{
	_bstr_t strFile(lpszFile);
	_bstr_t strFunction(lpszFunction);

	SYSTEMTIME st;
	GetLocalTime(&st);

	CString strMsg;

	CString strTime;
	strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	CString strModule;
	GetModuleFileName(NULL, strModule.GetBuffer(MAX_PATH), MAX_PATH);
	strModule.ReleaseBuffer();

	strMsg.Format(_T("错误代码:%08lX\n错误描述:%s\n错误消息:%s\n错误源:%s\n文件:%s\n行数:%-6d\n函数:%s\n时间:%s\n进程号:%-6d\n线程号:%-6d\n可执行文件:%s\n"),
		e.Error(), (LPCTSTR)e.Description(), (LPCTSTR)e.ErrorMessage(), (LPCTSTR)e.Source(),
		(LPCTSTR)strFile, nLine, (LPCTSTR)strFunction, strTime, GetCurrentProcessId(), GetCurrentThreadId(), strModule);


	return strMsg;

}