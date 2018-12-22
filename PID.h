// PID.h: interface for the CPID class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PID_H__E3AC5694_4027_45FB_A1D4_14CB73E27B2A__INCLUDED_)
#define AFX_PID_H__E3AC5694_4027_45FB_A1D4_14CB73E27B2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPID  
{
public:
	CPID();
	virtual ~CPID();

private:
	float SetPoint; //定义设定值
	float FeedBack;//反馈量
	float Out; //定义PID输出
	float err; //定义偏差值
	float err_next; //定义上一个偏差值
	float err_last; //定义最上前的偏差值
	float Kp,Ki,Kd; //定义比例、积分、微分系数
	
	float UpLimite;
	float LowLimite;//pid out limite
	float Ti, Ts,Td;//Ts为控制周期(PID的调用周期)；Ti为积分时间常数；Td为微分系数；Pout=Kp*(Er+1/Ti*积分+Td*微分)；离散化：Pout=Kp*(Er+ Ts/Ti*求和+ Td/Ts*(Er-Er-1))

public:
	void SetKd(float NewKd);
	void SetTi(float NewTi);
	void SetTs(float NewTs);
	void SetKp(float NewKp);
	void  PIDInit(float PSetValue,float Pout);
	float PIDOut(float speed);

};

#endif // !defined(AFX_PID_H__E3AC5694_4027_45FB_A1D4_14CB73E27B2A__INCLUDED_)
