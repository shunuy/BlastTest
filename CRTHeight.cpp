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
#define  OPENNI1         1  /*ȡniΪ��ը״̬Ϊ1�Ľ��*/
#define  OPENALLDATAASC  2  /*��ʱ��������������ݿ�*/
#define  OPENNI0         3  /*ȡniΪ0�Ľ��*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCRTHeight::CCRTHeight()
{
	m_HeightSet=250;//�趨�ĳ�ʼ�ش��߶�
	m_CurrHeight=250;//��ǰ�ĸ߶�
	m_LastHeight=250;
	m_i=0;/*�̼������*/
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
	m_S=0;//������
	m_SignN=0;
	
	m_d=0.1f;//��Ч�����Ķ���ֵ

	m_DataTable=_T("TestData");
	m_SpNo=_T("");

	DataSet.SetConnection(::GetConnection());
}

CCRTHeight::~CCRTHeight()
{

}
float CCRTHeight::CalY50()//����Y50
{
	if(m_n!=0)
		m_Y50=log10f(m_Y0/10) + (m_A/m_n - m_SignN/2)* log10f(m_d/10);//��m_SignN�빫ʽ�еĶ����෴����˹�ʽ�б���˼���,��cmΪ��λ
	else
		m_Y50=0;

	m_Y50=int(m_Y50*100);//������λС��
	m_Y50=m_Y50/100.0f;

	return m_Y50;
}
int CCRTHeight::CalAB()//����A��ֵ
{

	Caln();//����Ϊ1��ʾȡ�б�Ϊ1�Ľ����Ϊni,����ȡ�б�Ϊ0�Ľ��
	
	float Yi;
	
	Yi=m_Y0-4*m_d;
	for (int i=-4;i<5;i++)
	{
		if (m_SignN==1)
		{
			OpenCalData(OPENNI1,Yi);//��ѯ�߶�ΪYi���б����Ϊ1�ļ�¼
		}
		else
			OpenCalData(OPENNI0,Yi);//��ѯ�߶�ΪYi���б����Ϊ1�ļ�¼

		m_A=m_A+i*DataSet.GetRecordCount();
		m_B=m_B+i*i*DataSet.GetRecordCount();

		Yi+=m_d;
	}

	return -1;
}
float CCRTHeight::CalH50()//�����������
{
	m_H50=pow(10,m_Y50);

	m_H50=int(m_H50*100);//������λС��
	m_H50=m_H50/100.0f;

	return m_H50;
}
int CCRTHeight::Caln()//����n��ֵ
{
	float Yi;
	int Sum=0;

	Yi=m_Y0-4*m_d;
	for (int i=0;i<9;i++)
	{
		OpenCalData(OPENNI1,Yi);//��ѯ�߶�ΪYi���б����Ϊ1�ļ�¼
		Yi+=m_d;
		Sum+=DataSet.GetRecordCount();
	}
	OpenCalData(OPENALLDATAASC);//���������ݿ�

	if (Sum<=(DataSet.GetRecordCount()-Sum))
	{
		m_n=Sum;
		m_SignN=1;
		return 1; //���б����Ϊ1��n����
	}
	else
	{
		m_n=DataSet.GetRecordCount()-Sum;
		m_SignN=-1;
		return 0;//���б����Ϊ0��n����
	}

	return -1;
}
float CCRTHeight::CalS()//�����׼��
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

			b=int(b*10);//����1λС��
			b=b/10.0f;
			
			if(b>0.5) b=1-b;

			CString Strb,StrM;
			Strb.Format(_T("%.1f"),b);
			StrM.Format(_T("%.2f"),M);
			if((b==0) && (M<0.25))
			{
				AfxMessageBox(_T("��ѯ�����޶�Ӧ���ݣ�M=")+StrM+_T(";b=")+Strb);
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
					AfxMessageBox(_T("��ѯ�����޶�Ӧ���ݣ�M=")+StrM+_T(";b=")+Strb);

			}		
			
		}
	}
	else
		m_S=0;

	m_S=int(m_S*100);//������λС��
	m_S=m_S/100.0f;

	return m_S;
}

void CCRTHeight::CalY0()
{
	OpenCalData(OPENCALYO);
	m_Y0=DataSet.ReadFloatValue(_T("�ش��߶�"));
}
char CCRTHeight::CalCRTResult()//����ȫ�������д����Ʒ���ݱ�
{
	CalY0();
	Caln();
	CalAB();
	CalY50();
	CalH50();
	CalS();

	return -1;
}


char CCRTHeight::OpenCalData(UCHAR Mode,float FParam)//�����ݿ�
{
	CString sql;
	CString StrParam;
	StrParam.Format(_T("%.1f"),FParam);
	switch(Mode)
	{
	case OPENCALYO:
		sql=_T("select top 1 * from ")+m_DataTable+_T(" where �������='")+ m_SpNo +_T("' order by ʵ������  ASC");
		break;
	case OPENNI1:
		sql=_T("select * from ")+m_DataTable+_T(" where �������='")+ m_SpNo +_T("'")+ _T(" and �ش��߶�=")+StrParam+_T(" and ��ը״̬=1 ")+ _T(" order by ʵ������  ASC");
		break;
	case OPENNI0:
		sql=_T("select * from ")+m_DataTable+_T(" where �������='")+ m_SpNo +_T("'")+ _T(" and �ش��߶�=")+StrParam+_T(" and ��ը״̬=0 ")+ _T(" order by ʵ������  ASC");
		break;
	case OPENALLDATAASC:
		sql=_T("select * from ")+m_DataTable+_T(" where �������='")+ m_SpNo +_T("' order by ʵ������  ASC");
		break;
	default:
		break;
	}

	DataSet.Open(sql);

	return -1;
}

char CCRTHeight::WriteRecord()//������ļǹ�д�����ݿ�
{
	CString sql;
	sql=_T("select top 1 * from SampleTable where �������='")+ m_SpNo +_T("'");
	DataSet.Open(sql);
	DataSet.SetFieldValue(_T("H50"),_variant_t(m_H50));
	DataSet.SetFieldValue(_T("��׼��S"),_variant_t(m_S));
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

