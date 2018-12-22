#if !defined(MPORT)
#define MPORT


#ifdef __cplusplus
extern "C" 
{
#endif

	extern inline bool IsOpen();
	bool OpenPort(unsigned long xPort, unsigned long xBabd, unsigned char xDataSize, 
				unsigned char xParity, unsigned char xStopBit, unsigned long InputBuffSize, 
				unsigned long OutputBuffSize, unsigned long dwTimerOut);
	void ClosePort();
	void ClearBuffer();
	unsigned long ReadChar(unsigned long dwBufferLength, char *buff, unsigned long dwWaitTime);
	unsigned long WriteChar(unsigned long dwBufferLength, char *buff);
	unsigned long GetOutBuffCount();
	void Clear_Rec_buffer();//wf
	unsigned long GetInBuffCount();//获取接受缓冲区的字节数wf
	unsigned long GetRecBCount();//wf 20151209
    
#ifdef __cplusplus
}
#endif

#endif
