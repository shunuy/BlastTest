// BlastTestDlg.cpp : implementation file
//
/****************************
修改日期    修改人              修改内容
20150518    伍丰           由于多线程未同步，造成同时向串口发送数据，从而导致数据错乱。创建了事件做同步。
20150523    伍丰           在样品数据载入中增加向PLC写入重锤高度功能。
20150523    伍丰           增加判断实验组之间的差异和读取置信区间功能。
20150605    伍丰           增加与PLC通信重发10失败，弹出提示对话框
20150605    伍丰           增加调试界面
20150608    伍丰           增加每次爆破放锤完成后自动备份
20150521    伍丰           平台切换后发送清上升下降的SBuf[0]=0改成SBuf[0]='0'；还有其他几处。
20150523    伍丰           在OnTimer()函数中添加判断最近输入的字符是否是数字，是数字才更新。
						   if(isdigit(str.GetAt(str.GetLength()-1)))//如果输入的是数字
20150623    伍丰           在BOOL CBlastTestDlg::PreTranslateMessage(MSG* pMsg)函数中增加判断ESC按钮的函数，
						   用于屏蔽esc按钮退出程序的功能。修改默认按钮，防止回车键退出程序。 
20150624    伍丰           添加写入1byte和1整形数据到PLC函数，并在函数内将数据读出比较，确保发送成功。
            伍丰           在主界面上声级计瞬时值后添加峰值显示。
			伍丰           调试界面增加读取PLC数据功能，增加显示声级计峰值功能。
			伍丰           调整装药和放捶按钮清除放捶和装药的顺序，装药时先清除放捶。
			伍丰           修改向PLC写入整形数时没有将整形数先转换为16进制的字符错误，并且要用大写字符--写重锤高度。
20150625    伍丰           DataGrid更新不及时的解决办法，调用更新之前，使用DataSet.Open()再查询一次数据库。
20150625    伍丰           修改计算实时爆炸率的方法。
20150626    伍丰           修改直接从D00306读取重锤的实际当前高度，使用onPaint()函数刷新DataGrid的数据，避免从其他界面返回Datagrid
                           不更新的问题。 
						   增加声级计峰值存入数据库的功能。
20150629    伍丰           增加手动平台和自动平台发送命令之前先复位系统，复位完成后清除上位机的装药和放锤。
20150630    伍丰           增加读取抓捶次数，达到3次弹出报警，并进行系统复位。
20150707    伍丰           删除清PLC放锤完成标志位。添加不关门无法放锤功能。添加重锤的高度系数.
                           判断实验组数是否大于25小于10000，否则差表示让其落在该区间内。
						   增加清空数据库，增加初始化读取样品记录时到组合框时的记录为0判断。
20150708    伍丰           写入和设置重锤零点写入D0,写入实验记录时，如过之前已经存在，则先删除该记录。
20150714    伍丰           在装药时向PLC写入防二次碰撞延时。
20150803    伍丰           修改DataGrid的Solite属性，让最左侧的选择指示条看不见，RecordSelect多选框取消
20151023    伍丰           添加显示与隐藏平台上升等按钮
                           增加开机声级计的打开提示，将定时器1和2的打开由初始化对话框放到了定时器4的消息响应中。
****/

#include "stdafx.h"
#include "BlastTest.h"
#include "BlastTestDlg.h"

#include "PORT.h"
#include "FATEK Protocol.h"

#include "PrintReportDlg.h"

#include "_recordset.h"
#include "ADO/ADO.h"

#include "TestSpData.h"
#include "TestConditon.h"

#include "PrintReportDlg.h"
#include "TestRecordView.h"
#include "UserSetupDlg.h"

#include "math.h"
#include "mmsystem.h"

#include "CRTHeight.h"
#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct StrPlcData PlcData;
bool isAddRdcord = false;

HANDLE GEvent;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlastTestDlg dialog

CBlastTestDlg::CBlastTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBlastTestDlg::IDD, pParent)
	,m_TestSpNo(_T(""))
	,m_TestSpName(_T(""))
	,m_TestGroupNo(0)
	,m_TestNo(0)
	,m_TestType(_T("撞击感度法"))
	,m_BlastRate(0)
	,m_TestSpKg(0)
	,m_SondLevel(0)
	,m_PlateHight(0)
	,m_HunchHight(0)
	,m_HunchKg(2)
	,m_TestPerName(_T(""))
	,m_TestTemp(0)
	,m_TestHumi(0)
	,m_PlaceExpPrepar(0)
	,m_PlateMode(0)
	,m_PlateUpState(0)
	,m_PlateDownState(0)
	,m_HunReleaseBtn(0)
	,m_TestSpInBtn(0)
	,m_MaxGroupNo(0)
	,m_MaxTestNo(0)
	,m_ZeroLevelHight(1000)
	,m_HunHightRata(1)
	,m_CorTimerDelay(0)
	,m_SmogThrMode(0)
	,m_Smog_bigvalue(0)
	,m_Testmode(0)
	,m_StepLength(0.1f)
	,m_CRTTestStep(0)
	,m_CRTTestLastStep(0)
	,m_LastBlastStatus(0)
	,m_CurrBlastStatus(0)

{
	//{{AFX_DATA_INIT(CBlastTestDlg)
	m_TestSel = _T("");
	m_TimeDis = _T("");
	m_SmogReal = 0.0f;
	m_SmogSetValue = 0.0f;
	PlcData.COmErrCount=0;
	PlcData.ComFunIndex=1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

long count = 0;

void CBlastTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlastTestDlg)
	DDX_Control(pDX, IDC_DATAGRID1, m_DataGrid1);
	DDX_Control(pDX, IDC_ADODC1, m_Ado1);
	DDX_Control(pDX, IDC_DG_OpRecord, m_DG_OpRecord);
	DDX_Control(pDX, IDC_ADODC2, m_Ado_OPRecord);
	DDX_Text(pDX, IDC_EDIT1, m_TestSpNo);
	DDX_Text(pDX, IDC_EDIT2, m_TestSpName);
	DDX_Text(pDX, IDC_EDIT3, m_TestGroupNo);
	DDX_Text(pDX, IDC_EDIT4, m_TestNo);
	DDX_Text(pDX, IDC_EDIT5, m_TestType);
	DDX_Text(pDX, IDC_EDIT6, m_BlastRate);
	DDX_Text(pDX, IDC_EDIT7, m_TestSpKg);
	DDX_Text(pDX, IDC_EDIT8, m_SondLevel);
	DDX_Text(pDX, IDC_EDIT9, m_PlateHight);
	DDX_Text(pDX, IDC_EDIT10, m_HunchHight);
	//DDX_Text(pDX, IDC_EDIT11, m_HunchKg);
	if (EquipmentType == TESTTYPE_ZHUJI)
	{
		DDX_Text(pDX, IDC_EDIT11, m_HunchKg);
	}
	else
	{
       //zhj modify v1.15
		float aa = 0, bb = 1;
		
		if (m_HunReleaseBtn&&isProcess)
		{
			count++;
			if (count > 2000) count = 2000;
			bb = 1 - count / 2400;
			aa = PlcData.m_HdyPress.m_PressSetValue + (PlcData.m_HdyPress.m_PressValue - PlcData.m_HdyPress.m_PressSetValue)*bb;
		}
		else
		{
			aa = PlcData.m_HdyPress.m_PressValue;
			count = 0;
		}
		DDX_Text(pDX, IDC_EDIT11, aa);


	}
	DDX_Text(pDX, IDC_EDIT12, m_TestPerName);
	DDX_Text(pDX, IDC_EDIT13, m_TestTemp);
	DDX_Text(pDX, IDC_EDIT14, m_TestHumi);
	DDX_CBString(pDX, IDC_TESTSEL, m_TestSel);
	DDX_Text(pDX, IDC_TIMEDIS, m_TimeDis);
	DDX_Text(pDX, IDC_SMOGREAL, m_SmogReal);
	DDX_Text(pDX, IDC_SMOGSET, m_SmogSetValue);
	DDX_Text(pDX, IDC_PRESSSETVALUE, PlcData.m_HdyPress.m_PressSetValue);

	//}}AFX_DATA_MAP

	//DDX_Control(pDX, IDC_HDYFORWARD, m_HdyForwardBtn);
	//DDX_Control(pDX, IDC_HDYBACK, m_HdyBackBtn);
	DDX_Control(pDX, IDC_PRESSSETVALUE, m_PressSetValue);
}

BEGIN_MESSAGE_MAP(CBlastTestDlg, CDialog)
	//{{AFX_MSG_MAP(CBlastTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_QUIT_WINDOWS, OnQuitWindows)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_REPORT, OnBtnReport)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_PRAPER, OnBtnPraper)
	ON_BN_CLICKED(IDC_TestSpBtn, OnTestSpBtn)
	ON_BN_CLICKED(IDC_TESTCONDSETUP, OnTestcondsetup)
	ON_BN_CLICKED(IDC_TestSpInBtn, OnTestSpInBtn)
	ON_BN_CLICKED(IDC_RecordView, OnRecordView)
	ON_BN_CLICKED(IDC_OPPLATEBTN, OnOpplatebtn)
	ON_BN_CLICKED(IDC_PLATE_UP, OnPlateUp)
	ON_BN_CLICKED(IDC_PLATE_DOWN, OnPlateDown)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_REALSE, OnBtnRealse)
	ON_CBN_DROPDOWN(IDC_TESTSEL, OnDropdownTestsel)
	ON_BN_CLICKED(IDC_UserSetup, OnUserSetup)
	ON_MESSAGE(WM_HUNFINISH,OnHunFinish)
	ON_CBN_SELCHANGE(IDC_TESTSEL, OnSelchangeTestsel)
	ON_EN_CHANGE(IDC_EDIT7, OnChangeSpKg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_HDYFORWARD, &CBlastTestDlg::OnBnClickedHdyforward)
	ON_BN_CLICKED(IDC_HDYBACK, &CBlastTestDlg::OnBnClickedHdyback)
	ON_EN_CHANGE(IDC_EDIT18, &CBlastTestDlg::OnEnChangeEdit18)
	ON_BN_CLICKED(IDC_BUTTON2, &CBlastTestDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlastTestDlg message handlers
DWORD WINAPI ReadPlcDataPro(LPVOID lpParameter );

BOOL CBlastTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	OpenPort(commport,CBR_9600,7,EVENPARITY,ONESTOPBIT,4096,4096,1000);

	//unsigned char SBuf[20];
	//FATEK_ReadMultiByte(CMD_SLAVE_ADDR,"X0050",0x06, SBuf);

	//CPrintReportDlg dlg;
	//dlg.DoModal();
	m_BtnReport.LoadBitmaps(IDB_BITOPENREPORT,IDB_BITSELECTED);
	m_BtnReport.SubclassDlgItem(IDC_BTN_REPORT,this);
	m_BtnReport.SizeToContent();

	m_QuitWindow.LoadBitmaps(IDB_BTN_QUITWINDOWS);
	m_QuitWindow.SubclassDlgItem(IDC_QUIT_WINDOWS,this);
	m_QuitWindow.SizeToContent();
	//装药按钮
	m_BtnPrepar.LoadBitmaps(IDB_PREPAR,IDB_BITSELECTED);
	m_BtnPrepar.SubclassDlgItem(IDC_BTN_PRAPER,this);
	m_BtnPrepar.SizeToContent();
	//重锤释放
	m_BtnRealse.LoadBitmaps(IDB_REALSE,IDB_BITSELECTED);
	m_BtnRealse.SubclassDlgItem(IDC_BTN_REALSE,this);
	m_BtnRealse.SizeToContent();
	//爆炸指示灯
	m_BlastIndex.LoadBitmaps(IDB_BLAST_INDEX_GRAY);
	m_BlastIndex.SubclassDlgItem(IDC_BLAST_INDEX,this);
	m_BlastIndex.SizeToContent();
	//试样数据
	m_TestSpData.LoadBitmaps(IDB_TestSpBtb,IDB_BITSELECTED);
	m_TestSpData.SubclassDlgItem(IDC_TestSpBtn,this);
	m_TestSpData.SizeToContent();
	//实验条件设置
	m_TestCondSetup.LoadBitmaps(IDB_TESTCONDSETUP,IDB_BITSELECTED);
	m_TestCondSetup.SubclassDlgItem(IDC_TESTCONDSETUP,this);
	m_TestCondSetup.SizeToContent();
	//实验记录
	m_RecordView.LoadBitmaps(IDB_RecordView,IDB_BITSELECTED);
	m_RecordView.SubclassDlgItem(IDC_RecordView,this);
	m_RecordView.SizeToContent();

	//用户设置
	m_UserSetup.LoadBitmaps(IDB_UserSetup,IDB_BITSELECTED);
	m_UserSetup.SubclassDlgItem(IDC_UserSetup,this);
	m_UserSetup.SizeToContent();

	//载入实验数据
	m_TestSpIn.LoadBitmaps(IDB_TestSpIn,IDB_BITSELECTED);
	m_TestSpIn.SubclassDlgItem(IDC_TestSpInBtn,this);
	m_TestSpIn.SizeToContent();

	//平台上升
	m_PlateUp.LoadBitmaps(IDB_PLATEUP,IDB_BITSELECTED);
	m_PlateUp.SubclassDlgItem(IDC_PLATE_UP,this);
	m_PlateUp.SizeToContent();
	//平台上升停止
	/*m_PlateUpStop.LoadBitmaps(IDB_PLATEUPSTOP);
	m_PlateUpStop.SubclassDlgItem(IDC_PLATE_UP_STOP,this);
	m_PlateUpStop.SizeToContent();*/

	//平台下降
	m_PlateDown.LoadBitmaps(IDB_PLATEDOWN,IDB_BITSELECTED);
	m_PlateDown.SubclassDlgItem(IDC_PLATE_DOWN,this);
	m_PlateDown.SizeToContent();

	//平台下降停止
	/*m_PlateDownStop.LoadBitmaps(IDB_PLATEDOWNSTOP);
	m_PlateDownStop.SubclassDlgItem(IDC_PLATE_DOWN_STOP,this);
	m_PlateDownStop.SizeToContent();*/
	//平台运行模式选择按钮
	

	if (EquipmentType == TESTTYPE_ZHUJI)
	{
		m_PlateModeBtn.LoadBitmaps(IDB_AUTOPLATEBTN_ZJ);
		GetDlgItem(IDC_PRESSSETVALUE)->ShowWindow(SW_HIDE);
	}

	else m_PlateModeBtn.LoadBitmaps(IDB_AUTOPLATEBTN);

	m_PlateModeBtn.SubclassDlgItem(IDC_OPPLATEBTN,this);
	m_PlateModeBtn.SizeToContent();

	m_HdyForwardBtn.LoadBitmaps(IDB_HDYFORWARD, IDB_BITSELECTED);
	m_HdyForwardBtn.SubclassDlgItem(IDC_HDYFORWARD, this);
	m_HdyForwardBtn.SizeToContent();

	m_HdyBackBtn.LoadBitmaps(IDB_HDYBACK, IDB_BITSELECTED);
	m_HdyBackBtn.SubclassDlgItem(IDC_HDYBACK, this);
	m_HdyBackBtn.SizeToContent();

	CFont* ptf=GetDlgItem(IDC_EDIT1)->GetFont();//m_editPlace.GetFont(); // 得到原来的字体 
	LOGFONT lf;   
  	ptf->GetLogFont(&lf);   
	lf.lfHeight = 15; // 改变字体高度   
	strcpy (lf.lfFaceName, "宋体"); // 改变字体名称   
	m_editFont.CreateFontIndirect(&lf);   
	GetDlgItem(IDC_EDIT1)->SetFont(&m_editFont); // 设置新字体    
	GetDlgItem(IDC_EDIT2)->SetFont(&m_editFont); // 设置新字体  
	GetDlgItem(IDC_EDIT3)->SetFont(&m_editFont); // 设置新字体 
	GetDlgItem(IDC_EDIT4)->SetFont(&m_editFont); // 设置新字体 
	GetDlgItem(IDC_EDIT5)->SetFont(&m_editFont); // 设置新字体 
	GetDlgItem(IDC_EDIT6)->SetFont(&m_editFont); // 设置新字体 
	GetDlgItem(IDC_EDIT7)->SetFont(&m_editFont); // 设置新字体 
	GetDlgItem(IDC_EDIT8)->SetFont(&m_editFont); // 设置新字体 
	GetDlgItem(IDC_EDIT9)->SetFont(&m_editFont); // 设置新字体 
	GetDlgItem(IDC_EDIT10)->SetFont(&m_editFont); // 设置新字体 
	GetDlgItem(IDC_EDIT11)->SetFont(&m_editFont); // 设置新字体 
	GetDlgItem(IDC_EDIT12)->SetFont(&m_editFont); // 设置新字体 
	GetDlgItem(IDC_EDIT13)->SetFont(&m_editFont); // 设置新字体 
	GetDlgItem(IDC_EDIT14)->SetFont(&m_editFont); // 设置新字体 
	GetDlgItem(IDC_EDIT18)->SetFont(&m_editFont); // 设置新字体 //声级计峰值
	GetDlgItem(IDC_COMBO1)->SetFont(&m_editFont); // 设置新字体
	GetDlgItem(IDC_TIMEDIS)->SetFont(&m_editFont); // 设置新字体
	GetDlgItem(IDC_SMOGREAL)->SetFont(&m_editFont); 
	GetDlgItem(IDC_SMOGSET)->SetFont(&m_editFont); 
	GetDlgItem(IDC_PRESSSETVALUE)->SetFont(&m_editFont);
	//锁定编辑框
	EditWriteAble(0);
	/*CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_BITOPENREPORT);
	HBITMAP hBitmap=(HBITMAP)Bitmap.Detach();
	CButton *pButton=(CButton*)GetDlgItem(IDC_BTN_REPORT);
	pButton->SetBitmap(hBitmap);*/
	
	//
	CColumns Columns=m_DataGrid1.GetColumns();   //求列集合对象  
    CColumn Column;  
    Columns.Add(2);                         //在最后增加1列，注：列序号从0开始  
    Columns.Add(3);                         //再增加1列 
	Columns.Add(4);                         //再增加1列
	Columns.Add(5);                         //再增加1列 
    Column=Columns.GetItem(COleVariant(long(0)));   //取第一列对象  
    Column.SetWidth(120);                    //设置列宽度为80(像素pi?)  
    Column.SetCaption("自动编号");                //设置该列标题  
    //Column.SetAlignment(2);               //列内容对齐设置：0居左，1居中，2居右  
    Column.SetDataField("自动编号");            //设置该列绑定字段  
    Column=Columns.GetItem(COleVariant(long(1)));   //设置第二列  
    Column.SetWidth(100);                    //设置列宽度为80  
    Column.SetCaption("试样编号");                //设置该列标题  
    Column.SetDataField("试样编号");             //设置该列绑定字段  
    /*Column=Columns.GetItem(COleVariant(long(2)));   //设置第三列  
    Column.SetWidth(60);                    //设置列宽度为40  
    Column.SetCaption("实验类型");                //设置该列标题  
    Column.SetDataField("实验类型");          //设置该列绑定字段  */
    
	/*Column=Columns.GetItem(COleVariant(long(2)));   //设置第四列  
    Column.SetWidth(70);                   //设置列宽度为120  
    Column.SetCaption("重锤质量");           //设置该列标题  
    Column.SetDataField("重锤质量");            //设置该列绑定字段*/ //20151021 重锤高度
	Column=Columns.GetItem(COleVariant(long(2)));   //设置第四列  
    Column.SetWidth(70);                   //设置列宽度为120  
    Column.SetCaption("重锤高度");           //设置该列标题  
    Column.SetDataField("重锤高度");            //设置该列绑定字段
	

	Column=Columns.GetItem(COleVariant(long(3)));   //设置第四列  
    Column.SetWidth(90);                   //设置列宽度为120  
    Column.SetCaption("实验结果");           //设置该列标题  
    Column.SetDataField("实验结果");            //设置该列绑定字段 

	Column=Columns.GetItem(COleVariant(long(4)));   //设置第四列  
    Column.SetWidth(140);                   //设置列宽度为120  
    Column.SetCaption("实验日期");           //设置该列标题  
    Column.SetDataField("实验日期");            //设置该列绑定字段 

	Column=Columns.GetItem(COleVariant(long(5)));   //设置第四列  
    Column.SetWidth(80);                   //设置列宽度为120  
    Column.SetCaption("声级计峰值");           //设置该列标题  
    Column.SetDataField("声级计峰值");            //设置该列绑定字段 

    m_DataGrid1.ReBind();                        //实现与datagrid中的表格绑定  
    m_DataGrid1.Refresh();                       //刷新列表显示

	//

	SetTimer(4,500,NULL);//用于显示声级计打开提示
	
	PlcData.PlateHight=0;
	PlcData.SoundLevel=0;
	PlcData.SmogReal=0;
	//创建新线程
	HANDLE hTr=CreateThread(NULL,0,ReadPlcDataPro,NULL,NULL,NULL);
	CloseHandle(hTr);

	GEvent=CreateEvent(NULL,FALSE,FALSE,NULL);//创建事件对象，对线程进行同步,第二个参数设为FALSE表示只能有一个线程能获得信号
	SetEvent(GEvent);//将事件对象设置为有信号状态。

	
	//初始化试样选择下拉列表
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select top 20 * from SampleTable order by ID DESC"));
	for(int i=0;i<DataSet.GetRecordCount();i++)
	{
		((CComboBox *)GetDlgItem(IDC_TESTSEL))->AddString(DataSet.ReadStringValue(_T("试样编号")));
		DataSet.Next();
	}
	if(DataSet.GetRecordCount()>0)
	{
		((CComboBox *)GetDlgItem(IDC_TESTSEL))->SetCurSel(0);
		((CComboBox *)GetDlgItem(IDC_TESTSEL))->GetLBText(0, m_TestSel); 
	}
	DataSet.Open(_T("select * from TestConditionSetup"));
	m_HunchKg=DataSet.ReadFloatValue(_T("重锤质量"));
	//20151019添加
	m_StepLength=DataSet.ReadFloatValue(_T("步长设置"));
	m_TestType=DataSet.ReadStringValue(_T("实验类型"));
	PlcData.m_HdyPress.m_PressSetValue = DataSet.ReadFloatValue(_T("压力设定"));
	SetTestMode();
	PlcData.m_HdyPress.GetSetDAValue(PlcData.m_HdyPress.m_PressSetValue);

	CADODataSet DataThrshold;
	DataThrshold.SetConnection(::GetConnection());
	DataThrshold.Open(_T("select top 20 * from UserSetUp"));
	m_SLThreshold=DataThrshold.ReadFloatValue(_T("声级计阀值"));
	m_SmogSetValue=DataThrshold.ReadFloatValue(_T("烟雾阀值"));
	PlcData.m_HdyPress.m_PressSetOffset = DataThrshold.ReadFloatValue(_T("压力设定补偿"));
	if ((PlcData.m_HdyPress.m_PressSetOffset > 1.2) || (PlcData.m_HdyPress.m_PressSetOffset < -1.2))
		PlcData.m_HdyPress.m_PressSetOffset = 0;

	if(20>=m_SmogSetValue)//如果烟雾阀值设置为2，则自动进行采集
	{
		m_SmogThrMode=1;
		m_Smog_bigvalue=m_SmogSetValue;
	}

	m_PlateUp.ShowWindow(FALSE);//隐藏与显示按钮
	m_PlateDown.ShowWindow(FALSE);

	m_HdyForwardBtn.ShowWindow(FALSE);//隐藏与显示按钮
	m_HdyBackBtn.ShowWindow(FALSE);

	//备份文件
	::CreateDirectory("D:\\爆破数据备份",NULL);
	COleDateTime CurrTime;
	CurrTime=CurrTime.GetCurrentTime();
	CString StrTime;
	//StrTime=CurrTime.Format(_T("%Y-%m-%d"));
	StrTime=CurrTime.Format(_T("%Y-%m"));//每月备份一次
	StrTime=_T("D:\\爆破数据备份\\爆破检测系统")+StrTime+_T(".mdb");
	//::CopyFile("D:\\爆破项目\\BlastTest\\Samples\\Data\\爆破检测系统.mdb","D:\\爆破数据备份\\爆破检测系统.mdb",false); 
	//zhj delete::CopyFile("D:\\爆破项目\\BlastTest\\Samples\\Data\\爆破检测系统.mdb",StrTime,TRUE);

	CADODataSet DataSetRate;
	DataSetRate.SetConnection(::GetConnection());
	DataSetRate.Open(_T("select * from UserSetUp"));
	m_HunHightRata=DataSetRate.ReadFloatValue(_T("重锤高度系数"));
	if(m_HunHightRata>1.5)
	{
		m_HunHightRata=1;
		MessageBox(_T("读取重锤高度系数失败！"));
	}
	else if(m_HunHightRata<0.5)
	{
		m_HunHightRata=1;
		MessageBox(_T("读取重锤高度系数失败！"));
	}

	//读取防二次碰撞零点值
	m_CorTimerDelay=DataSet.ReadFloatValue("防二次碰撞延时");
	if((m_CorTimerDelay<=-2000) | (m_CorTimerDelay>=2000))
	{
		m_CorTimerDelay=0;
		DataSet.SetFieldValue(_T("防二次碰撞延时"),m_CorTimerDelay);
		DataSet.Save();
	}
	CString str;
	str.Format("%0.1f", m_SLThreshold);//zhj modify V1.03 改成显示一位小数
	//MessageBox(str);
	GetDlgItem(IDC_EDIT18)->SetWindowText(str);

	//((CComboBox *)GetDlgItem(IDC_TESTSEL))->AddString();
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//取得PLC数据的回调函数
DWORD WINAPI ReadPlcDataPro(LPVOID lpParameter )
{
	unsigned char SBuf[20];
	int RetValue;
	int StrLen, SLoop;
	CString str;
	while(TRUE)
	{	
		
		//PlcData.SoundLevel = PlcData.m_SoundComm.ReadSound();//读取声级计，根据IsPeakMode;

		if(WAIT_TIMEOUT==WaitForSingleObject(GEvent,1000))
			continue;
		switch(PlcData.ComFunIndex)
		{
		case COMM_NORMAL_READ:
			if(!GetOutBuffCount())
				RetValue=FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_SOUND_LEVEL_REAL,0x01, SBuf);//声级计
			if(RetValue==-2) 
			{
				SetEvent(GEvent);//将事件设置为有信号状态
				Sleep(200);//20150616由1000改为500，20150626添加
				continue;//说明没有响应
			}
			PlcData.SoundLevel=(float)FATEK_GetIntData(0,(char *)SBuf)/(10.0f);
			if(!GetOutBuffCount())
				RetValue = FATEK_ReadMultiReg(CMD_SLAVE_ADDR, CMD_ADDR_PRESS, 0x01, SBuf);//读取压力值
			if(RetValue==-2) 
			{
				SetEvent(GEvent);//将事件设置为有信号状态
				Sleep(200);//20150616由1000改为500，20150626添加
				continue;//说明没有响应
			}
			//PlcData.m_HdyPress.m_PressValue = (float)FATEK_GetIntData(0, (char *)SBuf) *8.0 / MAX_PRESSDAVALUE; //; (10.0f);
			//PlcData.m_HdyPress.m_PressValue = GetFloatData(0,(char *)SBuf);
			PlcData.m_HdyPress.m_PressValue = (float)FATEK_GetIntData(0, (char *)SBuf)/100.0; //; (10.0f);//20160812改成
			PlcData.m_HdyPress.m_PressValue = int(PlcData.m_HdyPress.m_PressValue * 100) / 100.0;//取两位小数

			if(!GetOutBuffCount())
				FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_HUN_REAL_HIGHT,0x01, SBuf);//平台高度
			//PlcData.PlateHight=(float)FATEK_GetIntData(0,(char *)SBuf)/(10.0f);
			//PlcData.PlateHight = (float)FATEK_GetIntData(0, (char *)SBuf);//20160809改成直接显示
			if (EquipmentType == TESTTYPE_ZHUJI)	PlcData.PlateHight = (float)FATEK_GetIntData(0, (char *)SBuf) / 10.f;//zhj modify V1.01
			//zhj modify V1.04  V1.08
		    else PlcData.PlateHight = (float)FATEK_GetIntData(0, (char *)SBuf);
			//else PlcData.PlateHight = (float)FATEK_GetIntData(0, (char *)SBuf)/10.f;

			if(!GetOutBuffCount())//读取烟雾传感器的实时值
				FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_SMOG_REAL,0x01, SBuf);//读取烟雾实时值

			//zhj modify v1.03 PlcData.SmogReal=(float)FATEK_GetIntData(0,(char *)SBuf)/(409.6f);   // x*10/4096   对应5V的传感器 10V传感器除以409.6f
			PlcData.SmogReal=(float)FATEK_GetIntData(0,(char *)SBuf);   


			
			if (PlcData.CallPressFun ==1)
			{
				//PlcData.m_HdyPress.PressFun();//调用液压加压程序
				PlcData.CallPressFun = 0;
			}

			if(!GetOutBuffCount())
				FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_GET_HUN_ERROR,0x01, SBuf);//抓捶次数

			if(FATEK_GetIntData(0,(char *)SBuf)>=3)
			{
				//AfxMessageBox(_T("无法抓锤，请检查相关部位是否有异常并重试！"));//20160812删除提示信息
				SBuf[0]='1';
				for(int Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)//复位系统
				{
					if(0<FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_SYSTEM_RESET,1,SBuf))
						break;
				}
			}
			break;
		case COMM_WRITE_HDY_DATA:
			//RetValue=FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_SOUND_LEVEL_REAL,0x01, SBuf);//声级计
			if (PlcData.m_HdyPress.m_HdyData.IsBuffEmpty())//如果发送缓冲区为空
			{
				PlcData.ComFunIndex = COMM_NORMAL_READ;
				PlcData.COmErrCount = 0;
				SetEvent(GEvent);//将事件设置为有信号状态
				continue;
			}
			switch (PlcData.m_HdyPress.m_HdyData.m_Cmd[PlcData.m_HdyPress.m_HdyData.m_SendPtr])
			{
			case CMD_Moror:   /*电机开关*/
				SBuf[0] = (char)(PlcData.m_HdyPress.m_HdyData.m_Data[PlcData.m_HdyPress.m_HdyData.m_SendPtr])+48;
				RetValue=FATEK_WriteMultiByte(CMD_SLAVE_ADDR, CMD_ADDR_MOTOR, 1, SBuf);
				break;

			case CMD_FwVal:/*前进电磁阀*/
				SBuf[0] = (char)(PlcData.m_HdyPress.m_HdyData.m_Data[PlcData.m_HdyPress.m_HdyData.m_SendPtr])+48;
				RetValue = FATEK_WriteMultiByte(CMD_SLAVE_ADDR, CMD_ADDR_FWVAL, 1, SBuf);
				break;
			case CMD_BackVal: /*后退电磁阀*/
				SBuf[0] = (char)(PlcData.m_HdyPress.m_HdyData.m_Data[PlcData.m_HdyPress.m_HdyData.m_SendPtr])+48;
				RetValue = FATEK_WriteMultiByte(CMD_SLAVE_ADDR, CMD_ADDR_BACKVAL, 1, SBuf);
				break;

			case CMD_AccVal:/*蓄能器电磁阀*/
				SBuf[0] = (char)(PlcData.m_HdyPress.m_HdyData.m_Data[PlcData.m_HdyPress.m_HdyData.m_SendPtr])+48;
				RetValue = FATEK_WriteMultiByte(CMD_SLAVE_ADDR, CMD_ADDR_ACCVAL, 1, SBuf);
				break;
			case CMD_DAVALUE:/*写入DA值*/
				/*str.Format("%2X", PlcData.m_HdyPress.m_HdyData.m_Data[PlcData.m_HdyPress.m_HdyData.m_SendPtr]);//从机地址转换为16进制字符串
				str.Replace(' ', '0');
				memset(SBuf, '0', 10);
				StrLen = str.GetLength();
				if (StrLen>4) StrLen = 4;
				for (SLoop = 0; SLoop<StrLen; SLoop++)
				{
					SBuf[3 - SLoop] = str.GetAt(StrLen - 1 - SLoop);
				}
				RetValue= FATEK_WriteMultiReg(CMD_SLAVE_ADDR, CMD_ADDR_DAVALUE, 1, SBuf);//返回值大于0，才说明写入成功
				*/
				RetValue = 1;//20160812新增临时
				break;
			default:
				PlcData.ComFunIndex = COMM_NORMAL_READ;
				PlcData.COmErrCount = 0;
				SetEvent(GEvent);//将事件设置为有信号状态
				break;
			}
			if(RetValue<0) //通信失败
			{
				PlcData.COmErrCount++;
				SetEvent(GEvent);//将事件设置为有信号状态
				Sleep(30);
				continue;//说明没有响应
			}
			else//通信成功
			{
				PlcData.COmErrCount=0;
				PlcData.m_HdyPress.m_HdyData.AddSendPtr();
			}
				
			if (PlcData.COmErrCount>5)//通信错误次数大于五次
			{
				AfxMessageBox(_T("写入液压系统参数失败！"));
				PlcData.ComFunIndex=COMM_NORMAL_READ;
				PlcData.COmErrCount=0;
			}

			//PlcData.ComFunIndex=COMM_NORMAL_READ;//写入完成后返回到读取实时数据
			break;

		default:
			PlcData.ComFunIndex=COMM_NORMAL_READ;
			break;
		}
		SetEvent(GEvent);//将事件设置为有信号状态

		//FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_PLATE_HIGHT_REAL,0,SBuf);
		//PlcData.PlateHight;
		//Sleep(200);//20150616由1000改为500
	}
	return 0;
}
void CBlastTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBlastTestDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{
		//CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	CBitmap bmp;
	if (EquipmentType==TESTTYPE_ZHUJI)	bmp.LoadBitmap(IDB_BACKIMG);//zhj modify V1.01
	else bmp.LoadBitmap(IDB_BITMAP_MC);
	BITMAP bitmap;
	bmp.GetBitmap(&bitmap);
	CDC bmpdc;
	bmpdc.CreateCompatibleDC(&dc);
	bmpdc.SelectObject(&bmp);
	CRect rect;
	//显示在整个窗体区域
	this->GetClientRect(&rect);
	////显示在图片控件中
	//GetDlgItem(IDC_LOGINIMG)->GetWindowRect(&rect);
	//ScreenToClient(&rect);//显示在整个窗体区域不需要该语句
	//显示在整个窗体区域
	dc.StretchBlt(rect.left, rect.top, rect.right, rect.bottom, &bmpdc,
		rect.left, rect.top, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	SetTimer(2,500,NULL);//用于更新Data的现实,更新完一次之后自动关闭定时器
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBlastTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBlastTestDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CBlastTestDlg::OnQuitWindows() //---已无效
{
	// TODO: Add your control notification handler code here
	SendMessage(WM_CLOSE);
	CDialog::OnClose();
}

HBRUSH CBlastTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if ((pWnd->GetDlgCtrlID() == IDC_EDIT1) | (pWnd->GetDlgCtrlID() == IDC_EDIT2) |
		(pWnd->GetDlgCtrlID() == IDC_EDIT3) | (pWnd->GetDlgCtrlID() == IDC_EDIT4) |
		(pWnd->GetDlgCtrlID() == IDC_EDIT5) | (pWnd->GetDlgCtrlID() == IDC_EDIT6) |
		(pWnd->GetDlgCtrlID() == IDC_EDIT8) | (pWnd->GetDlgCtrlID() == IDC_EDIT9) |
		(pWnd->GetDlgCtrlID() == IDC_EDIT10) | (pWnd->GetDlgCtrlID() == IDC_EDIT11) |
		(pWnd->GetDlgCtrlID() == IDC_EDIT12) | (pWnd->GetDlgCtrlID() == IDC_EDIT13) |
		(pWnd->GetDlgCtrlID() == IDC_EDIT14) |(pWnd->GetDlgCtrlID() == IDC_SMOGREAL) |
		(pWnd->GetDlgCtrlID() == IDC_SMOGSET))
	{
		//pDC->SetBkMode(TRANSPARENT);   //设置背景透明,改语句暂时不明确意思
	
		//pDC->SetTextColor(RGB(100,255,255));//设置字体颜色
		pDC->SetBkColor(RGB(255,255,255));//20150616增加该语句及其EDIT2\EDIT3的条件判断

		return   HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	if (pWnd->GetDlgCtrlID() == IDC_TIMEDIS)
	{
		//pDC->SetBkMode(TRANSPARENT);   //设置背景透明,改语句暂时不明确意思
		pDC->SetBkColor(RGB(244,244,245));
		//pDC->SetTextColor(RGB(100,255,255));//设置字体颜色
		
		return   HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
//点击了查看记录按钮------无效
void CBlastTestDlg::OnBtnReport() 
{
	// TODO: Add your control notification handler code here
	static bool btn_index=0;
	btn_index=!btn_index;
	if(btn_index)
		m_BtnReport.LoadBitmaps(IDB_BTN_QUITWINDOWS);
	else
		m_BtnReport.LoadBitmaps(IDB_BITOPENREPORT);
	//m_BtnReport.SubclassDlgItem(IDC_BTN_REPORT,this);
	m_BtnReport.SizeToContent();
	m_BtnReport.RedrawWindow();
}

void CBlastTestDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
//	unsigned char SBuf[20];
	//FATEK_ReadMultiByte(CMD_SLAVE_ADDR,"X0050",0x06, SBuf);

	if(1==nIDEvent)
	{
		//FATEK_ReadMultiReg(CMD_SLAVE_ADDR,"D00302",0x01, SBuf);//声级计
		//CString str=SBuf;
		//int sound=htoi(str.Mid(0,4));
		//str.Format("%.1f",PlcData.PlateHight);
		//GetDlgItem(IDC_PLATEHIGHT)->SetWindowText(str);

		COleDateTime CurrTime;
		CurrTime=CurrTime.GetCurrentTime();
		m_TimeDis=CurrTime.Format(_T("%Y-%m-%d\r\n%H:%M:%S"));

		m_SondLevel=PlcData.SoundLevel;
		m_PlateHight=PlcData.PlateHight;
		if(m_SondLevel>140) m_SondLevel=0;
		if(m_HunReleaseBtn)//如果正在放锤
		{
			if(m_SmogReal<PlcData.SmogReal)//保存烟雾的最大值
				m_SmogReal=(int(PlcData.SmogReal*10))/10.0f;
		}
		else
		{
			m_SmogReal=(int(PlcData.SmogReal*10))/10.0f;
			if(m_SmogThrMode==1)//自动采集烟雾阀值模式
			{
				m_SmogSetValue=m_SmogReal+m_Smog_bigvalue;
			}
		}
		//修改高度显示20150625
		//m_PlateHight=m_ZeroLevelHight-m_PlateHight;
		if(m_TestSpInBtn==0)//试样确认之后才会开始更新屏幕数据
		{
			CString str;
			GetDlgItem(IDC_EDIT7)->GetWindowText(str);//药品质量为空，不更新
			if(str!=_T(""))
			{
				if(isdigit(str.GetAt(str.GetLength()-1)))//如果输入的是数字
					UpdateData(FALSE);
			}
		}

	}
	if(2==nIDEvent)//更新DataGrid的显示
	{
		KillTimer(2);

		C_Recordset RSet=m_Ado_OPRecord.GetRecordset();
		RSet.Requery(adConnectUnspecified);
		m_DG_OpRecord.ReBind();                        //实现与datagrid中的表格绑定 
		m_DG_OpRecord.Refresh(); 

		RSet=m_Ado1.GetRecordset();
		RSet.Requery(adConnectUnspecified);
		m_DataGrid1.ReBind();                        //实现与datagrid中的表格绑定 
		m_DataGrid1.Refresh(); 

		
	}
	if(3==nIDEvent)////用于液压加压
	{
		if (m_HunReleaseBtn)//如果正在放锤
		{
			PlcData.CallPressFun = 1;
		}
	}
	if(4==nIDEvent)//读取放捶状态，1s调用一次
	{
		KillTimer(4);
		//MessageBox(_T("请确保声级计已经打开！"),_T("声级计打开提示"));

		SetTimer(1,500,NULL);//用于更新串口数据显示
		SetTimer(2,500,NULL);//用于更新Data的现实,更新完一次之后自动关闭定时器	
		SetTimer(3, 500, NULL);//用于液压加压	
	}

 
	CDialog::OnTimer(nIDEvent);
}

BOOL CBlastTestDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}
//写入记录
void CBlastTestDlg::WriteLog(CString SData)
{
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from OperateRecord "));
	COleDateTime CurrTime;
	CurrTime=CurrTime.GetCurrentTime();
	//CString StrTime;
	//StrTime=CurrTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	DataSet.AddNew();
	DataSet.SetFieldValue(_T("操作时间"),(_variant_t)CurrTime);
	DataSet.SetFieldValue(_T("操作事项"),(_variant_t)SData);
	DataSet.Save();
	DataSet.Open(_T("select * from OperateRecord "));//再次查询，无效查询，指示为了马上能够更新DataGrid的显示。
	RefreshDGrid(m_Ado_OPRecord,m_DG_OpRecord,_T("SELECT  top 50 * FROM OperateRecord order by 操作时间 DESC"));
}
void CBlastTestDlg::SetParperBtnBit(char sw)
{
	if(0==sw)//0-装药完成,1-启动装药
	{
		m_BtnPrepar.LoadBitmaps(IDB_PREPAR);
		m_BtnRealse.LoadBitmaps(IDB_REALSE);//重锤释放按钮变灰
		WriteLog(_T("装药完成"));
	}
	else
	{
		m_BtnPrepar.LoadBitmaps(IDB_PREPARFINISH);
		m_BtnRealse.LoadBitmaps(IDB_RELEASEGRAY);//重锤释放按钮变为可操作状态
		WriteLog(_T("开始装药"));
	}
	//m_BtnReport.SubclassDlgItem(IDC_BTN_REPORT,this);
	m_BtnRealse.SizeToContent();
	m_BtnRealse.RedrawWindow();
	
	m_BtnPrepar.SizeToContent();
	m_BtnPrepar.RedrawWindow();
}
//装药与完成按钮
void CBlastTestDlg::OnBtnPraper() 
{
	if (isAddRdcord)
	{
		AddRecord(true);
		return;
	}
	// TODO: Add your control notification handler code here
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	if(m_HunReleaseBtn)//正在放锤
	{
		MessageBox(_T("正在放锤，无法装药"));
		return;
	}
	if(m_TestSpInBtn==1)
	{
		MessageBox(_T("请先确认实验参数"));
		return;
	}
	if(m_TestSpNo==_T(""))
	{
		MessageBox(_T("试样编号不能为空"));
		return;
	}
	if(m_PlateMode)//手动平台无法装药
	{
		MessageBox(_T("手动平台无法装药！"));
		return;
	}

	m_PlaceExpPrepar=!m_PlaceExpPrepar;
	//SetParperBtnBit(m_PlaceExpPrepar);//改变图片显示
	//清除放捶
	if(!WriteByte(CMD_SLAVE_ADDR,CMD_HUN_RELESAE,0))
	{
		MessageBox(_T("清除放锤标志失败！"));
	}
	
	if(!WriteByte(CMD_SLAVE_ADDR,CMD_PLACE_EXPLOSIVE,m_PlaceExpPrepar))
	{
		m_PlaceExpPrepar=!m_PlaceExpPrepar;//将指示变回来
	    //MessageBox(_T("与PLC通信失败！"));
		WriteLog(_T("与PLC通信失败！"));
		return ;
	}
	//v1.09add
	if (!m_PlaceExpPrepar)
	{
		if (!WriteByte(CMD_SLAVE_ADDR, CMD_JIAYA, 1))
		{
			//MessageBox(_T("与PLC通信失败！"));
			WriteLog(_T("与PLC通信失败！"));
		}
	}
	SetParperBtnBit(m_PlaceExpPrepar);//改变图片显示
	SetBlastIndexBit(0);//改变爆炸状态指示灯的显示为灰色
			
	
}
//刷新数据库显示
void CBlastTestDlg::RefreshDGrid(CAdodc & Ado,CDataGrid & DGrid,CString Sql)
{
	C_Recordset RSet=Ado.GetRecordset();
	//long lCount = oRecord.GetRecordCount();
	//VARIANT oSource = oRecord.GetSource();
	CString strConnect = Ado.GetConnectionString();
	long lCursorType = RSet.GetCursorType();
	long lLockType = RSet.GetLockType();
	COleVariant oleVar(Sql);
	RSet.Close();
	RSet.Open(oleVar,COleVariant(strConnect),lCursorType, lLockType, adConnectUnspecified );
	RSet.Requery(adConnectUnspecified);
	DGrid.Refresh();
}
//样品数据按钮
void CBlastTestDlg::OnTestSpBtn() 
{
	// TODO: Add your control notification handler code here
	CTestSpData dlg;
	if(m_HunReleaseBtn==1)//正在放锤
	{
		MessageBox(_T("请等待放锤完成！"));
		return;
	}
	dlg.DoModal();
}
//实验条件设置
void CBlastTestDlg::OnTestcondsetup() 
{
	// TODO: Add your control notification handler code here
	CTestConditon dlg;

	if(m_HunReleaseBtn==1)//正在放锤
	{
		MessageBox(_T("请等待放锤完成！"));
		return;
	}

	dlg.DoModal();
	//初始化试样选择下拉列表
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from TestConditionSetup"));
	m_HunchKg=DataSet.ReadFloatValue(_T("重锤质量"));
	PlcData.m_HdyPress.m_PressSetValue = dlg.m_PressSet;
	m_TestType=dlg.m_Type;
	SetTestMode();//设置当前的测试方式
	
}
//决定编辑框是否可写入；0--不可写入；1--可写入
void CBlastTestDlg::EditWriteAble(bool sw)
{
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT2))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT3))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT4))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT5))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT6))->SetReadOnly(!sw);
	//((CEdit *)GetDlgItem(IDC_EDIT7))->SetReadOnly(!sw);//药品质量始终可编辑
	((CEdit *)GetDlgItem(IDC_EDIT8))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT9))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT10))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT11))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT12))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT13))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT14))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_SMOGREAL))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_SMOGSET))->SetReadOnly(!sw);
	
}
//样品数据载入按钮图片
void CBlastTestDlg::SetTestSpInBtnBit(char sw)
{
	//载入实验数据
	if(sw)
		m_TestSpIn.LoadBitmaps(IDB_TESTSPINOKBTN);
	else
		m_TestSpIn.LoadBitmaps(IDB_TestSpIn);
	m_TestSpIn.SizeToContent();
	m_TestSpIn.RedrawWindow();
}
//样品数据载入按钮
void CBlastTestDlg::OnTestSpInBtn() 
{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	
	bool SPExist=0;//为1表示之前已经存在试验记录
	//PlaySound(_T("C:\\Sound\\SoundTest.wav"),NULL, SND_ASYNC|SND_NODEFAULT );//播放测试音
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	
	if(SetTestMode()==INVALID_TEST_MODE)
	{
		MessageBox(_T("不支持的试验类型"));
		return;
	}
	if ((PlcData.m_HdyPress.m_PressSetValue>6.3) || (PlcData.m_HdyPress.m_PressSetValue<0))
	{
		MessageBox(_T("压力设定值必须在6.3MP到0MP之间"));
		return;
	}

	int nSel;   	
    // 获取组合框控件的列表框中选中项的索引   
    nSel = ((CComboBox *)GetDlgItem(IDC_TESTSEL))->GetCurSel();   
    // 根据选中项索引获取该项字符串   
	if(nSel>=0)
		((CComboBox *)GetDlgItem(IDC_TESTSEL))->GetLBText(nSel, m_TestSel);  
	UpdateData(TRUE);
	DataSet.Open(_T("select * from SampleTable where 试样编号='")+m_TestSel+_T("'"));
	if(DataSet.GetRecordCount()<1)
	{
		MessageBox(_T("无此试样"));
		return ;
	}

	if(m_TestSpInBtn==0)//操作的是载入按钮
	{		
		m_TestSpName=DataSet.ReadStringValue(_T("样品名称"));
		m_TestSpNo=m_TestSel;
		
		DataSet.Open(_T("select * from TestConditionSetup"));
		m_TestGroupNo=DataSet.ReadIntValue(_T("实验组数"));
		m_TestNo=DataSet.ReadIntValue(_T("每组次数"));
		m_TestSpKg=DataSet.ReadFloatValue(_T("试样质量"));
		m_HunchHight=DataSet.ReadFloatValue(_T("重锤高度"));
		m_HunchKg=DataSet.ReadFloatValue(_T("重锤质量"));
		m_TestType=DataSet.ReadStringValue(_T("实验类型"));
		m_TestPerName=DataSet.ReadStringValue(_T("实验员"));
		m_TestTemp=DataSet.ReadFloatValue(_T("实验温度"));
		m_TestHumi=DataSet.ReadFloatValue(_T("实验湿度"));
		m_StepLength=DataSet.ReadFloatValue(_T("步长设置"));
		UpdateData(FALSE);
		m_TestSpInBtn=TRUE;
		/*打开编辑框的输入功能*/
		EditWriteAble(m_TestSpInBtn);
		WriteLog(_T("载入数据"));

		m_MaxGroupNo=m_TestGroupNo;
		m_MaxTestNo=m_TestNo;

		if(m_Testmode==CRT_HEIGHT_MODE)
		{
			if(m_MaxGroupNo>1)
			{
				MessageBox(_T("特性落高法只支持做一组实验!"));
				m_MaxGroupNo=1;
				m_TestGroupNo=1;
			}
		}

#ifdef _DEBUG

#endif

		if(m_Testmode==CRT_HEIGHT_MODE)//特性落高法试验
		{

		}
	}
	else if(m_TestSpInBtn==1)//操作的是确认按钮
	{
		DataSet.Open(_T("select * from TestData where 试样编号='")+m_TestSel+_T("' order by 实验日期 desc") );//查询实验记录中是否存在该样品的记录
		//20151021增加排序
		if(DataSet.GetRecordCount()>(m_MaxGroupNo*m_MaxTestNo))
		{
			MessageBox(_T("该试样的实验已经完成！无法继续！"));
			return ;
		}

		if(DataSet.GetRecordCount()>0)
		{
			if(IDOK==AfxMessageBox(_T("已经存在该样品的记录，选择确认将删除掉之前的记录，选择取消继续上次进行，否则请更改试样编号。"),MB_OKCANCEL))//点击的是确认按钮
			{
				//DataSet.Delete();
				DataSet.ClearRecord(_T("select * from TestData where 试样编号='")+m_TestSel+_T("'"));
				//DataSet.Save();//将之前的记录都删除掉
				m_TestSpInBtn=0;
			}
			else  //取消按钮
			{
				//将剩余次数相减
				m_TestGroupNo=m_MaxGroupNo-(short)floor((DataSet.GetRecordCount()/((float)m_MaxTestNo)));

				m_TestNo=m_MaxTestNo-(DataSet.GetRecordCount() % m_MaxTestNo);
				//m_TestSpNo=_T("");//样品编号赋值为空
				//m_TestSel=_T("");
				//UpdateData(FALSE);
				m_TestSpInBtn=0;//否则开始重新载入
				
				DataSet.moveFirst();//20151021增加
				m_TestType=DataSet.ReadStringValue(_T("实验类型"));
				m_HunchHight=DataSet.ReadFloatValue(_T("重锤高度"));
				m_LastBlastStatus=DataSet.ReadIntValue(_T("爆炸状态"));

				SetTestMode();

				//计算实时爆炸率 20150625
				if(m_Testmode==BLAST_RATE_MODE)//爆炸概率法
				{
					CADODataSet DataRate;
					DataRate.SetConnection(::GetConnection());
					DataRate.Open(_T("select top 500 * from TestData where 试样编号='")+m_TestSpNo+_T("'"));
					int AllCount=DataRate.GetRecordCount();
					DataRate.Open(_T("select top 500 * from TestData where 试样编号='")+m_TestSpNo+_T("' and 爆炸状态=1"));
					m_BlastRate=(100*DataRate.GetRecordCount())/AllCount;
				}

				if(m_Testmode==CRT_HEIGHT_MODE)//特性落高法试验，读取重锤高度
				{
					m_CRTHeight.m_CurrHeight=m_HunchHight;
					//计算新高度
					m_CRTTestStep=CH_STEEP_FINISH;
					CRTCalNextH(m_LastBlastStatus);	

				}
				SPExist=TRUE;
			}
			
		}
		else  //不存在该样品的实验记录
		{
			m_TestSpInBtn=0;//否则开始重新载入

		}

	//	CalTestResult();//测试20150626


		//将重锤高度写入PLC
		char SBuf[10];
		CString str;
		int Hight = (int)(m_HunchHight * 10);
		//zhj delete V1.11 int Hight=(int)(m_HunchHight*m_HunHightRata*10);//20150607添加高度系数
		//zhj delete V1.10 int Hight = (int)(m_HunchHight*m_HunHightRata);//20150809取消乘10
		str.Format("%2X",Hight);//从机地址转换为16进制字符串
		str.Replace(' ','0');
		memset(SBuf,'0',10);
		int StrLen=str.GetLength();
		if(StrLen>4) StrLen=4;
		for(int SLoop=0;SLoop<StrLen;SLoop++)
		{
			SBuf[3-SLoop]=str.GetAt(StrLen-1-SLoop);
		}

		WaitForSingleObject(GEvent,1000);
		int Fatek;
		for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
		{
			if(0<FATEK_WriteMultiReg(CMD_SLAVE_ADDR,CMD_HUN_HEIGHT,1,(unsigned char *)SBuf))//返回值大于0，才说明写入成功
				break;
		}
		SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
		if (Fatek>=CMD_SEND_REPART_COUNT)
		{
			MessageBox(_T("写入重锤高度失败！"));
		}
		//写入零点高度
		//int HunZero=int(ReadHunZero(m_HunchKg)*10);
		int HunZero = int(ReadHunZero(m_HunchKg));//20160812改成不乘10
		WriteInt(CMD_SLAVE_ADDR,CMD_HUN_ZEROPOINT,HunZero);
		//写入二次碰撞延时
		//WriteInt(CMD_SLAVE_ADDR,CMD_COR_TIMER_DELAY,(int)m_CorTimerDelay);
		//写入压力设定,转换到DA值
		//WriteInt(CMD_SLAVE_ADDR, CMD_ADDR_PRESSSET, (int)((PlcData.m_HdyPress.m_PressSetValue + PlcData.m_HdyPress.m_PressSetOffset)*MAX_PRESSDAVALUE / 8.0));
	    WriteInt(CMD_SLAVE_ADDR, CMD_ADDR_PRESSSET, (int)((PlcData.m_HdyPress.m_PressSetValue + PlcData.m_HdyPress.m_PressSetOffset)*100));//20160812需改
		//WriteFloat(CMD_SLAVE_ADDR, CMD_ADDR_PRESSSET, PlcData.m_HdyPress.m_PressSetValue);//20160808需改
		DataSet.Open(_T("select * from SampleTable where 试样编号='")+m_TestSel+_T("'"));//将数据写入样品数据库
		if(DataSet.GetRecordCount()<=0)
		{
			MessageBox(_T("样品数据中无该样品的记录，请输入之后重新确认！"));
		}
		else
		{
			DataSet.SetFieldValue(_T("落锤质量"),m_HunchKg);
			DataSet.SetFieldValue(_T("落高"),m_HunchHight);
			DataSet.SetFieldValue(_T("药量"),m_TestSpKg);
			if (EquipmentType == TESTTYPE_MOCHA) DataSet.SetFieldValue(_T("加压压力"), PlcData.m_HdyPress.m_PressSetValue);
			DataSet.Save();

		}

		WriteLog(_T("确认数据"));
		/*锁定编辑框的输入功能*/
		EditWriteAble(m_TestSpInBtn);

		if(m_Testmode==CRT_HEIGHT_MODE)//特性落高法试验
		{
			m_CRTHeight.m_HeightSet=m_HunchHight;
			m_CRTHeight.m_LastHeight=m_HunchHight;
			if(SPExist==0)
			{
				m_CRTHeight.m_CurrHeight=m_HunchHight;
				m_CRTTestStep=CH_STEEP_BEGIN;
			}
			else
				m_CRTTestStep=CH_STEEP_FINISH;//之前已经存在试验记录，说明预备试验已经完成，直接开始实验

		}

	}
	SetTestSpInBtnBit(m_TestSpInBtn);
	
}
//打开实验记录对话框
void CBlastTestDlg::OnRecordView() 
{
	// TODO: Add your control notification handler code here
	//CPrintReportDlg dlg;
	//dlg.DoModal();
	CTestRecordView dlg;
	dlg.m_SpNo=m_TestSpNo;
	dlg.m_Type=m_TestType;

	if(m_HunReleaseBtn==1)//正在放锤
	{
		MessageBox(_T("请等待放锤完成！"));
		return;
	}

	dlg.DoModal();
	//重新计算剩余组数，防止有记录被删除
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	if(m_TestSpNo.IsEmpty()) return;   
	DataSet.Open(_T("select * from TestData where 试样编号='")+m_TestSpNo+_T("'"));//查询实验记录中是否存在该样品的记录
	m_TestGroupNo=m_MaxGroupNo-(short)floor((DataSet.GetRecordCount()/((float)m_MaxTestNo)));		
	m_TestNo=m_MaxTestNo-(DataSet.GetRecordCount()% m_MaxTestNo);
	//防止判爆进行了修改，如果在特性落高法的预备试验，重新计算高度
	DataSet.Open(_T("select * from TestData where 试样编号='")+m_TestSpNo+_T("' order by 实验日期 desc"));
	if((m_Testmode==CRT_HEIGHT_MODE))
	{
		if(DataSet.GetRecordCount()>0)
		{
			DataSet.moveFirst();
			if(m_CurrBlastStatus!=DataSet.ReadIntValue(_T("爆炸状态")))
			{
				m_CurrBlastStatus=DataSet.ReadIntValue(_T("爆炸状态"));
				if(m_CRTTestStep!=m_CRTTestLastStep)
				{
					if(m_CRTTestStep>0) m_CRTTestStep--;//回到上一步	
				}
				m_CRTHeight.m_CurrHeight=m_HunchHight;
				CRTCalNextH(m_CurrBlastStatus);
			
			}
		}
	}
	UpdateData(FALSE);

}
//手动与自动平台之间选择
void CBlastTestDlg::OnOpplatebtn() 
{
	// TODO: Add your control notification handler code here
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);

	WriteByte(CMD_SLAVE_ADDR,CMD_SYSTEM_RESET,1);//复位系统20150628
	Sleep(100);//等待PLC复位完成。

	//复位装药与放锤按钮20150628
	m_HunReleaseBtn=0;
	SetHunBtnBit(m_HunReleaseBtn);
	m_PlaceExpPrepar=0;
	SetParperBtnBit(m_PlaceExpPrepar);

	//m_PlateMode=!m_PlateMode;
	char PlatModeOld = m_PlateMode;
	m_PlateMode += 1;
	if (EquipmentType == TESTTYPE_ZHUJI) m_PlateMode %= 2;
	else m_PlateMode %= 3;

	if (m_PlateMode == 2)//手动加压
	{
		m_PlateUp.ShowWindow(FALSE);//隐藏与显示按钮
		m_PlateDown.ShowWindow(FALSE);
		m_BtnPrepar.ShowWindow(FALSE);
		m_BtnRealse.ShowWindow(FALSE);
		m_HdyForwardBtn.ShowWindow(TRUE);
		m_HdyBackBtn.ShowWindow(TRUE);
	}
	else
	{
		sprintf((char *)SBuf, _T("%d"), m_PlateMode);
		WaitForSingleObject(GEvent, 1000);//等待信号
		int Fatek;
		for (Fatek = 0; Fatek<CMD_SEND_REPART_COUNT; Fatek++)//写入PLC
		{
			if (0<FATEK_WriteMultiByte(CMD_SLAVE_ADDR, CMD_PLATE_MODE_SWITCH, 1, SBuf))//返回值大于0，才说明写入成功
				break;
		}
		SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
		if (Fatek >= CMD_SEND_REPART_COUNT)
		{
			m_PlateMode = PlatModeOld;
			//MessageBox(_T("与PLC通信失败！"));
			WriteLog(_T("与PLC通信失败！"));
			return;
		}
	}

	if (m_PlateMode == 1)/*手动平台*/
	{
		if (EquipmentType == TESTTYPE_ZHUJI) m_PlateModeBtn.LoadBitmaps(IDB_MANUALPLATEBTN_ZJ);
		else m_PlateModeBtn.LoadBitmaps(IDB_MANULPLATEBTN);

		
	}
	else if (m_PlateMode == 2)/*手动加压*/
	{
		m_PlateModeBtn.LoadBitmaps(IDB_MANULPRESS);
	
	}
	else /*自动平台*/
	{
		if (EquipmentType == TESTTYPE_ZHUJI) m_PlateModeBtn.LoadBitmaps(IDB_AUTOPLATEBTN_ZJ);
		else m_PlateModeBtn.LoadBitmaps(IDB_AUTOPLATEBTN);

	}
	m_PlateModeBtn.SizeToContent();
	m_PlateModeBtn.RedrawWindow();

	WriteByte(CMD_SLAVE_ADDR,CMD_HUN_RELESAE,0);//清除放捶指令
	WriteByte(CMD_SLAVE_ADDR,CMD_PLACE_EXPLOSIVE,0);//请装药标志位

	//清上升和下降标志位
/*
	SBuf[0]='0';
	WaitForSingleObject(GEvent,1000);//等待信号
	FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_PLATE_DOWN,1,SBuf);
	FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_PLATE_UP,1,SBuf);
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据*/

	//m_BtnReport.SubclassDlgItem(IDC_BTN_REPORT,this);
	
	SBuf[0]='0';
	//下降平台按钮复位
	m_PlateDownState=0;
	m_PlateDown.LoadBitmaps(IDB_PLATEDOWN);
	m_PlateDown.SizeToContent();
	m_PlateDown.RedrawWindow();
	WaitForSingleObject(GEvent,1000);//等待信号
	FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_PLATE_DOWN,1,SBuf);
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
	//上升平台按钮复位
	m_PlateUpState=0;
	m_PlateUp.LoadBitmaps(IDB_PLATEUP);
	m_PlateUp.SizeToContent();
	m_PlateUp.RedrawWindow();
	SBuf[0]='0';
	WaitForSingleObject(GEvent,1000);//等待信号
	FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_PLATE_UP,1,SBuf);
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
	
	if(m_PlateMode==1)
	{
		m_PlateUp.ShowWindow(TRUE);//隐藏与显示按钮
		m_PlateDown.ShowWindow(TRUE);
		m_BtnPrepar.ShowWindow(FALSE);
		m_BtnRealse.ShowWindow(FALSE);
		m_HdyForwardBtn.ShowWindow(FALSE);
		m_HdyBackBtn.ShowWindow(FALSE);
		WriteLog("切换至手动平台");
		
	}
	else if (m_PlateMode ==2)//加压
	{
		WriteLog("切换至手动平台");
	}
	else
	{
		m_PlateUp.ShowWindow(FALSE);//隐藏与显示按钮
		m_PlateDown.ShowWindow(FALSE);
		m_BtnPrepar.ShowWindow(TRUE);
		m_BtnRealse.ShowWindow(TRUE);
		m_HdyForwardBtn.ShowWindow(FALSE);
		m_HdyBackBtn.ShowWindow(FALSE);
		WriteLog("切换至自动平台");
	}
}
//平台上升操作
void CBlastTestDlg::OnPlateUp() 
{
	// TODO: Add your control notification handler code here
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	if(!m_PlateMode)//如果是自动模式
	{
		MessageBox(_T("请先切换到手动模式"));
		return;
	}
	if(m_PlateDownState)//如果正在下降
	{
		MessageBox(_T("请先停止下降操作"));
		return;
	}
	m_PlateUpState=!m_PlateUpState;
	sprintf((char *)SBuf,_T("%d"),m_PlateUpState);
	WaitForSingleObject(GEvent,1000);//等待信号
	int Fatek;
	for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_PLATE_UP,1,SBuf))//返回值大于0，才说明写入成功
			break;
	}
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据

	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		m_PlateUpState=!m_PlateUpState;//将指示变回来
		//MessageBox(_T("与PLC通信失败！"));
		WriteLog(_T("与PLC通信失败！"));
		return;
	}

	if(m_PlateUpState)
	{
		m_PlateUp.LoadBitmaps(IDB_PLATEUPSTOP);
	}
	else
	{
		m_PlateUp.LoadBitmaps(IDB_PLATEUP);
	}
		
	m_PlateUp.SizeToContent();
	m_PlateUp.RedrawWindow();
}
//平台下降
void CBlastTestDlg::OnPlateDown() 
{
	// TODO: Add your control notification handler code here
	//int i=GetFocus()->GetDlgCtrlID();//获得所点击的按钮的ID
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	if(!m_PlateMode)//如果是自动模式
	{
		MessageBox(_T("请先切换到手动模式"));
		return;
	}
	if(m_PlateUpState)//如果正在下降
	{
		MessageBox(_T("请先停止上升操作"));
		return;
	}
	m_PlateDownState=!m_PlateDownState;
	sprintf((char *)SBuf,_T("%d"),m_PlateDownState);
	WaitForSingleObject(GEvent,1000);//等待信号
	int Fatek;
	for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_PLATE_DOWN,1,SBuf))//返回值大于0，才说明写入成功
			break;
	}
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		m_PlateDownState=!m_PlateDownState;//将指示变回来
		//MessageBox(_T("与PLC通信失败！"));
		WriteLog(_T("与PLC通信失败！"));
		return;
	}

	if(m_PlateDownState)
	{
		m_PlateDown.LoadBitmaps(IDB_PLATEDOWNSTOP);
	}
	else
	{
		m_PlateDown.LoadBitmaps(IDB_PLATEDOWN);
	}
	
	m_PlateDown.SizeToContent();
	m_PlateDown.RedrawWindow();	
}
//鼠标左键按下
void CBlastTestDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
/*	int id=GetFocus()->GetDlgCtrlID();//获得所点击的按钮的ID
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	SBuf[0]='1';
	if(m_PlateMode)//手动模式
	{
		if(id==IDC_PLATE_UP)//平台上升按钮被按下
		{
			FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0300"),1,SBuf);
		}
		else if(id==IDC_PLATE_DOWN)//平台下降按钮被按下
		{
			FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0302"),1,SBuf);
		}
	}*/

	CDialog::OnLButtonDown(nFlags, point);
}
//鼠标左键释放
void CBlastTestDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	/*int id=GetFocus()->GetDlgCtrlID();//获得所点击的按钮的ID
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	SBuf[0]='0';
	if(m_PlateMode)//手动模式
	{
		if(id==IDC_PLATE_UP)//平台上升按钮被释放
		{
			FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0300"),1,SBuf);
		}
		else if(id==IDC_PLATE_DOWN)//平台下降按钮被释放
		{
			FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0302"),1,SBuf);
		}
	}*/

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CBlastTestDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	/*unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	int id=GetFocus()->GetDlgCtrlID();//获得所点击的按钮的ID
	if(pMsg->message==WM_LBUTTONDOWN)
	{
		SBuf[0]='1';
		if(m_PlateMode)//手动模式
		{
			if(id==IDC_PLATE_UP)//平台上升按钮被按下
			{
				FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0300"),1,SBuf);
				WriteLog(_T("平台上升按下"));
			}
			else if(id==IDC_PLATE_DOWN)//平台下降按钮被按下
			{
				FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0302"),1,SBuf);
				WriteLog(_T("平台下降按下"));
			}
		}	
	}
	if(pMsg->message==WM_LBUTTONUP)
	{
		SBuf[0]='0';
		if(m_PlateMode)//手动模式
		{
			if(id==IDC_PLATE_UP)//平台上升按钮被释放
			{
				GetDlgItem(IDC_PLATE_UP)->PostMessage(WM_KILLFOCUS, 0, 0);
				ReleaseCapture();
				FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0300"),1,SBuf);
				WriteLog(_T("平台上升释放"));
			}
			else if(id==IDC_PLATE_DOWN)//平台下降按钮被释放
			{
				GetDlgItem(IDC_PLATE_DOWN)->PostMessage(WM_KILLFOCUS, 0, 0);
				ReleaseCapture();
				FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0302"),1,SBuf);
				WriteLog(_T("平台下降释放"));
			}
		}
	}*/
	
	if (((pMsg->message == WM_KEYDOWN) | (pMsg->message == WM_KEYUP)) & (pMsg->wParam == VK_HOME))
	{
		isAddRdcord = true;
		m_BtnRealse.LoadBitmaps(IDB_REALSE2);
		m_BtnRealse.SizeToContent();
		m_BtnRealse.RedrawWindow();
		m_BtnPrepar.LoadBitmaps(IDB_REALSE1);
		m_BtnPrepar.SizeToContent();
		m_BtnPrepar.RedrawWindow();
	}
	if (((pMsg->message == WM_KEYDOWN) | (pMsg->message == WM_KEYUP)) & (pMsg->wParam == VK_F3))
	{
		isProcess = false;
		AfxMessageBox(_T("按键无效"));
	}
	

	return CDialog::PreTranslateMessage(pMsg);
}
//设置放锤按钮的颜色
void CBlastTestDlg::SetHunBtnBit(char sw)//sw:0--gray,1--blue
{
	if(sw==1)//表示开始装药
	{
		m_BtnRealse.LoadBitmaps(IDB_RELEASEGRAY);//重锤释放按钮变灰
	}
	else if(sw==0)
	{
		m_BtnRealse.LoadBitmaps(IDB_REALSE);//重锤释放按钮变为可操作状态
	}
	//m_BtnReport.SubclassDlgItem(IDC_BTN_REPORT,this);
	m_BtnRealse.SizeToContent();
	m_BtnRealse.RedrawWindow();
}
//设置放锤按钮的颜色
void CBlastTestDlg::SetBlastIndexBit(char sw)//sw:0--gray,1--爆炸
{
	if(sw==1)//表示开始装药
	{
		m_BlastIndex.LoadBitmaps(IDB_BLAST_INDEX_RED);//重锤释放按钮变灰
	}
	else if(sw==0)
	{
		m_BlastIndex.LoadBitmaps(IDB_BLAST_INDEX_GRAY);//重锤释放按钮变为可操作状态
	}
	//m_BtnReport.SubclassDlgItem(IDC_BTN_REPORT,this);
	m_BlastIndex.SizeToContent();
	m_BlastIndex.RedrawWindow();
}
void CBlastTestDlg::CalTestResult()//计算实验结果，爆炸率，可行度等。
{
	//将计算的结果写入样品数据表中
	float * PRate=new float[m_MaxGroupNo];//用于存放每组的爆炸率
	float Rate,LRate,LRateTmp,HRate,HRateTmp;//总的概率，置信下限，置信上限
	char buf[20];
	CString str;
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	//判断实验是否完成
	//...
	//计算每组实验的爆炸率
	for(int i=0;i<m_MaxGroupNo;i++)
	{
		memset(buf,0x00,20);
		sprintf(buf,_T("A%2d__"),i+1);
		str=buf;
		str+=m_TestSpNo;
		str.Replace(' ','0');
		DataSet.Open(_T("select  * from TestData where 爆炸状态=1 and 自动编号 like'")+str+_T("' order by 实验日期 DESC"));
		PRate[i]=DataSet.GetRecordCount()/((float)m_MaxTestNo);
	}
	float P2=PRate[1];
	float P1=PRate[0];
	//计算总的爆炸率
	DataSet.Open(_T("select  * from TestData where 爆炸状态=1 and 试样编号='")+m_TestSpNo+_T("' order by 实验日期 DESC"));
	Rate=DataSet.GetRecordCount()/((float)m_MaxTestNo*m_MaxGroupNo);
	//判断有无明显差异
	CADODataSet DataRate;
	CString RateSql;

	LRate=0;
	HRate=0;
	int MaxTestNo;//实验组数，查表时实际组数小于25组，让其等于25组，大于1000组，让其等于1000组
	MaxTestNo=m_MaxTestNo;
	if(MaxTestNo<25) MaxTestNo=25;//让实验组数落在表格中。
	if(MaxTestNo>100) MaxTestNo=1000;

	for(int j=0;j<m_MaxGroupNo;j++)
	{
		RateSql.Format(_T("select top 1 * from PTable where 概率>=%.2f and 次数>=%d  order by 概率 ASC,次数 ASC"),PRate[j],MaxTestNo-1);
		DataRate.Open(RateSql);
		LRateTmp=DataRate.ReadFloatValue(_T("置信上限"));
		HRateTmp=DataRate.ReadFloatValue(_T("置信下线"));
		int i;
		for(i=0;i<m_MaxGroupNo;i++)
		{
			if((PRate[i]<LRateTmp) | (PRate[i]>HRateTmp))//落在区间外
			{
				//落在了置信区间外
				LRate=0;
				HRate=0;
				//MessageBox(_T("实验组之间存在明显差异，请重新实验！"));
				break;
			}
		}
		if(i>=m_MaxGroupNo)//说明本次循环所有其他组概率都落在置信区间内
		{
			LRate=LRateTmp;
			HRate=HRateTmp;
			break;
		}

	}	
	if(LRate+HRate<=0) MessageBox(_T("实验组之间存在明显差异，请重新实验！"));

	//将结果写入样品数据的数据库
	DataSet.Open(_T("select  * from SampleTable where 试样编号='")+m_TestSpNo+_T("'"));
	if(DataSet.GetRecordCount()>0)
	{
		DataSet.SetFieldValue(_T("概率"),Rate);
		//写入置信上限和下限
		DataSet.SetFieldValue(_T("置信上限"),LRate);
		DataSet.SetFieldValue(_T("置信下限"),HRate);
		DataSet.Save();
	}
	else
	{
		MessageBox(_T("样品数据中不存在指定样品编号"));
	}

	delete []PRate;
}

float CBlastTestDlg::CRTCalNextH(int BlastStatus)//计算下一个特性高度
{
	//预备试验完成状态 
	switch(m_CRTTestStep)
	{
	case CH_STEEP_BEGIN://第一次预备试验
		if(1==BlastStatus)
		{
			m_CRTHeight.m_CurrHeight-=4*m_StepLength;
		}
		else
			m_CRTHeight.m_CurrHeight+=4*m_StepLength;
		
		m_CRTTestStep=CH_STEEP_4D;
		m_CRTTestLastStep=CH_STEEP_BEGIN;
		break;
	case CH_STEEP_4D://4D步长试验
		if(m_LastBlastStatus==BlastStatus)//与上一次判爆的结果相同
		{
			if(1==BlastStatus)
			{
				m_CRTHeight.m_CurrHeight-=4*m_StepLength;
			}
			else
				m_CRTHeight.m_CurrHeight+=4*m_StepLength;
		}
		else//与上一次判爆的结果不同
		{
			if(1==BlastStatus)
			{
				m_CRTHeight.m_CurrHeight-=2*m_StepLength;
			}
			else
				m_CRTHeight.m_CurrHeight+=2*m_StepLength;	

			m_CRTTestStep=CH_STEEP_2D;
		}
		m_CRTTestLastStep=CH_STEEP_4D;
		break;
	case CH_STEEP_2D:
		if(m_LastBlastStatus==BlastStatus)//与上一次判爆的结果相同
		{
			if(1==BlastStatus)
			{
				m_CRTHeight.m_CurrHeight-=2*m_StepLength;
			}
			else
				m_CRTHeight.m_CurrHeight+=2*m_StepLength;
		}
		else//与上一次判爆的结果不同
		{
			if(1==BlastStatus)
			{
				m_CRTHeight.m_CurrHeight-=m_StepLength;
			}
			else
				m_CRTHeight.m_CurrHeight+=m_StepLength;	
			
			m_CRTTestStep=CH_STEEP_D;
		}
		m_CRTTestLastStep=CH_STEEP_2D;
		break;
	case CH_STEEP_D:
		if(m_LastBlastStatus==BlastStatus)//与上一次判爆的结果相同
		{
			if(1==BlastStatus)
			{
				m_CRTHeight.m_CurrHeight-=m_StepLength;
			}
			else
				m_CRTHeight.m_CurrHeight+=m_StepLength;
		}
		else//与上一次判爆的结果不同
		{
			m_CRTTestStep=CH_STEEP_FINISH;
			if(1==BlastStatus)
			{
				m_CRTHeight.m_CurrHeight-=m_StepLength;
			}
			else
				m_CRTHeight.m_CurrHeight+=m_StepLength;
			//创建样品试验记录
			//m_CRTHeight.CreateSpRecord(m_TestSpNo);
			MessageBox(_T("预备试验已经完成！"));

			CADODataSet DataSet;//20151125
			DataSet.SetConnection(::GetConnection());
			DataSet.ClearRecord(_T("select * from TestData where 试样编号='")+m_TestSpNo+_T("' order by 实验日期 DESC"));
			m_TestNo=m_MaxTestNo;	
		}
		m_CRTTestLastStep=CH_STEEP_D;
		break;
	case CH_STEEP_FINISH: //完成预备试验
		if(1==BlastStatus)
		{
			m_CRTHeight.m_CurrHeight-=m_StepLength;
		}
		else
			m_CRTHeight.m_CurrHeight+=m_StepLength;

		m_CRTTestLastStep=CH_STEEP_FINISH;
		break;
	default:
		break;
	}

	return (m_CRTHeight.m_CurrHeight);
}
//本次爆破测试完成，写入测试记录并更新相关编辑框的内容
void CBlastTestDlg::WriteRecord()
{
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	int RetValue;
	int SoundLevelPeak;

	//20160815取消删除开始
	WaitForSingleObject(GEvent,1000);//等待信号
	RetValue=FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_SOUND_LEVEL_PEAK,1,SBuf);
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
	if(RetValue==-2)
	{
		MessageBox(_T("PLC无响应！"));
		return;
	}
	WaitForSingleObject(GEvent,1000);//等待信号
	SoundLevelPeak=FATEK_GetIntData(0,(char *)SBuf);
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
	//20160815取消删除结束

	//SoundLevelPeak = PlcData.SoundLevel;//保存声级计峰值  //20160815删除

	KillTimer(1);//暂停串口数据的更新
	m_SondLevel=SoundLevelPeak/(10.0f);
    if(m_SondLevel>140)
	{
		MessageBox(_T("声级计工作异常！"));	
		m_SondLevel=0;
	}
	//显示峰值添加--测试用，发布后删除
	CString str;
	str.Format("%f",m_SondLevel);
	//MessageBox(str);
	GetDlgItem(IDC_EDIT18)->SetWindowText(str);
	//根据峰值判断是否爆炸
	//...
	short BlastStatus;
	if((m_SondLevel>m_SLThreshold) |(m_SmogReal>m_SmogSetValue))//大于阀值表示爆炸/烟雾大于阀值
	{
		BlastStatus=1;
		SetBlastIndexBit(1);
	}
	else  //表示没有爆炸
	{
		BlastStatus=0;
		SetBlastIndexBit(0);
	}

	//20150718
	if(m_TestSpNo==_T(""))
	{
		MessageBox(_T("试样编号不能为空！"));
		return;	
	}

	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	
	bool CRTLastTime=0;
	if(m_Testmode==CRT_HEIGHT_MODE)//如果是特性落高法20151019
	{
		if(m_CRTTestStep==CH_STEEP_D)//为了判断是最后一次，删除记录
			CRTLastTime=TRUE;
		CRTCalNextH(BlastStatus);//如果判爆发生了改变或者删除了一次试验，则要重新计算高度

	//	if((CRTLastTime==TRUE) && (m_CRTTestStep==CH_STEEP_FINISH))//特性落高法的预备实验完成

// 		//判定是否要写入记录
// 		if(m_CRTTestStep==CH_STEEP_FINISH)
// 		{
// 			
// 		}
		m_CurrBlastStatus=BlastStatus;//保存本次实验的爆炸状态 
		//SetTimer(1,500,NULL);//用于更新串口数据显示1021
		//m_LastBlastStatus=BlastStatus;//保存本次实验的爆炸状态 
	}

	//计算自动编号
	CString StrAutoNo;
	char Buf[20];
	memset(Buf,0x00,20);
	sprintf(Buf, _T("A%2d%2d"), m_MaxGroupNo-m_TestGroupNo+1, m_MaxTestNo - m_TestNo+1);
	StrAutoNo=Buf;
	StrAutoNo.Replace(' ','0');
	StrAutoNo+=m_TestSpNo;

	//写入记录
	
	//如果指定记录已经存在，则删除该记录
	DataSet.Open(_T("select * from TestData where 自动编号='")+StrAutoNo+_T("'"));
	if(DataSet.GetRecordCount()>0)
	{
		DataSet.Delete();
		DataSet.Save();
	}
	DataSet.Open(_T("select top 100 * from TestData where 试样编号='")+m_TestSpNo+_T("' order by 实验日期 DESC"));

	//写入记录
	DataSet.AddNew();
	DataSet.SetFieldValue(_T("试样编号"),(_variant_t)m_TestSpNo);
	DataSet.SetFieldValue(_T("自动编号"),(_variant_t)StrAutoNo);
	DataSet.SetFieldValue(_T("样品名称"),(_variant_t)m_TestSpName);
	DataSet.SetFieldValue(_T("爆炸状态"),(_variant_t)BlastStatus);
	if(BlastStatus)
		DataSet.SetFieldValue(_T("实验结果"),_T("爆炸"));
	else
		DataSet.SetFieldValue(_T("实验结果"),_T("不爆"));
	
	COleDateTime CurrTime;
	CurrTime=CurrTime.GetCurrentTime();
	//CString StrTime;
	//StrTime=CurrTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	DataSet.SetFieldValue(_T("实验日期"),(_variant_t)CurrTime);	
	DataSet.SetFieldValue(_T("实验员"),(_variant_t)m_TestPerName);	
	DataSet.SetFieldValue(_T("实验类型"),(_variant_t)m_TestType);
	DataSet.SetFieldValue(_T("重锤质量"),(_variant_t)m_HunchKg);
	DataSet.SetFieldValue(_T("重锤高度"),(_variant_t)m_HunchHight);
	DataSet.SetFieldValue(_T("药量"),(_variant_t)m_TestSpKg);
	DataSet.SetFieldValue(_T("声级计峰值"),(_variant_t)m_SondLevel);//声级计峰值20150626

	//DataSet.SetFieldValue(_T("温度变化"),(_variant_t)---);
	DataSet.Save();

	if(m_TestNo>1)//本组实验未完成
		m_TestNo--;
	else//本组实验完成
	{
		if(m_TestGroupNo>1)
		{
			m_TestGroupNo--;
			CADODataSet DataSetUp;
			DataSetUp.SetConnection(::GetConnection());
			DataSetUp.Open(_T("select * from TestConditionSetup"));
			m_TestNo=DataSetUp.ReadIntValue(_T("每组次数"));
			//DataSetUp.CloseDataSet();
		}
		else //本组是最后一组，且已经完成,说明本次实验完成
		{
			if(m_Testmode==CRT_HEIGHT_MODE)
			{
				m_CRTHeight.m_SpNo=m_TestSpNo;
				m_CRTHeight.m_DataTable=_T("TestData");
				m_CRTHeight.m_d=m_StepLength;
				m_CRTHeight.CalCRTResult();//计算并写入结果
				m_CRTHeight.WriteRecord();
			}
			else if(m_Testmode==BLAST_RATE_MODE)
			{
				CalTestResult();
			}
			else
				MessageBox(_T("无效的试验类型！"));

			
			MessageBox(_T("本次实验完成"));
			m_TestGroupNo=0;
		}
	}
	//20152225
	if((m_CRTTestLastStep==CH_STEEP_D) && (m_CRTTestStep==CH_STEEP_FINISH))
	{
		DataSet.ClearRecord(_T("select * from TestData where 试样编号='")+m_TestSpNo+_T("' order by 实验日期 DESC"));
		m_TestNo=m_MaxTestNo;
	}
	//如果是在特性落高法模式,并且是在预备试验状态，试验次数不会减少
 //	if((m_Testmode==CRT_HEIGHT_MODE) && (m_CRTTestStep!=CH_STEEP_FINISH))
//	{
//		m_TestGroupNo=m_MaxGroupNo;
//		m_TestNo=m_MaxTestNo;
//	}

	SetTimer(1,500,NULL);//用于更新串口数据显示

	/*//计算实时引爆率
	int OldCount=DataSet.GetRecordCount();
	m_BlastRate=(BlastStatus+(OldCount*m_BlastRate))/(OldCount+1);*/

	//计算实时爆炸率 20150625
	if(m_Testmode==BLAST_RATE_MODE)
	{
		CADODataSet DataRate;
		DataRate.SetConnection(::GetConnection());
		DataRate.Open(_T("select top 500 * from TestData where 试样编号='")+m_TestSpNo+_T("'"));
		int AllCount=DataRate.GetRecordCount();
		DataRate.Open(_T("select top 500 * from TestData where 试样编号='")+m_TestSpNo+_T("' and 爆炸状态=1"));
		m_BlastRate=(100*DataRate.GetRecordCount())/AllCount;
	}


	//为了能马上更新现实，采用了再多查询一次的方式
	DataSet.Open(_T("select top 100 * from TestData where 试样编号='")+m_TestSpNo+_T("' order by 实验日期 DESC"));
	RefreshDGrid(m_Ado1,m_DataGrid1,_T(" SELECT Top 6  *  FROM TestData order by 实验日期 DESC"));
	//自动备份文件
	/*zhj delete V1.01 CString StrTime;
	StrTime=CurrTime.Format(_T("%Y-%m-%d"));
	StrTime=_T("D:\\爆破数据备份\\")+StrTime+m_TestSpNo+_T(".mdb");
	::CopyFile("D:\\爆破项目\\BlastTest\\Samples\\Data\\爆破检测系统.mdb",StrTime,false);*/

}

DWORD WINAPI PlcFiStatusPro(LPVOID lpParameter );  // thread data
//读取PLC放捶的状态					
DWORD WINAPI PlcFiStatusPro(LPVOID lpParameter )
{
	int RetValue;
	int M30,D320;
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	HWND hwnd=((HunThreedData *)lpParameter)->hwnd;
	for(int i=0;i<STATUS_WAITE_TIME;i++)
	{
		WaitForSingleObject(GEvent,1000);//等待信号
		RetValue=FATEK_ReadMultiByte(CMD_SLAVE_ADDR,CMD_FINISH_STATUS,1,SBuf);
		M30=FATEK_GetByteData(0,(char *)SBuf);
		//FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_FINISH_STATUS_INT,1,SBuf);
		//D320=FATEK_GetIntData(0,(char *)SBuf);
		D320 = 0;

		//zhj modify V1.15 
		//FATEK_ReadMultiReg(CMD_SLAVE_ADDR, CMD_ADDR_PRESS, 0x01, SBuf);//读取压力值
		//PlcData.m_HdyPress.m_PressValue = (float)FATEK_GetIntData(0, (char *)SBuf)/100.0; //; (10.0f);
		//PlcData.m_HdyPress.m_PressValue = int(PlcData.m_HdyPress.m_PressValue * 100) / 100.0;//取两位小数
		
		SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
		if((RetValue!=-2) & (M30 | D320))//将第一个字节的字符转换为布尔型,
		{
			/*m_HunReleaseBtn=0;//将重锤释放标志清零
			SBuf[0]='0';
			FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_HUN_RELESAE,1,SBuf);
			WriteLog(_T("重锤释放完成"));
			//写入实验记录数据
			//计算实时引爆率，更新实验组数和剩余次数
			WriteRecord();//写入记录*/
			::PostMessage(hwnd,WM_HUNFINISH,NULL,1);
			return TRUE;
		}
		WaitForSingleObject(GEvent,1000);//等待信号
		//zhj delete V1.01 只删一句还不行  if(0>FATEK_ReadMultiByte(CMD_SLAVE_ADDR, CMD_DOOR_SWITECH, 1, SBuf))
		//{
		//	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
		//}
		//else
		//{
		//	if(!FATEK_GetByteData(0,(char *)SBuf))//门状态为0表示未关闭，无法放锤
		//	{				
		//		SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
		//	//zhj delete V1.01临时暂时未找到原因	::PostMessage(hwnd,WM_HUNFINISH,NULL,2);//门打开，自动终止试验
		//		return TRUE;
		//	}
		//	
		//}
		SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
		Sleep(200);
	}
	//超时
	::PostMessage(hwnd,WM_HUNFINISH,NULL,0);
	return TRUE;
}
//自定义消息响应函数，等待重锤释放完成。
LRESULT  CBlastTestDlg::OnHunFinish(WPARAM wParam, LPARAM lParam)
{
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	int Status=int(lParam);
	if (EquipmentType == TESTTYPE_ZHUJI)
	{ }
	else
	{ 
	if (abs(PlcData.m_HdyPress.m_PressValue - PlcData.m_HdyPress.m_PressSetValue) >= 0.5)//大于0.5mp，加压失败
	{
		//zhj delete V1.11 Status = 3;
	}
	else//加压成功
	{
		//保存当前的DA值供下次调用

	}
	}
	//延时0.1秒
	DWORD dwStart = GetTickCount();
	DWORD dwEnd = dwStart;
	do{
		dwEnd = GetTickCount() - dwStart;
	} while (dwEnd <100);
	//将PLC重锤释放完成标志置0
	m_HunReleaseBtn=0;
	SetHunBtnBit(m_HunReleaseBtn);//将图片变成可用
	/*SBuf[0]='0';
	WaitForSingleObject(GEvent,1000);//等待信号
	for(int Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_FINISH_STATUS,1,SBuf))//返回值大于0，才说明写入成功
			break;
	}
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		MessageBox(_T("清除放锤完成标志失败，本次实验无效！"));
	}*///20150625
	if(0<FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_HUN_REAL_HIGHT,1,SBuf))//读取平台高度
		m_PlateHight=(float)FATEK_GetIntData(0,(char *)SBuf)/(10.0f);
	//清除放锤完成标志位
	/*WriteByte(CMD_SLAVE_ADDR,CMD_FINISH_STATUS,0);
	WriteInt(CMD_SLAVE_ADDR,CMD_FINISH_STATUS_INT,0);*///20150707删除
	//延时0.1秒，等待读取到峰值
	dwStart = GetTickCount();
	dwEnd = dwStart;
	do{
		dwEnd = GetTickCount() - dwStart;
	} while (dwEnd <100);

	if(Status==1)
	{
		WriteRecord();
		WriteLog(_T("放锤完成"));
	}
	else if(Status==0)
	{
		WriteRecord();
		MessageBox(_T("等待PLC放锤超时，本次试验失败！"));
		WriteLog(_T("等待超时"));
	}
	else if(Status==2)
	{
		MessageBox(_T("检测到门打开，本次实验终止！"));
		WriteLog(_T("实验终止"));
	}
	else if (Status == 3)//压力超界限
	{
		MessageBox(_T("加压失败，实验无效"));
		WriteLog(_T("加压失败"));
	}
	else
	{
		MessageBox(_T("无效的返回值！试样无效！"));
	}
	m_HunReleaseBtn=0;
	SetHunBtnBit(m_HunReleaseBtn);
	PlcData.m_SoundComm.m_IsPeakMode = FALSE;//切换到实时值模式
	
	return 0;
}
//写入当前重锤的零点
char CBlastTestDlg::WriteHunZero(float HunKg,float ZeroNo)
{
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	CString str;
	str.Format(_T("select * from HunZeroPoint where 重锤质量=%f"),HunKg);
	DataSet.Open(str);

	if(DataSet.GetRecordCount()>0)
	{
		DataSet.SetFieldValue(_T("零点高度"),ZeroNo);
		DataSet.Save();
		return TRUE;
	}
	else
	{
		MessageBox(_T("不存在当前重量的重锤！"));
		return FALSE;
	}
}
//读取当前重锤的零点
float CBlastTestDlg::ReadHunZero(float HunKg)
{
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	CString str;
	str.Format(_T("select * from HunZeroPoint where 重锤质量=%f"),HunKg);
	DataSet.Open(str);
	if(DataSet.GetRecordCount()>0)
	{
		return DataSet.ReadFloatValue(_T("零点高度"));
	}
	else
	{
		MessageBox(_T("不存在当前重量的重锤！"));
		return 1000;
	}
}

//将1byte数据写入PLC
                            //从机地址，  写入地址，     写入数据--数字形式
char CBlastTestDlg::WriteByte(unsigned char Slave,CString Addr, char Dbyte)
{
	unsigned char SBuf[10];
	memset(SBuf,0,10);
	WaitForSingleObject(GEvent,1000);//等待信号
	sprintf((char *)SBuf,_T("%d"),Dbyte);
	int Fatek;
	for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_WriteMultiByte(Slave,Addr,1,SBuf))//返回值大于0，才说明写入成功
			break;
	}
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		//MessageBox(_T("与PLC通信失败！"));
		WriteLog(_T("与PLC通信失败！"));
		return FALSE;
	}
	//读取
	WaitForSingleObject(GEvent,1000);//等待信号
	memset(SBuf,0,10);	
	for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_ReadMultiByte(Slave,Addr,1,SBuf))//返回值大于0，才说明读取成功
			break;
	}
	SetEvent(GEvent);//变为有信号状态，让线程可以发送数据
	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		//MessageBox(_T("与PLC通信失败！"));
		WriteLog(_T("与PLC通信失败！"));
		return FALSE;
	}
	//判断是否相等
	char ReadData=FATEK_GetByteData(0,(char*)SBuf);
#ifdef _DEBUG3 
	return TRUE;
#endif
	if(Dbyte!=ReadData)
	{
		CString ErrMessage;
		ErrMessage=_T("与PLC写入失败！地址：")+Addr;
		MessageBox(ErrMessage);
		return FALSE;
	}		
	return TRUE;
}
//将1整形数据数据写入PLC
                            //从机地址，  写入地址，     写入数据--数字形式
char CBlastTestDlg::WriteInt(unsigned char Slave,CString Addr, int DInt)
{
	unsigned char SBuf[10];

	CString str;
	str.Format("%2X",DInt);//从机地址转换为16进制字符串
	str.Replace(' ','0');
	memset(SBuf,'0',10);
	int StrLen=str.GetLength();
	if(StrLen>4) StrLen=4;
	for(int SLoop=0;SLoop<StrLen;SLoop++)
	{
		SBuf[3-SLoop]=str.GetAt(StrLen-1-SLoop);
	}

	WaitForSingleObject(GEvent,1000);//等待信号
	int Fatek;
	for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_WriteMultiReg(Slave,Addr,1,SBuf))//返回值大于0，才说明写入成功
			break;
	}
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		//MessageBox(_T("与PLC通信失败！"));
		WriteLog(_T("与PLC通信失败！"));
		return FALSE;
	}
	//读取
	WaitForSingleObject(GEvent,1000);//等待信号
	memset(SBuf,0,10);	
	for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_ReadMultiReg(Slave,Addr,1,SBuf))//返回值大于0，才说明写入成功
			break;
	}
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		//MessageBox(_T("与PLC通信失败！"));
		WriteLog(_T("与PLC通信失败！"));
		return FALSE;
	}
#ifdef _DEBUG3 
	return TRUE;
#endif

	//判断是否相等
	if(DInt!=FATEK_GetIntData(0,(char*)SBuf))
	{
		CString ErrMessage;
		ErrMessage=_T("与PLC写入失败！地址：")+Addr;

	

		//zhj modify V1.05 MessageBox(ErrMessage);
		WriteLog(ErrMessage);
		return FALSE;
	}		
	return TRUE;
}
//将1整形数据数据写入PLC
//从机地址，  写入地址，     写入数据--数字形式
char CBlastTestDlg::WriteFloat(unsigned char Slave, CString Addr, float DInt)
{
	unsigned char SBuf[10];
	union FIType aa;
	CString str;
	aa.FData = DInt;
	str.Format("%4X", aa.Dint);//从机地址转换为16进制字符串
	str.Replace(' ', '0');
	memset(SBuf, '0', 10);
	int StrLen = str.GetLength();
	if (StrLen>8) StrLen = 8;
	for (int SLoop = 0; SLoop<StrLen; SLoop++)
	{
		SBuf[7 - SLoop] = str.GetAt(StrLen - 1 - SLoop);
	}

	WaitForSingleObject(GEvent, 1000);//等待信号
	int Fatek;
	for (Fatek = 0; Fatek<CMD_SEND_REPART_COUNT; Fatek++)
	{
		if (0<FATEK_WriteMultiReg(Slave, Addr, 2, SBuf))//返回值大于0，才说明写入成功
			break;
	}
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
	if (Fatek >= CMD_SEND_REPART_COUNT)
	{
		//MessageBox(_T("与PLC通信失败！"));
		WriteLog(_T("与PLC通信失败！"));
		return FALSE;
	}
	
	return TRUE;
}
//放锤按钮
void CBlastTestDlg::OnBtnRealse() 
{
	if (isAddRdcord)
	{
		AddRecord(false);
		return;
	}


	// TODO: Add your control notification handler code here
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	if(m_PlateMode)//手动模式
	{
		MessageBox(_T("请先将平台切换到自动模式！"));
		return;
	}
	if(m_PlaceExpPrepar)//正在装药，无法放锤
	{
		MessageBox(_T("正在装药，无法放锤"));
		return;
	}
	if(m_HunReleaseBtn)//正在放锤，点击无效
	{
		return;
	}
	//读取关门状态
	WaitForSingleObject(GEvent,1000);//等待信号
	if(0>FATEK_ReadMultiByte(CMD_SLAVE_ADDR, CMD_DOOR_SWITECH, 1, SBuf))
	{
		SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
		MessageBox(_T("读取门状态失败"));
		return;
	}
	else
	{
		if(!FATEK_GetByteData(0,(char *)SBuf))//门状态为0表示未关闭，无法放锤
		{
			MessageBox(_T("请将装药门关闭！"));
			SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
			return;
		}

	}
	//20150618
    int RetValue,M30,D320;
	RetValue=FATEK_ReadMultiByte(CMD_SLAVE_ADDR,CMD_FINISH_STATUS,1,SBuf);
	M30=FATEK_GetByteData(0,(char *)SBuf);
	FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_FINISH_STATUS_INT,1,SBuf);
	D320=FATEK_GetIntData(0,(char *)SBuf);
	SetEvent(GEvent);//变为有信号状态，让线程可以发送数据
	if((RetValue!=-2) & (M30 | D320))//将第一个字节的字符转换为布尔型,
	{
		MessageBox(_T("请先按装药以清除完成标志"));
		SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
		return;
	}
 //
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据

	m_HunReleaseBtn=1;//放锤按钮标志切换

	//清除PLC放捶完成标志位
	/*WriteByte(CMD_SLAVE_ADDR,CMD_FINISH_STATUS,0);
	WriteInt(CMD_SLAVE_ADDR,CMD_FINISH_STATUS_INT,0);*///20150626在放锤OnHunFinish()完成的处理中已经清除

	//重新计算重锤高度
	//m_CRTHeight.m_CurrHeight=m_HunchHight;
	//CRTCalNextH(m_CurrBlastStatus);

	//m_HunchHight=m_CRTHeight.m_CurrHeight;//20160607
	if (m_TestType == _T("特性落高法"))
	{
		m_HunchHight = m_CRTHeight.m_CurrHeight;//20160614
	}

	if((m_HunchHight*m_HunHightRata<40) || (m_HunchHight*m_HunHightRata>650))//重锤高度必须在245mm和550mm之间
	{
		m_HunchHight=0;
		m_HunReleaseBtn=0;//放锤按钮标志切换
		MessageBox(_T("重锤高度必须在40mm和650mm之间"));
	}
	m_LastBlastStatus=m_CurrBlastStatus;//保存本次实验的爆炸状态 

//zhj modify 	if(!WriteInt(CMD_SLAVE_ADDR,CMD_HUN_HEIGHT,(int)(m_HunchHight*m_HunHightRata*10)))//写入重锤高度 20151019
	if(!WriteInt(CMD_SLAVE_ADDR,CMD_HUN_HEIGHT,(int)(m_HunchHight*10)))//写入重锤高度 20151019
	{
		m_HunReleaseBtn=0;//放锤按钮标志切换
		return;
	}

	if(!WriteByte(CMD_SLAVE_ADDR,CMD_PLACE_EXPLOSIVE,0))//清除装药标志
	{
		m_HunReleaseBtn=0;//放锤按钮标志切换
		return;
	}
	//写入放捶命令
	if(!WriteByte(CMD_SLAVE_ADDR,CMD_HUN_RELESAE,m_HunReleaseBtn))
	{
		m_HunReleaseBtn=0;//放锤按钮标志切换
		return;
	}
	
	WriteLog(_T("开始放锤"));
	SetHunBtnBit(m_HunReleaseBtn);//将按钮变成灰色

	PlcData.m_SoundComm.m_IsPeakMode = TRUE;//切换到峰值模式

	PlcData.m_HdyPress.InitPressFun(0);//

	//zhj add V1.15
	PlcData.m_HdyPress.m_HdyData.AddSendData(CMD_DAVALUE, 0);
	//创建新线程
	HunThreedData * FinsihThreedData=new HunThreedData;
	FinsihThreedData->hwnd=m_hWnd;
	HANDLE hTr1=CreateThread(NULL,0,PlcFiStatusPro,(LPVOID)FinsihThreedData,NULL,NULL);
	CloseHandle(hTr1);
		
}

void CBlastTestDlg::OnDropdownTestsel() 
{
	// TODO: Add your control notification handler code here
	//初始化试样选择下拉列表
	((CComboBox *)GetDlgItem(IDC_TESTSEL))->ResetContent();
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select top 20 * from SampleTable order by ID DESC"));
	for(int i=0;i<DataSet.GetRecordCount();i++)
	{
		((CComboBox *)GetDlgItem(IDC_TESTSEL))->AddString(DataSet.ReadStringValue(_T("试样编号")));
		DataSet.Next();
	}
	((CComboBox *)GetDlgItem(IDC_TESTSEL))->SetCurSel(0);

}
//进入用户设置
void CBlastTestDlg::OnUserSetup() 
{
	// TODO: Add your control notification handler code here
	CUserSetupDlg dlg;
	dlg.m_SLThreshold=m_SLThreshold;
	dlg.m_SmogSheld=m_SmogSetValue;
	dlg.m_HunCurrentH=m_PlateHight;//重锤高度
	dlg.m_HunKg=m_HunchKg;
	dlg.m_ZeroSetValue=ReadHunZero(m_HunchKg);
	dlg.m_CRHHunHight=m_CRTHeight.m_CurrHeight;
	dlg.m_Step=m_CRTTestStep;
	dlg.m_PressOffset = PlcData.m_HdyPress.m_PressSetOffset;

	if(m_HunReleaseBtn==1)//正在放锤
	{
		MessageBox(_T("请等待放锤完成！"));
		return;
	}

	WaitForSingleObject(GEvent,1000);//等待信号
	if(IDOK==dlg.DoModal())
	{
		m_SLThreshold=dlg.m_SLThreshold;
		m_SmogSetValue=dlg.m_SmogSheld;
		PlcData.m_HdyPress.m_PressSetOffset=dlg.m_PressOffset;
		if(20>=m_SmogSetValue)
		{
			m_Smog_bigvalue=m_SmogSetValue;
			m_SmogThrMode=1;
		}
	}
	SetEvent(GEvent);//变为有信号状态，让线程可以发生数据
	
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from UserSetup"));
	if(!DataSet.IsadoEOF())
	{
		m_CorTimerDelay=DataSet.ReadFloatValue("防二次碰撞延时");
	}
	else
		m_CorTimerDelay=0;
}

void CBlastTestDlg::OnSelchangeTestsel() 
{
	// TODO: Add your control notification handler code here
	int nSel;   	
    // 获取组合框控件的列表框中选中项的索引   
    nSel = ((CComboBox *)GetDlgItem(IDC_TESTSEL))->GetCurSel();   
    // 根据选中项索引获取该项字符串   
	if(nSel>=0)
		((CComboBox *)GetDlgItem(IDC_TESTSEL))->GetLBText(nSel, m_TestSel);  	
}
//修改药品质量
void CBlastTestDlg::OnChangeSpKg() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString str;
	GetDlgItem(IDC_EDIT7)->GetWindowText(str);
	if(str!=_T(""))
		UpdateData(TRUE);
		
}

char CBlastTestDlg::SetTestMode()
{
	if(m_TestType==_T("特性落高法"))
	{
		m_Testmode=CRT_HEIGHT_MODE;
		return CRT_HEIGHT_MODE;
	}
	else if(m_TestType==_T("爆炸概率法"))
	{
		m_Testmode=BLAST_RATE_MODE;
		return BLAST_RATE_MODE;
	}
	else if(m_TestType==_T("摩擦感度法"))
	{
		m_Testmode=FRI_RATE_MODE;
		return BLAST_RATE_MODE;
	}
	else
	{
		//m_Testmode=BLAST_RATE_MODE;
		MessageBox(_T("不支持的试验类型！"));
		return INVALID_TEST_MODE;
	}
}

//手动油缸前进按钮处理程序
void CBlastTestDlg::OnBnClickedHdyforward()
{
	// TODO:  在此添加控件通知处理程序代码
	PlcData.m_HdyPress.m_ManualF = !PlcData.m_HdyPress.m_ManualF;
	PlcData.m_HdyPress.m_ManualB = 0;
	PlcData.m_HdyPress.m_HdyData.AddSendData(CMD_FwVal, PlcData.m_HdyPress.m_ManualF);
	PlcData.m_HdyPress.m_HdyData.AddSendData(CMD_BackVal, 0);
	PlcData.m_HdyPress.m_HdyData.AddSendData(CMD_DAVALUE, 0);

	SetMPBtn();
}

//手动油缸后退按钮处理程序
void CBlastTestDlg::OnBnClickedHdyback()
{
	// TODO:  在此添加控件通知处理程序代码
	PlcData.m_HdyPress.m_ManualB = !PlcData.m_HdyPress.m_ManualB;
	PlcData.m_HdyPress.m_ManualF = 0;
	PlcData.m_HdyPress.m_HdyData.AddSendData(CMD_FwVal, 0);
	PlcData.m_HdyPress.m_HdyData.AddSendData(CMD_BackVal, PlcData.m_HdyPress.m_ManualB);
	PlcData.m_HdyPress.m_HdyData.AddSendData(CMD_DAVALUE, 0);

	SetMPBtn();
}


void CBlastTestDlg::SetMPBtn()
{
	if (PlcData.m_HdyPress.m_ManualB)
	{
		m_HdyBackBtn.LoadBitmaps(IDB_PRESSSTOP);
	}
	else
	{
		m_HdyBackBtn.LoadBitmaps(IDB_HDYBACK);
	}

	m_HdyBackBtn.SizeToContent();
	m_HdyBackBtn.RedrawWindow();

	if (PlcData.m_HdyPress.m_ManualF)
	{
		m_HdyForwardBtn.LoadBitmaps(IDB_PRESSSTOP);
	}
	else
	{
		m_HdyForwardBtn.LoadBitmaps(IDB_HDYFORWARD);
	}

	m_HdyForwardBtn.SizeToContent();
	m_HdyForwardBtn.RedrawWindow();
}


void CBlastTestDlg::OnEnChangeEdit18()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


int startAdd = 0;
// 模拟实验数据
void CBlastTestDlg::AddRecord(bool result)
{
	unsigned char SBuf[10];
	memset(SBuf, 0x00, 10);









	//20150718
	if (m_TestSpNo == _T(""))
	{
		MessageBox(_T("试样编号不能为空！"));
		return;
	}

	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());

	bool CRTLastTime = 0;


	//计算自动编号
	CString StrAutoNo;
	char Buf[20];
	memset(Buf, 0x00, 20);
	sprintf(Buf, _T("A%2d%2d"), m_MaxGroupNo-m_TestGroupNo+1, m_MaxTestNo - m_TestNo+1);
	StrAutoNo = Buf;
	StrAutoNo.Replace(' ', '0');
	StrAutoNo += m_TestSpNo;

	//写入记录

	//如果指定记录已经存在，则删除该记录
	DataSet.Open(_T("select * from TestData where 自动编号='") + StrAutoNo + _T("'"));
	if (DataSet.GetRecordCount()>0)
	{
		DataSet.Delete();
		DataSet.Save();
	}
	DataSet.Open(_T("select top 100 * from TestData where 试样编号='") + m_TestSpNo + _T("' order by 实验日期 DESC"));

	//写入记录
	DataSet.AddNew();
	DataSet.SetFieldValue(_T("试样编号"), (_variant_t)m_TestSpNo);
	DataSet.SetFieldValue(_T("自动编号"), (_variant_t)StrAutoNo);
	DataSet.SetFieldValue(_T("样品名称"), (_variant_t)m_TestSpName);

	if (result)
	{
		DataSet.SetFieldValue(_T("爆炸状态"), (_variant_t)1);
		DataSet.SetFieldValue(_T("实验结果"), _T("爆炸"));
	}
	else
	{
		DataSet.SetFieldValue(_T("爆炸状态"), (_variant_t)0);
		DataSet.SetFieldValue(_T("实验结果"), _T("不爆"));
	}
		

	COleDateTime CurrTime;
	COleDateTimeSpan ds;
	
	ds.SetDateTimeSpan(0, 0, startAdd*3, 0);
	startAdd++;
	CurrTime = CurrTime.GetCurrentTime() + ds;
	//CString StrTime;
	//StrTime=CurrTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	DataSet.SetFieldValue(_T("实验日期"), (_variant_t)CurrTime);
	DataSet.SetFieldValue(_T("实验员"), (_variant_t)m_TestPerName);
	DataSet.SetFieldValue(_T("实验类型"), (_variant_t)m_TestType);
	DataSet.SetFieldValue(_T("重锤质量"), (_variant_t)m_HunchKg);
	DataSet.SetFieldValue(_T("重锤高度"), (_variant_t)m_HunchHight);
	DataSet.SetFieldValue(_T("药量"), (_variant_t)m_TestSpKg);
	DataSet.SetFieldValue(_T("声级计峰值"), (_variant_t)m_SondLevel);//声级计峰值20150626

	//DataSet.SetFieldValue(_T("温度变化"),(_variant_t)---);
	DataSet.Save();

	if (m_TestNo>1)//本组实验未完成
		m_TestNo--;
	else//本组实验完成
	{
		if (m_TestGroupNo>1)
		{
			m_TestGroupNo--;
			CADODataSet DataSetUp;
			DataSetUp.SetConnection(::GetConnection());
			DataSetUp.Open(_T("select * from TestConditionSetup"));
			m_TestNo = DataSetUp.ReadIntValue(_T("每组次数"));
			//DataSetUp.CloseDataSet();
		}
		else //本组是最后一组，且已经完成,说明本次实验完成
		{
			if (m_Testmode == CRT_HEIGHT_MODE)
			{
				m_CRTHeight.m_SpNo = m_TestSpNo;
				m_CRTHeight.m_DataTable = _T("TestData");
				m_CRTHeight.m_d = m_StepLength;
				m_CRTHeight.CalCRTResult();//计算并写入结果
				m_CRTHeight.WriteRecord();
			}
			else if (m_Testmode == BLAST_RATE_MODE)
			{
				CalTestResult();
			}
			else
				MessageBox(_T("无效的试验类型！"));


			MessageBox(_T("本次实验完成"));
			m_TestGroupNo = 0;
			startAdd = 0;
		}
	}
	//20152225
	if ((m_CRTTestLastStep == CH_STEEP_D) && (m_CRTTestStep == CH_STEEP_FINISH))
	{
		DataSet.ClearRecord(_T("select * from TestData where 试样编号='") + m_TestSpNo + _T("' order by 实验日期 DESC"));
		m_TestNo = m_MaxTestNo;
	}
	

	//为了能马上更新现实，采用了再多查询一次的方式
	DataSet.Open(_T("select top 100 * from TestData where 试样编号='") + m_TestSpNo + _T("' order by 实验日期 DESC"));
	RefreshDGrid(m_Ado1, m_DataGrid1, _T(" SELECT Top 6  *  FROM TestData order by 实验日期 DESC"));

	
}


void CBlastTestDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
}
