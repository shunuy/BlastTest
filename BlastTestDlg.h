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

#define CMD_PLATE_UP  _T("M0300")  /*0Ϊֹͣ״̬��1Ϊ����*/
#define CMD_PLATE_DOWN _T("M0302")  /*0Ϊֹͣ״̬��1Ϊ�½�*/
#define CMD_PLACE_EXPLOSIVE _T("M0304") /*0-��ҩ��ɣ�1--��ʼ��ҩ*/
#define CMD_HUN_RELESAE  _T("M0306")  /*Ϊ1�����ͷţ�0�ͷ����*/
#define CMD_SYSTEM_RESET _T("M0308")   /*д1��λϵͳ*/
#define CMD_PLATE_MODE_SWITCH _T("M0309")  /*�ֶ�ƽ̨���Զ�ƽ̨�л�*/

//v1.09add
#define CMD_JIAYA _T("M0040") 

#define CMD_HUN_ZEROPOINT    _T("D00000")  /*�ش����*/
#define CMD_GET_HUN_ERROR    _T("D00005")  /*����3˵��ץ������*/
//#define CMD_SOUND_LEVEL_PEAK _T("D00304")   /*�����Ʒ�ֵ*/
//#define CMD_SOUND_LEVEL_REAL _T("D00305")   /*������˲ʱֵ*/
#define CMD_SOUND_LEVEL_PEAK _T("D00318")   /*�����Ʒ�ֵ*/
#define CMD_SOUND_LEVEL_REAL _T("D00316")   /*������˲ʱֵ*/
#define CMD_HUN_REAL_HIGHT   _T("D00306")   /*�ش�������ʵ�ʸ߶�,����10��ʾ*/
#define CMD_PLATE_HIGHT_REAL _T("D00302")    /*ʵ��ƽ̨�߶�*/
#define CMD_HUN_HEIGHT       _T("D00300")    /*��	���߶�����*/
#define CMD_COR_TIMER_DELAY  _T("D00013")    /*��������ײ��ʱ����λms*/
//#define CMD_SMOG_REAL        _T("R03840")    /*��ȡPLC����������ʵʱֵ*/
#define CMD_SMOG_REAL        _T("D00080")    /*��ȡPLC����������ʵʱֵ*/

//����PLC��ַ����
#define CMD_UP_LIMITE                _T("X0001")    /*���ſ���*/
#define CMD_DOOR_SWITECH             _T("X0001")   /*�ſ���*/
#define CMD_HUN_RELEASE_VALUE        _T("Y0006")    /*�ش��ͷŵ�ŷ�*/
#define CMD_HUN_COLLISION_VALUE      _T("Y0007")    /*��������ײ�����*/
#define CMD_IrAD_SWITCH              _T("X0000")     /*���俪��*/

#define CMD_FINISH_STATUS     _T("M0030")  /*�Ŵ���PLC�������״̬��1--��ʾ��ɣ�0��ʾδ���*/
#define CMD_FINISH_STATUS_INT _T("D00320")  /*�Ŵ���PLC�������״̬��1--��ʾ��ɣ�0��ʾδ���*/

#define STATUS_WAITE_TIME     120      /*�ȴ��ش��ͷ���ɣ�����0.5sΪʵ��ʱ��*/

#define WM_HUNFINISH WM_USER+10

#define CMD_SEND_REPART_COUNT  10   /*�ظ����͵�������������˵��д��ʧ��*/
//#define DAOFFSETVALUE          0.08 /**/

#define commport    1        //ͨ�Ŷ˿�

//��������
#define  BLAST_RATE_MODE  0  //��ը���ʷ�����ģʽ
#define  CRT_HEIGHT_MODE  1  //���Ը߶ȷ�����ģʽ
#define  FRI_RATE_MODE  2  //���Ը߶ȷ�����ģʽ
#define  INVALID_TEST_MODE  127  //��Ч����������

//�������״̬
#define CH_STEEP_BEGIN   0   //��һ��Ԥ������
#define CH_STEEP_4D      1   //4D��������
#define CH_STEEP_2D      2   //2D��������
#define CH_STEEP_D       3   //D��������
#define CH_STEEP_FINISH  4   //���Ԥ������

#define  COMM_NORMAL_READ  1   /*�����������ݵĶ�ȡ*/
#define  COMM_WRITE_HDY_DATA   2 /*д��Һѹϵͳ����*/

union FIType{
	unsigned int Dint;
	float       FData;
};

struct StrPlcData
{
	float SoundLevel;//��������
	float PlateHight;//ƽ̨�߶�
	float SmogReal;//����������ʵʱֵ
	UINT  ComFunIndex;//ָ�����ڵĹ��ܣ�1--��ȡʵʱ���ݣ�2--
	UINT  COmErrCount;//ͨ�Ŵ��������
	bool  CallPressFun;//����Һѹ��ѹ����

	CSLComm m_SoundComm;//������ͨ�ų���
	CHdyPress m_HdyPress;//Һѹ��ѹ����
};
struct HunThreedData
{
	HWND hwnd;
};

class CBlastTestDlg : public CDialog
{
// Construction
public:
	char SetTestMode();  //����ѡ��������Ӧ�Ĳ���ģʽ ������߷���Ħ���жȷ�
	CBlastTestDlg(CWnd* pParent = NULL);	// standard constructor
	void RefreshDGrid(CAdodc & Ado,CDataGrid & DGrid,CString Sql);
	void EditWriteAble(bool sw);//�༭���Ƿ��д��0--����д�룻1--��д��
	void WriteLog(CString SData);
	void SetHunBtnBit(char sw);//sw:0--gray,1--blue
	void SetParperBtnBit(char sw);//װҩ����ɰ�ť  0--��ʾװҩ��1��ʾ��ɣ�ͬʱ��ı�Ŵ���ť
	void SetTestSpInBtnBit(char sw);
	void SetBlastIndexBit(char sw);  //sw:0--gray,1--��ը
	void WriteRecord();//һ�α��Ʋ�����ɺ�д��ʵ���¼
	void CalTestResult();//����ʵ��������ը�ʣ����жȵȡ�
	char WriteByte(unsigned char Slave,CString Addr, char Dbyte);//�ӻ���ַ��  д���ַ��     д������    ���ɹ�����TRUE
	char WriteInt(unsigned char Slave,CString Addr, int DInt);//�ɹ�����TRUE
	char WriteFloat(unsigned char Slave, CString Addr, float DInt);//�ɹ�����TRUE
	char WriteHunZero(float HunKg,float ZeroNo);//д�뵱ǰ�ش������
	float ReadHunZero(float HunKg);//��ȡ��ǰ�ش������
	float CRTCalNextH(int BlastStatus);//������һ�����Ը߶�

	CFont m_editFont;  

// Dialog Data
	//{{AFX_DATA(CBlastTestDlg)
	enum { IDD = IDD_BLASTTEST_DIALOG };
	CBitmapButton	m_QuitWindow;
	CBitmapButton	m_BtnReport;   //�İ�ť�Ѿ�ʧЧ
	CBitmapButton	m_BtnPrepar;  //װҩ
	CBitmapButton	m_BtnRealse; //�ش��ͷ�
	CBitmapButton	m_BlastIndex; //��ըָʾ��
	CBitmapButton   m_TestSpData;//��Ʒ����
	CBitmapButton   m_TestCondSetup;//ʵ����������
	CBitmapButton   m_RecordView;//ʵ���¼
	CBitmapButton   m_UserSetup; //�û�����
	CBitmapButton   m_TestSpIn; //������Ʒ����
	CBitmapButton   m_PlateUp; //ƽ̨����
	CBitmapButton   m_PlateDown; //ƽ̨�½�
	CBitmapButton   m_PlateUpStop; //ƽ̨����ֹͣ--����Ч
	CBitmapButton   m_PlateDownStop; //--����Ч
	CBitmapButton   m_PlateModeBtn; //ƽ̨�ֶ��Զ�����ģʽѡ��
	CDataGrid	m_DataGrid1;
	CAdodc	m_Ado1;
	CDataGrid	m_DG_OpRecord;
	CAdodc	m_Ado_OPRecord;
	CString m_TestSpNo; //�������
	CString m_TestSpName;//ҩƷ����
	short     m_TestGroupNo;//ʵ������
	short     m_TestNo;     //�������
	CString m_TestType;   //��������
	short     m_BlastRate;   //ʵʱ������
	float     m_TestSpKg;    //��������
	float     m_SondLevel;    //��������
	float     m_PlateHight;   //ƽ̨�߶�,ʵʱ�߶�
	float     m_HunchHight;   //�ش��߶�
	float     m_HunchKg;      //�ش�����
	CString m_TestPerName;  //����Ա
	float m_TestTemp;    //�����¶�
	float m_TestHumi;    //����ʪ��
	bool m_PlaceExpPrepar; //װҩ��ťָʾ 1��ʾ����װҩ��0��ʾ���
	CString	m_TestSel;     //��������ѡ����Ͽ�
	CString	m_TimeDis;
	float   m_HunHightRata; //д���ش��ĸ߶�ϵ����
	float	m_SmogReal;
	float	m_SmogSetValue;
	//}}AFX_DATA
	char m_PlateMode;   //ƽ̨ģʽ��0Ϊ�Զ���1Ϊ�ֶ�ƽ̨��2Ϊ�ֶ���ѹ
	bool m_PlateUpState;//ƽ̨����״̬��0Ϊֹͣ��1Ϊ����
	bool m_PlateDownState;//ƽ̨�½�״̬��0Ϊֹͣ��1Ϊ�½�
	bool m_HunReleaseBtn;//�Ŵ���ť��0Ϊû�����ڷŴ���1Ϊ���ڷŴ�
	bool m_TestSpInBtn;  //���밴ťָʾ��0-�����������룻1--��������ȷ�ϲ���
	short   m_MaxGroupNo;//����ʵ�������
	short   m_MaxTestNo;     //��������ÿ�����
	float m_SLThreshold; //�����ƶ�ȡ�ķ�ֵ
	float m_ZeroLevelHight;//�ش�����λ��Ӧ�ı�����ֵ
	float m_CorTimerDelay;//��������ײ��ʱ
	bool m_SmogThrMode; //����ֵ����Ч��ʽ��1--�Զ��ɼ���0--�ֶ�����
	float m_Smog_bigvalue;//����ֵ������ֵ
	int m_LastBlastStatus;//��һ�εı�ը״̬  1--��ը��0--����ը
	int m_CurrBlastStatus;//��һ�εı�ը״̬  1--��ը��0--����ը
	int m_CRTTestStep;//������߷�������״̬
	int m_CRTTestLastStep;//������߷�����һ�ε�����״̬
	int m_Testmode;//���鷽ʽ��0--�������飬1--������߷�
	float m_StepLength;//���鲽��

	CCRTHeight m_CRTHeight;//������߷�����
	//CHdyPress m_HdyPress;//Һѹ��ѹ����
	//CWfComm m_SondComm;//������ͨ�Ŵ���

//	float m_GroupBlastRate[20];//����ÿ��ʵ���ʵʱ��ը��

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
	// �͸�ǰ����ť
	CBitmapButton m_HdyForwardBtn;
	// �͸׺��˰�ť
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
