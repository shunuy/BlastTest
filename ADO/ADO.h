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

#ifdef RUN_IN_WIN7_PLATEFORM   /*64��32λ�ı��뻷��������������ĳ���ֻ��������WIN7��������ϵͳʱʹ�ã���XPϵͳ�б���Ҳ����ʹ�ø����*/
#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF","adoEOF")//��win7ϵͳ�����еĳ������ʹ�ø����
#endif

#ifdef RUN_IN_XP_PLATEFORM     /*VS2013��32λ��ϵͳ�б���������,���ڿ���������32 WIN7λ�����ϰ�װ��Ӧ��ADO���*/
#import "C:\Program Files\Common Files\System\ado\msado28.tlb"  no_namespace rename("EOF","adoEOF")   //Ϊ��ʹ������xpϵͳ������ʹ��
#endif

#ifdef RUN_IN_XP_PLATEFORM_X64    /*VS2013��64λ��ϵͳ�б��������ã����ڿ���������64 WIN7λ�����ϰ�װ��Ӧ��ADO���*/
#import "C:\Program Files (x86)\Common Files\System\ado\msado28.tlb"  no_namespace rename("EOF","adoEOF")   //Ϊ��ʹ������xpϵͳ������ʹ��
#endif

/***********************ʹ   ��   ˵   ��*******************************************************************************
	1�������Appͷ�ļ�����appͷ�ļ��ж���int   m_DataBaseType;//���ݿ�����
	2����APP�ĳ�ʼ��ʵ������������������´��룺
		BOOL bCon;
		if(theApp.m_DataBaseType==ACCESSPROVIDE)
			bCon=GetConnection()->Open(GetConnection()->GetSQLConStr(_T("127.0.0.1"),_T("..\\BlastTest\\Samples\\Data\\���Ƽ��ϵͳ.mdb")));//����ACCESS
		if(m_DataBaseType==SQLPROVIDE)
			bCon=GetConnection()->Open(GetConnection()->GetSQLConStr(_T("127.0.0.1\\ZYXZ"),_T("FieldData")));//��½����,��˾����
		if (!bCon)
		{
			if(AfxMessageBox(_T("�������ݿ�ʧ�ܣ��Ƿ�����򿪳���"),MB_OKCANCEL)==IDCANCEL)
			return FALSE;
		}
	3�������XPϵͳ�б��룬ʹ�ú궨��RUN_IN_WIN7_PLATEFORM�������WIN7�����б��룬Ϊ����֧��XPϵͳ������ݰ�װ����ϵͳ�ĵ��Ե�ϵͳ���ͣ�64λ
	   ��32λ����ѡ����Ӧ�ĺ궨�壬�������ڿ�������ϵͳ�а�װ��Ӧ������(���������VS�İ�װ����Ŀ¼��)��
*/

/******************��   ��   ��   ¼************************************
�޸�����     �޸���      �޸�����
20150820      ���       ���Ӷ����\ɾ�������صĺ���
20150829      ���       �޸�����IsOpen()����������ͷ�ļ���#import���������궨�壬��֧��XPϵͳ�����С�
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
	_RecordsetPtr CreateTable(CString TableName);//����ָ�����Ƶı��
	_RecordsetPtr  DelTable(CString TableName);//ɾ��ָ�����Ƶı��
	_RecordsetPtr  CADOConnection::AddNewField(CString Table, CString Field, CString Type);//��ָ������в����ֶ�
	_RecordsetPtr  DelField(CString Table, CString Field);//ɾ��ָ�����Ƶı���е��ֶ�
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
	char SetFieldValue(CString FieldName, int nRow, CString& Value);//��cstring��ֵд�����ݿ�ָ�������У��������ݿ��ת����������
	void SetArrayFieldValue(CStringArray &FieldAr,int FieldStart, float Value[],int ValueStart,int len );//�������ʼλ�ô�1��ʼ���һ��
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
	void AddToCommb(HWND Dlg, int CommboId, CString TableName, CString FieldName);//���ƶ������ֶ����ӵ���Ͽ�
	void GetAccessField(CString &Tab, CStringArray &FieldAr);//���ACCESS���ݿ���ֶ���
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
