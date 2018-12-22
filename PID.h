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
	float SetPoint; //�����趨ֵ
	float FeedBack;//������
	float Out; //����PID���
	float err; //����ƫ��ֵ
	float err_next; //������һ��ƫ��ֵ
	float err_last; //��������ǰ��ƫ��ֵ
	float Kp,Ki,Kd; //������������֡�΢��ϵ��
	
	float UpLimite;
	float LowLimite;//pid out limite
	float Ti, Ts,Td;//TsΪ��������(PID�ĵ�������)��TiΪ����ʱ�䳣����TdΪ΢��ϵ����Pout=Kp*(Er+1/Ti*����+Td*΢��)����ɢ����Pout=Kp*(Er+ Ts/Ti*���+ Td/Ts*(Er-Er-1))

public:
	void SetKd(float NewKd);
	void SetTi(float NewTi);
	void SetTs(float NewTs);
	void SetKp(float NewKp);
	void  PIDInit(float PSetValue,float Pout);
	float PIDOut(float speed);

};

#endif // !defined(AFX_PID_H__E3AC5694_4027_45FB_A1D4_14CB73E27B2A__INCLUDED_)
