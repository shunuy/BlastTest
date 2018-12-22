// CRTHeight.h: interface for the CCRTHeight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRTHEIGHT_H__8BC419BA_3E34_49BA_B480_B10DEEFBF3D1__INCLUDED_)
#define AFX_CRTHEIGHT_H__8BC419BA_3E34_49BA_B480_B10DEEFBF3D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ADO/ADO.h"

class CCRTHeight  
{
public:
	CCRTHeight();
	virtual ~CCRTHeight();
	char CreateSpRecord(CString SpNO);//��������Ʒ�������¼
	void GetRecordCount(CADODataSet & DataSet);//�õ�����Ʒ�������¼����

public:
	float m_HeightSet;//�趨�ĳ�ʼ�ش��߶�
	float m_CurrHeight;//��ǰ�ĸ߶�
	float m_LastHeight;//��ǰ�ĸ߶�
	int m_i;/*�̼������*/
	int m_Ni1;
	int m_Ni0;
	int m_ini;
	int m_iini;
	int m_A;
	int m_B;
	float m_M;
	float m_b;
	short m_n;
	float m_Y0;
	float m_Y50;
	float m_H50;
	float m_S;//������
	float m_d;//��Ч����
	int  m_SignN;//������ţ��б���Ϊ1������С��Ϊ1������Ϊ-1

	CString m_DataTable;//���ڼ�������ݿ���
	CString m_SpNo;    //��Ʒ���

	CADODataSet DataSet;

public:
	void CalY0();
	float CalY50();//����Y50
	int CalAB();//����A��ֵ
	float CalH50();//�����������
	int Caln();//����n��ֵ
	float CalS();//�����׼��
	char CalCRTResult();//����ȫ�������д����Ʒ���ݱ�
	char OpenCalData(UCHAR Mode,float FParam=0);//�����ݿ�
	char WriteRecord();//������ļǹ�д�����ݿ�



};

#endif // !defined(AFX_CRTHEIGHT_H__8BC419BA_3E34_49BA_B480_B10DEEFBF3D1__INCLUDED_)





















