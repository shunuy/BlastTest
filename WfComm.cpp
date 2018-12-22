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
	return true;//��ʱ
}

//���ó�ʱ
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

//����DCB����
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
			printf ("����: %d << �ȵ�ͨѶ�ڲ���ʧ��.\n", GetLastError());
		#endif
		
		ClosePort();
		return (false);
	}   
	//����ͨѶ����   	
	dcb.DCBlength= sizeof(DCB);	
	dcb.BaudRate = xBabd;     // set the baud rate
	dcb.ByteSize = xDataSize; // data size, xmit, and rcv
	dcb.Parity =   xParity;   // no parity bit
	dcb.StopBits = xStopBit;  // one stop bit	
	if (!SetCommState(hCom, &dcb)) 
	{
		#ifdef _DEBUG
			printf ("����: %d << ����ͨѶ�˿ڲ���ʧ��.\n", GetLastError());
		#endif
		ClosePort();
		return (false);
	}   

	return true;
}

//���ö˿ڻ�������С
bool CWfComm::SetPortBuffSize(unsigned long InputBuffSize, unsigned long OutputBuffSize)
{
	if(!IsOpen())  
	{
		return false;   
	} 
	
	if(!SetupComm(hCom, InputBuffSize, OutputBuffSize))
	{
		#ifdef _DEBUG
			printf ("����: %d << ����ͨѶ�˿ڻ���ʧ��.\n", GetLastError());
		#endif
		ClosePort();
		return (false);
	}   

	return true;
}

//�������л�����
void CWfComm::ClearBuffer()
{
	if(!IsOpen())  
	{
		return;   
	}

	PurgeComm(hCom, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
}

//��ǰ���ջ������ֽ���Ŀ; 
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

//��ǰ���ͻ������ֽ���Ŀ; 
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

///�򿪴���//////////////////////////////////////////////////////////////////////////////////////////////////////////// 
bool CWfComm::OpenPort(unsigned long xPort, unsigned long xBabd, unsigned char xDataSize, 
				unsigned char xParity, unsigned char xStopBit, unsigned long InputBuffSize,
				unsigned long OutputBuffSize, unsigned long dwTimerOut)
{   
	if(IsOpen())
	{
		ClosePort();
	}	
	//�����¼� 
	memset(&m_overlappedRead,0,sizeof(OVERLAPPED));
	m_overlappedRead.hEvent= CreateEvent(NULL,FALSE,TRUE,"portread");
	ASSERT(m_overlappedRead.hEvent != INVALID_HANDLE_VALUE);

	memset(&m_overlappedWrite,0,sizeof(OVERLAPPED));   
	m_overlappedWrite.hEvent= CreateEvent(NULL,FALSE,TRUE,"portwrite");  
	ASSERT(m_overlappedWrite.hEvent != INVALID_HANDLE_VALUE);
	//ȡ�ô����ַ�
	char com_str[255];
	//strcpy(com_str, "COM");
	strcpy(com_str, "\\\\.\\COM");//����һ�����Ļ����������////.//�ַ������������Ϊ���ܴ򿪴���10���ϵĴ��ڶ����  wf
	ltoa(xPort, com_str + 7, 10);//��������޸���һ�����ǰ�Ǽ�3�����ڸĳ��˼�7  wf
	//��ͨѶ�˿� 
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
			//printf ("����: %d << ��ͨѶ��ʧ��,�����Ƿ��Ѿ���װ�����豸.\n", GetLastError());
		#endif
		CString str;
		str.Format("����: %d << ��ͨѶ��ʧ��,�����Ƿ��Ѿ���װ�����豸.\n", GetLastError());
		AfxMessageBox(str);
		return (false);
	}     
	SetPortBuffSize(InputBuffSize,OutputBuffSize);
	SetDCBParm(xBabd,xDataSize,xParity,xStopBit);	
	SetTimerOut(dwTimerOut);	
	//��������
	PurgeComm(hCom, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	//�����ɹ�
	#ifdef _DEBUG
		printf ("�ɹ������˿� %d.\n", com_str);
	#endif
	return(true);
}

//�رմ���;
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

//�첽������
unsigned long CWfComm::ReadChar(unsigned long dwBufferLength, char *buff, unsigned long dwWaitTime)
{   
	if(!IsOpen())  
	{
		return(0);   
	}
	
	DWORD		dwError;
	COMSTAT		Stat;  
	if(::ClearCommError(hCom, &dwError, &Stat) && dwError > 0)	//�������
	{
		::PurgeComm(hCom, PURGE_RXABORT | PURGE_RXCLEAR); /*������뻺����*/
		return 0;
	}
	if(!Stat.cbInQue)// ������������
	{
		return 0;
	}

	unsigned long uReadLength = 0;
	dwBufferLength = dwBufferLength - 1 > Stat.cbInQue ? Stat.cbInQue : dwBufferLength - 1;
	if(!::ReadFile(hCom, buff, dwBufferLength, &uReadLength, &m_overlappedRead)) //2000 �� ReadFile ʼ�շ��� True
	{
		if(::GetLastError() == ERROR_IO_PENDING) // �����첽I/O
		{
			WaitForSingleObject(m_overlappedRead.hEvent, dwWaitTime);	//�ȴ�20ms
			if(!::GetOverlappedResult(hCom, &m_overlappedRead, &uReadLength, false))
			{
				if(::GetLastError() != ERROR_IO_INCOMPLETE)//��������
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
//������ջ�����
void CWfComm::Clear_Rec_buffer()
{
	//char ClrBuf[254];
	//if(GetInBuffCount())//��ȡ��ǰ�������е��ֽ���
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

//�첽д���� 
unsigned long CWfComm::WriteChar(unsigned long dwBufferLength, char *buff)
{
	if(!IsOpen())
	{
		return 0;
	}

	DWORD dwError;
	if(ClearCommError(hCom, &dwError, NULL) && dwError > 0)	//�������
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
