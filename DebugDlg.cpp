// DebugDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BlastTest.h"
#include "DebugDlg.h"
#include "FATEK Protocol.h"
#include "BlastTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
20150605增加本程序

*/
/////////////////////////////////////////////////////////////////////////////
// CDebugDlg dialog


CDebugDlg::CDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDebugDlg)
	m_IrADSwitch = 0;
	m_HunColliValue = 0;
	m_UpLimite = 0;
	m_HunReleaseValue = 0;
	m_ByteAdd = _T("");
	m_ByteValue = _T("");
	m_MenAdd = _T("");
	m_MenValue = _T("");
	m_SoundLevelPeak = 0.0f;
	//}}AFX_DATA_INIT
}


void CDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugDlg)
	DDX_Text(pDX, IDC_EDIT1, m_IrADSwitch);
	DDX_Text(pDX, IDC_EDIT2, m_HunColliValue);
	DDX_Text(pDX, IDC_EDIT3, m_UpLimite);
	DDX_Text(pDX, IDC_EDIT4, m_HunReleaseValue);
	DDX_Text(pDX, IDC_EDIT15, m_ByteAdd);
	DDX_Text(pDX, IDC_EDIT16, m_ByteValue);
	DDX_Text(pDX, IDC_EDIT5, m_MenAdd);
	DDX_Text(pDX, IDC_EDIT6, m_MenValue);
	DDX_Text(pDX, IDC_EDIT17, m_SoundLevelPeak);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDebugDlg, CDialog)
	//{{AFX_MSG_MAP(CDebugDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebugDlg message handlers

void CDebugDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	unsigned char SBuf[10];
	memset(SBuf,0,10);

	if(nIDEvent==1)//更新数据
	{
		UpdateData(TRUE);
		FATEK_ReadMultiByte(CMD_SLAVE_ADDR, CMD_DOOR_SWITECH, 1, SBuf);//上升限位开关、、门开关
		m_UpLimite=FATEK_GetIntData(0,(char *)SBuf);
		FATEK_ReadMultiByte(CMD_SLAVE_ADDR, CMD_IrAD_SWITCH, 1, SBuf);//对射开关
		m_IrADSwitch=FATEK_GetIntData(0,(char *)SBuf);
		FATEK_ReadMultiByte(CMD_SLAVE_ADDR, CMD_HUN_COLLISION_VALUE, 1, SBuf);//防二次碰撞电磁阀
		m_HunColliValue=FATEK_GetIntData(0,(char *)SBuf);
		FATEK_ReadMultiByte(CMD_SLAVE_ADDR, CMD_HUN_RELEASE_VALUE, 1, SBuf);//重锤释放电磁阀
		m_HunReleaseValue=FATEK_GetIntData(0,(char *)SBuf);

		//读取BYTE
		memset(SBuf,0,10);
		if(m_ByteAdd.GetLength()==5)//如果地址长度合法
		{
			FATEK_ReadMultiByte(CMD_SLAVE_ADDR, m_ByteAdd, 1, SBuf);//重锤释放电磁阀
			m_ByteValue=SBuf;
		}
		memset(SBuf,0,10);
		if((m_MenAdd.GetLength()==6) |(m_MenAdd.GetLength()==7))//如果地址长度合法
		{
			FATEK_ReadMultiReg(CMD_SLAVE_ADDR,m_MenAdd,1,SBuf);
			m_MenValue=SBuf;
		}
		memset(SBuf,0,10);
		FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_SOUND_LEVEL_PEAK,1,SBuf);//读取声级计的峰值
		m_SoundLevelPeak=FATEK_GetIntData(0,(char *)SBuf)/10.0f;

		UpdateData(FALSE);
		
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CDebugDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetTimer(1,500,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
