
#ifndef _FATEK_PROTOCOL_H
#define _FATEK_PROTOCOL_H

extern int FATEK_GetIntData(int Start,char * buf);//从起始地址读取一个16进制表示的4字节字符串，并将其转换为整形
extern bool FATEK_GetByteData(int Start,char * buf);//从指定地址读取一个字符，并将其转换为布尔型

extern char FATEK_ReadMultiByte(unsigned char rtu,  CString SAddr, unsigned char Count, unsigned char * ret);
extern char FATEK_ReadMultiReg16(unsigned char rtu, CString SAddr, unsigned char Count, unsigned char * ret);
extern char FATEK_ReadMultiReg32(unsigned char rtu, CString SAddr, unsigned char Count, unsigned char * ret);

extern char FATEK_WriteMultiByte(unsigned char rtu, CString SAddr,unsigned char Count, unsigned char *buf1);
extern char FATEK_WriteMultiReg16(unsigned char rtu, CString SAddr,unsigned char Count, unsigned char *buf1);
extern char FATEK_WriteMultiReg32(unsigned char rtu, CString SAddr,unsigned char Count, unsigned char *buf1);
extern char FATEK_WriteMultiReg(unsigned char rtu,CString SAddr, unsigned char Count, unsigned char * ret);
extern char FATEK_ReadMultiReg(unsigned char rtu,CString SAddr, unsigned char Count, unsigned char * ret);

extern int htoi(const char * s);

extern int FATEK_GetIntData(int Start,char * buf);//从接收的缓冲区获取四个字节的数据，并对其进行字符到数字的转换
extern bool FATEK_GetByteData(int Start,char * buf);//从接收的缓冲区获取一个字节的数据，并对其进行字符到数字的转换
extern float GetFloatData(int Start, char * buf);

#endif