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
	bool IsRecTimeOut_ms(unsigned int DelayTime_ms, int MaxFunRet);//���뼶��ʱ��������λΪms����ʱͬʱ���Բ�����������Ϣ,����1��ʾ��ʱ
	bool IsRecTimeOut_s(unsigned int DelayTime_s, int MaxFunRet);//�뼶��ʱ��������λΪ�룬��ʱͬʱ���Դ���������Ϣ
};

#endif // !defined(AFX_DELAYFUN_H__3C2BA0CC_B35F_47F7_A35A_D10BBCC4DDA3__INCLUDED_)
