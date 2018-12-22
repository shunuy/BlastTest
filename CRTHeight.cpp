// CRTHeight.cpp: implementation of the CCRTHeight class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlastTest.h"
#include "CRTHeight.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define  OPENCALYO       0 
#define  OPENNI1         1  /*取ni为爆炸状态为1的结果*/
#define  OPENALLDATAASC  2  /*按时间升序打开整个数据库*/
#define  OPENNI0         3  /*取ni为0的结果*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCRTHeight::CCRTHeight()
{
	m_HeightSet=250;//设定的初始重锤高度
	m_CurrHeight=250;//当前的高度
	m_LastHeight=250;
	m_i=0;/*刺激量序号*/
	m_Ni1=0;
	m_Ni0=0;
	m_ini=0;
	m_iini=0;
	m_A=0;
	m_B=0;
	m_M=0;
	m_b=0;
	m_n=0;
	m_Y0=0;
	m_Y50=0;
	m_H50=0;
	m_S=0;//均方差
	m_SignN=0;
	
	m_d=0.1f;//有效步长的对数值

	m_DataTable=_T("TestData");
	m_SpNo=_T("");

	DataSet.SetConnection(::GetConnection());
}

CCRTHeight::~CCRTHeight()
{

}
float CCRTHeight::CalY50()//计算Y50
{
	if(m_n!=0)
		m_Y50=log10f(m_Y0/10) + (m_A/m_n - m_SignN/2)* log10f(m_d/10);//因m_SignN与公式中的定义相反，因此公式中变成了减号,以cm为单位
	else
		m_Y50=0;

	m_Y50=int(m_Y50*100);//保留两位小数
	m_Y50=m_Y50/100.0f;

	return m_Y50;
}
int CCRTHeight::CalAB()//计算A的值
{

	Caln();//返回为1表示取判爆为1的结果作为ni,否则取判爆为0的结果
	
	float Yi;
	
	Yi=m_Y0-4*m_d;
	for (int i=-4;i<5;i++)
	{
		if (m_SignN==1)
		{
			OpenCalData(OPENNI1,Yi);//查询高度为Yi，判爆结果为1的记录
		}
		else
			OpenCalData(OPENNI0,Yi);//查询高度为Yi，判爆结果为1的记录

		m_A=m_A+i*DataSet.GetRecordCount();
		m_B=m_B+i*i*DataSet.GetRecordCount();

		Yi+=m_d;
	}

	return -1;
}
float CCRTHeight::CalH50()//计算特性落高
{
	m_H50=pow(10,m_Y50);

	m_H50=int(m_H50*100);//保留两位小数
	m_H50=m_H50/100.0f;

	return m_H50;
}
int CCRTHeight::Caln()//计算n的值
{
	float Yi;
	int Sum=0;

	Yi=m_Y0-4*m_d;
	for (int i=0;i<9;i++)
	{
		OpenCalData(OPENNI1,Yi);//查询高度为Yi，判爆结果为1的记录
		Yi+=m_d;
		Sum+=DataSet.GetRecordCount();
	}
	OpenCalData(OPENALLDATAASC);//打开整个数据库

	if (Sum<=(DataSet.GetRecordCount()-Sum))
	{
		m_n=Sum;
		m_SignN=1;
		return 1; //按判爆结果为1的n计算
	}
	else
	{
		m_n=DataSet.GetRecordCount()-Sum;
		m_SignN=-1;
		return 0;//按判爆结果为0的n计算
	}

	return -1;
}
float CCRTHeight::CalS()//计算标准差
{
	float M=0;

	m_S=0;
	if(m_n!=0)
	{
		M=m_B/m_n -(m_A*m_A)/(m_n*m_n);
		if(M>0.3)
			m_S=(1.62*( M+0.029) )*log10f(m_d/10);
		else
		{
			float b;
			b=m_A*abs(m_A/m_n -0.5);

			b=int(b*10);//保留1位小数
			b=b/10.0f;
			
			if(b>0.5) b=1-b;

			CString Strb,StrM;
			Strb.Format(_T("%.1f"),b);
			StrM.Format(_T("%.2f"),M);
			if((b==0) && (M<0.25))
			{
				AfxMessageBox(_T("查询表中无对应内容：M=")+StrM+_T(";b=")+Strb);
			}
			else
			{
				float SValue;
				CString sql=_T("select top 1 * from CRTTableMb where M=")+ StrM +_T(" and b=")+ Strb ;
				DataSet.Open(sql);
				if (DataSet.GetRecordCount()>0)
				{
					SValue=DataSet.ReadFloatValue(_T("SValue"));
					m_S=SValue*log10f(m_d/10);
				}
				else
					AfxMessageBox(_T("查询表中无对应内容：M=")+StrM+_T(";b=")+Strb);

			}		
			
		}
	}
	else
		m_S=0;

	m_S=int(m_S*100);//保留两位小数
	m_S=m_S/100.0f;

	return m_S;
}

void CCRTHeight::CalY0()
{
	OpenCalData(OPENCALYO);
	m_Y0=DataSet.ReadFloatValue(_T("重锤高度"));
}
char CCRTHeight::CalCRTResult()//计算全部结果并写入样品数据表
{
	CalY0();
	Caln();
	CalAB();
	CalY50();
	CalH50();
	CalS();

	return -1;
}


char CCRTHeight::OpenCalData(UCHAR Mode,float FParam)//打开数据库
{
	CString sql;
	CString StrParam;
	StrParam.Format(_T("%.1f"),FParam);
	switch(Mode)
	{
	case OPENCALYO:
		sql=_T("select top 1 * from ")+m_DataTable+_T(" where 试样编号='")+ m_SpNo +_T("' order by 实验日期  ASC");
		break;
	case OPENNI1:
		sql=_T("select * from ")+m_DataTable+_T(" where 试样编号='")+ m_SpNo +_T("'")+ _T(" and 重锤高度=")+StrParam+_T(" and 爆炸状态=1 ")+ _T(" order by 实验日期  ASC");
		break;
	case OPENNI0:
		sql=_T("select * from ")+m_DataTable+_T(" where 试样编号='")+ m_SpNo +_T("'")+ _T(" and 重锤高度=")+StrParam+_T(" and 爆炸状态=0 ")+ _T(" order by 实验日期  ASC");
		break;
	case OPENALLDATAASC:
		sql=_T("select * from ")+m_DataTable+_T(" where 试样编号='")+ m_SpNo +_T("' order by 实验日期  ASC");
		break;
	default:
		break;
	}

	DataSet.Open(sql);

	return -1;
}

char CCRTHeight::WriteRecord()//将计算的记过写入数据库
{
	CString sql;
	sql=_T("select top 1 * from SampleTable where 试样编号='")+ m_SpNo +_T("'");
	DataSet.Open(sql);
	DataSet.SetFieldValue(_T("H50"),_variant_t(m_H50));
	DataSet.SetFieldValue(_T("标准差S"),_variant_t(m_S));
	DataSet.SetFieldValue(_T("n"),m_n);
	DataSet.Save();

	return -1;
}

char CCRTHeight::CreateSpRecord(CString SpNO)
{
	return TRUE;

}
void CCRTHeight::GetRecordCount(CADODataSet & DataSet)
{
	
}

