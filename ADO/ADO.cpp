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
CADOConnection g_Connection;//ȫ�����ݿ����Ӷ���



//���ӹ�����ʾ����
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
//�����ƶ����Ƶı��
/*_RecordsetPtr Connection15::Execute (  _bstr_t   CommandText, VARIANT   *RecordsAffected, long   Options );
����CommandTextͨ����SQL�����ַ�����
RecordsAffected�ǲ�����ɺ���Ӱ���������
Options��ʾCommandText�����ݵ����ͣ�Options����ȡ����ֵ֮һ��  ?
adCmdText������CommandText��SQL�ı����� ?
adCmdTable������CommandText��һ������ ?
adCmdProc������CommandText��һ���洢���� ?
adCmdUnknown��δ֪  Executeִ����󷵻�һ��ָ���¼����ָ��*/

//���÷�����	GetConnection()->CreateTable(_T("�ҵı��"));
                //��GetConnection()->CreateTable(_T("�ҵı�� (��� LONG, ���� TEXT)"));//ָ���ֶ�

/*���û������ָ�����ӵ��ĸ����ݿ⣬����ֱ�Ӷ���CADOConnection������øú�������Ϊ���û��ʹ��GetConnection��open()���ض������ݿ⣬
ϵͳ����֪�����������ĸ����ݿ�(.mdb)���������ᱨ��
���ʹ��GetConnection()���ȫ�ֵ����ݿ����ӣ��ٵ��øú����������*/
//�������Ͱ��� Boolean��Integer��Long��Currency��Single��Double��Date��String �� Variant��Ĭ�ϣ���
//���ָ���ı���Ѿ����ڣ����᷵�ؿ�?*/
_RecordsetPtr  CADOConnection::CreateTable(CString TableName)//����ָ�����Ƶı��
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
_RecordsetPtr  CADOConnection::DelTable(CString TableName)//����ָ�����Ƶı��
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
//ɾ��ָ�����

//��ָ������в����ֶ�,���ָ�����ֶδ��ڣ���᷵��NULL
_RecordsetPtr  CADOConnection::AddNewField(CString Table, CString Field, CString Type)//��ָ������������ֶ�
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
//��ָ������в����ֶ�,���ָ�����ֶδ��ڣ���᷵��NULL  
_RecordsetPtr  CADOConnection::DelField(CString Table, CString Field)//ɾ��ָ�����Ƶı���е��ֶ�
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
		//m_Connection->ConnectionString=(_bstr_t)ConStr;    //�˷���������
		//m_Connection->Open("","","",adConnectUnspecified); 
	}
	catch(_com_error e)///��׽�쳣
	{
		//CString errormessage;
		//errormessage.Format(_T("�������ݿ�ʧ�ܣ�/r/n������Ϣ��%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///��ʾ������Ϣ
//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
		AfxMessageBox(_T("���ݿ�����ʧ�ܣ�") + ConStr);
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
	//����SQL���ݿ� Provider=SQLNCLI10.1;Integrated Security="";Persist Security Info=False;User ID=wufeng;Initial Catalog=FieldData;Data Source=DELL-PC-11\WF;Extended Properties="Server=127.0.0.1,1434;Database=info;Trusted_Connection=yes";Initial File Name="";Server SPN=""
	//Provider=SQLNCLI10.1;Integrated Security="";Persist Security Info=False;User ID=wufeng;Initial Catalog=FieldData;Data Source=DELL-PC-11\WF;Extended Properties="Server=127.0.0.1,1434;Database=info;Trusted_Connection=yes";Initial File Name="";Server SPN=""
	if(theApp.m_DataBaseType==SQLPROVIDE)
	{
		//Str.Format(_T("Provider=SQLOLEDB.1;Persist Security Info=False;User ID=wufeng;Password=2317000;Initial Catalog=%s;Data Source=%s"),DBName,IP);
		//Str.Format(_T("Provider=SQLNCLI10.1;Persist Security Info=False;User ID=wufeng;Password=123456;Initial Catalog=%s;Data Source=%s"),DBName,IP);//��˾����
		//Str.Format(_T("Provider=SQLNCLI10.1;Persist Security Info=False;User ID=wufeng1;Password=2317000;Initial Catalog=%s;Data Source=%s"),DBName,IP);//�Լ�����	
		//Str.Format(_T("Provider=SQLNCLI10.1;Persist Security Info=False;User ID=wufeng;Password=2317000;Initial Catalog=%s;Data Source=DELL-PC-11\\WF;Server=%s;Database=info;Trusted_Connection=yes"),DBName,IP);

	}
	//����access���ݿ�
	if(theApp.m_DataBaseType==ACCESSPROVIDE)
	{
		Str.Format(_T("DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s;"),DBName);//���ص��Կ���������½ʹ��
		//Str.Format(_T("uid=admin;pwd=111;DRIVER={Microsoft Access Driver (*.mdb)};Data Source=\\Data\\%s;"),DBName);//��ʱ�޷�����
		//Str.Format(_T("Provider=Microsoft.Jet.OleDb.4.0;Jet OleDb:DataBase Password=;Data Source=%s;Persist Security Info=False"),DBName);//���ذ���ô�����������½
		//Str.Format(_T("Provider=Microsoft.Jet.OleDb.4.0;Jet OleDb:DataBase Password=111;Data Source=\\\\120.24.222.237\\C:\\Data\\%s;Persist Security Info=False"),DBName);//�������δ���Գɹ�
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
			//m_DataSet->Open(_bstr_t(SQLStr), (g_Connection.GetConnection()).GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);//ʹ�ô˷����޷�ʹ��GetRecordCount()ֱ�ӻ�ȡ��¼����
			m_DataSet->Open(_bstr_t(SQLStr),
				_variant_t((IDispatch*)g_Connection.GetConnection(), true),
				adOpenKeyset, adLockOptimistic, adCmdText);
		}

	}
	catch(_com_error e)///��׽�쳣
	{
		//CString errormessage;
		//errormessage.Format(_T("�����ݿ�ʧ�ܣ�/r/n������Ϣ��%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///��ʾ������Ϣ
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
	Open(cName);//��ȡȫ���ֶ���
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
//�ж��Ƿ��ڼ�¼����ͷ����β��
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

	catch(_com_error e)///��׽�쳣
	{
		//CString errormessage;
		//errormessage.Format(_T("�����ݿ�ʧ�ܣ�/r/n������Ϣ��%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///��ʾ������Ϣ
//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
	}
}
//20160612����
char CADODataSet::SetFieldValue(CString FieldName, int nRow, CString& Value)//��cstring��ֵд�����ݿ�ָ�������У��������ݿ��ת����������
{
	BSTR bsName;
	//LPCTSTR LpFieldName = FieldName;
	move(nRow);//�ƶ���ָ������
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
		case VT_R4:/*��������*/
		case VT_R8:
			//��cstringת��Ϊ��������

			break;
		default:
			return 1;
			break;
		}

		//m_DataSet->PutCollect((_bstr_t)FieldName, (_variant_t)vtFld);
		SetFieldValue(FieldName, (_variant_t)vtFld);
		Save();
	}

	catch (_com_error e)///��׽�쳣
	{
		//CString errormessage;
		//errormessage.Format(_T("�����ݿ�ʧ�ܣ�/r/n������Ϣ��%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///��ʾ������Ϣ
		//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
	}
	return 0;
}
void CADODataSet::SetArrayFieldValue(CStringArray &FieldAr,int FieldStart, float Value[],int ValueStart,int len )//�������ʼλ�ô�1��ʼ���һ��
{
	//if((FieldStart+len)>FieldAr.GetCount())
	if((FieldStart+len)>FieldAr.GetSize())
	{
		AfxMessageBox(_T("���鳤�Ȳ�����"));
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
//���ָ����ѯ��ȫ����¼
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
	catch(_com_error e)///��׽�쳣
	{
		//CString errormessage;
		//errormessage.Format(_T("�����ݿ�ʧ�ܣ�/r/n������Ϣ��%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///��ʾ������Ϣ
//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
	}

	return IsOpen();
}
//???????????????????��??????????
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
	catch(_com_error e)///��׽�쳣
	{
		//CString errormessage;
		//errormessage.Format(_T("�����ݿ�ʧ�ܣ�/r/n������Ϣ��%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///��ʾ������Ϣ
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
	catch(_com_error e)///��׽�쳣
	{
		//CString errormessage;
		//errormessage.Format(_T("�����ݿ�ʧ�ܣ�/r/n������Ϣ��%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///��ʾ������Ϣ
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
	catch(_com_error e)///��׽�쳣
	{
		//CString errormessage;
		//errormessage.Format(_T("�����ݿ�ʧ�ܣ�/r/n������Ϣ��%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///��ʾ������Ϣ
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
	catch(_com_error e)///��׽�쳣
	{
		//CString errormessage;
		//errormessage.Format(_T("�����ݿ�ʧ�ܣ�/r/n������Ϣ��%s"), e.ErrorMessage());
		//AfxMessageBox(errormessage);///��ʾ������Ϣ
		str=_T("**��Ч�ֶ�**");
//		AfxMessageBox(ADO_FORMATMSG(e), MB_ICONERROR);
		//return FALSE;
	}

	return str;
}
//������е�ĳ���ֶμ�����Ͽ�
void CADODataSet::AddToCommb(HWND Dlg, int CommboId, CString TableName, CString FieldName)
{
	TableName = _T("select * from ") + TableName;
	Open(TableName);//���û���
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


//��ʾ���ϴ���
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

	strMsg.Format(_T("�������:%08lX\n��������:%s\n������Ϣ:%s\n����Դ:%s\n�ļ�:%s\n����:%-6d\n����:%s\nʱ��:%s\n���̺�:%-6d\n�̺߳�:%-6d\n��ִ���ļ�:%s\n"),
		e.Error(), (LPCTSTR)e.Description(), (LPCTSTR)e.ErrorMessage(), (LPCTSTR)e.Source(),
		(LPCTSTR)strFile, nLine, (LPCTSTR)strFunction, strTime, GetCurrentProcessId(), GetCurrentThreadId(), strModule);


	return strMsg;

}