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
bool CDelayFun::IsRecTimeOut_ms(unsigned int DelayTime_ms,int MaxFunRet)//毫秒级延时，参数单位为ms，延时同时可以不处理其他消息
{
	DWORD dwStart = GetTickCount();        
	DWORD dwEnd   = dwStart;        
	do{ 
		dwEnd = GetTickCount()-dwStart; 
		if (GetInBuffCount() >= MaxFunRet)
			return false;
	}while(dwEnd <DelayTime_ms);
	return true;//超时
}

bool CDelayFun::IsRecTimeOut_s(unsigned int DelayTime_s, int MaxFunRet)//秒级延时，参数单位为秒，延时同时可以处理其他消息
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
	return true;//超时
}

