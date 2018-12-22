// DelayFun.cpp: implementation of the CDelayFun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DelayFun.h"
#include "PORT.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDelayFun::CDelayFun()
{

}

CDelayFun::~CDelayFun()
{

}
/*int ReadBuff()
{
	return GetInBuffCount();
}*/
bool CDelayFun::IsRecTimeOut_ms(unsigned int DelayTime_ms,int MaxFunRet)//���뼶��ʱ��������λΪms����ʱͬʱ���Բ�����������Ϣ
{
	DWORD dwStart = GetTickCount();        
	DWORD dwEnd   = dwStart;        
	do{ 
		dwEnd = GetTickCount()-dwStart; 
		if (GetInBuffCount() >= MaxFunRet)
			return false;
	}while(dwEnd <DelayTime_ms);
	return true;//��ʱ
}

bool CDelayFun::IsRecTimeOut_s(unsigned int DelayTime_s, int MaxFunRet)//�뼶��ʱ��������λΪ�룬��ʱͬʱ���Դ���������Ϣ
{
	DWORD dwStart = GetTickCount();        
	DWORD dwEnd   = dwStart;        
	do {
		MSG   msg; 
		GetMessage(&msg,NULL,0,0);               
		TranslateMessage(&msg);                
		DispatchMessage(&msg);                
		dwEnd = GetTickCount()-dwStart;
		if (GetInBuffCount() >= MaxFunRet)
			return false;
	}while(dwEnd <DelayTime_s*60); 
	return true;//��ʱ
}

