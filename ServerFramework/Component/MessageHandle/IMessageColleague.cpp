#include "IMessageColleague.h"
#include "IMessageHandle.h"
#include "../../MainServer/IMainServer.h"
#include "../../Controller/ControllerManage/IControllerManage.h"
#include "../../CommonFile/CommonDefine.h"

IMessageColleague::IMessageColleague(IMessageHandle * pHandle)
{
	m_pMsgHandle = pHandle;

	m_pTcpCmmnt = nullptr;
	m_pMainCtrl = nullptr;
}

IMessageColleague::~IMessageColleague()
{
	m_pMsgHandle = nullptr;
}

bool IMessageColleague::Start()
{
	IMainServer*	pMain = m_pMsgHandle->GetMainServer();
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

void IMessageColleague::Stop()
{
	m_pTcpCmmnt = nullptr;
	m_pMainCtrl = nullptr;
}
