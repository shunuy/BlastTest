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
	bool m_IsPeakMode;//当前是峰值模式
	float m_SoundValue;//声级计的当前值		
public:
	CSLComm();
	virtual ~CSLComm();
	float CmdPeakMode();
	float CmdRealMode();
	float ReadSound();
};

#endif // !defined(AFX_SLCOMM_H__D88E07A3_A09C_4EF6_88FC_043B48E4F96C__INCLUDED_)
