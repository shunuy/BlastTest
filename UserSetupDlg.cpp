// UserSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BlastTest.h"
#include "UserSetupDlg.h"
#include "ADO/ADO.h"
#include "DebugDlg.h"
#include "BlastTestDlg.h"
#include "FATEK Protocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserSetupDlg dialog
/*
20150608  伍丰   添加文件立即备份与恢复
*/

CUserSetupDlg::CUserSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserSetupDlg)
	m_SLThreshold = 0.0f;
	m_HunCurrentH=0.0f;
	m_HunRealH=0.0f;
	m_ZeroSetValue = 0.0f;
	m_HunHight = 0.0f;
	m_CorSetValue = 0.0f;
	m_SmogSheld = 0.0f;
	m_CRHHunHight = 0.0f;
	m_Step = 0;
	//}}AFX_DATA_INIT
}


void CUserSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserSetupDlg)
	DDX_Text(pDX, IDC_SLSHRESHOLD, m_SLThreshold);
	DDX_Text(pDX, IDC_CURRENTHIGHT, m_HunCurrentH);
	DDX_Text(pDX, IDC_REALHIGHT, m_HunRealH);
	DDX_Text(pDX, IDC_SETHIGHT, m_ZeroSetValue);
	DDX_Text(pDX, IDC_CODEHIGHT, m_HunHight);
	DDX_Text(pDX, IDC_CORSET, m_CorSetValue);
	DDX_Text(pDX, IDC_SMOGSHELD, m_SmogSheld);
	DDX_Text(pDX, IDC_HUNHIGHT, m_CRHHunHight);
	DDX_Text(pDX, IDC_STEP, m_Step);
	DDX_Text(pDX, IDC_PRESSSETOFFSET, m_PressOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CUserSetupDlg)
	ON_BN_CLICKED(IDC_STARTKEYBOARD, OnStartkeyboard)
	ON_BN_CLICKED(IDC_UserSetupSave, OnUserSetupSave)
	ON_BN_CLICKED(IDC_DEBUGDLG, OnDebugdlg)
	ON_BN_CLICKED(IDC_BACKUP, OnBackup)
	ON_BN_CLICKED(IDC_BACKUPRECORVE, OnBackuprecorve)
	ON_BN_CLICKED(IDC_SYSRESET, OnSysreset)
	ON_BN_CLICKED(IDC_ADJUSTHIGHT, OnAdjusthight)
	ON_BN_CLICKED(IDC_CLEARDATABASE, OnCleardatabase)
	ON_BN_CLICKED(IDC_ZEROSETBTN, OnZerosetbtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CORSETBTN, OnCorsetbtn)
	ON_BN_CLICKED(IDC_BTNFILEEXPORT, OnBtnfileexport)
	ON_BN_CLICKED(IDC_SAVEVALUE, OnSavevalue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserSetupDlg message handlers

void CUserSetupDlg::OnStartkeyboard() 
{
	// TODO: Add your control notification handler code here
	//WinExec(_T("C:\\Documents and Settings\\Administrator\\Application Data\\360se6\\Application\\360se.exe"),SW_NORMAL);
	//WinExec(_T("C:\\Program Files\\HotVirtualkeyboard\\hvk.exe"),SW_NORMAL);
	WinExec(_T("C:\\Program Files\\ComfortKeyboard\\CKeyboard.exe"),SW_NORMAL);

}
//保存设置
void CUserSetupDlg::OnUserSetupSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from UserSetUp"));
	if((m_SLThreshold>0) & (m_SLThreshold<160))
	{
		if(DataSet.IsadoEOF())
			DataSet.AddNew();
		DataSet.SetFieldValue(_T("声级计阀值"),m_SLThreshold);

		if(m_SmogSheld<100)//烟雾阀值的有效范围，该值需要修改
		{
			DataSet.SetFieldValue(_T("烟雾阀值"),m_SmogSheld);
		}
		if (m_PressOffset < 1.2)//压力设定值不大于1.2MP的补偿
		{
			DataSet.SetFieldValue(_T("压力设定补偿"), m_PressOffset);
		}
		long nSel;   	
		// 获取组合框控件的列表框中选中项的索引   
		nSel = ((CComboBox *)GetDlgItem(IDC_SAVEFILEINTERVAL))->GetCurSel(); 
		DataSet.SetFieldValue(_T("文件保存间隔"),nSel);
		DataSet.Save();
	}

	CDialog::OnOK();
}

BOOL CUserSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CComboBox *)GetDlgItem(IDC_SAVEFILEINTERVAL))->AddString(_T("自动"));
	((CComboBox *)GetDlgItem(IDC_SAVEFILEINTERVAL))->AddString(_T("每天"));
	((CComboBox *)GetDlgItem(IDC_SAVEFILEINTERVAL))->AddString(_T("每月"));

	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from UserSetup"));
	if(!DataSet.IsadoEOF())
	{
		((CComboBox *)GetDlgItem(IDC_SAVEFILEINTERVAL))->SetCurSel(DataSet.ReadIntValue(_T("文件保存间隔")));
		CString str;
		str.Format("%.1f",DataSet.ReadFloatValue("防二次碰撞延时"));
		GetDlgItem(IDC_CORCURRENT)->SetWindowText(str);
	}
	SetTimer(1,500,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserSetupDlg::OnDebugdlg() 
{
	// TODO: Add your control notification handler code here
	CDebugDlg dlg;
	dlg.DoModal();
}
//立即备份按钮
void CUserSetupDlg::OnBackup() 
{
	// TODO: Add your control notification handler code here
	COleDateTime CurrTime;
	CurrTime=CurrTime.GetCurrentTime();
	CString StrTime;
	//zhj delete V1.01 StrTime=CurrTime.Format(_T("%Y-%m-%d_%H-%M"));
	//StrTime=_T("D:\\爆破数据备份\\爆破检测系统")+StrTime+_T(".mdb");
	////::CopyFile("D:\\爆破项目\\BlastTest\\Samples\\Data\\爆破检测系统.mdb","D:\\爆破数据备份\\爆破检测系统.mdb",false); 
	//::CopyFile("D:\\爆破项目\\BlastTest\\Samples\\Data\\爆破检测系统.mdb",StrTime,TRUE);	
}
//备份恢复按钮
void CUserSetupDlg::OnBackuprecorve() 
{
	// TODO: Add your control notification handler code here
	CString strFile = _T("");
	
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL);
	
	if (dlgFile.DoModal()==IDOK)
	{
		strFile = dlgFile.GetPathName();
	//zhj delete	::CopyFile(strFile,"D:\\爆破项目\\BlastTest\\Samples\\Data\\爆破检测系统.mdb",false);
	}
}
//复位系统
void CUserSetupDlg::OnSysreset() 
{
	// TODO: Add your control notification handler code here

	unsigned char SBuf[10];
	SBuf[0]='1';
	int Fatek;
	for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_SYSTEM_RESET,1,SBuf))
			break;
	}
	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		MessageBox(_T("系统复位失败！"));
		return;
	}
	MessageBox(_T("系统复位成功！"));

	//清装药和放锤标志20150625
	((CBlastTestDlg *)GetParent())->m_HunReleaseBtn=0;
	((CBlastTestDlg *)GetParent())->m_PlaceExpPrepar=0;
	((CBlastTestDlg *)GetParent())->SetHunBtnBit(0);
	((CBlastTestDlg *)GetParent())->SetParperBtnBit(0);

}
//校正重锤高度
void CUserSetupDlg::OnAdjusthight() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from UserSetUp"));
	if((m_SLThreshold>0) & (m_SLThreshold<10000) & (abs(m_HunCurrentH-m_HunRealH)<100) & (m_HunCurrentH>0))
	{
		if(DataSet.IsadoEOF())
			DataSet.AddNew();
		DataSet.SetFieldValue(_T("重锤高度系数"),m_HunCurrentH/m_HunRealH);

		DataSet.Save();
	}
}
//将数据库中的数据清空
void CUserSetupDlg::OnCleardatabase() 
{
	// TODO: Add your control notification handler code here
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	int RecordCount=0;
	if(IDOK==MessageBox(_T("是否清空---操作记录"),_T("删除确认"),MB_OKCANCEL))
	{
		DataSet.Open(_T("select * from OperateRecord"));
		RecordCount=DataSet.GetRecordCount();
		for(int i=0;i<RecordCount;i++)
		{
			DataSet.Delete();
			DataSet.Save();
			DataSet.Next();
		}
	}
	if(IDOK==MessageBox(_T("是否清空---实验记录"),_T("删除确认"),MB_OKCANCEL))
	{
		DataSet.Open(_T("select * from TestData"));
		RecordCount=DataSet.GetRecordCount();
		for(int i=0;i<RecordCount;i++)
		{
			DataSet.Delete();
			DataSet.Save();
			DataSet.Next();
		}
	}
	if(IDOK==MessageBox(_T("是否清空---样品数据"),_T("删除确认"),MB_OKCANCEL))
	{
		DataSet.Open(_T("select * from SampleTable"));
		RecordCount=DataSet.GetRecordCount();
		for(int i=0;i<RecordCount;i++)
		{
			DataSet.Delete();
			DataSet.Save();
			DataSet.Next();
		}
	}
	if(IDOK==MessageBox(_T("是否清空---实验员信息"),_T("删除确认"),MB_OKCANCEL))
	{
		DataSet.Open(_T("select * from UserTable"));
		RecordCount=DataSet.GetRecordCount();
		for(int i=0;i<RecordCount;i++)
		{
			DataSet.Delete();
			DataSet.Save();
			DataSet.Next();
		}
	}

}
//设定重锤的零点
void CUserSetupDlg::OnZerosetbtn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if((m_ZeroSetValue<600) | (m_ZeroSetValue>1000))
	{
		MessageBox(_T("设置值应在600到1000之间"));
	}
	else
	{
		if(((CBlastTestDlg *)GetParent())->WriteHunZero(m_HunKg,m_ZeroSetValue))
		{
			MessageBox(_T("设置成功！"));
		}
	}
}

void CUserSetupDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	unsigned char SBuf[10];
	memset(SBuf,0,10);

	if(nIDEvent==1)
	{
		if(0<FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_PLATE_HIGHT_REAL,0x01, SBuf))//平台高度，还需要进行转换
			m_HunHight=(float)FATEK_GetIntData(0,(char *)SBuf)/(10.0f);
		CString str;
		str.Format(_T("%.1f"),m_HunHight);
		GetDlgItem(IDC_CODEHIGHT)->SetWindowText(str);
		//UpdateData(FALSE);
	}
	CDialog::OnTimer(nIDEvent);
}

//二次碰撞延时修改按钮
void CUserSetupDlg::OnCorsetbtn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if((-2000 <m_CorSetValue) & (m_CorSetValue<2000))
	{
		CADODataSet DataSet;
		DataSet.SetConnection(::GetConnection());
		DataSet.Open(_T("select * from UserSetUp"));
		DataSet.SetFieldValue(_T("防二次碰撞延时"),m_CorSetValue);
		DataSet.Save();
		MessageBox(_T("修改成功"));		
	}
	else
	{
		MessageBox(_T("输入值必须在-2000到2000之间！"));
	}
}

//导出文件
void CUserSetupDlg::OnBtnfileexport() 
{
	// TODO: Add your control notification handler code here
	CFileDialog FileOpenDlg(TRUE);
	CString OpenTitle;
	OpenTitle=_T("选择要导出的文件");
	FileOpenDlg.m_ofn.lpstrTitle=OpenTitle;
	if(IDOK!=FileOpenDlg.DoModal())
	{
		return;
	}
	OpenTitle=_T("选择存储路径");
	CFileDialog FileSaveDlg(FALSE,NULL,FileOpenDlg.GetFileName());
	FileSaveDlg.m_ofn.lpstrTitle=OpenTitle;
	if(IDOK!=FileSaveDlg.DoModal())
	{
		return;
	}
	CString str=FileSaveDlg.GetPathName();
	::CopyFile(FileOpenDlg.GetPathName(),FileSaveDlg.GetPathName(),false);//如果文件已经存在，则文件会被覆盖
}
//保存阀值设定,与设置保存按钮同功能
void CUserSetupDlg::OnSavevalue() 
{
	// TODO: Add your control notification handler code here
	OnUserSetupSave();
}
