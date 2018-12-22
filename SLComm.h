// SLComm.h: interface for the CSLComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLCOMM_H__D88E07A3_A09C_4EF6_88FC_043B48E4F96C__INCLUDED_)
#define AFX_SLCOMM_H__D88E07A3_A09C_4EF6_88FC_043B48E4F96C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "WfComm.h"

class CSLComm  :public CWfComm
{
public:
	bool m_IsPeakMode;//��ǰ�Ƿ�ֵģʽ
	float m_SoundValue;//�����Ƶĵ�ǰֵ		
public:
	CSLComm();
	virtual ~CSLComm();
	float CmdPeakMode();
	float CmdRealMode();
	float ReadSound();
};

#endif // !defined(AFX_SLCOMM_H__D88E07A3_A09C_4EF6_88FC_043B48E4F96C__INCLUDED_)
