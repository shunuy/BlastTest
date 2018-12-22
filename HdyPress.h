// HdyPress.h: interface for the CHdyPress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HDYPRESS_H__655D87E3_D7C8_430C_B06D_91F04B286ED0__INCLUDED_)
#define AFX_HDYPRESS_H__655D87E3_D7C8_430C_B06D_91F04B286ED0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//typedef void (*PVFN)();  /*定义PCFN为指向函数的指针*/
//PVFN fun;
//fun=fun1;
//(*fun)();
            //快速靠近；正在快速加压；微调加压；加压完成；不需要加压流程
enum FSTEEP{FASTCLOSE,FASTPRESS,SMALLPRESS,FINISHPRESS,PRESSFAILD,NOPRESS};
enum BSTEEP{FASTBACK,FINISH,NOBACK};

#define FASTForwardDA   100   /*让油缸快速靠近时的DA值*/
#define FASTBACKDA      100   /*让油缸快速返回时的DA值*/
#define DAMAXBALUE      4096*4   /*DA的最大值*/

#define PRESS_OFFSET_VALUE  0 /*液压系统停止工作前的压力的补偿值*/
#define ACCEPT_ERR_PRESS    10  /*可接受的压力误差范围Mpa*/
#define DA_MIN_STEEP_VALUE  1  /*DA调节的最小单位*/

#define SEND_BUFF_COUNT    20   //发送缓冲区大小
//发送代码m_Cmd
#define CMD_Moror   0    /*电机开关*/
#define CMD_FwVal   1  /*前进电磁阀*/
#define CMD_BackVal 2  /*后退电磁阀*/
#define CMD_AccVal  3 /*蓄能器电磁阀*/
#define CMD_DAVALUE  4/*蓄能器电磁阀*/

//
#define CMD_ADDR_MOTOR  _T("M0111")    /*加压电机*/
#define CMD_ADDR_FWVAL  _T("M0108")    /*加压电机*/
#define CMD_ADDR_BACKVAL  _T("M0109")    /*加压电机*/
#define CMD_ADDR_ACCVAL  _T("M0110")    /*加压电机*/
#define CMD_ADDR_DAVALUE  _T("R03904")    /*DA的值*/

#define CMD_ADDR_PRESS  _T("D00312")    /*压力实际值*/
#define CMD_ADDR_PRESSSET  _T("D00322")    /*压力设定值*/

#define MAX_PRESSDAVALUE   2048   /*压力AD转换的最大值*/

class HdyData{
public:
	HdyData();
	~HdyData();
	void AddSendData(char Cmd,int Data);
	void AddSendData(char Cmd, bool Data);
	void AddSendPtr();//讲发送指针加1
	bool IsBuffEmpty();//发送缓冲区为空
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
	void MaintainPress();//启动保压程序
	void StopWork();
	short GetSetDAValue(float PressValue);
	bool SetDAValue(short int value);
	bool DelSetDAValue();//删除对应压力存储的DA值
	void InitPressFun(float PressValue);
	int PressFun();
	void OpenAccVal(bool SW);//启动蓄能器电磁阀
	void SetDrawBack();
	void SetForward();
	void ManualForward();//手动油缸前进
	void ManualBack(); //手动油缸后退

	CHdyPress();
	virtual ~CHdyPress();
public:
	bool m_ManualF;//手动前进,0--退出手动前进模式，1-启动手动前进模式
	bool m_ManualB;//手动油缸后退；
	HdyData m_HdyData;//数据发送
	float m_PressValue;//压力值
	float m_PressSetValue;//压力设定值
	float m_PressSetOffset;//压力设定补偿值
protected:
	bool m_M;    //电机开关
	bool m_FwVal;  //前进电磁阀
	bool m_BackVal; //后退电磁阀
	bool m_AccVal;//蓄能器电磁阀
	bool m_BeginPressing;//为1说明油缸已经到位，可以开始加压
	short int m_ValDAValue;//比例阀DA值
	short int m_SetValDAValue;//比例阀DA的目标值

	enum FSTEEP m_FSteep;//当前加压的状态步，
	enum BSTEEP m_BSteep;//当前后退的状态步  

};

#endif // !defined(AFX_HDYPRESS_H__655D87E3_D7C8_430C_B06D_91F04B286ED0__INCLUDED_)
