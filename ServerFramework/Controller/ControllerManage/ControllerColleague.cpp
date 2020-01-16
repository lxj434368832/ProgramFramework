#include "ControllerColleague.h"
#include "IControllerManage.h"
#include "../../MainServer/IMainServer.h"
#include "../../Component/MessageHandle/IMessageHandle.h"

ControllerColleague::ControllerColleague(IControllerManage *pCtrlMng)
{
	m_pMain = pCtrlMng->GetMainClient();
	m_pCtrlMng = pCtrlMng;
	m_pModelMng = m_pMain->GetModelManage();

	IMessageHandle	*pMsgHandle = m_pMain->GetMessageHandle();
	m_pHandleRqMsg = pMsgHandle->GetHandleRequestMessage();
	m_pHandleRsMsg = pMsgHandle->GetHandleRespondMessage();
	m_pHandleNtMsg = pMsgHandle->GetHandleNotifyMessage();
}

ControllerColleague::~ControllerColleague()
{
    m_pMain = nullptr;
    m_pCtrlMng = nullptr;
	m_pModelMng = nullptr;

	m_pTcpCmmnt = nullptr;
	m_pHandleRqMsg = nullptr;
	m_pHandleRsMsg = nullptr;
	m_pHandleNtMsg = nullptr;
}
