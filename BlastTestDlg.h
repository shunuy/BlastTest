// BlastTestDlg.h : header file
//
//{{AFX_INCLUDES()
#include "datagrid.h"
#include "columns.h"
#include "Column.h"
#include "adodc.h"
#include "HdyPress.h"

//}}AFX_INCLUDES

#if !defined(AFX_BLASTTESTDLG_H__A35ABFCC_A29C_4445_8C87_FB6552F7E967__INCLUDED_)
#define AFX_BLASTTESTDLG_H__A35ABFCC_A29C_4445_8C87_FB6552F7E967__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBlastTestDlg dialog
#include "PlateDownBtn.h"
#include "CRTHeight.h"
#include "WfComm.h"
#include "SLComm.h"
#include "afxwin.h"

#define  CMD_SLAVE_ADDR  0X02

#define CMD_PLATE_UP  _T("M0300")  /*0为停止状态，1为上升*/
#define CMD_PLATE_DOWN _T("M0302")  /*0为停止状态，1为下降*/
#define CMD_PLACE_EXPLOSIVE _T("M0304") /*0-方药完成，1--开始方药*/
#define CMD_HUN_RELESAE  _T("M0306")  /*为1正在释放，0释放完成*/
#define CMD_SYSTEM_RESET _T("M0308")   /*写1复位系统*/
#define CMD_PLATE_MODE_SWITCH _T("M0309")  /*手动平台和自动平台切换*/

//v1.09add
#define CMD_JIAYA _T("M0040") 

#define CMD_HUN_ZEROPOINT    _T("D00000")  /*重锤零点*/
#define CMD_GET_HUN_ERROR    _T("D00005")  /*大于3说明抓捶故障*/
//#define CMD_SOUND_LEVEL_PEAK _T("D00304")   /*声级计峰值*/
//#define CMD_SOUND_LEVEL_REAL _T("D00305")   /*声级计瞬时值*/
#define CMD_SOUND_LEVEL_PEAK _T("D00318")   /*声级计峰值*/
#define CMD_SOUND_LEVEL_REAL _T("D00316")   /*声级计瞬时值*/
#define CMD_HUN_REAL_HIGHT   _T("D00306")   /*重锤提升的实际高度,除以10显示*/
#define CMD_PLATE_HIGHT_REAL _T("D00302")    /*实际平台高度*/
#define CMD_HUN_HEIGHT       _T("D00300")    /*重	锤高度设置*/
#define CMD_COR_TIMER_DELAY  _T("D00013")    /*防二次碰撞延时，单位ms*/
//#define CMD_SMOG_REAL        _T("R03840")    /*读取PLC烟雾传感器的实时值*/
#define CMD_SMOG_REAL        _T("D00080")    /*读取PLC烟雾传感器的实时值*/

//调试PLC地址定义
#define CMD_UP_LIMITE                _T("X0001")    /*关门开关*/
#define CMD_DOOR_SWITECH             _T("X0001")   /*门开关*/
#define CMD_HUN_RELEASE_VALUE        _T("Y0006")    /*重锤释放电磁阀*/
#define CMD_HUN_COLLISION_VALUE      _T("Y0007")    /*防二次碰撞电磁铁*/
#define CMD_IrAD_SWITCH              _T("X0000")     /*对射开关*/

#define CMD_FINISH_STATUS     _T("M0030")  /*放锤后PLC给的完成状态，1--表示完成，0表示未完成*/
#define CMD_FINISH_STATUS_INT _T("D00320")  /*放锤后PLC给的完成状态，1--表示完成，0表示未完成*/

#define STATUS_WAITE_TIME     120      /*等待重锤释放完成，乘以0.5s为实际时间*/

#define WM_HUNFINISH WM_USER+10

#define CMD_SEND_REPART_COUNT  10   /*重复发送的最大次数，超过说明写入失败*/
//#define DAOFFSETVALUE          0.08 /**/

#define commport    1        //通信端口

//试验类型
#define  BLAST_RATE_MODE  0  //爆炸概率法试验模式
#define  CRT_HEIGHT_MODE  1  //特性高度法试验模式
#define  FRI_RATE_MODE  2  //特性高度法试验模式
#define  INVALID_TEST_MODE  127  //无效的试验类型

//试验完成状态
#define CH_STEEP_BEGIN   0   //第一次预备试验
#define CH_STEEP_4D      1   //4D步长试验
#define CH_STEEP_2D      2   //2D步长试验
#define CH_STEEP_D       3   //D步长试验
#define CH_STEEP_FINISH  4   //完成预备试验

#define  COMM_NORMAL_READ  1   /*串口正常数据的读取*/
#define  COMM_WRITE_HDY_DATA   2 /*写入液压系统参数*/

union FIType{
	unsigned int Dint;
	float       FData;
};

struct StrPlcData
{
	float SoundLevel;//声级计数
	float PlateHight;//平台高度
	float SmogReal;//烟雾传感器的实时值
	UINT  ComFunIndex;//指定串口的功能；1--读取实时数据；2--
	UINT  COmErrCount;//通信错误计数器
	bool  CallPressFun;//调用液压加压程序

	CSLComm m_SoundComm;//声级计通信程序
	CHdyPress m_HdyPress;//液压加压对象
};
struct HunThreedData
{
	HWND hwnd;
};

class CBlastTestDlg : public CDialog
{
// Construction
public:
	char SetTestMode();  //根据选择设置相应的测试模式 特性落高法，摩擦感度法
	CBlastTestDlg(CWnd* pParent = NULL);	// standard constructor
	void RefreshDGrid(CAdodc & Ado,CDataGrid & DGrid,CString Sql);
	void EditWriteAble(bool sw);//编辑框是否可写，0--不可写入；1--可写入
	void WriteLog(CString SData);
	void SetHunBtnBit(char sw);//sw:0--gray,1--blue
	void SetParperBtnBit(char sw);//装药与完成按钮  0--显示装药，1显示完成，同时会改变放捶按钮
	void SetTestSpInBtnBit(char sw);
	void SetBlastIndexBit(char sw);  //sw:0--gray,1--爆炸
	void WriteRecord();//一次爆破测试完成后，写入实验记录
	void CalTestResult();//计算实验结果，爆炸率，可行度等。
	char WriteByte(unsigned char Slave,CString Addr, char Dbyte);//从机地址，  写入地址，     写入数据    ，成功返回TRUE
	char WriteInt(unsigned char Slave,CString Addr, int DInt);//成功返回TRUE
	char WriteFloat(unsigned char Slave, CString Addr, float DInt);//成功返回TRUE
	char WriteHunZero(float HunKg,float ZeroNo);//写入当前重锤的零点
	float ReadHunZero(float HunKg);//读取当前重锤的零点
	float CRTCalNextH(int BlastStatus);//计算下一个特性高度

	CFont m_editFont;  

// Dialog Data
	//{{AFX_DATA(CBlastTestDlg)
	enum { IDD = IDD_BLASTTEST_DIALOG };
	CBitmapButton	m_QuitWindow;
	CBitmapButton	m_BtnReport;   //改按钮已经失效
	CBitmapButton	m_BtnPrepar;  //装药
	CBitmapButton	m_BtnRealse; //重锤释放
	CBitmapButton	m_BlastIndex; //爆炸指示灯
	CBitmapButton   m_TestSpData;//样品数据
	CBitmapButton   m_TestCondSetup;//实验条件设置
	CBitmapButton   m_RecordView;//实验记录
	CBitmapButton   m_UserSetup; //用户设置
	CBitmapButton   m_TestSpIn; //载入样品数据
	CBitmapButton   m_PlateUp; //平台上升
	CBitmapButton   m_PlateDown; //平台下降
	CBitmapButton   m_PlateUpStop; //平台上升停止--已无效
	CBitmapButton   m_PlateDownStop; //--已无效
	CBitmapButton   m_PlateModeBtn; //平台手动自动运行模式选择
	CDataGrid	m_DataGrid1;
	CAdodc	m_Ado1;
	CDataGrid	m_DG_OpRecord;
	CAdodc	m_Ado_OPRecord;
	CString m_TestSpNo; //试样编号
	CString m_TestSpName;//药品名称
	short     m_TestGroupNo;//实验组数
	short     m_TestNo;     //试验次数
	CString m_TestType;   //试验类型
	short     m_BlastRate;   //实时引爆率
	float     m_TestSpKg;    //试样质量
	float     m_SondLevel;    //声级计数
	float     m_PlateHight;   //平台高度,实时高度
	float     m_HunchHight;   //重锤高度
	float     m_HunchKg;      //重锤质量
	CString m_TestPerName;  //试验员
	float m_TestTemp;    //试验温度
	float m_TestHumi;    //试验湿度
	bool m_PlaceExpPrepar; //装药按钮指示 1表示正在装药，0表示完成
	CString	m_TestSel;     //试验下拉选择组合框
	CString	m_TimeDis;
	float   m_HunHightRata; //写入重锤的高度系数。
	float	m_SmogReal;
	float	m_SmogSetValue;
	//}}AFX_DATA
	char m_PlateMode;   //平台模式，0为自动，1为手动平台，2为手动加压
	bool m_PlateUpState;//平台上升状态，0为停止，1为上升
	bool m_PlateDownState;//平台下降状态，0为停止，1为下降
	bool m_HunReleaseBtn;//放锤按钮，0为没有正在放锤，1为正在放锤
	bool m_TestSpInBtn;  //载入按钮指示，0-操作的是载入；1--操作的是确认参数
	short   m_MaxGroupNo;//本次实验的组数
	short   m_MaxTestNo;     //本次试验每组次数
	float m_SLThreshold; //声级计读取的阀值
	float m_ZeroLevelHight;//重锤的零位对应的编码器值
	float m_CorTimerDelay;//防二次碰撞延时
	bool m_SmogThrMode; //烟雾阀值的有效形式，1--自动采集；0--手动设置
	float m_Smog_bigvalue;//烟雾阀值的增量值
	int m_LastBlastStatus;//上一次的爆炸状态  1--爆炸；0--不爆炸
	int m_CurrBlastStatus;//上一次的爆炸状态  1--爆炸；0--不爆炸
	int m_CRTTestStep;//特性落高法的试验状态
	int m_CRTTestLastStep;//特性落高法的上一次的试验状态
	int m_Testmode;//试验方式，0--爆破试验，1--特性落高法
	float m_StepLength;//试验步长

	CCRTHeight m_CRTHeight;//特性落高法对象
	//CHdyPress m_HdyPress;//液压加压对象
	//CWfComm m_SondComm;//声级计通信串口

//	float m_GroupBlastRate[20];//保存每组实验的实时爆炸率

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlastTestDlg)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBlastTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnQuitWindows();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnReport();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnPraper();
	afx_msg void OnTestSpBtn();
	afx_msg void OnTestcondsetup();
	afx_msg void OnTestSpInBtn();
	afx_msg void OnRecordView();
	afx_msg void OnOpplatebtn();
	afx_msg void OnPlateUp();
	afx_msg void OnPlateDown();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBtnRealse();
	afx_msg void OnDropdownTestsel();
	afx_msg void OnUserSetup();
	afx_msg LRESULT  OnHunFinish(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeTestsel();
	afx_msg void OnChangeSpKg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	// 油缸前进按钮
	CBitmapButton m_HdyForwardBtn;
	// 油缸后退按钮
	CBitmapButton m_HdyBackBtn;
	afx_msg void OnBnClickedHdyforward();
	afx_msg void OnBnClickedHdyback();
	void SetMPBtn();
	CEdit m_PressSetValue;
	afx_msg void OnEnChangeEdit11();
	afx_msg void OnEnChangeEdit18();
};
extern struct StrPlcData PlcData;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLASTTESTDLG_H__A35ABFCC_A29C_4445_8C87_FB6552F7E967__INCLUDED_)
