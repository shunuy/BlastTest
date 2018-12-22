// HdyPress.h: interface for the CHdyPress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HDYPRESS_H__655D87E3_D7C8_430C_B06D_91F04B286ED0__INCLUDED_)
#define AFX_HDYPRESS_H__655D87E3_D7C8_430C_B06D_91F04B286ED0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//typedef void (*PVFN)();  /*����PCFNΪָ������ָ��*/
//PVFN fun;
//fun=fun1;
//(*fun)();
            //���ٿ��������ڿ��ټ�ѹ��΢����ѹ����ѹ��ɣ�����Ҫ��ѹ����
enum FSTEEP{FASTCLOSE,FASTPRESS,SMALLPRESS,FINISHPRESS,PRESSFAILD,NOPRESS};
enum BSTEEP{FASTBACK,FINISH,NOBACK};

#define FASTForwardDA   100   /*���͸׿��ٿ���ʱ��DAֵ*/
#define FASTBACKDA      100   /*���͸׿��ٷ���ʱ��DAֵ*/
#define DAMAXBALUE      4096*4   /*DA�����ֵ*/

#define PRESS_OFFSET_VALUE  0 /*Һѹϵͳֹͣ����ǰ��ѹ���Ĳ���ֵ*/
#define ACCEPT_ERR_PRESS    10  /*�ɽ��ܵ�ѹ����ΧMpa*/
#define DA_MIN_STEEP_VALUE  1  /*DA���ڵ���С��λ*/

#define SEND_BUFF_COUNT    20   //���ͻ�������С
//���ʹ���m_Cmd
#define CMD_Moror   0    /*�������*/
#define CMD_FwVal   1  /*ǰ����ŷ�*/
#define CMD_BackVal 2  /*���˵�ŷ�*/
#define CMD_AccVal  3 /*��������ŷ�*/
#define CMD_DAVALUE  4/*��������ŷ�*/

//
#define CMD_ADDR_MOTOR  _T("M0111")    /*��ѹ���*/
#define CMD_ADDR_FWVAL  _T("M0108")    /*��ѹ���*/
#define CMD_ADDR_BACKVAL  _T("M0109")    /*��ѹ���*/
#define CMD_ADDR_ACCVAL  _T("M0110")    /*��ѹ���*/
#define CMD_ADDR_DAVALUE  _T("R03904")    /*DA��ֵ*/

#define CMD_ADDR_PRESS  _T("D00312")    /*ѹ��ʵ��ֵ*/
#define CMD_ADDR_PRESSSET  _T("D00322")    /*ѹ���趨ֵ*/

#define MAX_PRESSDAVALUE   2048   /*ѹ��ADת�������ֵ*/

class HdyData{
public:
	HdyData();
	~HdyData();
	void AddSendData(char Cmd,int Data);
	void AddSendData(char Cmd, bool Data);
	void AddSendPtr();//������ָ���1
	bool IsBuffEmpty();//���ͻ�����Ϊ��
protected:
	char m_BuffPtr;
public:
	char m_SendPtr;
	char m_Cmd[SEND_BUFF_COUNT];
	int m_Data[SEND_BUFF_COUNT];
};

class CHdyPress  
{
public:
	void SaveDAValue(short int value);
	bool ManualBack(bool IsBack);
	bool ManualForward(bool IsForward);
	void MaintainPress();//������ѹ����
	void StopWork();
	short GetSetDAValue(float PressValue);
	bool SetDAValue(short int value);
	bool DelSetDAValue();//ɾ����Ӧѹ���洢��DAֵ
	void InitPressFun(float PressValue);
	int PressFun();
	void OpenAccVal(bool SW);//������������ŷ�
	void SetDrawBack();
	void SetForward();
	void ManualForward();//�ֶ��͸�ǰ��
	void ManualBack(); //�ֶ��͸׺���

	CHdyPress();
	virtual ~CHdyPress();
public:
	bool m_ManualF;//�ֶ�ǰ��,0--�˳��ֶ�ǰ��ģʽ��1-�����ֶ�ǰ��ģʽ
	bool m_ManualB;//�ֶ��͸׺��ˣ�
	HdyData m_HdyData;//���ݷ���
	float m_PressValue;//ѹ��ֵ
	float m_PressSetValue;//ѹ���趨ֵ
	float m_PressSetOffset;//ѹ���趨����ֵ
protected:
	bool m_M;    //�������
	bool m_FwVal;  //ǰ����ŷ�
	bool m_BackVal; //���˵�ŷ�
	bool m_AccVal;//��������ŷ�
	bool m_BeginPressing;//Ϊ1˵���͸��Ѿ���λ�����Կ�ʼ��ѹ
	short int m_ValDAValue;//������DAֵ
	short int m_SetValDAValue;//������DA��Ŀ��ֵ

	enum FSTEEP m_FSteep;//��ǰ��ѹ��״̬����
	enum BSTEEP m_BSteep;//��ǰ���˵�״̬��  

};

#endif // !defined(AFX_HDYPRESS_H__655D87E3_D7C8_430C_B06D_91F04B286ED0__INCLUDED_)
