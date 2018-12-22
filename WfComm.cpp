// WfComm.cpp: implementation of the CWfComm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlastTest.h"
#include "WfComm.h"
#include "PORT.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWfComm::CWfComm()
{
	hCom= INVALID_HANDLE_VALUE;
	PortNum=1;
	ErrCount=0;
}
CWfComm::CWfComm(unsigned long xPortNum,unsigned long xBabd, unsigned char xDataSize, 
				unsigned char xParity, unsigned char xStopBit)
{
	hCom= INVALID_HANDLE_VALUE;
	PortNum=xPortNum;
	ErrCount=0;
	OpenPort(xPortNum,xBabd,xDataSize,xParity,xStopBit,4096,4096,1000);
}

CWfComm::~CWfComm()
{

}
bool CWfComm::IsOpen()
{
	return hCom != INVALID_HANDLE_VALUE;
} 
bool CWfComm::Delay_ms(unsigned long Ms, unsigned int ReceiveCount)
{
	DWORD dwStart = GetTickCount();
	DWORD dwEnd = dwStart;
	do{
		dwEnd = GetTickCount() - dwStart;
		if (GetInBuffCount() >= ReceiveCount)
			return false;
	} while (dwEnd <Ms);
	return true;//超时
}

//设置超时
void CWfComm::SetTimerOut(unsigned long dwTimerOut)
{
	if(!IsOpen())  
	{
		return;   
	}
	
	CommTimerOuts.ReadIntervalTimeout= MAXDWORD;
	CommTimerOuts.ReadTotalTimeoutConstant= 0;
	CommTimerOuts.ReadTotalTimeoutMultiplier= 0;
	CommTimerOuts.WriteTotalTimeoutConstant= dwTimerOut;
	CommTimerOuts.WriteTotalTimeoutMultiplier= 0;

	SetCommTimeouts(hCom, &CommTimerOuts);
}

//设置DCB参数
bool CWfComm::SetDCBParm(unsigned long xBabd, unsigned char xDataSize, 
				unsigned char xParity, unsigned char xStopBit)
{
	if(!IsOpen())  
	{
		return false;   
	}
	
	if (!GetCommState(hCom, &dcb)) 
	{
		#ifdef _DEBUG
			printf ("错误: %d << 等到通讯口参数失败.\n", GetLastError());
		#endif
		
		ClosePort();
		return (false);
	}   
	//设置通讯参数   	
	dcb.DCBlength= sizeof(DCB);	
	dcb.BaudRate = xBabd;     // set the baud rate
	dcb.ByteSize = xDataSize; // data size, xmit, and rcv
	dcb.Parity =   xParity;   // no parity bit
	dcb.StopBits = xStopBit;  // one stop bit	
	if (!SetCommState(hCom, &dcb)) 
	{
		#ifdef _DEBUG
			printf ("错误: %d << 设置通讯端口参数失败.\n", GetLastError());
		#endif
		ClosePort();
		return (false);
	}   

	return true;
}

//设置端口缓冲区大小
bool CWfComm::SetPortBuffSize(unsigned long InputBuffSize, unsigned long OutputBuffSize)
{
	if(!IsOpen())  
	{
		return false;   
	} 
	
	if(!SetupComm(hCom, InputBuffSize, OutputBuffSize))
	{
		#ifdef _DEBUG
			printf ("错误: %d << 设置通讯端口缓冲失败.\n", GetLastError());
		#endif
		ClosePort();
		return (false);
	}   

	return true;
}

//清理所有缓冲区
void CWfComm::ClearBuffer()
{
	if(!IsOpen())  
	{
		return;   
	}

	PurgeComm(hCom, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
}

//当前接收缓冲区字节数目; 
unsigned long CWfComm::GetInBuffCount()
{
	if(!IsOpen())  
	{
	return(0);   
	}

	DWORD     dwErrorFalgs;
	COMSTAT   Comstat;

	ClearCommError(hCom, &dwErrorFalgs, &Comstat);
	return Comstat.cbInQue;
}

//当前发送缓冲区字节数目; 
unsigned long CWfComm::GetOutBuffCount()
{
	if(!IsOpen())  
	{
		return false;   
	}

	DWORD     dwErrorFalgs;
	COMSTAT   Comstat;

	ClearCommError(hCom, &dwErrorFalgs, &Comstat);
	return Comstat.cbOutQue;
}
/*
#define NOPARITY            0
#define ODDPARITY           1
#define EVENPARITY          2
#define MARKPARITY          3
#define SPACEPARITY         4

#define ONESTOPBIT          0
#define ONE5STOPBITS        1
#define TWOSTOPBITS         2

#define IGNORE              0       // Ignore signal
#define INFINITE            0xFFFFFFFF  // Infinite timeout

//
// Baud rates at which the communication device operates
//

#define CBR_110             110
#define CBR_300             300
#define CBR_600             600
#define CBR_1200            1200
#define CBR_2400            2400
#define CBR_4800            4800
#define CBR_9600            9600
#define CBR_14400           14400
#define CBR_19200           19200
#define CBR_38400           38400
#define CBR_56000           56000
#define CBR_57600           57600
#define CBR_115200          115200
#define CBR_128000          128000
#define CBR_256000          256000
*/

///打开串口//////////////////////////////////////////////////////////////////////////////////////////////////////////// 
bool CWfComm::OpenPort(unsigned long xPort, unsigned long xBabd, unsigned char xDataSize, 
				unsigned char xParity, unsigned char xStopBit, unsigned long InputBuffSize,
				unsigned long OutputBuffSize, unsigned long dwTimerOut)
{   
	if(IsOpen())
	{
		ClosePort();
	}	
	//设置事件 
	memset(&m_overlappedRead,0,sizeof(OVERLAPPED));
	m_overlappedRead.hEvent= CreateEvent(NULL,FALSE,TRUE,"portread");
	ASSERT(m_overlappedRead.hEvent != INVALID_HANDLE_VALUE);

	memset(&m_overlappedWrite,0,sizeof(OVERLAPPED));   
	m_overlappedWrite.hEvent= CreateEvent(NULL,FALSE,TRUE,"portwrite");  
	ASSERT(m_overlappedWrite.hEvent != INVALID_HANDLE_VALUE);
	//取得串口字符
	char com_str[255];
	//strcpy(com_str, "COM");
	strcpy(com_str, "\\\\.\\COM");//在上一条语句的基础上添加了////.//字符串，本语句是为了能打开串口10以上的串口而添加  wf
	ltoa(xPort, com_str + 7, 10);//本语句在修改上一天语句前是加3，现在改成了加7  wf
	//打开通讯端口 
	hCom = CreateFile(com_str,
					GENERIC_READ | GENERIC_WRITE,
					0,    
					NULL, 
					OPEN_EXISTING, 
					FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,    
					NULL  
					);
	if (hCom == INVALID_HANDLE_VALUE) 
	{
		#ifdef _DEBUG
			//printf ("错误: %d << 打开通讯口失败,请检查是否已经安装串口设备.\n", GetLastError());
		#endif
		CString str;
		str.Format("错误: %d << 打开通讯口失败,请检查是否已经安装串口设备.\n", GetLastError());
		AfxMessageBox(str);
		return (false);
	}     
	SetPortBuffSize(InputBuffSize,OutputBuffSize);
	SetDCBParm(xBabd,xDataSize,xParity,xStopBit);	
	SetTimerOut(dwTimerOut);	
	//清理缓冲器
	PurgeComm(hCom, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	//启动成功
	#ifdef _DEBUG
		printf ("成功开启端口 %d.\n", com_str);
	#endif
	return(true);
}

//关闭串口;
void CWfComm::ClosePort()
{
	if(IsOpen())
	{
		CloseHandle(hCom);		
		hCom= INVALID_HANDLE_VALUE;
	}    
	
	//ResetEvent(m_overlappedRead.hEvent);
	//ResetEvent(m_overlappedWrite.hEvent);	

    if(m_overlappedRead.hEvent!=NULL) 
    {
        CloseHandle(m_overlappedRead.hEvent);        
    }
    if(m_overlappedWrite.hEvent!=NULL) 
    {
        CloseHandle(m_overlappedWrite.hEvent);        
    }	
}

//异步读数据
unsigned long CWfComm::ReadChar(unsigned long dwBufferLength, char *buff, unsigned long dwWaitTime)
{   
	if(!IsOpen())  
	{
		return(0);   
	}
	
	DWORD		dwError;
	COMSTAT		Stat;  
	if(::ClearCommError(hCom, &dwError, &Stat) && dwError > 0)	//清除错误
	{
		::PurgeComm(hCom, PURGE_RXABORT | PURGE_RXCLEAR); /*清除输入缓冲区*/
		return 0;
	}
	if(!Stat.cbInQue)// 缓冲区无数据
	{
		return 0;
	}

	unsigned long uReadLength = 0;
	dwBufferLength = dwBufferLength - 1 > Stat.cbInQue ? Stat.cbInQue : dwBufferLength - 1;
	if(!::ReadFile(hCom, buff, dwBufferLength, &uReadLength, &m_overlappedRead)) //2000 下 ReadFile 始终返回 True
	{
		if(::GetLastError() == ERROR_IO_PENDING) // 结束异步I/O
		{
			WaitForSingleObject(m_overlappedRead.hEvent, dwWaitTime);	//等待20ms
			if(!::GetOverlappedResult(hCom, &m_overlappedRead, &uReadLength, false))
			{
				if(::GetLastError() != ERROR_IO_INCOMPLETE)//其他错误
				{
					uReadLength = 0;
				}
			}
		}
		else
		{
			uReadLength = 0;
		}
	}
		
	return uReadLength;
}
//清楚接收缓冲区
void CWfComm::Clear_Rec_buffer()
{
	//char ClrBuf[254];
	//if(GetInBuffCount())//获取当前缓冲区中的字节数
	//	while(ReadChar(254, ClrBuf, 10));
	//When a synchronous read operation reaches the end of a file, ReadFile returns TRUE and 
	//sets *lpNumberOfBytesRead to zero. 
	//The following sample code tests for end-of-file for a synchronous read operation
	CString str;
	int len=GetInBuffCount();
	ReadChar(GetInBuffCount()+1, str.GetBuffer(GetInBuffCount()+1), 10);
	str.ReleaseBuffer();
	len=GetInBuffCount();
}

//异步写数据 
unsigned long CWfComm::WriteChar(unsigned long dwBufferLength, char *buff)
{
	if(!IsOpen())
	{
		return 0;
	}

	DWORD dwError;
	if(ClearCommError(hCom, &dwError, NULL) && dwError > 0)	//清除错误
	{
		PurgeComm(hCom, PURGE_TXABORT | PURGE_TXCLEAR);	
	}

	unsigned long uWriteLength = 0;
	if(!WriteFile(hCom, buff, dwBufferLength, &uWriteLength, &m_overlappedWrite))
	{
		if(GetLastError() == ERROR_IO_PENDING)
		{
			DWORD m_tmp=0;
			m_tmp= WaitForSingleObject(m_overlappedWrite.hEvent, 1000);
			if(m_tmp== WAIT_TIMEOUT || m_tmp== WAIT_ABANDONED)
			{
				return(0);
			}
			else if(m_tmp== WAIT_OBJECT_0)
			{			
				if(!GetOverlappedResult(hCom,&m_overlappedWrite,&uWriteLength,false))
				{
					return(0);		
				}
				else
				{
					return uWriteLength;		
				}
			}
			
			uWriteLength = 0;
		}
	}

	return uWriteLength;  
}
