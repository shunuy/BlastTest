// BlastTestDlg.cpp : implementation file
//
/****************************
�޸�����    �޸���              �޸�����
20150518    ���           ���ڶ��߳�δͬ�������ͬʱ�򴮿ڷ������ݣ��Ӷ��������ݴ��ҡ��������¼���ͬ����
20150523    ���           ����Ʒ����������������PLCд���ش��߶ȹ��ܡ�
20150523    ���           �����ж�ʵ����֮��Ĳ���Ͷ�ȡ�������书�ܡ�
20150605    ���           ������PLCͨ���ط�10ʧ�ܣ�������ʾ�Ի���
20150605    ���           ���ӵ��Խ���
20150608    ���           ����ÿ�α��ƷŴ���ɺ��Զ�����
20150521    ���           ƽ̨�л������������½���SBuf[0]=0�ĳ�SBuf[0]='0'����������������
20150523    ���           ��OnTimer()����������ж����������ַ��Ƿ������֣������ֲŸ��¡�
						   if(isdigit(str.GetAt(str.GetLength()-1)))//��������������
20150623    ���           ��BOOL CBlastTestDlg::PreTranslateMessage(MSG* pMsg)�����������ж�ESC��ť�ĺ�����
						   ��������esc��ť�˳�����Ĺ��ܡ��޸�Ĭ�ϰ�ť����ֹ�س����˳����� 
20150624    ���           ���д��1byte��1�������ݵ�PLC���������ں����ڽ����ݶ����Ƚϣ�ȷ�����ͳɹ���
            ���           ����������������˲ʱֵ����ӷ�ֵ��ʾ��
			���           ���Խ������Ӷ�ȡPLC���ݹ��ܣ�������ʾ�����Ʒ�ֵ���ܡ�
			���           ����װҩ�ͷŴ���ť����Ŵ���װҩ��˳��װҩʱ������Ŵ���
			���           �޸���PLCд��������ʱû�н���������ת��Ϊ16���Ƶ��ַ����󣬲���Ҫ�ô�д�ַ�--д�ش��߶ȡ�
20150625    ���           DataGrid���²���ʱ�Ľ���취�����ø���֮ǰ��ʹ��DataSet.Open()�ٲ�ѯһ�����ݿ⡣
20150625    ���           �޸ļ���ʵʱ��ը�ʵķ�����
20150626    ���           �޸�ֱ�Ӵ�D00306��ȡ�ش���ʵ�ʵ�ǰ�߶ȣ�ʹ��onPaint()����ˢ��DataGrid�����ݣ�������������淵��Datagrid
                           �����µ����⡣ 
						   ���������Ʒ�ֵ�������ݿ�Ĺ��ܡ�
20150629    ���           �����ֶ�ƽ̨���Զ�ƽ̨��������֮ǰ�ȸ�λϵͳ����λ��ɺ������λ����װҩ�ͷŴ���
20150630    ���           ���Ӷ�ȡץ���������ﵽ3�ε���������������ϵͳ��λ��
20150707    ���           ɾ����PLC�Ŵ���ɱ�־λ����Ӳ������޷��Ŵ����ܡ�����ش��ĸ߶�ϵ��.
                           �ж�ʵ�������Ƿ����25С��10000��������ʾ�������ڸ������ڡ�
						   ����������ݿ⣬���ӳ�ʼ����ȡ��Ʒ��¼ʱ����Ͽ�ʱ�ļ�¼Ϊ0�жϡ�
20150708    ���           д��������ش����д��D0,д��ʵ���¼ʱ�����֮ǰ�Ѿ����ڣ�����ɾ���ü�¼��
20150714    ���           ��װҩʱ��PLCд���������ײ��ʱ��
20150803    ���           �޸�DataGrid��Solite���ԣ���������ѡ��ָʾ����������RecordSelect��ѡ��ȡ��
20151023    ���           �����ʾ������ƽ̨�����Ȱ�ť
                           ���ӿ��������ƵĴ���ʾ������ʱ��1��2�Ĵ��ɳ�ʼ���Ի���ŵ��˶�ʱ��4����Ϣ��Ӧ�С�
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
	,m_TestType(_T("ײ���жȷ�"))
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
	//װҩ��ť
	m_BtnPrepar.LoadBitmaps(IDB_PREPAR,IDB_BITSELECTED);
	m_BtnPrepar.SubclassDlgItem(IDC_BTN_PRAPER,this);
	m_BtnPrepar.SizeToContent();
	//�ش��ͷ�
	m_BtnRealse.LoadBitmaps(IDB_REALSE,IDB_BITSELECTED);
	m_BtnRealse.SubclassDlgItem(IDC_BTN_REALSE,this);
	m_BtnRealse.SizeToContent();
	//��ըָʾ��
	m_BlastIndex.LoadBitmaps(IDB_BLAST_INDEX_GRAY);
	m_BlastIndex.SubclassDlgItem(IDC_BLAST_INDEX,this);
	m_BlastIndex.SizeToContent();
	//��������
	m_TestSpData.LoadBitmaps(IDB_TestSpBtb,IDB_BITSELECTED);
	m_TestSpData.SubclassDlgItem(IDC_TestSpBtn,this);
	m_TestSpData.SizeToContent();
	//ʵ����������
	m_TestCondSetup.LoadBitmaps(IDB_TESTCONDSETUP,IDB_BITSELECTED);
	m_TestCondSetup.SubclassDlgItem(IDC_TESTCONDSETUP,this);
	m_TestCondSetup.SizeToContent();
	//ʵ���¼
	m_RecordView.LoadBitmaps(IDB_RecordView,IDB_BITSELECTED);
	m_RecordView.SubclassDlgItem(IDC_RecordView,this);
	m_RecordView.SizeToContent();

	//�û�����
	m_UserSetup.LoadBitmaps(IDB_UserSetup,IDB_BITSELECTED);
	m_UserSetup.SubclassDlgItem(IDC_UserSetup,this);
	m_UserSetup.SizeToContent();

	//����ʵ������
	m_TestSpIn.LoadBitmaps(IDB_TestSpIn,IDB_BITSELECTED);
	m_TestSpIn.SubclassDlgItem(IDC_TestSpInBtn,this);
	m_TestSpIn.SizeToContent();

	//ƽ̨����
	m_PlateUp.LoadBitmaps(IDB_PLATEUP,IDB_BITSELECTED);
	m_PlateUp.SubclassDlgItem(IDC_PLATE_UP,this);
	m_PlateUp.SizeToContent();
	//ƽ̨����ֹͣ
	/*m_PlateUpStop.LoadBitmaps(IDB_PLATEUPSTOP);
	m_PlateUpStop.SubclassDlgItem(IDC_PLATE_UP_STOP,this);
	m_PlateUpStop.SizeToContent();*/

	//ƽ̨�½�
	m_PlateDown.LoadBitmaps(IDB_PLATEDOWN,IDB_BITSELECTED);
	m_PlateDown.SubclassDlgItem(IDC_PLATE_DOWN,this);
	m_PlateDown.SizeToContent();

	//ƽ̨�½�ֹͣ
	/*m_PlateDownStop.LoadBitmaps(IDB_PLATEDOWNSTOP);
	m_PlateDownStop.SubclassDlgItem(IDC_PLATE_DOWN_STOP,this);
	m_PlateDownStop.SizeToContent();*/
	//ƽ̨����ģʽѡ��ť
	

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

	CFont* ptf=GetDlgItem(IDC_EDIT1)->GetFont();//m_editPlace.GetFont(); // �õ�ԭ�������� 
	LOGFONT lf;   
  	ptf->GetLogFont(&lf);   
	lf.lfHeight = 15; // �ı�����߶�   
	strcpy (lf.lfFaceName, "����"); // �ı���������   
	m_editFont.CreateFontIndirect(&lf);   
	GetDlgItem(IDC_EDIT1)->SetFont(&m_editFont); // ����������    
	GetDlgItem(IDC_EDIT2)->SetFont(&m_editFont); // ����������  
	GetDlgItem(IDC_EDIT3)->SetFont(&m_editFont); // ���������� 
	GetDlgItem(IDC_EDIT4)->SetFont(&m_editFont); // ���������� 
	GetDlgItem(IDC_EDIT5)->SetFont(&m_editFont); // ���������� 
	GetDlgItem(IDC_EDIT6)->SetFont(&m_editFont); // ���������� 
	GetDlgItem(IDC_EDIT7)->SetFont(&m_editFont); // ���������� 
	GetDlgItem(IDC_EDIT8)->SetFont(&m_editFont); // ���������� 
	GetDlgItem(IDC_EDIT9)->SetFont(&m_editFont); // ���������� 
	GetDlgItem(IDC_EDIT10)->SetFont(&m_editFont); // ���������� 
	GetDlgItem(IDC_EDIT11)->SetFont(&m_editFont); // ���������� 
	GetDlgItem(IDC_EDIT12)->SetFont(&m_editFont); // ���������� 
	GetDlgItem(IDC_EDIT13)->SetFont(&m_editFont); // ���������� 
	GetDlgItem(IDC_EDIT14)->SetFont(&m_editFont); // ���������� 
	GetDlgItem(IDC_EDIT18)->SetFont(&m_editFont); // ���������� //�����Ʒ�ֵ
	GetDlgItem(IDC_COMBO1)->SetFont(&m_editFont); // ����������
	GetDlgItem(IDC_TIMEDIS)->SetFont(&m_editFont); // ����������
	GetDlgItem(IDC_SMOGREAL)->SetFont(&m_editFont); 
	GetDlgItem(IDC_SMOGSET)->SetFont(&m_editFont); 
	GetDlgItem(IDC_PRESSSETVALUE)->SetFont(&m_editFont);
	//�����༭��
	EditWriteAble(0);
	/*CBitmap Bitmap;
	Bitmap.LoadBitmap(IDB_BITOPENREPORT);
	HBITMAP hBitmap=(HBITMAP)Bitmap.Detach();
	CButton *pButton=(CButton*)GetDlgItem(IDC_BTN_REPORT);
	pButton->SetBitmap(hBitmap);*/
	
	//
	CColumns Columns=m_DataGrid1.GetColumns();   //���м��϶���  
    CColumn Column;  
    Columns.Add(2);                         //���������1�У�ע������Ŵ�0��ʼ  
    Columns.Add(3);                         //������1�� 
	Columns.Add(4);                         //������1��
	Columns.Add(5);                         //������1�� 
    Column=Columns.GetItem(COleVariant(long(0)));   //ȡ��һ�ж���  
    Column.SetWidth(120);                    //�����п��Ϊ80(����pi?)  
    Column.SetCaption("�Զ����");                //���ø��б���  
    //Column.SetAlignment(2);               //�����ݶ������ã�0����1���У�2����  
    Column.SetDataField("�Զ����");            //���ø��а��ֶ�  
    Column=Columns.GetItem(COleVariant(long(1)));   //���õڶ���  
    Column.SetWidth(100);                    //�����п��Ϊ80  
    Column.SetCaption("�������");                //���ø��б���  
    Column.SetDataField("�������");             //���ø��а��ֶ�  
    /*Column=Columns.GetItem(COleVariant(long(2)));   //���õ�����  
    Column.SetWidth(60);                    //�����п��Ϊ40  
    Column.SetCaption("ʵ������");                //���ø��б���  
    Column.SetDataField("ʵ������");          //���ø��а��ֶ�  */
    
	/*Column=Columns.GetItem(COleVariant(long(2)));   //���õ�����  
    Column.SetWidth(70);                   //�����п��Ϊ120  
    Column.SetCaption("�ش�����");           //���ø��б���  
    Column.SetDataField("�ش�����");            //���ø��а��ֶ�*/ //20151021 �ش��߶�
	Column=Columns.GetItem(COleVariant(long(2)));   //���õ�����  
    Column.SetWidth(70);                   //�����п��Ϊ120  
    Column.SetCaption("�ش��߶�");           //���ø��б���  
    Column.SetDataField("�ش��߶�");            //���ø��а��ֶ�
	

	Column=Columns.GetItem(COleVariant(long(3)));   //���õ�����  
    Column.SetWidth(90);                   //�����п��Ϊ120  
    Column.SetCaption("ʵ����");           //���ø��б���  
    Column.SetDataField("ʵ����");            //���ø��а��ֶ� 

	Column=Columns.GetItem(COleVariant(long(4)));   //���õ�����  
    Column.SetWidth(140);                   //�����п��Ϊ120  
    Column.SetCaption("ʵ������");           //���ø��б���  
    Column.SetDataField("ʵ������");            //���ø��а��ֶ� 

	Column=Columns.GetItem(COleVariant(long(5)));   //���õ�����  
    Column.SetWidth(80);                   //�����п��Ϊ120  
    Column.SetCaption("�����Ʒ�ֵ");           //���ø��б���  
    Column.SetDataField("�����Ʒ�ֵ");            //���ø��а��ֶ� 

    m_DataGrid1.ReBind();                        //ʵ����datagrid�еı���  
    m_DataGrid1.Refresh();                       //ˢ���б���ʾ

	//

	SetTimer(4,500,NULL);//������ʾ�����ƴ���ʾ
	
	PlcData.PlateHight=0;
	PlcData.SoundLevel=0;
	PlcData.SmogReal=0;
	//�������߳�
	HANDLE hTr=CreateThread(NULL,0,ReadPlcDataPro,NULL,NULL,NULL);
	CloseHandle(hTr);

	GEvent=CreateEvent(NULL,FALSE,FALSE,NULL);//�����¼����󣬶��߳̽���ͬ��,�ڶ���������ΪFALSE��ʾֻ����һ���߳��ܻ���ź�
	SetEvent(GEvent);//���¼���������Ϊ���ź�״̬��

	
	//��ʼ������ѡ�������б�
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select top 20 * from SampleTable order by ID DESC"));
	for(int i=0;i<DataSet.GetRecordCount();i++)
	{
		((CComboBox *)GetDlgItem(IDC_TESTSEL))->AddString(DataSet.ReadStringValue(_T("�������")));
		DataSet.Next();
	}
	if(DataSet.GetRecordCount()>0)
	{
		((CComboBox *)GetDlgItem(IDC_TESTSEL))->SetCurSel(0);
		((CComboBox *)GetDlgItem(IDC_TESTSEL))->GetLBText(0, m_TestSel); 
	}
	DataSet.Open(_T("select * from TestConditionSetup"));
	m_HunchKg=DataSet.ReadFloatValue(_T("�ش�����"));
	//20151019���
	m_StepLength=DataSet.ReadFloatValue(_T("��������"));
	m_TestType=DataSet.ReadStringValue(_T("ʵ������"));
	PlcData.m_HdyPress.m_PressSetValue = DataSet.ReadFloatValue(_T("ѹ���趨"));
	SetTestMode();
	PlcData.m_HdyPress.GetSetDAValue(PlcData.m_HdyPress.m_PressSetValue);

	CADODataSet DataThrshold;
	DataThrshold.SetConnection(::GetConnection());
	DataThrshold.Open(_T("select top 20 * from UserSetUp"));
	m_SLThreshold=DataThrshold.ReadFloatValue(_T("�����Ʒ�ֵ"));
	m_SmogSetValue=DataThrshold.ReadFloatValue(_T("����ֵ"));
	PlcData.m_HdyPress.m_PressSetOffset = DataThrshold.ReadFloatValue(_T("ѹ���趨����"));
	if ((PlcData.m_HdyPress.m_PressSetOffset > 1.2) || (PlcData.m_HdyPress.m_PressSetOffset < -1.2))
		PlcData.m_HdyPress.m_PressSetOffset = 0;

	if(20>=m_SmogSetValue)//�������ֵ����Ϊ2�����Զ����вɼ�
	{
		m_SmogThrMode=1;
		m_Smog_bigvalue=m_SmogSetValue;
	}

	m_PlateUp.ShowWindow(FALSE);//��������ʾ��ť
	m_PlateDown.ShowWindow(FALSE);

	m_HdyForwardBtn.ShowWindow(FALSE);//��������ʾ��ť
	m_HdyBackBtn.ShowWindow(FALSE);

	//�����ļ�
	::CreateDirectory("D:\\�������ݱ���",NULL);
	COleDateTime CurrTime;
	CurrTime=CurrTime.GetCurrentTime();
	CString StrTime;
	//StrTime=CurrTime.Format(_T("%Y-%m-%d"));
	StrTime=CurrTime.Format(_T("%Y-%m"));//ÿ�±���һ��
	StrTime=_T("D:\\�������ݱ���\\���Ƽ��ϵͳ")+StrTime+_T(".mdb");
	//::CopyFile("D:\\������Ŀ\\BlastTest\\Samples\\Data\\���Ƽ��ϵͳ.mdb","D:\\�������ݱ���\\���Ƽ��ϵͳ.mdb",false); 
	//zhj delete::CopyFile("D:\\������Ŀ\\BlastTest\\Samples\\Data\\���Ƽ��ϵͳ.mdb",StrTime,TRUE);

	CADODataSet DataSetRate;
	DataSetRate.SetConnection(::GetConnection());
	DataSetRate.Open(_T("select * from UserSetUp"));
	m_HunHightRata=DataSetRate.ReadFloatValue(_T("�ش��߶�ϵ��"));
	if(m_HunHightRata>1.5)
	{
		m_HunHightRata=1;
		MessageBox(_T("��ȡ�ش��߶�ϵ��ʧ�ܣ�"));
	}
	else if(m_HunHightRata<0.5)
	{
		m_HunHightRata=1;
		MessageBox(_T("��ȡ�ش��߶�ϵ��ʧ�ܣ�"));
	}

	//��ȡ��������ײ���ֵ
	m_CorTimerDelay=DataSet.ReadFloatValue("��������ײ��ʱ");
	if((m_CorTimerDelay<=-2000) | (m_CorTimerDelay>=2000))
	{
		m_CorTimerDelay=0;
		DataSet.SetFieldValue(_T("��������ײ��ʱ"),m_CorTimerDelay);
		DataSet.Save();
	}
	CString str;
	str.Format("%0.1f", m_SLThreshold);//zhj modify V1.03 �ĳ���ʾһλС��
	//MessageBox(str);
	GetDlgItem(IDC_EDIT18)->SetWindowText(str);

	//((CComboBox *)GetDlgItem(IDC_TESTSEL))->AddString();
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//ȡ��PLC���ݵĻص�����
DWORD WINAPI ReadPlcDataPro(LPVOID lpParameter )
{
	unsigned char SBuf[20];
	int RetValue;
	int StrLen, SLoop;
	CString str;
	while(TRUE)
	{	
		
		//PlcData.SoundLevel = PlcData.m_SoundComm.ReadSound();//��ȡ�����ƣ�����IsPeakMode;

		if(WAIT_TIMEOUT==WaitForSingleObject(GEvent,1000))
			continue;
		switch(PlcData.ComFunIndex)
		{
		case COMM_NORMAL_READ:
			if(!GetOutBuffCount())
				RetValue=FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_SOUND_LEVEL_REAL,0x01, SBuf);//������
			if(RetValue==-2) 
			{
				SetEvent(GEvent);//���¼�����Ϊ���ź�״̬
				Sleep(200);//20150616��1000��Ϊ500��20150626���
				continue;//˵��û����Ӧ
			}
			PlcData.SoundLevel=(float)FATEK_GetIntData(0,(char *)SBuf)/(10.0f);
			if(!GetOutBuffCount())
				RetValue = FATEK_ReadMultiReg(CMD_SLAVE_ADDR, CMD_ADDR_PRESS, 0x01, SBuf);//��ȡѹ��ֵ
			if(RetValue==-2) 
			{
				SetEvent(GEvent);//���¼�����Ϊ���ź�״̬
				Sleep(200);//20150616��1000��Ϊ500��20150626���
				continue;//˵��û����Ӧ
			}
			//PlcData.m_HdyPress.m_PressValue = (float)FATEK_GetIntData(0, (char *)SBuf) *8.0 / MAX_PRESSDAVALUE; //; (10.0f);
			//PlcData.m_HdyPress.m_PressValue = GetFloatData(0,(char *)SBuf);
			PlcData.m_HdyPress.m_PressValue = (float)FATEK_GetIntData(0, (char *)SBuf)/100.0; //; (10.0f);//20160812�ĳ�
			PlcData.m_HdyPress.m_PressValue = int(PlcData.m_HdyPress.m_PressValue * 100) / 100.0;//ȡ��λС��

			if(!GetOutBuffCount())
				FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_HUN_REAL_HIGHT,0x01, SBuf);//ƽ̨�߶�
			//PlcData.PlateHight=(float)FATEK_GetIntData(0,(char *)SBuf)/(10.0f);
			//PlcData.PlateHight = (float)FATEK_GetIntData(0, (char *)SBuf);//20160809�ĳ�ֱ����ʾ
			if (EquipmentType == TESTTYPE_ZHUJI)	PlcData.PlateHight = (float)FATEK_GetIntData(0, (char *)SBuf) / 10.f;//zhj modify V1.01
			//zhj modify V1.04  V1.08
		    else PlcData.PlateHight = (float)FATEK_GetIntData(0, (char *)SBuf);
			//else PlcData.PlateHight = (float)FATEK_GetIntData(0, (char *)SBuf)/10.f;

			if(!GetOutBuffCount())//��ȡ����������ʵʱֵ
				FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_SMOG_REAL,0x01, SBuf);//��ȡ����ʵʱֵ

			//zhj modify v1.03 PlcData.SmogReal=(float)FATEK_GetIntData(0,(char *)SBuf)/(409.6f);   // x*10/4096   ��Ӧ5V�Ĵ����� 10V����������409.6f
			PlcData.SmogReal=(float)FATEK_GetIntData(0,(char *)SBuf);   


			
			if (PlcData.CallPressFun ==1)
			{
				//PlcData.m_HdyPress.PressFun();//����Һѹ��ѹ����
				PlcData.CallPressFun = 0;
			}

			if(!GetOutBuffCount())
				FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_GET_HUN_ERROR,0x01, SBuf);//ץ������

			if(FATEK_GetIntData(0,(char *)SBuf)>=3)
			{
				//AfxMessageBox(_T("�޷�ץ����������ز�λ�Ƿ����쳣�����ԣ�"));//20160812ɾ����ʾ��Ϣ
				SBuf[0]='1';
				for(int Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)//��λϵͳ
				{
					if(0<FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_SYSTEM_RESET,1,SBuf))
						break;
				}
			}
			break;
		case COMM_WRITE_HDY_DATA:
			//RetValue=FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_SOUND_LEVEL_REAL,0x01, SBuf);//������
			if (PlcData.m_HdyPress.m_HdyData.IsBuffEmpty())//������ͻ�����Ϊ��
			{
				PlcData.ComFunIndex = COMM_NORMAL_READ;
				PlcData.COmErrCount = 0;
				SetEvent(GEvent);//���¼�����Ϊ���ź�״̬
				continue;
			}
			switch (PlcData.m_HdyPress.m_HdyData.m_Cmd[PlcData.m_HdyPress.m_HdyData.m_SendPtr])
			{
			case CMD_Moror:   /*�������*/
				SBuf[0] = (char)(PlcData.m_HdyPress.m_HdyData.m_Data[PlcData.m_HdyPress.m_HdyData.m_SendPtr])+48;
				RetValue=FATEK_WriteMultiByte(CMD_SLAVE_ADDR, CMD_ADDR_MOTOR, 1, SBuf);
				break;

			case CMD_FwVal:/*ǰ����ŷ�*/
				SBuf[0] = (char)(PlcData.m_HdyPress.m_HdyData.m_Data[PlcData.m_HdyPress.m_HdyData.m_SendPtr])+48;
				RetValue = FATEK_WriteMultiByte(CMD_SLAVE_ADDR, CMD_ADDR_FWVAL, 1, SBuf);
				break;
			case CMD_BackVal: /*���˵�ŷ�*/
				SBuf[0] = (char)(PlcData.m_HdyPress.m_HdyData.m_Data[PlcData.m_HdyPress.m_HdyData.m_SendPtr])+48;
				RetValue = FATEK_WriteMultiByte(CMD_SLAVE_ADDR, CMD_ADDR_BACKVAL, 1, SBuf);
				break;

			case CMD_AccVal:/*��������ŷ�*/
				SBuf[0] = (char)(PlcData.m_HdyPress.m_HdyData.m_Data[PlcData.m_HdyPress.m_HdyData.m_SendPtr])+48;
				RetValue = FATEK_WriteMultiByte(CMD_SLAVE_ADDR, CMD_ADDR_ACCVAL, 1, SBuf);
				break;
			case CMD_DAVALUE:/*д��DAֵ*/
				/*str.Format("%2X", PlcData.m_HdyPress.m_HdyData.m_Data[PlcData.m_HdyPress.m_HdyData.m_SendPtr]);//�ӻ���ַת��Ϊ16�����ַ���
				str.Replace(' ', '0');
				memset(SBuf, '0', 10);
				StrLen = str.GetLength();
				if (StrLen>4) StrLen = 4;
				for (SLoop = 0; SLoop<StrLen; SLoop++)
				{
					SBuf[3 - SLoop] = str.GetAt(StrLen - 1 - SLoop);
				}
				RetValue= FATEK_WriteMultiReg(CMD_SLAVE_ADDR, CMD_ADDR_DAVALUE, 1, SBuf);//����ֵ����0����˵��д��ɹ�
				*/
				RetValue = 1;//20160812������ʱ
				break;
			default:
				PlcData.ComFunIndex = COMM_NORMAL_READ;
				PlcData.COmErrCount = 0;
				SetEvent(GEvent);//���¼�����Ϊ���ź�״̬
				break;
			}
			if(RetValue<0) //ͨ��ʧ��
			{
				PlcData.COmErrCount++;
				SetEvent(GEvent);//���¼�����Ϊ���ź�״̬
				Sleep(30);
				continue;//˵��û����Ӧ
			}
			else//ͨ�ųɹ�
			{
				PlcData.COmErrCount=0;
				PlcData.m_HdyPress.m_HdyData.AddSendPtr();
			}
				
			if (PlcData.COmErrCount>5)//ͨ�Ŵ�������������
			{
				AfxMessageBox(_T("д��Һѹϵͳ����ʧ�ܣ�"));
				PlcData.ComFunIndex=COMM_NORMAL_READ;
				PlcData.COmErrCount=0;
			}

			//PlcData.ComFunIndex=COMM_NORMAL_READ;//д����ɺ󷵻ص���ȡʵʱ����
			break;

		default:
			PlcData.ComFunIndex=COMM_NORMAL_READ;
			break;
		}
		SetEvent(GEvent);//���¼�����Ϊ���ź�״̬

		//FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_PLATE_HIGHT_REAL,0,SBuf);
		//PlcData.PlateHight;
		//Sleep(200);//20150616��1000��Ϊ500
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
	//��ʾ��������������
	this->GetClientRect(&rect);
	////��ʾ��ͼƬ�ؼ���
	//GetDlgItem(IDC_LOGINIMG)->GetWindowRect(&rect);
	//ScreenToClient(&rect);//��ʾ����������������Ҫ�����
	//��ʾ��������������
	dc.StretchBlt(rect.left, rect.top, rect.right, rect.bottom, &bmpdc,
		rect.left, rect.top, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	SetTimer(2,500,NULL);//���ڸ���Data����ʵ,������һ��֮���Զ��رն�ʱ��
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

void CBlastTestDlg::OnQuitWindows() //---����Ч
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
		//pDC->SetBkMode(TRANSPARENT);   //���ñ���͸��,�������ʱ����ȷ��˼
	
		//pDC->SetTextColor(RGB(100,255,255));//����������ɫ
		pDC->SetBkColor(RGB(255,255,255));//20150616���Ӹ���估��EDIT2\EDIT3�������ж�

		return   HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	if (pWnd->GetDlgCtrlID() == IDC_TIMEDIS)
	{
		//pDC->SetBkMode(TRANSPARENT);   //���ñ���͸��,�������ʱ����ȷ��˼
		pDC->SetBkColor(RGB(244,244,245));
		//pDC->SetTextColor(RGB(100,255,255));//����������ɫ
		
		return   HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
//����˲鿴��¼��ť------��Ч
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
		//FATEK_ReadMultiReg(CMD_SLAVE_ADDR,"D00302",0x01, SBuf);//������
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
		if(m_HunReleaseBtn)//������ڷŴ�
		{
			if(m_SmogReal<PlcData.SmogReal)//������������ֵ
				m_SmogReal=(int(PlcData.SmogReal*10))/10.0f;
		}
		else
		{
			m_SmogReal=(int(PlcData.SmogReal*10))/10.0f;
			if(m_SmogThrMode==1)//�Զ��ɼ�����ֵģʽ
			{
				m_SmogSetValue=m_SmogReal+m_Smog_bigvalue;
			}
		}
		//�޸ĸ߶���ʾ20150625
		//m_PlateHight=m_ZeroLevelHight-m_PlateHight;
		if(m_TestSpInBtn==0)//����ȷ��֮��ŻῪʼ������Ļ����
		{
			CString str;
			GetDlgItem(IDC_EDIT7)->GetWindowText(str);//ҩƷ����Ϊ�գ�������
			if(str!=_T(""))
			{
				if(isdigit(str.GetAt(str.GetLength()-1)))//��������������
					UpdateData(FALSE);
			}
		}

	}
	if(2==nIDEvent)//����DataGrid����ʾ
	{
		KillTimer(2);

		C_Recordset RSet=m_Ado_OPRecord.GetRecordset();
		RSet.Requery(adConnectUnspecified);
		m_DG_OpRecord.ReBind();                        //ʵ����datagrid�еı��� 
		m_DG_OpRecord.Refresh(); 

		RSet=m_Ado1.GetRecordset();
		RSet.Requery(adConnectUnspecified);
		m_DataGrid1.ReBind();                        //ʵ����datagrid�еı��� 
		m_DataGrid1.Refresh(); 

		
	}
	if(3==nIDEvent)////����Һѹ��ѹ
	{
		if (m_HunReleaseBtn)//������ڷŴ�
		{
			PlcData.CallPressFun = 1;
		}
	}
	if(4==nIDEvent)//��ȡ�Ŵ�״̬��1s����һ��
	{
		KillTimer(4);
		//MessageBox(_T("��ȷ���������Ѿ��򿪣�"),_T("�����ƴ���ʾ"));

		SetTimer(1,500,NULL);//���ڸ��´���������ʾ
		SetTimer(2,500,NULL);//���ڸ���Data����ʵ,������һ��֮���Զ��رն�ʱ��	
		SetTimer(3, 500, NULL);//����Һѹ��ѹ	
	}

 
	CDialog::OnTimer(nIDEvent);
}

BOOL CBlastTestDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}
//д���¼
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
	DataSet.SetFieldValue(_T("����ʱ��"),(_variant_t)CurrTime);
	DataSet.SetFieldValue(_T("��������"),(_variant_t)SData);
	DataSet.Save();
	DataSet.Open(_T("select * from OperateRecord "));//�ٴβ�ѯ����Ч��ѯ��ָʾΪ�������ܹ�����DataGrid����ʾ��
	RefreshDGrid(m_Ado_OPRecord,m_DG_OpRecord,_T("SELECT  top 50 * FROM OperateRecord order by ����ʱ�� DESC"));
}
void CBlastTestDlg::SetParperBtnBit(char sw)
{
	if(0==sw)//0-װҩ���,1-����װҩ
	{
		m_BtnPrepar.LoadBitmaps(IDB_PREPAR);
		m_BtnRealse.LoadBitmaps(IDB_REALSE);//�ش��ͷŰ�ť���
		WriteLog(_T("װҩ���"));
	}
	else
	{
		m_BtnPrepar.LoadBitmaps(IDB_PREPARFINISH);
		m_BtnRealse.LoadBitmaps(IDB_RELEASEGRAY);//�ش��ͷŰ�ť��Ϊ�ɲ���״̬
		WriteLog(_T("��ʼװҩ"));
	}
	//m_BtnReport.SubclassDlgItem(IDC_BTN_REPORT,this);
	m_BtnRealse.SizeToContent();
	m_BtnRealse.RedrawWindow();
	
	m_BtnPrepar.SizeToContent();
	m_BtnPrepar.RedrawWindow();
}
//װҩ����ɰ�ť
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
	if(m_HunReleaseBtn)//���ڷŴ�
	{
		MessageBox(_T("���ڷŴ����޷�װҩ"));
		return;
	}
	if(m_TestSpInBtn==1)
	{
		MessageBox(_T("����ȷ��ʵ�����"));
		return;
	}
	if(m_TestSpNo==_T(""))
	{
		MessageBox(_T("������Ų���Ϊ��"));
		return;
	}
	if(m_PlateMode)//�ֶ�ƽ̨�޷�װҩ
	{
		MessageBox(_T("�ֶ�ƽ̨�޷�װҩ��"));
		return;
	}

	m_PlaceExpPrepar=!m_PlaceExpPrepar;
	//SetParperBtnBit(m_PlaceExpPrepar);//�ı�ͼƬ��ʾ
	//����Ŵ�
	if(!WriteByte(CMD_SLAVE_ADDR,CMD_HUN_RELESAE,0))
	{
		MessageBox(_T("����Ŵ���־ʧ�ܣ�"));
	}
	
	if(!WriteByte(CMD_SLAVE_ADDR,CMD_PLACE_EXPLOSIVE,m_PlaceExpPrepar))
	{
		m_PlaceExpPrepar=!m_PlaceExpPrepar;//��ָʾ�����
	    //MessageBox(_T("��PLCͨ��ʧ�ܣ�"));
		WriteLog(_T("��PLCͨ��ʧ�ܣ�"));
		return ;
	}
	//v1.09add
	if (!m_PlaceExpPrepar)
	{
		if (!WriteByte(CMD_SLAVE_ADDR, CMD_JIAYA, 1))
		{
			//MessageBox(_T("��PLCͨ��ʧ�ܣ�"));
			WriteLog(_T("��PLCͨ��ʧ�ܣ�"));
		}
	}
	SetParperBtnBit(m_PlaceExpPrepar);//�ı�ͼƬ��ʾ
	SetBlastIndexBit(0);//�ı䱬ը״ָ̬ʾ�Ƶ���ʾΪ��ɫ
			
	
}
//ˢ�����ݿ���ʾ
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
//��Ʒ���ݰ�ť
void CBlastTestDlg::OnTestSpBtn() 
{
	// TODO: Add your control notification handler code here
	CTestSpData dlg;
	if(m_HunReleaseBtn==1)//���ڷŴ�
	{
		MessageBox(_T("��ȴ��Ŵ���ɣ�"));
		return;
	}
	dlg.DoModal();
}
//ʵ����������
void CBlastTestDlg::OnTestcondsetup() 
{
	// TODO: Add your control notification handler code here
	CTestConditon dlg;

	if(m_HunReleaseBtn==1)//���ڷŴ�
	{
		MessageBox(_T("��ȴ��Ŵ���ɣ�"));
		return;
	}

	dlg.DoModal();
	//��ʼ������ѡ�������б�
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from TestConditionSetup"));
	m_HunchKg=DataSet.ReadFloatValue(_T("�ش�����"));
	PlcData.m_HdyPress.m_PressSetValue = dlg.m_PressSet;
	m_TestType=dlg.m_Type;
	SetTestMode();//���õ�ǰ�Ĳ��Է�ʽ
	
}
//�����༭���Ƿ��д�룻0--����д�룻1--��д��
void CBlastTestDlg::EditWriteAble(bool sw)
{
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT2))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT3))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT4))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT5))->SetReadOnly(!sw);
	((CEdit *)GetDlgItem(IDC_EDIT6))->SetReadOnly(!sw);
	//((CEdit *)GetDlgItem(IDC_EDIT7))->SetReadOnly(!sw);//ҩƷ����ʼ�տɱ༭
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
//��Ʒ�������밴ťͼƬ
void CBlastTestDlg::SetTestSpInBtnBit(char sw)
{
	//����ʵ������
	if(sw)
		m_TestSpIn.LoadBitmaps(IDB_TESTSPINOKBTN);
	else
		m_TestSpIn.LoadBitmaps(IDB_TestSpIn);
	m_TestSpIn.SizeToContent();
	m_TestSpIn.RedrawWindow();
}
//��Ʒ�������밴ť
void CBlastTestDlg::OnTestSpInBtn() 
{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	
	bool SPExist=0;//Ϊ1��ʾ֮ǰ�Ѿ����������¼
	//PlaySound(_T("C:\\Sound\\SoundTest.wav"),NULL, SND_ASYNC|SND_NODEFAULT );//���Ų�����
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	
	if(SetTestMode()==INVALID_TEST_MODE)
	{
		MessageBox(_T("��֧�ֵ���������"));
		return;
	}
	if ((PlcData.m_HdyPress.m_PressSetValue>6.3) || (PlcData.m_HdyPress.m_PressSetValue<0))
	{
		MessageBox(_T("ѹ���趨ֵ������6.3MP��0MP֮��"));
		return;
	}

	int nSel;   	
    // ��ȡ��Ͽ�ؼ����б����ѡ���������   
    nSel = ((CComboBox *)GetDlgItem(IDC_TESTSEL))->GetCurSel();   
    // ����ѡ����������ȡ�����ַ���   
	if(nSel>=0)
		((CComboBox *)GetDlgItem(IDC_TESTSEL))->GetLBText(nSel, m_TestSel);  
	UpdateData(TRUE);
	DataSet.Open(_T("select * from SampleTable where �������='")+m_TestSel+_T("'"));
	if(DataSet.GetRecordCount()<1)
	{
		MessageBox(_T("�޴�����"));
		return ;
	}

	if(m_TestSpInBtn==0)//�����������밴ť
	{		
		m_TestSpName=DataSet.ReadStringValue(_T("��Ʒ����"));
		m_TestSpNo=m_TestSel;
		
		DataSet.Open(_T("select * from TestConditionSetup"));
		m_TestGroupNo=DataSet.ReadIntValue(_T("ʵ������"));
		m_TestNo=DataSet.ReadIntValue(_T("ÿ�����"));
		m_TestSpKg=DataSet.ReadFloatValue(_T("��������"));
		m_HunchHight=DataSet.ReadFloatValue(_T("�ش��߶�"));
		m_HunchKg=DataSet.ReadFloatValue(_T("�ش�����"));
		m_TestType=DataSet.ReadStringValue(_T("ʵ������"));
		m_TestPerName=DataSet.ReadStringValue(_T("ʵ��Ա"));
		m_TestTemp=DataSet.ReadFloatValue(_T("ʵ���¶�"));
		m_TestHumi=DataSet.ReadFloatValue(_T("ʵ��ʪ��"));
		m_StepLength=DataSet.ReadFloatValue(_T("��������"));
		UpdateData(FALSE);
		m_TestSpInBtn=TRUE;
		/*�򿪱༭������빦��*/
		EditWriteAble(m_TestSpInBtn);
		WriteLog(_T("��������"));

		m_MaxGroupNo=m_TestGroupNo;
		m_MaxTestNo=m_TestNo;

		if(m_Testmode==CRT_HEIGHT_MODE)
		{
			if(m_MaxGroupNo>1)
			{
				MessageBox(_T("������߷�ֻ֧����һ��ʵ��!"));
				m_MaxGroupNo=1;
				m_TestGroupNo=1;
			}
		}

#ifdef _DEBUG

#endif

		if(m_Testmode==CRT_HEIGHT_MODE)//������߷�����
		{

		}
	}
	else if(m_TestSpInBtn==1)//��������ȷ�ϰ�ť
	{
		DataSet.Open(_T("select * from TestData where �������='")+m_TestSel+_T("' order by ʵ������ desc") );//��ѯʵ���¼���Ƿ���ڸ���Ʒ�ļ�¼
		//20151021��������
		if(DataSet.GetRecordCount()>(m_MaxGroupNo*m_MaxTestNo))
		{
			MessageBox(_T("��������ʵ���Ѿ���ɣ��޷�������"));
			return ;
		}

		if(DataSet.GetRecordCount()>0)
		{
			if(IDOK==AfxMessageBox(_T("�Ѿ����ڸ���Ʒ�ļ�¼��ѡ��ȷ�Ͻ�ɾ����֮ǰ�ļ�¼��ѡ��ȡ�������ϴν��У����������������š�"),MB_OKCANCEL))//�������ȷ�ϰ�ť
			{
				//DataSet.Delete();
				DataSet.ClearRecord(_T("select * from TestData where �������='")+m_TestSel+_T("'"));
				//DataSet.Save();//��֮ǰ�ļ�¼��ɾ����
				m_TestSpInBtn=0;
			}
			else  //ȡ����ť
			{
				//��ʣ��������
				m_TestGroupNo=m_MaxGroupNo-(short)floor((DataSet.GetRecordCount()/((float)m_MaxTestNo)));

				m_TestNo=m_MaxTestNo-(DataSet.GetRecordCount() % m_MaxTestNo);
				//m_TestSpNo=_T("");//��Ʒ��Ÿ�ֵΪ��
				//m_TestSel=_T("");
				//UpdateData(FALSE);
				m_TestSpInBtn=0;//����ʼ��������
				
				DataSet.moveFirst();//20151021����
				m_TestType=DataSet.ReadStringValue(_T("ʵ������"));
				m_HunchHight=DataSet.ReadFloatValue(_T("�ش��߶�"));
				m_LastBlastStatus=DataSet.ReadIntValue(_T("��ը״̬"));

				SetTestMode();

				//����ʵʱ��ը�� 20150625
				if(m_Testmode==BLAST_RATE_MODE)//��ը���ʷ�
				{
					CADODataSet DataRate;
					DataRate.SetConnection(::GetConnection());
					DataRate.Open(_T("select top 500 * from TestData where �������='")+m_TestSpNo+_T("'"));
					int AllCount=DataRate.GetRecordCount();
					DataRate.Open(_T("select top 500 * from TestData where �������='")+m_TestSpNo+_T("' and ��ը״̬=1"));
					m_BlastRate=(100*DataRate.GetRecordCount())/AllCount;
				}

				if(m_Testmode==CRT_HEIGHT_MODE)//������߷����飬��ȡ�ش��߶�
				{
					m_CRTHeight.m_CurrHeight=m_HunchHight;
					//�����¸߶�
					m_CRTTestStep=CH_STEEP_FINISH;
					CRTCalNextH(m_LastBlastStatus);	

				}
				SPExist=TRUE;
			}
			
		}
		else  //�����ڸ���Ʒ��ʵ���¼
		{
			m_TestSpInBtn=0;//����ʼ��������

		}

	//	CalTestResult();//����20150626


		//���ش��߶�д��PLC
		char SBuf[10];
		CString str;
		int Hight = (int)(m_HunchHight * 10);
		//zhj delete V1.11 int Hight=(int)(m_HunchHight*m_HunHightRata*10);//20150607��Ӹ߶�ϵ��
		//zhj delete V1.10 int Hight = (int)(m_HunchHight*m_HunHightRata);//20150809ȡ����10
		str.Format("%2X",Hight);//�ӻ���ַת��Ϊ16�����ַ���
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
			if(0<FATEK_WriteMultiReg(CMD_SLAVE_ADDR,CMD_HUN_HEIGHT,1,(unsigned char *)SBuf))//����ֵ����0����˵��д��ɹ�
				break;
		}
		SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
		if (Fatek>=CMD_SEND_REPART_COUNT)
		{
			MessageBox(_T("д���ش��߶�ʧ�ܣ�"));
		}
		//д�����߶�
		//int HunZero=int(ReadHunZero(m_HunchKg)*10);
		int HunZero = int(ReadHunZero(m_HunchKg));//20160812�ĳɲ���10
		WriteInt(CMD_SLAVE_ADDR,CMD_HUN_ZEROPOINT,HunZero);
		//д�������ײ��ʱ
		//WriteInt(CMD_SLAVE_ADDR,CMD_COR_TIMER_DELAY,(int)m_CorTimerDelay);
		//д��ѹ���趨,ת����DAֵ
		//WriteInt(CMD_SLAVE_ADDR, CMD_ADDR_PRESSSET, (int)((PlcData.m_HdyPress.m_PressSetValue + PlcData.m_HdyPress.m_PressSetOffset)*MAX_PRESSDAVALUE / 8.0));
	    WriteInt(CMD_SLAVE_ADDR, CMD_ADDR_PRESSSET, (int)((PlcData.m_HdyPress.m_PressSetValue + PlcData.m_HdyPress.m_PressSetOffset)*100));//20160812���
		//WriteFloat(CMD_SLAVE_ADDR, CMD_ADDR_PRESSSET, PlcData.m_HdyPress.m_PressSetValue);//20160808���
		DataSet.Open(_T("select * from SampleTable where �������='")+m_TestSel+_T("'"));//������д����Ʒ���ݿ�
		if(DataSet.GetRecordCount()<=0)
		{
			MessageBox(_T("��Ʒ�������޸���Ʒ�ļ�¼��������֮������ȷ�ϣ�"));
		}
		else
		{
			DataSet.SetFieldValue(_T("�䴸����"),m_HunchKg);
			DataSet.SetFieldValue(_T("���"),m_HunchHight);
			DataSet.SetFieldValue(_T("ҩ��"),m_TestSpKg);
			if (EquipmentType == TESTTYPE_MOCHA) DataSet.SetFieldValue(_T("��ѹѹ��"), PlcData.m_HdyPress.m_PressSetValue);
			DataSet.Save();

		}

		WriteLog(_T("ȷ������"));
		/*�����༭������빦��*/
		EditWriteAble(m_TestSpInBtn);

		if(m_Testmode==CRT_HEIGHT_MODE)//������߷�����
		{
			m_CRTHeight.m_HeightSet=m_HunchHight;
			m_CRTHeight.m_LastHeight=m_HunchHight;
			if(SPExist==0)
			{
				m_CRTHeight.m_CurrHeight=m_HunchHight;
				m_CRTTestStep=CH_STEEP_BEGIN;
			}
			else
				m_CRTTestStep=CH_STEEP_FINISH;//֮ǰ�Ѿ����������¼��˵��Ԥ�������Ѿ���ɣ�ֱ�ӿ�ʼʵ��

		}

	}
	SetTestSpInBtnBit(m_TestSpInBtn);
	
}
//��ʵ���¼�Ի���
void CBlastTestDlg::OnRecordView() 
{
	// TODO: Add your control notification handler code here
	//CPrintReportDlg dlg;
	//dlg.DoModal();
	CTestRecordView dlg;
	dlg.m_SpNo=m_TestSpNo;
	dlg.m_Type=m_TestType;

	if(m_HunReleaseBtn==1)//���ڷŴ�
	{
		MessageBox(_T("��ȴ��Ŵ���ɣ�"));
		return;
	}

	dlg.DoModal();
	//���¼���ʣ����������ֹ�м�¼��ɾ��
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	if(m_TestSpNo.IsEmpty()) return;   
	DataSet.Open(_T("select * from TestData where �������='")+m_TestSpNo+_T("'"));//��ѯʵ���¼���Ƿ���ڸ���Ʒ�ļ�¼
	m_TestGroupNo=m_MaxGroupNo-(short)floor((DataSet.GetRecordCount()/((float)m_MaxTestNo)));		
	m_TestNo=m_MaxTestNo-(DataSet.GetRecordCount()% m_MaxTestNo);
	//��ֹ�б��������޸ģ������������߷���Ԥ�����飬���¼���߶�
	DataSet.Open(_T("select * from TestData where �������='")+m_TestSpNo+_T("' order by ʵ������ desc"));
	if((m_Testmode==CRT_HEIGHT_MODE))
	{
		if(DataSet.GetRecordCount()>0)
		{
			DataSet.moveFirst();
			if(m_CurrBlastStatus!=DataSet.ReadIntValue(_T("��ը״̬")))
			{
				m_CurrBlastStatus=DataSet.ReadIntValue(_T("��ը״̬"));
				if(m_CRTTestStep!=m_CRTTestLastStep)
				{
					if(m_CRTTestStep>0) m_CRTTestStep--;//�ص���һ��	
				}
				m_CRTHeight.m_CurrHeight=m_HunchHight;
				CRTCalNextH(m_CurrBlastStatus);
			
			}
		}
	}
	UpdateData(FALSE);

}
//�ֶ����Զ�ƽ̨֮��ѡ��
void CBlastTestDlg::OnOpplatebtn() 
{
	// TODO: Add your control notification handler code here
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);

	WriteByte(CMD_SLAVE_ADDR,CMD_SYSTEM_RESET,1);//��λϵͳ20150628
	Sleep(100);//�ȴ�PLC��λ��ɡ�

	//��λװҩ��Ŵ���ť20150628
	m_HunReleaseBtn=0;
	SetHunBtnBit(m_HunReleaseBtn);
	m_PlaceExpPrepar=0;
	SetParperBtnBit(m_PlaceExpPrepar);

	//m_PlateMode=!m_PlateMode;
	char PlatModeOld = m_PlateMode;
	m_PlateMode += 1;
	if (EquipmentType == TESTTYPE_ZHUJI) m_PlateMode %= 2;
	else m_PlateMode %= 3;

	if (m_PlateMode == 2)//�ֶ���ѹ
	{
		m_PlateUp.ShowWindow(FALSE);//��������ʾ��ť
		m_PlateDown.ShowWindow(FALSE);
		m_BtnPrepar.ShowWindow(FALSE);
		m_BtnRealse.ShowWindow(FALSE);
		m_HdyForwardBtn.ShowWindow(TRUE);
		m_HdyBackBtn.ShowWindow(TRUE);
	}
	else
	{
		sprintf((char *)SBuf, _T("%d"), m_PlateMode);
		WaitForSingleObject(GEvent, 1000);//�ȴ��ź�
		int Fatek;
		for (Fatek = 0; Fatek<CMD_SEND_REPART_COUNT; Fatek++)//д��PLC
		{
			if (0<FATEK_WriteMultiByte(CMD_SLAVE_ADDR, CMD_PLATE_MODE_SWITCH, 1, SBuf))//����ֵ����0����˵��д��ɹ�
				break;
		}
		SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
		if (Fatek >= CMD_SEND_REPART_COUNT)
		{
			m_PlateMode = PlatModeOld;
			//MessageBox(_T("��PLCͨ��ʧ�ܣ�"));
			WriteLog(_T("��PLCͨ��ʧ�ܣ�"));
			return;
		}
	}

	if (m_PlateMode == 1)/*�ֶ�ƽ̨*/
	{
		if (EquipmentType == TESTTYPE_ZHUJI) m_PlateModeBtn.LoadBitmaps(IDB_MANUALPLATEBTN_ZJ);
		else m_PlateModeBtn.LoadBitmaps(IDB_MANULPLATEBTN);

		
	}
	else if (m_PlateMode == 2)/*�ֶ���ѹ*/
	{
		m_PlateModeBtn.LoadBitmaps(IDB_MANULPRESS);
	
	}
	else /*�Զ�ƽ̨*/
	{
		if (EquipmentType == TESTTYPE_ZHUJI) m_PlateModeBtn.LoadBitmaps(IDB_AUTOPLATEBTN_ZJ);
		else m_PlateModeBtn.LoadBitmaps(IDB_AUTOPLATEBTN);

	}
	m_PlateModeBtn.SizeToContent();
	m_PlateModeBtn.RedrawWindow();

	WriteByte(CMD_SLAVE_ADDR,CMD_HUN_RELESAE,0);//����Ŵ�ָ��
	WriteByte(CMD_SLAVE_ADDR,CMD_PLACE_EXPLOSIVE,0);//��װҩ��־λ

	//���������½���־λ
/*
	SBuf[0]='0';
	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
	FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_PLATE_DOWN,1,SBuf);
	FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_PLATE_UP,1,SBuf);
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������*/

	//m_BtnReport.SubclassDlgItem(IDC_BTN_REPORT,this);
	
	SBuf[0]='0';
	//�½�ƽ̨��ť��λ
	m_PlateDownState=0;
	m_PlateDown.LoadBitmaps(IDB_PLATEDOWN);
	m_PlateDown.SizeToContent();
	m_PlateDown.RedrawWindow();
	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
	FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_PLATE_DOWN,1,SBuf);
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
	//����ƽ̨��ť��λ
	m_PlateUpState=0;
	m_PlateUp.LoadBitmaps(IDB_PLATEUP);
	m_PlateUp.SizeToContent();
	m_PlateUp.RedrawWindow();
	SBuf[0]='0';
	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
	FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_PLATE_UP,1,SBuf);
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
	
	if(m_PlateMode==1)
	{
		m_PlateUp.ShowWindow(TRUE);//��������ʾ��ť
		m_PlateDown.ShowWindow(TRUE);
		m_BtnPrepar.ShowWindow(FALSE);
		m_BtnRealse.ShowWindow(FALSE);
		m_HdyForwardBtn.ShowWindow(FALSE);
		m_HdyBackBtn.ShowWindow(FALSE);
		WriteLog("�л����ֶ�ƽ̨");
		
	}
	else if (m_PlateMode ==2)//��ѹ
	{
		WriteLog("�л����ֶ�ƽ̨");
	}
	else
	{
		m_PlateUp.ShowWindow(FALSE);//��������ʾ��ť
		m_PlateDown.ShowWindow(FALSE);
		m_BtnPrepar.ShowWindow(TRUE);
		m_BtnRealse.ShowWindow(TRUE);
		m_HdyForwardBtn.ShowWindow(FALSE);
		m_HdyBackBtn.ShowWindow(FALSE);
		WriteLog("�л����Զ�ƽ̨");
	}
}
//ƽ̨��������
void CBlastTestDlg::OnPlateUp() 
{
	// TODO: Add your control notification handler code here
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	if(!m_PlateMode)//������Զ�ģʽ
	{
		MessageBox(_T("�����л����ֶ�ģʽ"));
		return;
	}
	if(m_PlateDownState)//��������½�
	{
		MessageBox(_T("����ֹͣ�½�����"));
		return;
	}
	m_PlateUpState=!m_PlateUpState;
	sprintf((char *)SBuf,_T("%d"),m_PlateUpState);
	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
	int Fatek;
	for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_PLATE_UP,1,SBuf))//����ֵ����0����˵��д��ɹ�
			break;
	}
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������

	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		m_PlateUpState=!m_PlateUpState;//��ָʾ�����
		//MessageBox(_T("��PLCͨ��ʧ�ܣ�"));
		WriteLog(_T("��PLCͨ��ʧ�ܣ�"));
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
//ƽ̨�½�
void CBlastTestDlg::OnPlateDown() 
{
	// TODO: Add your control notification handler code here
	//int i=GetFocus()->GetDlgCtrlID();//���������İ�ť��ID
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	if(!m_PlateMode)//������Զ�ģʽ
	{
		MessageBox(_T("�����л����ֶ�ģʽ"));
		return;
	}
	if(m_PlateUpState)//��������½�
	{
		MessageBox(_T("����ֹͣ��������"));
		return;
	}
	m_PlateDownState=!m_PlateDownState;
	sprintf((char *)SBuf,_T("%d"),m_PlateDownState);
	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
	int Fatek;
	for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_PLATE_DOWN,1,SBuf))//����ֵ����0����˵��д��ɹ�
			break;
	}
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		m_PlateDownState=!m_PlateDownState;//��ָʾ�����
		//MessageBox(_T("��PLCͨ��ʧ�ܣ�"));
		WriteLog(_T("��PLCͨ��ʧ�ܣ�"));
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
//����������
void CBlastTestDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
/*	int id=GetFocus()->GetDlgCtrlID();//���������İ�ť��ID
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	SBuf[0]='1';
	if(m_PlateMode)//�ֶ�ģʽ
	{
		if(id==IDC_PLATE_UP)//ƽ̨������ť������
		{
			FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0300"),1,SBuf);
		}
		else if(id==IDC_PLATE_DOWN)//ƽ̨�½���ť������
		{
			FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0302"),1,SBuf);
		}
	}*/

	CDialog::OnLButtonDown(nFlags, point);
}
//�������ͷ�
void CBlastTestDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	/*int id=GetFocus()->GetDlgCtrlID();//���������İ�ť��ID
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	SBuf[0]='0';
	if(m_PlateMode)//�ֶ�ģʽ
	{
		if(id==IDC_PLATE_UP)//ƽ̨������ť���ͷ�
		{
			FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0300"),1,SBuf);
		}
		else if(id==IDC_PLATE_DOWN)//ƽ̨�½���ť���ͷ�
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
	int id=GetFocus()->GetDlgCtrlID();//���������İ�ť��ID
	if(pMsg->message==WM_LBUTTONDOWN)
	{
		SBuf[0]='1';
		if(m_PlateMode)//�ֶ�ģʽ
		{
			if(id==IDC_PLATE_UP)//ƽ̨������ť������
			{
				FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0300"),1,SBuf);
				WriteLog(_T("ƽ̨��������"));
			}
			else if(id==IDC_PLATE_DOWN)//ƽ̨�½���ť������
			{
				FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0302"),1,SBuf);
				WriteLog(_T("ƽ̨�½�����"));
			}
		}	
	}
	if(pMsg->message==WM_LBUTTONUP)
	{
		SBuf[0]='0';
		if(m_PlateMode)//�ֶ�ģʽ
		{
			if(id==IDC_PLATE_UP)//ƽ̨������ť���ͷ�
			{
				GetDlgItem(IDC_PLATE_UP)->PostMessage(WM_KILLFOCUS, 0, 0);
				ReleaseCapture();
				FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0300"),1,SBuf);
				WriteLog(_T("ƽ̨�����ͷ�"));
			}
			else if(id==IDC_PLATE_DOWN)//ƽ̨�½���ť���ͷ�
			{
				GetDlgItem(IDC_PLATE_DOWN)->PostMessage(WM_KILLFOCUS, 0, 0);
				ReleaseCapture();
				FATEK_WriteMultiByte(CMD_SLAVE_ADDR,_T("M0302"),1,SBuf);
				WriteLog(_T("ƽ̨�½��ͷ�"));
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
		AfxMessageBox(_T("������Ч"));
	}
	

	return CDialog::PreTranslateMessage(pMsg);
}
//���÷Ŵ���ť����ɫ
void CBlastTestDlg::SetHunBtnBit(char sw)//sw:0--gray,1--blue
{
	if(sw==1)//��ʾ��ʼװҩ
	{
		m_BtnRealse.LoadBitmaps(IDB_RELEASEGRAY);//�ش��ͷŰ�ť���
	}
	else if(sw==0)
	{
		m_BtnRealse.LoadBitmaps(IDB_REALSE);//�ش��ͷŰ�ť��Ϊ�ɲ���״̬
	}
	//m_BtnReport.SubclassDlgItem(IDC_BTN_REPORT,this);
	m_BtnRealse.SizeToContent();
	m_BtnRealse.RedrawWindow();
}
//���÷Ŵ���ť����ɫ
void CBlastTestDlg::SetBlastIndexBit(char sw)//sw:0--gray,1--��ը
{
	if(sw==1)//��ʾ��ʼװҩ
	{
		m_BlastIndex.LoadBitmaps(IDB_BLAST_INDEX_RED);//�ش��ͷŰ�ť���
	}
	else if(sw==0)
	{
		m_BlastIndex.LoadBitmaps(IDB_BLAST_INDEX_GRAY);//�ش��ͷŰ�ť��Ϊ�ɲ���״̬
	}
	//m_BtnReport.SubclassDlgItem(IDC_BTN_REPORT,this);
	m_BlastIndex.SizeToContent();
	m_BlastIndex.RedrawWindow();
}
void CBlastTestDlg::CalTestResult()//����ʵ��������ը�ʣ����жȵȡ�
{
	//������Ľ��д����Ʒ���ݱ���
	float * PRate=new float[m_MaxGroupNo];//���ڴ��ÿ��ı�ը��
	float Rate,LRate,LRateTmp,HRate,HRateTmp;//�ܵĸ��ʣ��������ޣ���������
	char buf[20];
	CString str;
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	//�ж�ʵ���Ƿ����
	//...
	//����ÿ��ʵ��ı�ը��
	for(int i=0;i<m_MaxGroupNo;i++)
	{
		memset(buf,0x00,20);
		sprintf(buf,_T("A%2d__"),i+1);
		str=buf;
		str+=m_TestSpNo;
		str.Replace(' ','0');
		DataSet.Open(_T("select  * from TestData where ��ը״̬=1 and �Զ���� like'")+str+_T("' order by ʵ������ DESC"));
		PRate[i]=DataSet.GetRecordCount()/((float)m_MaxTestNo);
	}
	float P2=PRate[1];
	float P1=PRate[0];
	//�����ܵı�ը��
	DataSet.Open(_T("select  * from TestData where ��ը״̬=1 and �������='")+m_TestSpNo+_T("' order by ʵ������ DESC"));
	Rate=DataSet.GetRecordCount()/((float)m_MaxTestNo*m_MaxGroupNo);
	//�ж��������Բ���
	CADODataSet DataRate;
	CString RateSql;

	LRate=0;
	HRate=0;
	int MaxTestNo;//ʵ�����������ʱʵ������С��25�飬�������25�飬����1000�飬�������1000��
	MaxTestNo=m_MaxTestNo;
	if(MaxTestNo<25) MaxTestNo=25;//��ʵ���������ڱ���С�
	if(MaxTestNo>100) MaxTestNo=1000;

	for(int j=0;j<m_MaxGroupNo;j++)
	{
		RateSql.Format(_T("select top 1 * from PTable where ����>=%.2f and ����>=%d  order by ���� ASC,���� ASC"),PRate[j],MaxTestNo-1);
		DataRate.Open(RateSql);
		LRateTmp=DataRate.ReadFloatValue(_T("��������"));
		HRateTmp=DataRate.ReadFloatValue(_T("��������"));
		int i;
		for(i=0;i<m_MaxGroupNo;i++)
		{
			if((PRate[i]<LRateTmp) | (PRate[i]>HRateTmp))//����������
			{
				//����������������
				LRate=0;
				HRate=0;
				//MessageBox(_T("ʵ����֮��������Բ��죬������ʵ�飡"));
				break;
			}
		}
		if(i>=m_MaxGroupNo)//˵������ѭ��������������ʶ���������������
		{
			LRate=LRateTmp;
			HRate=HRateTmp;
			break;
		}

	}	
	if(LRate+HRate<=0) MessageBox(_T("ʵ����֮��������Բ��죬������ʵ�飡"));

	//�����д����Ʒ���ݵ����ݿ�
	DataSet.Open(_T("select  * from SampleTable where �������='")+m_TestSpNo+_T("'"));
	if(DataSet.GetRecordCount()>0)
	{
		DataSet.SetFieldValue(_T("����"),Rate);
		//д���������޺�����
		DataSet.SetFieldValue(_T("��������"),LRate);
		DataSet.SetFieldValue(_T("��������"),HRate);
		DataSet.Save();
	}
	else
	{
		MessageBox(_T("��Ʒ�����в�����ָ����Ʒ���"));
	}

	delete []PRate;
}

float CBlastTestDlg::CRTCalNextH(int BlastStatus)//������һ�����Ը߶�
{
	//Ԥ���������״̬ 
	switch(m_CRTTestStep)
	{
	case CH_STEEP_BEGIN://��һ��Ԥ������
		if(1==BlastStatus)
		{
			m_CRTHeight.m_CurrHeight-=4*m_StepLength;
		}
		else
			m_CRTHeight.m_CurrHeight+=4*m_StepLength;
		
		m_CRTTestStep=CH_STEEP_4D;
		m_CRTTestLastStep=CH_STEEP_BEGIN;
		break;
	case CH_STEEP_4D://4D��������
		if(m_LastBlastStatus==BlastStatus)//����һ���б��Ľ����ͬ
		{
			if(1==BlastStatus)
			{
				m_CRTHeight.m_CurrHeight-=4*m_StepLength;
			}
			else
				m_CRTHeight.m_CurrHeight+=4*m_StepLength;
		}
		else//����һ���б��Ľ����ͬ
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
		if(m_LastBlastStatus==BlastStatus)//����һ���б��Ľ����ͬ
		{
			if(1==BlastStatus)
			{
				m_CRTHeight.m_CurrHeight-=2*m_StepLength;
			}
			else
				m_CRTHeight.m_CurrHeight+=2*m_StepLength;
		}
		else//����һ���б��Ľ����ͬ
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
		if(m_LastBlastStatus==BlastStatus)//����һ���б��Ľ����ͬ
		{
			if(1==BlastStatus)
			{
				m_CRTHeight.m_CurrHeight-=m_StepLength;
			}
			else
				m_CRTHeight.m_CurrHeight+=m_StepLength;
		}
		else//����һ���б��Ľ����ͬ
		{
			m_CRTTestStep=CH_STEEP_FINISH;
			if(1==BlastStatus)
			{
				m_CRTHeight.m_CurrHeight-=m_StepLength;
			}
			else
				m_CRTHeight.m_CurrHeight+=m_StepLength;
			//������Ʒ�����¼
			//m_CRTHeight.CreateSpRecord(m_TestSpNo);
			MessageBox(_T("Ԥ�������Ѿ���ɣ�"));

			CADODataSet DataSet;//20151125
			DataSet.SetConnection(::GetConnection());
			DataSet.ClearRecord(_T("select * from TestData where �������='")+m_TestSpNo+_T("' order by ʵ������ DESC"));
			m_TestNo=m_MaxTestNo;	
		}
		m_CRTTestLastStep=CH_STEEP_D;
		break;
	case CH_STEEP_FINISH: //���Ԥ������
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
//���α��Ʋ�����ɣ�д����Լ�¼��������ر༭�������
void CBlastTestDlg::WriteRecord()
{
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	int RetValue;
	int SoundLevelPeak;

	//20160815ȡ��ɾ����ʼ
	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
	RetValue=FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_SOUND_LEVEL_PEAK,1,SBuf);
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
	if(RetValue==-2)
	{
		MessageBox(_T("PLC����Ӧ��"));
		return;
	}
	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
	SoundLevelPeak=FATEK_GetIntData(0,(char *)SBuf);
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
	//20160815ȡ��ɾ������

	//SoundLevelPeak = PlcData.SoundLevel;//���������Ʒ�ֵ  //20160815ɾ��

	KillTimer(1);//��ͣ�������ݵĸ���
	m_SondLevel=SoundLevelPeak/(10.0f);
    if(m_SondLevel>140)
	{
		MessageBox(_T("�����ƹ����쳣��"));	
		m_SondLevel=0;
	}
	//��ʾ��ֵ���--�����ã�������ɾ��
	CString str;
	str.Format("%f",m_SondLevel);
	//MessageBox(str);
	GetDlgItem(IDC_EDIT18)->SetWindowText(str);
	//���ݷ�ֵ�ж��Ƿ�ը
	//...
	short BlastStatus;
	if((m_SondLevel>m_SLThreshold) |(m_SmogReal>m_SmogSetValue))//���ڷ�ֵ��ʾ��ը/������ڷ�ֵ
	{
		BlastStatus=1;
		SetBlastIndexBit(1);
	}
	else  //��ʾû�б�ը
	{
		BlastStatus=0;
		SetBlastIndexBit(0);
	}

	//20150718
	if(m_TestSpNo==_T(""))
	{
		MessageBox(_T("������Ų���Ϊ�գ�"));
		return;	
	}

	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	
	bool CRTLastTime=0;
	if(m_Testmode==CRT_HEIGHT_MODE)//�����������߷�20151019
	{
		if(m_CRTTestStep==CH_STEEP_D)//Ϊ���ж������һ�Σ�ɾ����¼
			CRTLastTime=TRUE;
		CRTCalNextH(BlastStatus);//����б������˸ı����ɾ����һ�����飬��Ҫ���¼���߶�

	//	if((CRTLastTime==TRUE) && (m_CRTTestStep==CH_STEEP_FINISH))//������߷���Ԥ��ʵ�����

// 		//�ж��Ƿ�Ҫд���¼
// 		if(m_CRTTestStep==CH_STEEP_FINISH)
// 		{
// 			
// 		}
		m_CurrBlastStatus=BlastStatus;//���汾��ʵ��ı�ը״̬ 
		//SetTimer(1,500,NULL);//���ڸ��´���������ʾ1021
		//m_LastBlastStatus=BlastStatus;//���汾��ʵ��ı�ը״̬ 
	}

	//�����Զ����
	CString StrAutoNo;
	char Buf[20];
	memset(Buf,0x00,20);
	sprintf(Buf, _T("A%2d%2d"), m_MaxGroupNo-m_TestGroupNo+1, m_MaxTestNo - m_TestNo+1);
	StrAutoNo=Buf;
	StrAutoNo.Replace(' ','0');
	StrAutoNo+=m_TestSpNo;

	//д���¼
	
	//���ָ����¼�Ѿ����ڣ���ɾ���ü�¼
	DataSet.Open(_T("select * from TestData where �Զ����='")+StrAutoNo+_T("'"));
	if(DataSet.GetRecordCount()>0)
	{
		DataSet.Delete();
		DataSet.Save();
	}
	DataSet.Open(_T("select top 100 * from TestData where �������='")+m_TestSpNo+_T("' order by ʵ������ DESC"));

	//д���¼
	DataSet.AddNew();
	DataSet.SetFieldValue(_T("�������"),(_variant_t)m_TestSpNo);
	DataSet.SetFieldValue(_T("�Զ����"),(_variant_t)StrAutoNo);
	DataSet.SetFieldValue(_T("��Ʒ����"),(_variant_t)m_TestSpName);
	DataSet.SetFieldValue(_T("��ը״̬"),(_variant_t)BlastStatus);
	if(BlastStatus)
		DataSet.SetFieldValue(_T("ʵ����"),_T("��ը"));
	else
		DataSet.SetFieldValue(_T("ʵ����"),_T("����"));
	
	COleDateTime CurrTime;
	CurrTime=CurrTime.GetCurrentTime();
	//CString StrTime;
	//StrTime=CurrTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	DataSet.SetFieldValue(_T("ʵ������"),(_variant_t)CurrTime);	
	DataSet.SetFieldValue(_T("ʵ��Ա"),(_variant_t)m_TestPerName);	
	DataSet.SetFieldValue(_T("ʵ������"),(_variant_t)m_TestType);
	DataSet.SetFieldValue(_T("�ش�����"),(_variant_t)m_HunchKg);
	DataSet.SetFieldValue(_T("�ش��߶�"),(_variant_t)m_HunchHight);
	DataSet.SetFieldValue(_T("ҩ��"),(_variant_t)m_TestSpKg);
	DataSet.SetFieldValue(_T("�����Ʒ�ֵ"),(_variant_t)m_SondLevel);//�����Ʒ�ֵ20150626

	//DataSet.SetFieldValue(_T("�¶ȱ仯"),(_variant_t)---);
	DataSet.Save();

	if(m_TestNo>1)//����ʵ��δ���
		m_TestNo--;
	else//����ʵ�����
	{
		if(m_TestGroupNo>1)
		{
			m_TestGroupNo--;
			CADODataSet DataSetUp;
			DataSetUp.SetConnection(::GetConnection());
			DataSetUp.Open(_T("select * from TestConditionSetup"));
			m_TestNo=DataSetUp.ReadIntValue(_T("ÿ�����"));
			//DataSetUp.CloseDataSet();
		}
		else //���������һ�飬���Ѿ����,˵������ʵ�����
		{
			if(m_Testmode==CRT_HEIGHT_MODE)
			{
				m_CRTHeight.m_SpNo=m_TestSpNo;
				m_CRTHeight.m_DataTable=_T("TestData");
				m_CRTHeight.m_d=m_StepLength;
				m_CRTHeight.CalCRTResult();//���㲢д����
				m_CRTHeight.WriteRecord();
			}
			else if(m_Testmode==BLAST_RATE_MODE)
			{
				CalTestResult();
			}
			else
				MessageBox(_T("��Ч���������ͣ�"));

			
			MessageBox(_T("����ʵ�����"));
			m_TestGroupNo=0;
		}
	}
	//20152225
	if((m_CRTTestLastStep==CH_STEEP_D) && (m_CRTTestStep==CH_STEEP_FINISH))
	{
		DataSet.ClearRecord(_T("select * from TestData where �������='")+m_TestSpNo+_T("' order by ʵ������ DESC"));
		m_TestNo=m_MaxTestNo;
	}
	//�������������߷�ģʽ,��������Ԥ������״̬����������������
 //	if((m_Testmode==CRT_HEIGHT_MODE) && (m_CRTTestStep!=CH_STEEP_FINISH))
//	{
//		m_TestGroupNo=m_MaxGroupNo;
//		m_TestNo=m_MaxTestNo;
//	}

	SetTimer(1,500,NULL);//���ڸ��´���������ʾ

	/*//����ʵʱ������
	int OldCount=DataSet.GetRecordCount();
	m_BlastRate=(BlastStatus+(OldCount*m_BlastRate))/(OldCount+1);*/

	//����ʵʱ��ը�� 20150625
	if(m_Testmode==BLAST_RATE_MODE)
	{
		CADODataSet DataRate;
		DataRate.SetConnection(::GetConnection());
		DataRate.Open(_T("select top 500 * from TestData where �������='")+m_TestSpNo+_T("'"));
		int AllCount=DataRate.GetRecordCount();
		DataRate.Open(_T("select top 500 * from TestData where �������='")+m_TestSpNo+_T("' and ��ը״̬=1"));
		m_BlastRate=(100*DataRate.GetRecordCount())/AllCount;
	}


	//Ϊ�������ϸ�����ʵ���������ٶ��ѯһ�εķ�ʽ
	DataSet.Open(_T("select top 100 * from TestData where �������='")+m_TestSpNo+_T("' order by ʵ������ DESC"));
	RefreshDGrid(m_Ado1,m_DataGrid1,_T(" SELECT Top 6  *  FROM TestData order by ʵ������ DESC"));
	//�Զ������ļ�
	/*zhj delete V1.01 CString StrTime;
	StrTime=CurrTime.Format(_T("%Y-%m-%d"));
	StrTime=_T("D:\\�������ݱ���\\")+StrTime+m_TestSpNo+_T(".mdb");
	::CopyFile("D:\\������Ŀ\\BlastTest\\Samples\\Data\\���Ƽ��ϵͳ.mdb",StrTime,false);*/

}

DWORD WINAPI PlcFiStatusPro(LPVOID lpParameter );  // thread data
//��ȡPLC�Ŵ���״̬					
DWORD WINAPI PlcFiStatusPro(LPVOID lpParameter )
{
	int RetValue;
	int M30,D320;
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	HWND hwnd=((HunThreedData *)lpParameter)->hwnd;
	for(int i=0;i<STATUS_WAITE_TIME;i++)
	{
		WaitForSingleObject(GEvent,1000);//�ȴ��ź�
		RetValue=FATEK_ReadMultiByte(CMD_SLAVE_ADDR,CMD_FINISH_STATUS,1,SBuf);
		M30=FATEK_GetByteData(0,(char *)SBuf);
		//FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_FINISH_STATUS_INT,1,SBuf);
		//D320=FATEK_GetIntData(0,(char *)SBuf);
		D320 = 0;

		//zhj modify V1.15 
		//FATEK_ReadMultiReg(CMD_SLAVE_ADDR, CMD_ADDR_PRESS, 0x01, SBuf);//��ȡѹ��ֵ
		//PlcData.m_HdyPress.m_PressValue = (float)FATEK_GetIntData(0, (char *)SBuf)/100.0; //; (10.0f);
		//PlcData.m_HdyPress.m_PressValue = int(PlcData.m_HdyPress.m_PressValue * 100) / 100.0;//ȡ��λС��
		
		SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
		if((RetValue!=-2) & (M30 | D320))//����һ���ֽڵ��ַ�ת��Ϊ������,
		{
			/*m_HunReleaseBtn=0;//���ش��ͷű�־����
			SBuf[0]='0';
			FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_HUN_RELESAE,1,SBuf);
			WriteLog(_T("�ش��ͷ����"));
			//д��ʵ���¼����
			//����ʵʱ�����ʣ�����ʵ��������ʣ�����
			WriteRecord();//д���¼*/
			::PostMessage(hwnd,WM_HUNFINISH,NULL,1);
			return TRUE;
		}
		WaitForSingleObject(GEvent,1000);//�ȴ��ź�
		//zhj delete V1.01 ֻɾһ�仹����  if(0>FATEK_ReadMultiByte(CMD_SLAVE_ADDR, CMD_DOOR_SWITECH, 1, SBuf))
		//{
		//	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
		//}
		//else
		//{
		//	if(!FATEK_GetByteData(0,(char *)SBuf))//��״̬Ϊ0��ʾδ�رգ��޷��Ŵ�
		//	{				
		//		SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
		//	//zhj delete V1.01��ʱ��ʱδ�ҵ�ԭ��	::PostMessage(hwnd,WM_HUNFINISH,NULL,2);//�Ŵ򿪣��Զ���ֹ����
		//		return TRUE;
		//	}
		//	
		//}
		SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
		Sleep(200);
	}
	//��ʱ
	::PostMessage(hwnd,WM_HUNFINISH,NULL,0);
	return TRUE;
}
//�Զ�����Ϣ��Ӧ�������ȴ��ش��ͷ���ɡ�
LRESULT  CBlastTestDlg::OnHunFinish(WPARAM wParam, LPARAM lParam)
{
	unsigned char SBuf[10];
	memset(SBuf,0x00,10);
	int Status=int(lParam);
	if (EquipmentType == TESTTYPE_ZHUJI)
	{ }
	else
	{ 
	if (abs(PlcData.m_HdyPress.m_PressValue - PlcData.m_HdyPress.m_PressSetValue) >= 0.5)//����0.5mp����ѹʧ��
	{
		//zhj delete V1.11 Status = 3;
	}
	else//��ѹ�ɹ�
	{
		//���浱ǰ��DAֵ���´ε���

	}
	}
	//��ʱ0.1��
	DWORD dwStart = GetTickCount();
	DWORD dwEnd = dwStart;
	do{
		dwEnd = GetTickCount() - dwStart;
	} while (dwEnd <100);
	//��PLC�ش��ͷ���ɱ�־��0
	m_HunReleaseBtn=0;
	SetHunBtnBit(m_HunReleaseBtn);//��ͼƬ��ɿ���
	/*SBuf[0]='0';
	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
	for(int Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_WriteMultiByte(CMD_SLAVE_ADDR,CMD_FINISH_STATUS,1,SBuf))//����ֵ����0����˵��д��ɹ�
			break;
	}
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		MessageBox(_T("����Ŵ���ɱ�־ʧ�ܣ�����ʵ����Ч��"));
	}*///20150625
	if(0<FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_HUN_REAL_HIGHT,1,SBuf))//��ȡƽ̨�߶�
		m_PlateHight=(float)FATEK_GetIntData(0,(char *)SBuf)/(10.0f);
	//����Ŵ���ɱ�־λ
	/*WriteByte(CMD_SLAVE_ADDR,CMD_FINISH_STATUS,0);
	WriteInt(CMD_SLAVE_ADDR,CMD_FINISH_STATUS_INT,0);*///20150707ɾ��
	//��ʱ0.1�룬�ȴ���ȡ����ֵ
	dwStart = GetTickCount();
	dwEnd = dwStart;
	do{
		dwEnd = GetTickCount() - dwStart;
	} while (dwEnd <100);

	if(Status==1)
	{
		WriteRecord();
		WriteLog(_T("�Ŵ����"));
	}
	else if(Status==0)
	{
		WriteRecord();
		MessageBox(_T("�ȴ�PLC�Ŵ���ʱ����������ʧ�ܣ�"));
		WriteLog(_T("�ȴ���ʱ"));
	}
	else if(Status==2)
	{
		MessageBox(_T("��⵽�Ŵ򿪣�����ʵ����ֹ��"));
		WriteLog(_T("ʵ����ֹ"));
	}
	else if (Status == 3)//ѹ��������
	{
		MessageBox(_T("��ѹʧ�ܣ�ʵ����Ч"));
		WriteLog(_T("��ѹʧ��"));
	}
	else
	{
		MessageBox(_T("��Ч�ķ���ֵ��������Ч��"));
	}
	m_HunReleaseBtn=0;
	SetHunBtnBit(m_HunReleaseBtn);
	PlcData.m_SoundComm.m_IsPeakMode = FALSE;//�л���ʵʱֵģʽ
	
	return 0;
}
//д�뵱ǰ�ش������
char CBlastTestDlg::WriteHunZero(float HunKg,float ZeroNo)
{
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	CString str;
	str.Format(_T("select * from HunZeroPoint where �ش�����=%f"),HunKg);
	DataSet.Open(str);

	if(DataSet.GetRecordCount()>0)
	{
		DataSet.SetFieldValue(_T("���߶�"),ZeroNo);
		DataSet.Save();
		return TRUE;
	}
	else
	{
		MessageBox(_T("�����ڵ�ǰ�������ش���"));
		return FALSE;
	}
}
//��ȡ��ǰ�ش������
float CBlastTestDlg::ReadHunZero(float HunKg)
{
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	CString str;
	str.Format(_T("select * from HunZeroPoint where �ش�����=%f"),HunKg);
	DataSet.Open(str);
	if(DataSet.GetRecordCount()>0)
	{
		return DataSet.ReadFloatValue(_T("���߶�"));
	}
	else
	{
		MessageBox(_T("�����ڵ�ǰ�������ش���"));
		return 1000;
	}
}

//��1byte����д��PLC
                            //�ӻ���ַ��  д���ַ��     д������--������ʽ
char CBlastTestDlg::WriteByte(unsigned char Slave,CString Addr, char Dbyte)
{
	unsigned char SBuf[10];
	memset(SBuf,0,10);
	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
	sprintf((char *)SBuf,_T("%d"),Dbyte);
	int Fatek;
	for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_WriteMultiByte(Slave,Addr,1,SBuf))//����ֵ����0����˵��д��ɹ�
			break;
	}
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		//MessageBox(_T("��PLCͨ��ʧ�ܣ�"));
		WriteLog(_T("��PLCͨ��ʧ�ܣ�"));
		return FALSE;
	}
	//��ȡ
	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
	memset(SBuf,0,10);	
	for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_ReadMultiByte(Slave,Addr,1,SBuf))//����ֵ����0����˵����ȡ�ɹ�
			break;
	}
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		//MessageBox(_T("��PLCͨ��ʧ�ܣ�"));
		WriteLog(_T("��PLCͨ��ʧ�ܣ�"));
		return FALSE;
	}
	//�ж��Ƿ����
	char ReadData=FATEK_GetByteData(0,(char*)SBuf);
#ifdef _DEBUG3 
	return TRUE;
#endif
	if(Dbyte!=ReadData)
	{
		CString ErrMessage;
		ErrMessage=_T("��PLCд��ʧ�ܣ���ַ��")+Addr;
		MessageBox(ErrMessage);
		return FALSE;
	}		
	return TRUE;
}
//��1������������д��PLC
                            //�ӻ���ַ��  д���ַ��     д������--������ʽ
char CBlastTestDlg::WriteInt(unsigned char Slave,CString Addr, int DInt)
{
	unsigned char SBuf[10];

	CString str;
	str.Format("%2X",DInt);//�ӻ���ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	memset(SBuf,'0',10);
	int StrLen=str.GetLength();
	if(StrLen>4) StrLen=4;
	for(int SLoop=0;SLoop<StrLen;SLoop++)
	{
		SBuf[3-SLoop]=str.GetAt(StrLen-1-SLoop);
	}

	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
	int Fatek;
	for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_WriteMultiReg(Slave,Addr,1,SBuf))//����ֵ����0����˵��д��ɹ�
			break;
	}
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		//MessageBox(_T("��PLCͨ��ʧ�ܣ�"));
		WriteLog(_T("��PLCͨ��ʧ�ܣ�"));
		return FALSE;
	}
	//��ȡ
	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
	memset(SBuf,0,10);	
	for(Fatek=0;Fatek<CMD_SEND_REPART_COUNT;Fatek++)
	{
		if(0<FATEK_ReadMultiReg(Slave,Addr,1,SBuf))//����ֵ����0����˵��д��ɹ�
			break;
	}
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
	if (Fatek>=CMD_SEND_REPART_COUNT)
	{
		//MessageBox(_T("��PLCͨ��ʧ�ܣ�"));
		WriteLog(_T("��PLCͨ��ʧ�ܣ�"));
		return FALSE;
	}
#ifdef _DEBUG3 
	return TRUE;
#endif

	//�ж��Ƿ����
	if(DInt!=FATEK_GetIntData(0,(char*)SBuf))
	{
		CString ErrMessage;
		ErrMessage=_T("��PLCд��ʧ�ܣ���ַ��")+Addr;

	

		//zhj modify V1.05 MessageBox(ErrMessage);
		WriteLog(ErrMessage);
		return FALSE;
	}		
	return TRUE;
}
//��1������������д��PLC
//�ӻ���ַ��  д���ַ��     д������--������ʽ
char CBlastTestDlg::WriteFloat(unsigned char Slave, CString Addr, float DInt)
{
	unsigned char SBuf[10];
	union FIType aa;
	CString str;
	aa.FData = DInt;
	str.Format("%4X", aa.Dint);//�ӻ���ַת��Ϊ16�����ַ���
	str.Replace(' ', '0');
	memset(SBuf, '0', 10);
	int StrLen = str.GetLength();
	if (StrLen>8) StrLen = 8;
	for (int SLoop = 0; SLoop<StrLen; SLoop++)
	{
		SBuf[7 - SLoop] = str.GetAt(StrLen - 1 - SLoop);
	}

	WaitForSingleObject(GEvent, 1000);//�ȴ��ź�
	int Fatek;
	for (Fatek = 0; Fatek<CMD_SEND_REPART_COUNT; Fatek++)
	{
		if (0<FATEK_WriteMultiReg(Slave, Addr, 2, SBuf))//����ֵ����0����˵��д��ɹ�
			break;
	}
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
	if (Fatek >= CMD_SEND_REPART_COUNT)
	{
		//MessageBox(_T("��PLCͨ��ʧ�ܣ�"));
		WriteLog(_T("��PLCͨ��ʧ�ܣ�"));
		return FALSE;
	}
	
	return TRUE;
}
//�Ŵ���ť
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
	if(m_PlateMode)//�ֶ�ģʽ
	{
		MessageBox(_T("���Ƚ�ƽ̨�л����Զ�ģʽ��"));
		return;
	}
	if(m_PlaceExpPrepar)//����װҩ���޷��Ŵ�
	{
		MessageBox(_T("����װҩ���޷��Ŵ�"));
		return;
	}
	if(m_HunReleaseBtn)//���ڷŴ��������Ч
	{
		return;
	}
	//��ȡ����״̬
	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
	if(0>FATEK_ReadMultiByte(CMD_SLAVE_ADDR, CMD_DOOR_SWITECH, 1, SBuf))
	{
		SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
		MessageBox(_T("��ȡ��״̬ʧ��"));
		return;
	}
	else
	{
		if(!FATEK_GetByteData(0,(char *)SBuf))//��״̬Ϊ0��ʾδ�رգ��޷��Ŵ�
		{
			MessageBox(_T("�뽫װҩ�Źرգ�"));
			SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
			return;
		}

	}
	//20150618
    int RetValue,M30,D320;
	RetValue=FATEK_ReadMultiByte(CMD_SLAVE_ADDR,CMD_FINISH_STATUS,1,SBuf);
	M30=FATEK_GetByteData(0,(char *)SBuf);
	FATEK_ReadMultiReg(CMD_SLAVE_ADDR,CMD_FINISH_STATUS_INT,1,SBuf);
	D320=FATEK_GetIntData(0,(char *)SBuf);
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
	if((RetValue!=-2) & (M30 | D320))//����һ���ֽڵ��ַ�ת��Ϊ������,
	{
		MessageBox(_T("���Ȱ�װҩ�������ɱ�־"));
		SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
		return;
	}
 //
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������

	m_HunReleaseBtn=1;//�Ŵ���ť��־�л�

	//���PLC�Ŵ���ɱ�־λ
	/*WriteByte(CMD_SLAVE_ADDR,CMD_FINISH_STATUS,0);
	WriteInt(CMD_SLAVE_ADDR,CMD_FINISH_STATUS_INT,0);*///20150626�ڷŴ�OnHunFinish()��ɵĴ������Ѿ����

	//���¼����ش��߶�
	//m_CRTHeight.m_CurrHeight=m_HunchHight;
	//CRTCalNextH(m_CurrBlastStatus);

	//m_HunchHight=m_CRTHeight.m_CurrHeight;//20160607
	if (m_TestType == _T("������߷�"))
	{
		m_HunchHight = m_CRTHeight.m_CurrHeight;//20160614
	}

	if((m_HunchHight*m_HunHightRata<40) || (m_HunchHight*m_HunHightRata>650))//�ش��߶ȱ�����245mm��550mm֮��
	{
		m_HunchHight=0;
		m_HunReleaseBtn=0;//�Ŵ���ť��־�л�
		MessageBox(_T("�ش��߶ȱ�����40mm��650mm֮��"));
	}
	m_LastBlastStatus=m_CurrBlastStatus;//���汾��ʵ��ı�ը״̬ 

//zhj modify 	if(!WriteInt(CMD_SLAVE_ADDR,CMD_HUN_HEIGHT,(int)(m_HunchHight*m_HunHightRata*10)))//д���ش��߶� 20151019
	if(!WriteInt(CMD_SLAVE_ADDR,CMD_HUN_HEIGHT,(int)(m_HunchHight*10)))//д���ش��߶� 20151019
	{
		m_HunReleaseBtn=0;//�Ŵ���ť��־�л�
		return;
	}

	if(!WriteByte(CMD_SLAVE_ADDR,CMD_PLACE_EXPLOSIVE,0))//���װҩ��־
	{
		m_HunReleaseBtn=0;//�Ŵ���ť��־�л�
		return;
	}
	//д��Ŵ�����
	if(!WriteByte(CMD_SLAVE_ADDR,CMD_HUN_RELESAE,m_HunReleaseBtn))
	{
		m_HunReleaseBtn=0;//�Ŵ���ť��־�л�
		return;
	}
	
	WriteLog(_T("��ʼ�Ŵ�"));
	SetHunBtnBit(m_HunReleaseBtn);//����ť��ɻ�ɫ

	PlcData.m_SoundComm.m_IsPeakMode = TRUE;//�л�����ֵģʽ

	PlcData.m_HdyPress.InitPressFun(0);//

	//zhj add V1.15
	PlcData.m_HdyPress.m_HdyData.AddSendData(CMD_DAVALUE, 0);
	//�������߳�
	HunThreedData * FinsihThreedData=new HunThreedData;
	FinsihThreedData->hwnd=m_hWnd;
	HANDLE hTr1=CreateThread(NULL,0,PlcFiStatusPro,(LPVOID)FinsihThreedData,NULL,NULL);
	CloseHandle(hTr1);
		
}

void CBlastTestDlg::OnDropdownTestsel() 
{
	// TODO: Add your control notification handler code here
	//��ʼ������ѡ�������б�
	((CComboBox *)GetDlgItem(IDC_TESTSEL))->ResetContent();
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select top 20 * from SampleTable order by ID DESC"));
	for(int i=0;i<DataSet.GetRecordCount();i++)
	{
		((CComboBox *)GetDlgItem(IDC_TESTSEL))->AddString(DataSet.ReadStringValue(_T("�������")));
		DataSet.Next();
	}
	((CComboBox *)GetDlgItem(IDC_TESTSEL))->SetCurSel(0);

}
//�����û�����
void CBlastTestDlg::OnUserSetup() 
{
	// TODO: Add your control notification handler code here
	CUserSetupDlg dlg;
	dlg.m_SLThreshold=m_SLThreshold;
	dlg.m_SmogSheld=m_SmogSetValue;
	dlg.m_HunCurrentH=m_PlateHight;//�ش��߶�
	dlg.m_HunKg=m_HunchKg;
	dlg.m_ZeroSetValue=ReadHunZero(m_HunchKg);
	dlg.m_CRHHunHight=m_CRTHeight.m_CurrHeight;
	dlg.m_Step=m_CRTTestStep;
	dlg.m_PressOffset = PlcData.m_HdyPress.m_PressSetOffset;

	if(m_HunReleaseBtn==1)//���ڷŴ�
	{
		MessageBox(_T("��ȴ��Ŵ���ɣ�"));
		return;
	}

	WaitForSingleObject(GEvent,1000);//�ȴ��ź�
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
	SetEvent(GEvent);//��Ϊ���ź�״̬�����߳̿��Է�������
	
	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());
	DataSet.Open(_T("select * from UserSetup"));
	if(!DataSet.IsadoEOF())
	{
		m_CorTimerDelay=DataSet.ReadFloatValue("��������ײ��ʱ");
	}
	else
		m_CorTimerDelay=0;
}

void CBlastTestDlg::OnSelchangeTestsel() 
{
	// TODO: Add your control notification handler code here
	int nSel;   	
    // ��ȡ��Ͽ�ؼ����б����ѡ���������   
    nSel = ((CComboBox *)GetDlgItem(IDC_TESTSEL))->GetCurSel();   
    // ����ѡ����������ȡ�����ַ���   
	if(nSel>=0)
		((CComboBox *)GetDlgItem(IDC_TESTSEL))->GetLBText(nSel, m_TestSel);  	
}
//�޸�ҩƷ����
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
	if(m_TestType==_T("������߷�"))
	{
		m_Testmode=CRT_HEIGHT_MODE;
		return CRT_HEIGHT_MODE;
	}
	else if(m_TestType==_T("��ը���ʷ�"))
	{
		m_Testmode=BLAST_RATE_MODE;
		return BLAST_RATE_MODE;
	}
	else if(m_TestType==_T("Ħ���жȷ�"))
	{
		m_Testmode=FRI_RATE_MODE;
		return BLAST_RATE_MODE;
	}
	else
	{
		//m_Testmode=BLAST_RATE_MODE;
		MessageBox(_T("��֧�ֵ��������ͣ�"));
		return INVALID_TEST_MODE;
	}
}

//�ֶ��͸�ǰ����ť�������
void CBlastTestDlg::OnBnClickedHdyforward()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	PlcData.m_HdyPress.m_ManualF = !PlcData.m_HdyPress.m_ManualF;
	PlcData.m_HdyPress.m_ManualB = 0;
	PlcData.m_HdyPress.m_HdyData.AddSendData(CMD_FwVal, PlcData.m_HdyPress.m_ManualF);
	PlcData.m_HdyPress.m_HdyData.AddSendData(CMD_BackVal, 0);
	PlcData.m_HdyPress.m_HdyData.AddSendData(CMD_DAVALUE, 0);

	SetMPBtn();
}

//�ֶ��͸׺��˰�ť�������
void CBlastTestDlg::OnBnClickedHdyback()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


int startAdd = 0;
// ģ��ʵ������
void CBlastTestDlg::AddRecord(bool result)
{
	unsigned char SBuf[10];
	memset(SBuf, 0x00, 10);









	//20150718
	if (m_TestSpNo == _T(""))
	{
		MessageBox(_T("������Ų���Ϊ�գ�"));
		return;
	}

	CADODataSet DataSet;
	DataSet.SetConnection(::GetConnection());

	bool CRTLastTime = 0;


	//�����Զ����
	CString StrAutoNo;
	char Buf[20];
	memset(Buf, 0x00, 20);
	sprintf(Buf, _T("A%2d%2d"), m_MaxGroupNo-m_TestGroupNo+1, m_MaxTestNo - m_TestNo+1);
	StrAutoNo = Buf;
	StrAutoNo.Replace(' ', '0');
	StrAutoNo += m_TestSpNo;

	//д���¼

	//���ָ����¼�Ѿ����ڣ���ɾ���ü�¼
	DataSet.Open(_T("select * from TestData where �Զ����='") + StrAutoNo + _T("'"));
	if (DataSet.GetRecordCount()>0)
	{
		DataSet.Delete();
		DataSet.Save();
	}
	DataSet.Open(_T("select top 100 * from TestData where �������='") + m_TestSpNo + _T("' order by ʵ������ DESC"));

	//д���¼
	DataSet.AddNew();
	DataSet.SetFieldValue(_T("�������"), (_variant_t)m_TestSpNo);
	DataSet.SetFieldValue(_T("�Զ����"), (_variant_t)StrAutoNo);
	DataSet.SetFieldValue(_T("��Ʒ����"), (_variant_t)m_TestSpName);

	if (result)
	{
		DataSet.SetFieldValue(_T("��ը״̬"), (_variant_t)1);
		DataSet.SetFieldValue(_T("ʵ����"), _T("��ը"));
	}
	else
	{
		DataSet.SetFieldValue(_T("��ը״̬"), (_variant_t)0);
		DataSet.SetFieldValue(_T("ʵ����"), _T("����"));
	}
		

	COleDateTime CurrTime;
	COleDateTimeSpan ds;
	
	ds.SetDateTimeSpan(0, 0, startAdd*3, 0);
	startAdd++;
	CurrTime = CurrTime.GetCurrentTime() + ds;
	//CString StrTime;
	//StrTime=CurrTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	DataSet.SetFieldValue(_T("ʵ������"), (_variant_t)CurrTime);
	DataSet.SetFieldValue(_T("ʵ��Ա"), (_variant_t)m_TestPerName);
	DataSet.SetFieldValue(_T("ʵ������"), (_variant_t)m_TestType);
	DataSet.SetFieldValue(_T("�ش�����"), (_variant_t)m_HunchKg);
	DataSet.SetFieldValue(_T("�ش��߶�"), (_variant_t)m_HunchHight);
	DataSet.SetFieldValue(_T("ҩ��"), (_variant_t)m_TestSpKg);
	DataSet.SetFieldValue(_T("�����Ʒ�ֵ"), (_variant_t)m_SondLevel);//�����Ʒ�ֵ20150626

	//DataSet.SetFieldValue(_T("�¶ȱ仯"),(_variant_t)---);
	DataSet.Save();

	if (m_TestNo>1)//����ʵ��δ���
		m_TestNo--;
	else//����ʵ�����
	{
		if (m_TestGroupNo>1)
		{
			m_TestGroupNo--;
			CADODataSet DataSetUp;
			DataSetUp.SetConnection(::GetConnection());
			DataSetUp.Open(_T("select * from TestConditionSetup"));
			m_TestNo = DataSetUp.ReadIntValue(_T("ÿ�����"));
			//DataSetUp.CloseDataSet();
		}
		else //���������һ�飬���Ѿ����,˵������ʵ�����
		{
			if (m_Testmode == CRT_HEIGHT_MODE)
			{
				m_CRTHeight.m_SpNo = m_TestSpNo;
				m_CRTHeight.m_DataTable = _T("TestData");
				m_CRTHeight.m_d = m_StepLength;
				m_CRTHeight.CalCRTResult();//���㲢д����
				m_CRTHeight.WriteRecord();
			}
			else if (m_Testmode == BLAST_RATE_MODE)
			{
				CalTestResult();
			}
			else
				MessageBox(_T("��Ч���������ͣ�"));


			MessageBox(_T("����ʵ�����"));
			m_TestGroupNo = 0;
			startAdd = 0;
		}
	}
	//20152225
	if ((m_CRTTestLastStep == CH_STEEP_D) && (m_CRTTestStep == CH_STEEP_FINISH))
	{
		DataSet.ClearRecord(_T("select * from TestData where �������='") + m_TestSpNo + _T("' order by ʵ������ DESC"));
		m_TestNo = m_MaxTestNo;
	}
	

	//Ϊ�������ϸ�����ʵ���������ٶ��ѯһ�εķ�ʽ
	DataSet.Open(_T("select top 100 * from TestData where �������='") + m_TestSpNo + _T("' order by ʵ������ DESC"));
	RefreshDGrid(m_Ado1, m_DataGrid1, _T(" SELECT Top 6  *  FROM TestData order by ʵ������ DESC"));

	
}


void CBlastTestDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
