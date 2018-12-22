#include "stdafx.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <math.h>
#include "PORT.h"

#include "calSum.h"

#ifdef _DEBUG3
#define PLCComErr  1
#else
#define PLCComErr  -2
#endif

#ifdef _DEBUG3
#define PLCComErr2  1
#else
#define PLCComErr2  -1



#endif

unsigned char FATEK_Buff[256];

/*
通讯功能中最主要的功能在于读取或写入PLC内部组件（单点或缓存器）的状态或数值数据。因为首先
必须明白PLC内部到底有哪些组件可供运用，以及如何来指定它们。下表为永宏PLC可供存取的单点与缓存
器及其地址指定方法（关于组件的特性大小范围及意义请参考基础功能篇手册第2.1节的说明）。
----------------------------------------------------------------------------------------------------------
组件|
类型|  代  号    名称          单点地址指定（5个字符）  16位缓存器指定（6个字符）  32位缓存器指定（7个字符）
----|-----------------------------------------------------------------------------------------------------
单  |   X        输入接点      X0000～X9999             WX0000～WX9984                DWX0000～DWX9968 
点  |   Y       输出继电器     Y0000～Y9999             WY0000～WY9984                DWY0000～DWY9968 
状  |   M       内部继电器     M0000～M9999             WM0000～WM9984                DWM0000～DWM9968 
态  |   S       步进继电器     S0000～S9999             WS0000～WS9984                DWS0000～DWS9968 
    |   T       定时器接点     T0000～T9999             WT0000～WT9984                DWT0000～DWT9968 
	|   C       计数器接点     C0000～C9999             WC0000～WC9984                DWC0000～DWC9968
 ---|-----------------------------------------------------------------------------------------------------
暂  |  TMR      定时器缓存器     --                     RT0000～RT9999                 DRT0000～DRT9998 
存	|  CTR      计数器缓存器     ---                    RC0000～RC9999                 DRC0000～DRC9998
器  |  HR       数据缓存器       ---                    R00000～R65535                 DR00000～DR65534
资	|  DR       数据缓存器       ---                    D00000～D65535                 DD00000～DD65534
料  |  FR       档案缓存器       ---                    F00000～F65535                 DF00000～DF65534
 
*/
/*
关键转换：
	数字到16进制字符串 示例：
	str.Format("%2X",rtu);//从机地址转换为16进制字符串
	str.Replace(' ','0');
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	字符串中取指定长度转换成整形
	int atoi( const char *string );
	CString Mid( int nFirst, int nCount ) 
	int a=atoi(str.Mid(2,2))//该方法无法转换16进制表示的字符串
	int a=htoi(str1.Mid(0,4));//调用从网络上找的函数

*/
/*
将16进制表示的字符串转换为整形
*/
#define     YES     1  
#define     NO      0 
int htoi(const char * s)  
{  
    int hexdigit, i, inhex, n;  
	
    i = 0;  
    if(s[i] == '0')         //如果字符串以0x或0X开头，则跳过这两个字符  
    {  
        ++i;  
        if(s[i] == 'x' || s[i] == 'X')    
            ++i;  
    }  
    n = 0;  
    inhex = YES;            //如果字符是有效16进制字符，则置inhex为YES  
    for(; inhex == YES; ++i)  
    {  
        if(s[i] >= '0' && s[i] <= '9')  
            hexdigit = s[i] - '0';  
        else if(s[i] >= 'a' && s[i] <= 'f')  
            hexdigit = s[i] - 'a' + 10;  
        else if(s[i] >= 'A' && s[i] <= 'F')  
            hexdigit = s[i] - 'A' + 10;  
        else  
            inhex = NO;  
        if(inhex == YES)  
            n = n * 16 + hexdigit;  
    }  
	
    return n;  
}  
//16进制字节字符串转换为字节
unsigned char HstrToByte(const char * s)
{
	unsigned char hexdigit, inhex, n;
	hexdigit = 0;
	n = 0;
	inhex = YES;            //如果字符是有效16进制字符，则置inhex为YES 
	for (int i = 0; i < 2; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
			hexdigit = s[i] - '0';
		else if (s[i] >= 'a' && s[i] <= 'f')
			hexdigit = s[i] - 'a' + 10;
		else if (s[i] >= 'A' && s[i] <= 'F')
			hexdigit = s[i] - 'A' + 10;
		else
			inhex = NO;
		if (inhex == YES)
			n = n * 16 + hexdigit;
	}
	return n;
}
//从起始地址读取一个16进制表示的4字节字符串，并将其转换为整形
int FATEK_GetIntData(int Start,char * buf)
{
	CString str=buf;
	return htoi(str.Mid(Start,4));
}
float GetFloatData(int Start, char * buf)
{
	BYTE s[4];
	CString str = buf;
	s[3] = HstrToByte(str.Mid(Start, 2));
	Start += 2;
	s[2] = HstrToByte(str.Mid(Start, 2));
	Start += 2;
	s[1] = HstrToByte(str.Mid(Start, 2));
	Start += 2;
	s[0] = HstrToByte(str.Mid(Start, 2));
	float f;
	memcpy(&f, s, 4);

	return f;
}
//从指定地址读取一个字符，并将其转换为布尔型
bool FATEK_GetByteData(int Start,char * buf)
{
	char ch;
	int RDate=0;
	ch=buf[Start];
	RDate=atoi(&ch);
	if(RDate)
		return TRUE;
	return FALSE;
}
/*读一个或多个开关量  参数：站号，地址类型 ，开始地址， 读的数量， 返回数据变量指针  返回：－1表示LRC校验失败，－2表示无应答， 大于零表示接收数据成功*/
/*00000*/ 
//命令码44（多个连续单点状态读取）
/*
rtu:从机地址
SAddr:加大写的引导字符，长度为5个字节
Count：读取的数量
Ret:返回的读取数据的有效部分。
  */
char FATEK_ReadMultiByte(unsigned char rtu, CString SAddr, unsigned char Count, unsigned char * ret)
{
	unsigned char LRC8;
	unsigned char AddrLen;
	CString str;
	memset(FATEK_Buff, 0x00, 255);

	FATEK_Buff[0]=0x02;//起始码

	str.Format("%2X",rtu);//从机地址转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	FATEK_Buff[3]='4';
	FATEK_Buff[4]='4';

	str.Format("%2X",Count);//读取地址转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[5]=str.GetAt(0);
	FATEK_Buff[6]=str.GetAt(1);
	
	//读取地址转换为16进制字符串
	AddrLen=SAddr.GetLength();
	for(int Addri=0;Addri<AddrLen;Addri++)
		FATEK_Buff[7+Addri]=SAddr.GetAt(Addri);//RegType;
	
	LRC8=CalSum(FATEK_Buff,AddrLen+7);

	str.Format("%2X",LRC8);//读取地址转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[AddrLen+7]=str.GetAt(0);
	FATEK_Buff[AddrLen+8]=str.GetAt(1);
	FATEK_Buff[AddrLen+9]=0x03;
		//发送数据
	unsigned int strlen;
	if(IsOpen())
	{
		//发送数据
		strlen= WriteChar(AddrLen+10, (char *)FATEK_Buff);
		if(strlen==unsigned int(AddrLen+10))
		{
			//读数据
			//Clear_Rec_buffer();//清除串口接收缓冲区中的内容
			memset(FATEK_Buff, 0x00, 255);
			Sleep(50);
			strlen= ReadChar(255, (char *)FATEK_Buff, 1000);
			if(strlen==0)
			{
				//无返回
				return(PLCComErr);				
			}
			else
			{
				//返回长度有效,解析接收缓冲区
				if((strlen==unsigned int(Count+9)) && FATEK_Buff[0]==0x02)
				{
					LRC8=CalSum(FATEK_Buff,strlen-3);
					str.Format("%2X",LRC8);//将计算的校验码转换为字符串
					str.Replace(' ','0');
					str.MakeUpper();
					char i=str.GetAt(0);
					char j=str.GetAt(1);
					if((FATEK_Buff[strlen-3]== str.GetAt(0))  && (FATEK_Buff[strlen-2]== str.GetAt(1)))
					{					
						for(int i=0; i<Count; i++)
						{											
							ret[i]= FATEK_Buff[6+i];
						}								
					}
					else
					{
						return(PLCComErr2);
					}
				}
				else
				{
					return(PLCComErr2);
				}
			}
		}
		else
		{			
			return(PLCComErr);
		}		
	}
	else
	{
		return(PLCComErr);
	}
	
	return(1);

}

/*读一个或多个开关量  参数：站号，地址类型 ，开始地址， 读的数量， 返回数据变量指针  返回：－1表示LRC校验失败，－2表示无应答， 大于零表示接收数据成功*/
/*00000*/ 
//命令码46（多个连续缓存器数据读取）16位缓存器号码为6个字符，而其数据为4个字符的Hex数值（可表示0000H～FFFFH）
/*
rtu:从机地址
SAddr:加大写的引导字符，长度为5个字节
Count：读取的16位寄存器的数量
Ret:返回的读取数据的有效部分。
  */
char FATEK_ReadMultiReg16(unsigned char rtu,CString SAddr, unsigned char Count, unsigned char * ret)
{
	unsigned char LRC8;
	unsigned char AddrLen;
	CString str;
	memset(FATEK_Buff, 0x00, 255);

	FATEK_Buff[0]=0x02;//起始码

	str.Format("%2X",rtu);//从机地址转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	FATEK_Buff[3]='4';
	FATEK_Buff[4]='6';

	str.Format("%2X",Count);//读取地址转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[5]=str.GetAt(0);
	FATEK_Buff[6]=str.GetAt(1);

	//读取地址转换为16进制字符串
	AddrLen=SAddr.GetLength();
	for(int Addri=0;Addri<AddrLen;Addri++)
		FATEK_Buff[7+Addri]=SAddr.GetAt(Addri);//RegType;
	
	LRC8=CalSum(FATEK_Buff,AddrLen+7);

	str.Format("%2X",LRC8);//和校验转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[AddrLen+7]=str.GetAt(0);
	FATEK_Buff[AddrLen+8]=str.GetAt(1);
	FATEK_Buff[AddrLen+9]=0x03;
	
	//发送数据
	unsigned int strlen;
	if(IsOpen())
	{
		//发送数据
		strlen= WriteChar(AddrLen+10, (char *)FATEK_Buff);
		if(strlen==unsigned int(AddrLen+10))
		{
			//读数据
			memset(FATEK_Buff, 0x00, 255);
			Sleep(50);
			strlen= ReadChar(255, (char *)FATEK_Buff, 1000);
			if(strlen==0)
			{
				//无返回
				return(PLCComErr);				
			}
			else
			{
				//返回长度有效,解析接收缓冲区
				if((strlen==unsigned int(Count*4+9)) && FATEK_Buff[0]==0x02)
				{
					LRC8=CalSum(FATEK_Buff,strlen-3);
					str.Format("%2X",LRC8);//将计算的校验码转换为字符串
					str.Replace(' ','0');
					str.MakeUpper();
					if((FATEK_Buff[strlen-3]== str.GetAt(0))  && (FATEK_Buff[strlen-2]== str.GetAt(1)))
					{					
						for(int i=0; i<Count*4; i++)
						{											
							ret[i]= FATEK_Buff[6+i];
						}								
					}
					else
					{
						return(PLCComErr2);
					}
				}
				else
				{
					return(PLCComErr2);
				}
			}
		}
		else
		{			
			return(PLCComErr);
		}		
	}
	else
	{
		return(PLCComErr);
	}
	
	return(1);

}
/*读一个或多个开关量  参数：站号，地址类型 ，开始地址， 读的数量， 返回数据变量指针  返回：－1表示LRC校验失败，－2表示无应答， 大于零表示接收数据成功*/
/*00000*/ 
//命令码46（多个连续缓存器数据读取）16位缓存器号码为6个字符，而其数据为4个字符的Hex数值（可表示0000H～FFFFH）
//32 位缓存器号码为7 个字符，而其资枓为8 个字符的Hex数值（可表示00000000H～FFFFFFFFH）
/*
rtu:从机地址
RegType：所读取的寄存器前的引导字符，例如'R','D'等,用大写
SReg：读取的寄存器的启示地址
Count：读取的32位寄存器的数量
Ret:返回的读取数据的有效部分。
  */
  
char FATEK_ReadMultiReg32(unsigned char rtu,CString SAddr, unsigned char Count, unsigned char * ret)
{
	unsigned char LRC8;
	unsigned char AddrLen;
	CString str;
	memset(FATEK_Buff, 0x00, 255);

	FATEK_Buff[0]=0x02;//起始码

	str.Format("%2X",rtu);//从机地址转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	FATEK_Buff[3]='4';
	FATEK_Buff[4]='6';

	str.Format("%2X",Count);//读取地址转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[5]=str.GetAt(0);
	FATEK_Buff[6]=str.GetAt(1);

	//读取地址转换为16进制字符串
	AddrLen=SAddr.GetLength();
	for(int Addri=0;Addri<AddrLen;Addri++)
		FATEK_Buff[7+Addri]=SAddr.GetAt(Addri);//RegType;
	
	LRC8=CalSum(FATEK_Buff,AddrLen+7);

	str.Format("%2X",LRC8);//和校验转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[AddrLen+7]=str.GetAt(0);
	FATEK_Buff[AddrLen+8]=str.GetAt(1);
	FATEK_Buff[AddrLen+9]=0x03;
	
		//发送数据
	unsigned int strlen;
	if(IsOpen())
	{
		//发送数据
		strlen= WriteChar(AddrLen+10, (char *)FATEK_Buff);
		if(strlen==unsigned int(AddrLen+10))
		{
			//读数据
			memset(FATEK_Buff, 0x00, 255);
			Sleep(50);
			strlen= ReadChar(255, (char *)FATEK_Buff, 1000);
			if(strlen==0)
			{
				//无返回
				return(PLCComErr);				
			}
			else
			{
				//返回长度有效,解析接收缓冲区
				if((strlen==unsigned int(Count*8+9)) && FATEK_Buff[0]==0x02)
				{
					LRC8=CalSum(FATEK_Buff,strlen-3);
					str.Format("%2X",LRC8);//将计算的校验码转换为字符串
					str.Replace(' ','0');
					str.MakeUpper();
					if((FATEK_Buff[strlen-3]== str.GetAt(0))  && (FATEK_Buff[strlen-2]== str.GetAt(1)))
					{						
						for(int i=0; i<Count*8; i++)
						{											
							ret[i]= FATEK_Buff[6+i];
						}								
					}
					else
					{
						return(PLCComErr2);
					}
				}
				else
				{
					return(PLCComErr2);
				}
			}
		}
		else
		{			
			return(PLCComErr);
		}		
	}
	else
	{
		return(PLCComErr);
	}
	
	return(1);

}
//
char FATEK_ReadMultiReg(unsigned char rtu,CString SAddr, unsigned char Count, unsigned char * ret)
{
	if(SAddr.GetLength()==6)
		return	FATEK_ReadMultiReg16(rtu,SAddr,Count,ret);
	else if(SAddr.GetLength()==7)
		return	FATEK_ReadMultiReg32(rtu,SAddr,Count,ret);
	else
	{
		AfxMessageBox("地址不合法！");
		return 0;
	}
	
}

/*写一个模拟量 参数：站号，写地址， 返回数据变量指针  返回：小于零失败，大于零表示成功*/
/*40000*/
//命令码45（多个连续单点状态写入）
//buf为ASSII码
char FATEK_WriteMultiByte(unsigned char rtu, CString SAddr,unsigned char Count, unsigned char *buf1)
{
	unsigned char LRC8;
	unsigned char AddrLen;
	CString str;
	memset(FATEK_Buff, 0x00, 255);

	FATEK_Buff[0]=0x02;//起始码

	str.Format("%2X",rtu);//从机地址转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	FATEK_Buff[3]='4';
	FATEK_Buff[4]='5';

	str.Format("%2X",Count);//读取地址转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[5]=str.GetAt(0);
	FATEK_Buff[6]=str.GetAt(1);

	//读取地址转换为16进制字符串
	AddrLen=SAddr.GetLength();
	for(int Addri=0;Addri<AddrLen;Addri++)
		FATEK_Buff[7+Addri]=SAddr.GetAt(Addri);//RegType;
	//开始写线圈
	for(int Wi=0;Wi<Count;Wi++)
		FATEK_Buff[Wi+7+AddrLen]=buf1[Wi];
	
	LRC8=CalSum(FATEK_Buff,Count+7+AddrLen);

	str.Format("%2X",LRC8);//和校验转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[Count+7+AddrLen]=str.GetAt(0);
	FATEK_Buff[Count+7+AddrLen+1]=str.GetAt(1);
	FATEK_Buff[Count+7+AddrLen+2]=0x03;

	//发送数据
	unsigned int strlen;
	if(IsOpen())
	{
		//发送数据
		strlen= WriteChar(Count+7+AddrLen+3, (char *)FATEK_Buff);
		if(strlen==unsigned int(Count+7+AddrLen+3))
		{
			Sleep(50);
			strlen= ReadChar(255, (char *)FATEK_Buff, 1000);
			//返回长度有效,解析接收缓冲区
			if(!(strlen==9 && FATEK_Buff[0]== 0x02 ))
			{
				return(PLCComErr2);
			}
		}
		else
		{			
			return(PLCComErr2);
		}		
	}
	else
	{
		return(PLCComErr2);
	}
	
	return(1);		
}
/*写一个模拟量 参数：站号，写地址， 返回数据变量指针  返回：小于零失败，大于零表示成功*/
/*40000*/
//命令码47（多个连续缓存器数据写入）
//16位缓存器号码为6个字符，而其数据为4个字符（可表示0000H～FFFFH）
char FATEK_WriteMultiReg16(unsigned char rtu,  CString SAddr,unsigned char Count, unsigned char *buf1)
{
	unsigned char LRC8;
	unsigned char AddrLen;
	CString str;
	memset(FATEK_Buff, 0x00, 255);

	FATEK_Buff[0]=0x02;//起始码

	str.Format("%2X",rtu);//从机地址转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	FATEK_Buff[3]='4';
	FATEK_Buff[4]='7';

	str.Format("%2X",Count);//读取地址转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[5]=str.GetAt(0);
	FATEK_Buff[6]=str.GetAt(1);

	
	//读取地址转换为16进制字符串
	AddrLen=SAddr.GetLength();
	for(int Addri=0;Addri<AddrLen;Addri++)
		FATEK_Buff[7+Addri]=SAddr.GetAt(Addri);//RegType;
	//开始写线圈
	for(int Wi=0;Wi<Count*4;Wi++)
		FATEK_Buff[Wi+7+AddrLen]=buf1[Wi];
	
	LRC8=CalSum(FATEK_Buff,Count*4+7+AddrLen);

	str.Format("%2X",LRC8);//和校验转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[Count*4+7+AddrLen]=str.GetAt(0);
	FATEK_Buff[Count*4+7+AddrLen+1]=str.GetAt(1);
	FATEK_Buff[Count*4+7+AddrLen+2]=0x03;

	//发送数据
	unsigned int strlen;
	if(IsOpen())
	{
		//发送数据
		strlen= WriteChar(Count*4+7+AddrLen+3, (char *)FATEK_Buff);
		if(strlen==unsigned int(Count*4+7+AddrLen+3))
		{
			Sleep(50);
			strlen= ReadChar(255, (char *)FATEK_Buff, 1000);
			//返回长度有效,解析接收缓冲区
			if(!(strlen==9 && FATEK_Buff[0]== 0x02 ))
			{
				return(PLCComErr2);
			}
		}
		else
		{			
			return(PLCComErr2);
		}		
	}
	else
	{
		return(PLCComErr2);
	}
	
	return(1);		
}
/*写一个模拟量 参数：站号，写地址， 返回数据变量指针  返回：小于零失败，大于零表示成功*/
/*40000*/
//命令码47（多个连续缓存器数据写入）
//16位缓存器号码为6个字符，而其数据为4个字符（可表示0000H～FFFFH）
//32位缓存器号码为7个字符，而其资枓为8个字符（可表示00000000H～FFFFFFFFH）
char FATEK_WriteMultiReg32(unsigned char rtu, CString SAddr,unsigned char Count, unsigned char *buf1)
{
	unsigned char LRC8;
	unsigned char AddrLen;
	CString str;
	memset(FATEK_Buff, 0x00, 255);

	FATEK_Buff[0]=0x02;//起始码

	str.Format("%2X",rtu);//从机地址转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	FATEK_Buff[3]='4';
	FATEK_Buff[4]='7';

	str.Format("%2X",Count);//读取地址转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[5]=str.GetAt(0);
	FATEK_Buff[6]=str.GetAt(1);

	
	//读取地址转换为16进制字符串
	AddrLen=SAddr.GetLength();
	for(int Addri=0;Addri<AddrLen;Addri++)
		FATEK_Buff[7+Addri]=SAddr.GetAt(Addri);//RegType;
	//开始写线圈
	for(int Wi=0;Wi<Count*8;Wi++)
		FATEK_Buff[Wi+7+AddrLen]=buf1[Wi];
	
	LRC8=CalSum(FATEK_Buff,Count*8+7+AddrLen);

	str.Format("%2X",LRC8);//和校验转换为16进制字符串
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[Count*8+7+AddrLen]=str.GetAt(0);
	FATEK_Buff[Count*8+7+AddrLen+1]=str.GetAt(1);
	FATEK_Buff[Count*8+7+AddrLen+2]=0x03;

	//发送数据
	unsigned int strlen;
	if(IsOpen())
	{
		//发送数据
		strlen= WriteChar(Count*8+7+AddrLen+3, (char *)FATEK_Buff);
		if(strlen==unsigned int(Count*8+7+AddrLen+3))
		{
			Sleep(50);
			strlen= ReadChar(255, (char *)FATEK_Buff, 1000);
			//返回长度有效,解析接收缓冲区
			if(!(strlen==9 && FATEK_Buff[0]== 0x02 ))
			{
				return(PLCComErr2);
			}
		}
		else
		{			
			return(PLCComErr2);
		}		
	}
	else
	{
		return(PLCComErr2);
	}
	
	return(1);		
}
char FATEK_WriteMultiReg(unsigned char rtu,CString SAddr, unsigned char Count, unsigned char * ret)
{
	if(SAddr.GetLength()==6)
		return	FATEK_WriteMultiReg16(rtu,SAddr,Count,ret);
	else if(SAddr.GetLength()==7)
		return	FATEK_WriteMultiReg32(rtu,SAddr,Count,ret);
	else
	{
		AfxMessageBox("地址不合法！");
		return 0;
	}
	
}