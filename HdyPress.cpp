// HdyPress.cpp: implementation of the CHdyPress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlastTest.h"
#include "HdyPress.h"
#include "math.h"
#include "BlastTestDlg.h"
#include "ADO\ADO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HdyData::HdyData()
{
	m_SendPtr = 0;
	m_BuffPtr = 0;
}
HdyData::~HdyData()
{

}
void HdyData::AddSendData(char Cmd, int Data)
{
	m_Data[m_BuffPtr] = Data;
	m_Cmd[m_BuffPtr] = Cmd;
	m_BuffPtr++;
	m_BuffPtr %= SEND_BUFF_COUNT;
	PlcData.ComFunIndex = COMM_WRITE_HDY_DATA;//����Һѹͨ��  20160808ɾ����ֱ��PLC����
}

void HdyData::AddSendData(char Cmd, bool Data)
{
	m_Data[m_BuffPtr] = Data;
	m_Cmd[m_BuffPtr] = (int)Cmd;
	m_BuffPtr++;
	m_BuffPtr %= SEND_BUFF_COUNT;
}
void HdyData::AddSendPtr()
{
	m_SendPtr++;
	m_SendPtr %= SEND_BUFF_COUNT;
}
bool HdyData::IsBuffEmpty()//���ͻ�����Ϊ��,���ͻ�����λ�շ���1
{
	if (m_BuffPtr != m_SendPtr)
		return false;
	return true;
}


CHdyPress::CHdyPress()
	:m_M(0)   
	,m_FwVal(0)   
	,m_BackVal(0)   
	,m_AccVal(0)  
	,m_ValDAValue(0) 
	,m_SetValDAValue(0)
	,m_PressValue(0)  
	,m_PressSetValue(0)  
	,m_BeginPressing(0)
	,m_FSteep(FINISHPRESS)  
	,m_BSteep(FINISH)   
	, m_ManualF(0)
	, m_ManualB(0)
	, m_PressSetOffset(0.00)
{

}

CHdyPress::~CHdyPress()
{

}

void CHdyPress::SetForward()
{
	m_HdyData.AddSendData(CMD_BackVal, 0);
	m_HdyData.AddSendData(CMD_FwVal, 1);	
}

void CHdyPress::SetDrawBack()
{
	m_HdyData.AddSendData(CMD_BackVal, 1);
	m_HdyData.AddSendData(CMD_FwVal, 0);
}

void CHdyPress::OpenAccVal(bool SW) //������������ŷ�
{
	m_AccVal=SW;	
}

bool CHdyPress::SetDAValue(short value)
{
	m_ValDAValue=value;
	m_HdyData.AddSendData(CMD_DAVALUE, value);
	return true;
}

void CHdyPress::StopWork()
{
	m_HdyData.AddSendData(CMD_Moror,0);
	m_HdyData.AddSendData(CMD_BackVal, 0);
	m_HdyData.AddSendData(CMD_FwVal, 0);
}

void CHdyPress::InitPressFun(float PressValue)
{
	m_FSteep=FASTCLOSE;
	m_BSteep=NOBACK;
	SetDAValue(0);
	//GetSetDAValue(PressValue);
	//SetForward();
}

int CHdyPress::PressFun()
{
	if(m_ValDAValue>DAMAXBALUE)
		m_FSteep=PRESSFAILD;
	if (PlcData.m_HdyPress.m_PressValue>=PlcData.m_HdyPress.m_PressSetValue)
	{
		m_FSteep = FINISHPRESS;
	}
	else if (PlcData.m_HdyPress.m_PressValue <= (PlcData.m_HdyPress.m_PressSetValue-0.02))
	{
		m_FSteep = FASTPRESS;
	}
	switch(m_FSteep)
	{
	case FASTCLOSE:
		SetDAValue(FASTForwardDA);
		m_BeginPressing = 1;
		if(m_BeginPressing)
			m_FSteep=FASTPRESS;
		break;
	case FASTPRESS:

		if (m_PressSetValue<3)
		{
			if (m_PressSetValue>(m_PressValue + 0.5))
				m_ValDAValue += 100;
			else
				m_ValDAValue += 50;
		}
		if ((m_PressSetValue<4) && (m_PressSetValue>=3) )
		{
			if (m_PressSetValue>(m_PressValue + 0.5))
				m_ValDAValue = +500;
			else
				m_ValDAValue += 100;
		}
		if ((m_PressSetValue<5) && (m_PressSetValue>=4))
		{
			if (m_PressSetValue>(m_PressValue + 0.5))
				m_ValDAValue += 1000;
			else
				m_ValDAValue +=200;
		}
		if ((m_PressSetValue<6) && (m_PressSetValue >= 5))
		{
			if (m_PressSetValue>(m_PressValue + 0.5))
				m_ValDAValue += 1500;
			else
				m_ValDAValue += 300;
		}
		if ((m_PressSetValue<6.8) && (m_PressSetValue >= 6))
		{
			if (m_PressSetValue>(m_PressValue + 0.5))
				m_ValDAValue += 1800;
			else
				m_ValDAValue += 300;
		}
		

		m_HdyData.AddSendData(CMD_DAVALUE, m_ValDAValue);
		break;
	case SMALLPRESS:
		/*if(abs(m_PressValue-PRESS_OFFSET_VALUE-m_PressSetValue)>ACCEPT_ERR_PRESS)//��ѹ�����������Χ��
		{
			if((m_PressValue-PRESS_OFFSET_VALUE)>m_PressSetValue)//�����趨ֵ
			{
				m_ValDAValue=m_ValDAValue-DA_MIN_STEEP_VALUE;
			}
			else
			{
				m_ValDAValue=m_ValDAValue+DA_MIN_STEEP_VALUE;
			}
			m_HdyData.AddSendData(CMD_DAVALUE, m_ValDAValue);
		}
		else //��ѹ���
		{
			m_FSteep=FINISHPRESS;
			MaintainPress();//������ѹ����
			SaveDAValue(m_ValDAValue);//���µ�DAֵд�룬�����´ε���
			m_SetValDAValue=m_ValDAValue;//ʹ���µ�DAֵ��
		}*/
		break;
	case FINISHPRESS:
		
		break;
	case PRESSFAILD:
		
		break;
	case NOPRESS:
		
		break;
	default:
		m_FSteep=NOPRESS;
		break;
	}

	return 0;
}


short CHdyPress::GetSetDAValue(float PressValue)
{
	CADODataSet Dat;
	Dat.SetConnection(::GetConnection());
	CString SQL,str;
	str.Format(_T("%.2f"), m_PressSetValue);
	SQL = _T("select * from DAValue where �趨Ŀ��ѹ��= ") + str;
	Dat.Open(SQL);
	if (Dat.GetRecordCount() > 0)
		m_SetValDAValue = Dat.ReadIntValue(_T("��ӦDAֵ"));
	else
		m_SetValDAValue = DAMAXBALUE;
	return true;
}
void CHdyPress::SaveDAValue(short value)
{
	int Davalue = value;
	CADODataSet Dat;
	Dat.SetConnection(::GetConnection());
	CString SQL, str;
	str.Format(_T("%.2f"), m_PressValue);
	SQL = _T("select * from DAValue where �趨Ŀ��ѹ��= ") + str ;
	Dat.Open(SQL);
	if (Dat.GetRecordCount() > 0)
		Dat.SetFieldValue(_T("��ӦDAֵ"), Davalue);
	else
	{
		Dat.AddNew();
		Dat.SetFieldValue(_T("�趨Ŀ��ѹ��"), m_PressSetValue);
		Dat.SetFieldValue(_T("��ӦDAֵ"), Davalue);
		Dat.Save();
	}

}
bool CHdyPress::DelSetDAValue()
{
	CADODataSet Dat;
	Dat.SetConnection(::GetConnection());
	CString SQL, str;
	str.Format(_T("%.2f"), m_PressSetValue);
	SQL = _T("select * from DAValue where �趨Ŀ��ѹ��= ") + str;
	Dat.Open(SQL);
	if (Dat.GetRecordCount() > 0)
	{
		Dat.Delete();
		Dat.Save();
		return TRUE;
	}
	else
		return FALSE;
}

void CHdyPress::MaintainPress()//������ѹ����
{
	
}

bool CHdyPress::ManualForward(bool IsForward)
{
	
	return true;
}

bool CHdyPress::ManualBack(bool IsBack)
{
	
	return true;
}

void CHdyPress::ManualForward()
{
	m_ValDAValue = 1000;
	if (m_ManualF)
		SetForward();


}
void CHdyPress::ManualBack()
{
	m_ValDAValue = 1000;
	if (m_ManualB)
		SetDrawBack();
}