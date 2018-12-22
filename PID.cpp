// PID.cpp: implementation of the CPID class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlastTest.h"
#include "PID.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPID::CPID()
{
	SetPoint=0.0;
	Out=0.0;
	err=0.0;
	err_last=0.0;
	err_next=0.0;
	Kp=0.2;
	Ts = 0.01;
	Ti = 0.133;
	Td = 0.01;
}

CPID::~CPID()
{

}

void CPID::PIDInit(float PSetValue,float Pout)
{
	SetPoint=PSetValue;
	Out=Pout;
	err=0.0;
	err_last=0.0;
	err_next=0.0;	
	Ki = Kp*Ts / Ti;
	Kd = Kp*Td / Ts;
}

float CPID::PIDOut(float speed)
{
	SetPoint=speed;
	err = SetPoint - FeedBack;
	float incrementSpeed=Kp*(err-err_next)+Ki*err+Kd*(err-2*err_next+err_last);
	Out+=incrementSpeed;
	err_last=err_next;
	err_next=err;
	if (Out > UpLimite)
		Out = UpLimite;
	else if (Out < LowLimite)
		Out = LowLimite;

	return Out;
}

void CPID::SetKp(float NewKp)
{
	Kp=NewKp;	
	Ki = Kp*Ts / Ti;
	Kd = Kp*Td / Ts;
}

void CPID::SetTs(float NewTs)
{
	Ts=NewTs;
	Ki = Kp*Ts / Ti;
	Kd = Kp*Td / Ts;
}

void CPID::SetTi(float NewTi)
{
	Ti=NewTi;
	Ki = Kp*Ts / Ti;
	Kd = Kp*Td / Ts;
}

void CPID::SetKd(float NewKd)
{
	Kd=NewKd;
	Ki = Kp*Ts / Ti;
	Kd = Kp*Td / Ts;
}
