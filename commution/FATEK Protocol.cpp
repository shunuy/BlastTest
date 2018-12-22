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
ͨѶ����������Ҫ�Ĺ������ڶ�ȡ��д��PLC�ڲ����������򻺴�������״̬����ֵ���ݡ���Ϊ����
��������PLC�ڲ���������Щ����ɹ����ã��Լ������ָ�����ǡ��±�Ϊ����PLC�ɹ���ȡ�ĵ����뻺��
�������ַָ��������������������Դ�С��Χ��������ο���������ƪ�ֲ��2.1�ڵ�˵������
----------------------------------------------------------------------------------------------------------
���|
����|  ��  ��    ����          �����ַָ����5���ַ���  16λ������ָ����6���ַ���  32λ������ָ����7���ַ���
----|-----------------------------------------------------------------------------------------------------
��  |   X        ����ӵ�      X0000��X9999             WX0000��WX9984                DWX0000��DWX9968 
��  |   Y       ����̵���     Y0000��Y9999             WY0000��WY9984                DWY0000��DWY9968 
״  |   M       �ڲ��̵���     M0000��M9999             WM0000��WM9984                DWM0000��DWM9968 
̬  |   S       �����̵���     S0000��S9999             WS0000��WS9984                DWS0000��DWS9968 
    |   T       ��ʱ���ӵ�     T0000��T9999             WT0000��WT9984                DWT0000��DWT9968 
	|   C       �������ӵ�     C0000��C9999             WC0000��WC9984                DWC0000��DWC9968
 ---|-----------------------------------------------------------------------------------------------------
��  |  TMR      ��ʱ��������     --                     RT0000��RT9999                 DRT0000��DRT9998 
��	|  CTR      ������������     ---                    RC0000��RC9999                 DRC0000��DRC9998
��  |  HR       ���ݻ�����       ---                    R00000��R65535                 DR00000��DR65534
��	|  DR       ���ݻ�����       ---                    D00000��D65535                 DD00000��DD65534
��  |  FR       ����������       ---                    F00000��F65535                 DF00000��DF65534
 
*/
/*
�ؼ�ת����
	���ֵ�16�����ַ��� ʾ����
	str.Format("%2X",rtu);//�ӻ���ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	�ַ�����ȡָ������ת��������
	int atoi( const char *string );
	CString Mid( int nFirst, int nCount ) 
	int a=atoi(str.Mid(2,2))//�÷����޷�ת��16���Ʊ�ʾ���ַ���
	int a=htoi(str1.Mid(0,4));//���ô��������ҵĺ���

*/
/*
��16���Ʊ�ʾ���ַ���ת��Ϊ����
*/
#define     YES     1  
#define     NO      0 
int htoi(const char * s)  
{  
    int hexdigit, i, inhex, n;  
	
    i = 0;  
    if(s[i] == '0')         //����ַ�����0x��0X��ͷ���������������ַ�  
    {  
        ++i;  
        if(s[i] == 'x' || s[i] == 'X')    
            ++i;  
    }  
    n = 0;  
    inhex = YES;            //����ַ�����Ч16�����ַ�������inhexΪYES  
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
//16�����ֽ��ַ���ת��Ϊ�ֽ�
unsigned char HstrToByte(const char * s)
{
	unsigned char hexdigit, inhex, n;
	hexdigit = 0;
	n = 0;
	inhex = YES;            //����ַ�����Ч16�����ַ�������inhexΪYES 
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
//����ʼ��ַ��ȡһ��16���Ʊ�ʾ��4�ֽ��ַ�����������ת��Ϊ����
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
//��ָ����ַ��ȡһ���ַ���������ת��Ϊ������
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
/*��һ������������  ������վ�ţ���ַ���� ����ʼ��ַ�� ���������� �������ݱ���ָ��  ���أ���1��ʾLRCУ��ʧ�ܣ���2��ʾ��Ӧ�� �������ʾ�������ݳɹ�*/
/*00000*/ 
//������44�������������״̬��ȡ��
/*
rtu:�ӻ���ַ
SAddr:�Ӵ�д�������ַ�������Ϊ5���ֽ�
Count����ȡ������
Ret:���صĶ�ȡ���ݵ���Ч���֡�
  */
char FATEK_ReadMultiByte(unsigned char rtu, CString SAddr, unsigned char Count, unsigned char * ret)
{
	unsigned char LRC8;
	unsigned char AddrLen;
	CString str;
	memset(FATEK_Buff, 0x00, 255);

	FATEK_Buff[0]=0x02;//��ʼ��

	str.Format("%2X",rtu);//�ӻ���ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	FATEK_Buff[3]='4';
	FATEK_Buff[4]='4';

	str.Format("%2X",Count);//��ȡ��ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[5]=str.GetAt(0);
	FATEK_Buff[6]=str.GetAt(1);
	
	//��ȡ��ַת��Ϊ16�����ַ���
	AddrLen=SAddr.GetLength();
	for(int Addri=0;Addri<AddrLen;Addri++)
		FATEK_Buff[7+Addri]=SAddr.GetAt(Addri);//RegType;
	
	LRC8=CalSum(FATEK_Buff,AddrLen+7);

	str.Format("%2X",LRC8);//��ȡ��ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[AddrLen+7]=str.GetAt(0);
	FATEK_Buff[AddrLen+8]=str.GetAt(1);
	FATEK_Buff[AddrLen+9]=0x03;
		//��������
	unsigned int strlen;
	if(IsOpen())
	{
		//��������
		strlen= WriteChar(AddrLen+10, (char *)FATEK_Buff);
		if(strlen==unsigned int(AddrLen+10))
		{
			//������
			//Clear_Rec_buffer();//������ڽ��ջ������е�����
			memset(FATEK_Buff, 0x00, 255);
			Sleep(50);
			strlen= ReadChar(255, (char *)FATEK_Buff, 1000);
			if(strlen==0)
			{
				//�޷���
				return(PLCComErr);				
			}
			else
			{
				//���س�����Ч,�������ջ�����
				if((strlen==unsigned int(Count+9)) && FATEK_Buff[0]==0x02)
				{
					LRC8=CalSum(FATEK_Buff,strlen-3);
					str.Format("%2X",LRC8);//�������У����ת��Ϊ�ַ���
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

/*��һ������������  ������վ�ţ���ַ���� ����ʼ��ַ�� ���������� �������ݱ���ָ��  ���أ���1��ʾLRCУ��ʧ�ܣ���2��ʾ��Ӧ�� �������ʾ�������ݳɹ�*/
/*00000*/ 
//������46������������������ݶ�ȡ��16λ����������Ϊ6���ַ�����������Ϊ4���ַ���Hex��ֵ���ɱ�ʾ0000H��FFFFH��
/*
rtu:�ӻ���ַ
SAddr:�Ӵ�д�������ַ�������Ϊ5���ֽ�
Count����ȡ��16λ�Ĵ���������
Ret:���صĶ�ȡ���ݵ���Ч���֡�
  */
char FATEK_ReadMultiReg16(unsigned char rtu,CString SAddr, unsigned char Count, unsigned char * ret)
{
	unsigned char LRC8;
	unsigned char AddrLen;
	CString str;
	memset(FATEK_Buff, 0x00, 255);

	FATEK_Buff[0]=0x02;//��ʼ��

	str.Format("%2X",rtu);//�ӻ���ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	FATEK_Buff[3]='4';
	FATEK_Buff[4]='6';

	str.Format("%2X",Count);//��ȡ��ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[5]=str.GetAt(0);
	FATEK_Buff[6]=str.GetAt(1);

	//��ȡ��ַת��Ϊ16�����ַ���
	AddrLen=SAddr.GetLength();
	for(int Addri=0;Addri<AddrLen;Addri++)
		FATEK_Buff[7+Addri]=SAddr.GetAt(Addri);//RegType;
	
	LRC8=CalSum(FATEK_Buff,AddrLen+7);

	str.Format("%2X",LRC8);//��У��ת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[AddrLen+7]=str.GetAt(0);
	FATEK_Buff[AddrLen+8]=str.GetAt(1);
	FATEK_Buff[AddrLen+9]=0x03;
	
	//��������
	unsigned int strlen;
	if(IsOpen())
	{
		//��������
		strlen= WriteChar(AddrLen+10, (char *)FATEK_Buff);
		if(strlen==unsigned int(AddrLen+10))
		{
			//������
			memset(FATEK_Buff, 0x00, 255);
			Sleep(50);
			strlen= ReadChar(255, (char *)FATEK_Buff, 1000);
			if(strlen==0)
			{
				//�޷���
				return(PLCComErr);				
			}
			else
			{
				//���س�����Ч,�������ջ�����
				if((strlen==unsigned int(Count*4+9)) && FATEK_Buff[0]==0x02)
				{
					LRC8=CalSum(FATEK_Buff,strlen-3);
					str.Format("%2X",LRC8);//�������У����ת��Ϊ�ַ���
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
/*��һ������������  ������վ�ţ���ַ���� ����ʼ��ַ�� ���������� �������ݱ���ָ��  ���أ���1��ʾLRCУ��ʧ�ܣ���2��ʾ��Ӧ�� �������ʾ�������ݳɹ�*/
/*00000*/ 
//������46������������������ݶ�ȡ��16λ����������Ϊ6���ַ�����������Ϊ4���ַ���Hex��ֵ���ɱ�ʾ0000H��FFFFH��
//32 λ����������Ϊ7 ���ַ��������ʖ�Ϊ8 ���ַ���Hex��ֵ���ɱ�ʾ00000000H��FFFFFFFFH��
/*
rtu:�ӻ���ַ
RegType������ȡ�ļĴ���ǰ�������ַ�������'R','D'��,�ô�д
SReg����ȡ�ļĴ�������ʾ��ַ
Count����ȡ��32λ�Ĵ���������
Ret:���صĶ�ȡ���ݵ���Ч���֡�
  */
  
char FATEK_ReadMultiReg32(unsigned char rtu,CString SAddr, unsigned char Count, unsigned char * ret)
{
	unsigned char LRC8;
	unsigned char AddrLen;
	CString str;
	memset(FATEK_Buff, 0x00, 255);

	FATEK_Buff[0]=0x02;//��ʼ��

	str.Format("%2X",rtu);//�ӻ���ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	FATEK_Buff[3]='4';
	FATEK_Buff[4]='6';

	str.Format("%2X",Count);//��ȡ��ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[5]=str.GetAt(0);
	FATEK_Buff[6]=str.GetAt(1);

	//��ȡ��ַת��Ϊ16�����ַ���
	AddrLen=SAddr.GetLength();
	for(int Addri=0;Addri<AddrLen;Addri++)
		FATEK_Buff[7+Addri]=SAddr.GetAt(Addri);//RegType;
	
	LRC8=CalSum(FATEK_Buff,AddrLen+7);

	str.Format("%2X",LRC8);//��У��ת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[AddrLen+7]=str.GetAt(0);
	FATEK_Buff[AddrLen+8]=str.GetAt(1);
	FATEK_Buff[AddrLen+9]=0x03;
	
		//��������
	unsigned int strlen;
	if(IsOpen())
	{
		//��������
		strlen= WriteChar(AddrLen+10, (char *)FATEK_Buff);
		if(strlen==unsigned int(AddrLen+10))
		{
			//������
			memset(FATEK_Buff, 0x00, 255);
			Sleep(50);
			strlen= ReadChar(255, (char *)FATEK_Buff, 1000);
			if(strlen==0)
			{
				//�޷���
				return(PLCComErr);				
			}
			else
			{
				//���س�����Ч,�������ջ�����
				if((strlen==unsigned int(Count*8+9)) && FATEK_Buff[0]==0x02)
				{
					LRC8=CalSum(FATEK_Buff,strlen-3);
					str.Format("%2X",LRC8);//�������У����ת��Ϊ�ַ���
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
		AfxMessageBox("��ַ���Ϸ���");
		return 0;
	}
	
}

/*дһ��ģ���� ������վ�ţ�д��ַ�� �������ݱ���ָ��  ���أ�С����ʧ�ܣ��������ʾ�ɹ�*/
/*40000*/
//������45�������������״̬д�룩
//bufΪASSII��
char FATEK_WriteMultiByte(unsigned char rtu, CString SAddr,unsigned char Count, unsigned char *buf1)
{
	unsigned char LRC8;
	unsigned char AddrLen;
	CString str;
	memset(FATEK_Buff, 0x00, 255);

	FATEK_Buff[0]=0x02;//��ʼ��

	str.Format("%2X",rtu);//�ӻ���ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	FATEK_Buff[3]='4';
	FATEK_Buff[4]='5';

	str.Format("%2X",Count);//��ȡ��ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[5]=str.GetAt(0);
	FATEK_Buff[6]=str.GetAt(1);

	//��ȡ��ַת��Ϊ16�����ַ���
	AddrLen=SAddr.GetLength();
	for(int Addri=0;Addri<AddrLen;Addri++)
		FATEK_Buff[7+Addri]=SAddr.GetAt(Addri);//RegType;
	//��ʼд��Ȧ
	for(int Wi=0;Wi<Count;Wi++)
		FATEK_Buff[Wi+7+AddrLen]=buf1[Wi];
	
	LRC8=CalSum(FATEK_Buff,Count+7+AddrLen);

	str.Format("%2X",LRC8);//��У��ת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[Count+7+AddrLen]=str.GetAt(0);
	FATEK_Buff[Count+7+AddrLen+1]=str.GetAt(1);
	FATEK_Buff[Count+7+AddrLen+2]=0x03;

	//��������
	unsigned int strlen;
	if(IsOpen())
	{
		//��������
		strlen= WriteChar(Count+7+AddrLen+3, (char *)FATEK_Buff);
		if(strlen==unsigned int(Count+7+AddrLen+3))
		{
			Sleep(50);
			strlen= ReadChar(255, (char *)FATEK_Buff, 1000);
			//���س�����Ч,�������ջ�����
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
/*дһ��ģ���� ������վ�ţ�д��ַ�� �������ݱ���ָ��  ���أ�С����ʧ�ܣ��������ʾ�ɹ�*/
/*40000*/
//������47�������������������д�룩
//16λ����������Ϊ6���ַ�����������Ϊ4���ַ����ɱ�ʾ0000H��FFFFH��
char FATEK_WriteMultiReg16(unsigned char rtu,  CString SAddr,unsigned char Count, unsigned char *buf1)
{
	unsigned char LRC8;
	unsigned char AddrLen;
	CString str;
	memset(FATEK_Buff, 0x00, 255);

	FATEK_Buff[0]=0x02;//��ʼ��

	str.Format("%2X",rtu);//�ӻ���ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	FATEK_Buff[3]='4';
	FATEK_Buff[4]='7';

	str.Format("%2X",Count);//��ȡ��ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[5]=str.GetAt(0);
	FATEK_Buff[6]=str.GetAt(1);

	
	//��ȡ��ַת��Ϊ16�����ַ���
	AddrLen=SAddr.GetLength();
	for(int Addri=0;Addri<AddrLen;Addri++)
		FATEK_Buff[7+Addri]=SAddr.GetAt(Addri);//RegType;
	//��ʼд��Ȧ
	for(int Wi=0;Wi<Count*4;Wi++)
		FATEK_Buff[Wi+7+AddrLen]=buf1[Wi];
	
	LRC8=CalSum(FATEK_Buff,Count*4+7+AddrLen);

	str.Format("%2X",LRC8);//��У��ת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[Count*4+7+AddrLen]=str.GetAt(0);
	FATEK_Buff[Count*4+7+AddrLen+1]=str.GetAt(1);
	FATEK_Buff[Count*4+7+AddrLen+2]=0x03;

	//��������
	unsigned int strlen;
	if(IsOpen())
	{
		//��������
		strlen= WriteChar(Count*4+7+AddrLen+3, (char *)FATEK_Buff);
		if(strlen==unsigned int(Count*4+7+AddrLen+3))
		{
			Sleep(50);
			strlen= ReadChar(255, (char *)FATEK_Buff, 1000);
			//���س�����Ч,�������ջ�����
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
/*дһ��ģ���� ������վ�ţ�д��ַ�� �������ݱ���ָ��  ���أ�С����ʧ�ܣ��������ʾ�ɹ�*/
/*40000*/
//������47�������������������д�룩
//16λ����������Ϊ6���ַ�����������Ϊ4���ַ����ɱ�ʾ0000H��FFFFH��
//32λ����������Ϊ7���ַ��������ʖ�Ϊ8���ַ����ɱ�ʾ00000000H��FFFFFFFFH��
char FATEK_WriteMultiReg32(unsigned char rtu, CString SAddr,unsigned char Count, unsigned char *buf1)
{
	unsigned char LRC8;
	unsigned char AddrLen;
	CString str;
	memset(FATEK_Buff, 0x00, 255);

	FATEK_Buff[0]=0x02;//��ʼ��

	str.Format("%2X",rtu);//�ӻ���ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[1]=str.GetAt(0);
	FATEK_Buff[2]=str.GetAt(1);

	FATEK_Buff[3]='4';
	FATEK_Buff[4]='7';

	str.Format("%2X",Count);//��ȡ��ַת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[5]=str.GetAt(0);
	FATEK_Buff[6]=str.GetAt(1);

	
	//��ȡ��ַת��Ϊ16�����ַ���
	AddrLen=SAddr.GetLength();
	for(int Addri=0;Addri<AddrLen;Addri++)
		FATEK_Buff[7+Addri]=SAddr.GetAt(Addri);//RegType;
	//��ʼд��Ȧ
	for(int Wi=0;Wi<Count*8;Wi++)
		FATEK_Buff[Wi+7+AddrLen]=buf1[Wi];
	
	LRC8=CalSum(FATEK_Buff,Count*8+7+AddrLen);

	str.Format("%2X",LRC8);//��У��ת��Ϊ16�����ַ���
	str.Replace(' ','0');
	str.MakeUpper();
	FATEK_Buff[Count*8+7+AddrLen]=str.GetAt(0);
	FATEK_Buff[Count*8+7+AddrLen+1]=str.GetAt(1);
	FATEK_Buff[Count*8+7+AddrLen+2]=0x03;

	//��������
	unsigned int strlen;
	if(IsOpen())
	{
		//��������
		strlen= WriteChar(Count*8+7+AddrLen+3, (char *)FATEK_Buff);
		if(strlen==unsigned int(Count*8+7+AddrLen+3))
		{
			Sleep(50);
			strlen= ReadChar(255, (char *)FATEK_Buff, 1000);
			//���س�����Ч,�������ջ�����
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
		AfxMessageBox("��ַ���Ϸ���");
		return 0;
	}
	
}