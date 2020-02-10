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
		loge()<<"获取通讯模块失败！";
			return false;
	}

	IControllerManage	*pCtrlMng = pMain->GetControllerManage();
	if (nullptr == pCtrlMng) 
	{
		loge() << "获取控制管理器失败！";
		return false;
	}
	if (nullptr == (m_pMainCtrl = pCtrlMng->GetMainController()))
	{
		loge() << "获取主控制器失败！";
		return false;
	}

	return true;
}

void HandleMessageColleague::Uninitialize()
{
	m_pTcpCmmnt = nullptr;
	m_pMainCtrl = nullptr;
}
