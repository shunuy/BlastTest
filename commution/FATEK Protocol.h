
#ifndef _FATEK_PROTOCOL_H
#define _FATEK_PROTOCOL_H

extern int FATEK_GetIntData(int Start,char * buf);//����ʼ��ַ��ȡһ��16���Ʊ�ʾ��4�ֽ��ַ�����������ת��Ϊ����
extern bool FATEK_GetByteData(int Start,char * buf);//��ָ����ַ��ȡһ���ַ���������ת��Ϊ������

extern char FATEK_ReadMultiByte(unsigned char rtu,  CString SAddr, unsigned char Count, unsigned char * ret);
extern char FATEK_ReadMultiReg16(unsigned char rtu, CString SAddr, unsigned char Count, unsigned char * ret);
extern char FATEK_ReadMultiReg32(unsigned char rtu, CString SAddr, unsigned char Count, unsigned char * ret);

extern char FATEK_WriteMultiByte(unsigned char rtu, CString SAddr,unsigned char Count, unsigned char *buf1);
extern char FATEK_WriteMultiReg16(unsigned char rtu, CString SAddr,unsigned char Count, unsigned char *buf1);
extern char FATEK_WriteMultiReg32(unsigned char rtu, CString SAddr,unsigned char Count, unsigned char *buf1);
extern char FATEK_WriteMultiReg(unsigned char rtu,CString SAddr, unsigned char Count, unsigned char * ret);
extern char FATEK_ReadMultiReg(unsigned char rtu,CString SAddr, unsigned char Count, unsigned char * ret);

extern int htoi(const char * s);

extern int FATEK_GetIntData(int Start,char * buf);//�ӽ��յĻ�������ȡ�ĸ��ֽڵ����ݣ�����������ַ������ֵ�ת��
extern bool FATEK_GetByteData(int Start,char * buf);//�ӽ��յĻ�������ȡһ���ֽڵ����ݣ�����������ַ������ֵ�ת��
extern float GetFloatData(int Start, char * buf);

#endif