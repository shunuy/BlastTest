// ADO.h: interface for the CADO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADO_H__EE0962C4_DC53_489D_981F_B112A39F9C1B__INCLUDED_)
#define AFX_ADO_H__EE0962C4_DC53_489D_981F_B112A39F9C1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RUN_IN_XP_PLATEFORM
//#define RUN_IN_XP_PLATEFORM_X64
//#define RUN_IN_WIN7_PLATEFORM

#ifdef RUN_IN_WIN7_PLATEFORM   /*64或32位的编译环境，如果待发布的程序只需运行在WIN7及其以上系统时使用，在XP系统中编译也可以使用该语句*/
#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF","adoEOF")//在win7系统中运行的程序可以使用该语句
#endif

#ifdef RUN_IN_XP_PLATEFORM     /*VS2013在32位的系统中编译程序调用,需在开发环境的32 WIN7位电脑上安装对应的ADO插件*/
#import "C:\Program Files\Common Files\System\ado\msado28.tlb"  no_namespace rename("EOF","adoEOF")   //为了使程序在xp系统中运行使用
#endif

#ifdef RUN_IN_XP_PLATEFORM_X64    /*VS2013在64位的系统中编译程序调用，需在开发环境的64 WIN7位电脑上安装对应的ADO插件*/
#import "C:\Program Files (x86)\Common Files\System\ado\msado28.tlb"  no_namespace rename("EOF","adoEOF")   //为了使程序在xp系统中运行使用
#endif

/***********************使   用   说   明*******************************************************************************
	1、需包含App头文件，在app头文件中定义int   m_DataBaseType;//数据库类型
	2、在APP的初始化实例函数函数中添加如下代码：
		BOOL bCon;
		if(theApp.m_DataBaseType==ACCESSPROVIDE)
			bCon=GetConnection()->Open(GetConnection()->GetSQLConStr(_T("127.0.0.1"),_T("..\\BlastTest\\Samples\\Data\\爆破检测系统.mdb")));//连接ACCESS
		if(m_DataBaseType==SQLPROVIDE)
			bCon=GetConnection()->Open(GetConnection()->GetSQLConStr(_T("127.0.0.1\\ZYXZ"),_T("FieldData")));//登陆网络,公司电脑
		if (!bCon)
		{
			if(AfxMessageBox(_T("连接数据库失败！是否继续打开程序？"),MB_OKCANCEL)==IDCANCEL)
			return FALSE;
		}
	3、如果在XP系统中编译，使用宏定义RUN_IN_WIN7_PLATEFORM，如果在WIN7电脑中编译，为了能支持XP系统，需根据安装编译系统的电脑的系统类型（64位
	   或32位），选择相应的宏定义，并且需在开发环境系统中安装相应的驱动(驱动存放在VS的安装程序目录下)。
*/

/******************修   改   记   录************************************
修改日期     修改人      修改内容
20150820      伍丰       增加对添加\删除表格相关的函数
20150829      伍丰       修改两个IsOpen()函数，增加头文件中#import语句的三个宏定义，以支持XP系统中运行。
*/

#define ACCESSPROVIDE 0
#define SQLPROVIDE    1
 
class CADOConnection
{
private:
	static void InitADO();
	static void UnInitADO();
protected:
	_ConnectionPtr m_Connection;
public:
	BOOL IsOpen();
	_ConnectionPtr GetConnection();
	_RecordsetPtr CreateTable(CString TableName);//创建指定名称的表格
	_RecordsetPtr  DelTable(CString TableName);//删除指定名称的表格
	_RecordsetPtr  CADOConnection::AddNewField(CString Table, CString Field, CString Type);//在指定表格中插入字段
	_RecordsetPtr  DelField(CString Table, CString Field);//删除指定名称的表格中的字段
	CString GetSQLConStr(CString IP,CString DBName);
	BOOL Open(CString ConStr);
	CADOConnection();
	virtual ~CADOConnection();

};

class CADODataSet
{
protected:
	_RecordsetPtr m_DataSet;
	CADOConnection *m_Connection;
public:
	BOOL Open(CString SQLStr,int LockType);
	void Delete();
	int GetRecordNo();
	void move(int nIndex);
	void Save();
	void SetFieldValue(CString FieldName,_variant_t Value);
	char SetFieldValue(CString FieldName, int nRow, CString& Value);//将cstring的值写入数据库指定的行列，根据数据库的转换数据类型
	void SetArrayFieldValue(CStringArray &FieldAr,int FieldStart, float Value[],int ValueStart,int len );//数组的起始位置从1开始算第一个
	void AddNew();
	BOOL Next();
	void moveFirst();
	FieldsPtr GetFields();
	void ClearRecord(CString sql);
	BOOL IsBOF();
	BOOL IsadoEOF();
	_variant_t ReadValue(CString FieldStr);
	float ReadFloatValue(CString FieldStr);
	int ReadIntValue(CString FieldStr);
	double  ReadDbValue(CString FieldStr);
	CString ReadStringValue(CString FieldStr);
	int GetRecordCount();
	void SetConnection(CADOConnection *pCon);
	void AddToCommb(HWND Dlg, int CommboId, CString TableName, CString FieldName);//将制定表格的字段增加到组合框
	void GetAccessField(CString &Tab, CStringArray &FieldAr);//获得ACCESS数据库的字段名
	BOOL Open(CString SQLStr);
	_RecordsetPtr GetDataSet();
	void CloseDataSet(); 
	BOOL AddToTable(CString TableName,CString FieldName,CString Content);

	CADODataSet();
	virtual ~CADODataSet();
private:
	BOOL IsOpen();
};


CADOConnection * GetConnection();
inline CString AdoFormatMsg(const _com_error& e,LPCSTR lpszFile, LPCSTR lpszFunction,int nLine);

#endif // !defined(AFX_ADO_H__EE0962C4_DC53_489D_981F_B112A39F9C1B__INCLUDED_)
