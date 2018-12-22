// WfComm.h: interface for the CWfComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WFCOMM_H__CA7D89C2_5F76_45B3_8510_6EC25322E79B__INCLUDED_)
#define AFX_WFCOMM_H__CA7D89C2_5F76_45B3_8510_6EC25322E79B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "PORT.h"

class CWfComm  
{
public:
	CWfComm();
	CWfComm::CWfComm(unsigned long xPortNum,unsigned long xBabd=CBR_9600, unsigned char xDataSize=8, 
				unsigned char xParity=NOPARITY, unsigned char xStopBit=ONESTOPBIT);
	virtual ~CWfComm();

public:
	DCB dcb;
	COMMTIMEOUTS CommTimerOuts;
	HANDLE hCom;
	unsigned long PortNum;//串口编号
	unsigned long ErrCount;//通信错误的次数
	
	OVERLAPPED m_overlappedRead;
	OVERLAPPED m_overlappedWrite;
	
public:
	unsigned long WriteChar(unsigned long dwBufferLength, char *buff);
	void Clear_Rec_buffer();
	unsigned long ReadChar(unsigned long dwBufferLength, char *buff, unsigned long dwWaitTime=20);
	void ClosePort();
	bool OpenPort(unsigned long xPort, unsigned long xBabd, unsigned char xDataSize, 
		unsigned char xParity, unsigned char xStopBit, unsigned long InputBuffSize,
				unsigned long OutputBuffSize, unsigned long dwTimerOut);
	unsigned long GetOutBuffCount();
	unsigned long GetInBuffCount();
	void ClearBuffer();
	bool SetPortBuffSize(unsigned long InputBuffSize, unsigned long OutputBuffSize);
	bool SetDCBParm(unsigned long xBabd, unsigned char xDataSize, 
				unsigned char xParity, unsigned char xStopBit);
	void SetTimerOut(unsigned long dwTimerOut= 5000);
	bool IsOpen();
	bool Delay_ms(unsigned long Ms, unsigned int ReceiveCount);
};

#endif // !defined(AFX_WFCOMM_H__CA7D89C2_5F76_45B3_8510_6EC25322E79B__INCLUDED_)
