#include "ControllerColleague.h"
#include "IControllerManage.h"
#include "../../MainClient/IMainClient.h"
#include "ControllerManage.h"
#include "../../Component/MessageHandle/IMessageHandle.h"

ControllerColleague::ControllerColleague(ControllerManage *pCtrlMng, QObject *parent)
    :QObject(parent)
{
    m_pMain = pCtrlMng->GetMainClient();
	m_pCtrlMng = pCtrlMng;
    m_pModel = m_pMain->GetModelInterface();
	m_pTcpCmmnt = m_pMain->GetTCPCommunication();

	IMessageHandle	*pMsgHandle = m_pMain->GetMessageHandle();
	m_pHandleRqMsg = pMsgHandle->GetHandleRequestMessage();
	m_pHandleRsMsg = pMsgHandle->GetHandleRespondMessage();
	m_pHandleNtMsg = pMsgHandle->GetHandleNotifyMessage();
}

ControllerColleague::~ControllerColleague()
{
    m_pMain = nullptr;
	m_pCtrlMng = nullptr;
    m_pModel = nullptr;

	m_pTcpCmmnt = nullptr;
	m_pHandleRqMsg = nullptr;
	m_pHandleRsMsg = nullptr;
	m_pHandleNtMsg = nullptr;
}
