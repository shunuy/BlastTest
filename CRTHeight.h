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
	char CreateSpRecord(CString SpNO);//创建本样品的试验记录
	void GetRecordCount(CADODataSet & DataSet);//得到本样品的试验记录数量

public:
	float m_HeightSet;//设定的初始重锤高度
	float m_CurrHeight;//当前的高度
	float m_LastHeight;//当前的高度
	int m_i;/*刺激量序号*/
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
	float m_S;//均方差
	float m_d;//有效步长
	int  m_SignN;//计算符号，判爆和为1的数量小则为1，否则为-1

	CString m_DataTable;//用于计算的数据库表格
	CString m_SpNo;    //样品编号

	CADODataSet DataSet;

public:
	void CalY0();
	float CalY50();//计算Y50
	int CalAB();//计算A的值
	float CalH50();//计算特性落高
	int Caln();//计算n的值
	float CalS();//计算标准差
	char CalCRTResult();//计算全部结果并写入样品数据表
	char OpenCalData(UCHAR Mode,float FParam=0);//打开数据库
	char WriteRecord();//将计算的记过写入数据库



};

#endif // !defined(AFX_CRTHEIGHT_H__8BC419BA_3E34_49BA_B480_B10DEEFBF3D1__INCLUDED_)





















