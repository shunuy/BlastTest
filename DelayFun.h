// DelayFun.h: interface for the CDelayFun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELAYFUN_H__3C2BA0CC_B35F_47F7_A35A_D10BBCC4DDA3__INCLUDED_)
#define AFX_DELAYFUN_H__3C2BA0CC_B35F_47F7_A35A_D10BBCC4DDA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef    unsigned long(*FUNC1)();   
typedef   int(*FUNC2)(int*,int*,int*); 

class CDelayFun  
{
public:
	CDelayFun();
	virtual ~CDelayFun();
	bool IsRecTimeOut_ms(unsigned int DelayTime_ms, int MaxFunRet);//毫秒级延时，参数单位为ms，延时同时可以不处理其他消息,返回1表示超时
	bool IsRecTimeOut_s(unsigned int DelayTime_s, int MaxFunRet);//秒级延时，参数单位为秒，延时同时可以处理其他消息
};

#endif // !defined(AFX_DELAYFUN_H__3C2BA0CC_B35F_47F7_A35A_D10BBCC4DDA3__INCLUDED_)
