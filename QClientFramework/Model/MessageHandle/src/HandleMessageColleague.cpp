#include "HandleMessageColleague.h"
#include "IMessageHandle.h"
#include "../../MainClient/IMainClient.h"
#include "../../Controller/ControllerManage/IControllerManage.h"
#include "../../CommonFile/CommonDefine.h"

HandleMessageColleague::HandleMessageColleague(IMessageHandle * pHandle)
{
	m_pMsgHandle = pHandle;

	m_pTcpCmmnt = nullptr;
	m_pMainCtrl = nullptr;
}

HandleMessageColleague::~HandleMessageColleague()
{
	m_pMsgHandle = nullptr;
}

bool HandleMessageColleague::Initialize()
{
	IMainClient*	pMain = m_pMsgHandle->GetMainClient();
	if (nullptr == (m_pTcpCmmnt = pMain->GetCommunication()))
	{
		loge()<<"��ȡͨѶģ��ʧ�ܣ�";
			return false;
	}

	IControllerManage	*pCtrlMng = pMain->GetControllerManage();
	if (nullptr == pCtrlMng) 
	{
		loge() << "��ȡ���ƹ�����ʧ�ܣ�";
		return false;
	}
	if (nullptr == (m_pMainCtrl = pCtrlMng->GetMainController()))
	{
		loge() << "��ȡ��������ʧ�ܣ�";
		return false;
	}

	return true;
}

void HandleMessageColleague::Uninitialize()
{
	m_pTcpCmmnt = nullptr;
	m_pMainCtrl = nullptr;
}
