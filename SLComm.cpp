// SLComm.cpp: implementation of the CSLComm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlastTest.h"
#include "SLComm.h"
//#include "DelayFun.h"
#include "math.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSLComm::CSLComm()
{
	OpenPort(2,9600,8,NOPARITY,ONESTOPBIT,1024,1024,500);
	m_IsPeakMode = false;
}

CSLComm::~CSLComm()
{
		
}

//bcd�������ε��໥ת��
unsigned long  BCDToDec(unsigned char *bcd, int length)   
{   
    int tmp;   
    unsigned long dec = 0;   
	
    for(int i = 0; i < length; i++)   
    {   
        tmp = ((bcd[i] >> 4) & 0x0F) * 10 + (bcd[i] & 0x0F);      
        dec += tmp * pow(100.0f, length - 1 - i);             
    }   
	
    return dec;   
}   

int DecToBCD(int Dec, unsigned char *Bcd, int length)   
{   
    int tmp;   
	
    for(int i = length - 1; i >= 0; i--)   
    {   
        tmp = Dec % 100;   
        Bcd[i] = ((tmp / 10) << 4) + ((tmp % 10) & 0x0F);   
        Dec /= 100;   
    }   
	
    return 0;   
} 

float CSLComm::CmdPeakMode()
{
	char buff[10];

	buff[0]=0xBA;
	WriteChar(1,buff);//����0xBA����
	//CDelayFun Delay;
	if(Delay_ms(1500,3))
		return -2;//��ʱ
	ReadChar(10,buff);
	if(buff[0]==char(0xBB))//������ͨ��Э�������⣬��ʱ�����ΪOXBB,�����ʱoxBA
	{
		buff[0]=buff[1];
		buff[1]=buff[2];
		buff[2]=0;
		m_IsPeakMode=true;
		return BCDToDec((unsigned char *)buff,2)/10.0f;
	}

	return -1;
}

float CSLComm::CmdRealMode()
{
	char buff[10];

	buff[0]=0xBB;
	WriteChar(1,buff);//����0xBA����
	//CDelayFun Delay;
	//if(Delay.IsRecTimeOut_ms(1500,3))
	//	return -2;//��ʱ
	if (Delay_ms(1500, 3))
		return -2;//��ʱ
	ReadChar(10,buff);
	if (buff[0] == char(0xBB))
	{
		buff[0]=buff[1];
		buff[1]=buff[2];
		buff[2]=0;
		m_IsPeakMode=false;
		return BCDToDec((unsigned char *)buff,2)/10.0f;
	}
	
	return -1;//���ش���
}

float CSLComm::ReadSound()
{
	if (IsOpen())
	{
		if (m_IsPeakMode)
			return CmdPeakMode();
		else
			return CmdRealMode();
	}
	return -4;
}