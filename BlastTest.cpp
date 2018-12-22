// BlastTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BlastTest.h"
#include "BlastTestDlg.h"
#include "ADO/ADO.h"
#include "common.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CBlastTestApp

BEGIN_MESSAGE_MAP(CBlastTestApp, CWinApp)
	//{{AFX_MSG_MAP(CBlastTestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlastTestApp construction

CBlastTestApp::CBlastTestApp()
{
	m_DataBaseType=0;//access数据库
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBlastTestApp object

CBlastTestApp theApp;

//wf
//ATL support module
CComModule _Module;
//wf end

/////////////////////////////////////////////////////////////////////////////
// CBlastTestApp initialization

BOOL CBlastTestApp::InitInstance()
{
	//wf
	//启动时增加程序对 COM 的支持
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));
    _Module.Init(0, AfxGetInstanceHandle());
	//wf end
	
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	//登陆界面；如果要登陆SQL数据库，需要修改相应的宏定义
	BOOL bCon;
	if(theApp.m_DataBaseType==ACCESSPROVIDE)
		//bCon=GetConnection()->Open(GetConnection()->GetSQLConStr(_T("127.0.0.1"),_T("..\\BlastTest\\Samples\\Data\\爆破检测系统.mdb")));//连接ACCESS
		bCon=GetConnection()->Open(GetConnection()->GetSQLConStr(_T("127.0.0.1"),_T("C:\\BlastTest\\Samples\\Data\\爆破检测系统.mdb")));//连接ACCESS
	
	if(m_DataBaseType==SQLPROVIDE)
		bCon=GetConnection()->Open(GetConnection()->GetSQLConStr(_T("127.0.0.1\\ZYXZ"),_T("FieldData")));//登陆网络,公司电脑
	//BOOL bCon=GetConnection()->Open(GetConnection()->GetSQLConStr(_T("127.0.0.1\\WF"),_T("FieldData")));//登陆网络，自己电脑
	
	//BOOL bCon=GetConnection()->Open(GetConnection()->GetSQLConStr(_T("DELL-PC-11\\ZYXZ"),_T("FieldData")));//登陆本机
	if (!bCon)
	{
		if(AfxMessageBox(_T("连接数据库失败！是否继续打开程序？"),MB_OKCANCEL)==IDCANCEL)
			return FALSE;
	}



#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//zhj add V1.03
	CFile file;
	if (file.Open("C:\\BlastTest\\mocha.ini", CFile::modeRead)) EquipmentType = TESTTYPE_MOCHA;
	

	
	CBlastTestDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CBlastTestApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	//wf
	//退出时对 COM 的支持的释放操作
    _Module.Term();
	::CoUninitialize();
	//wf end
	return CWinApp::ExitInstance();
}
