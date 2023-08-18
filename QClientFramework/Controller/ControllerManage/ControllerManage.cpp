#include "ControllerManage.h"
#include "MainController.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../Model/ModelManage/ModelManage.h"
#include "../../MainClient/IMainClient.h"
#include "../../Component/Communication/Communication.h"
#include "..\..\Component\MessageHandle\MessageHandle.h"


ControllerManage::ControllerManage(IMainClient *_main) :
    IControllerManage(_main)
{
	m_pCmmnt = new Communication(m_pMain);
	m_pMsgHandle = new MessageHandle(m_pMain);
    m_pMainCtrl = new MainController(this);
}

ControllerManage::~ControllerManage()
{
	RELEASE(m_pMainCtrl);
	RELEASE(m_pMsgHandle);
	RELEASE(m_pCmmnt);
}

bool ControllerManage::Initialize()
{
	ClientConfig* cfg = m_pMain->GetClientConfig();
	if (nullptr == cfg)
	{
		loge() << "ÅäÖÃÏîÎª¿Õ£¡";
		return false;
	}

	if (false == m_pCmmnt->Initialize()) return false;
	if (false == m_pMsgHandle->Initialize()) return false;
	if (false == m_pMainCtrl->Initialize()) return false;

	return true;
}

void ControllerManage::Uninitialize()
{
	m_pMainCtrl->Uninitialize();
	m_pMsgHandle->Uninitialize();
	m_pCmmnt->Uninitialize();
}

MainController *ControllerManage::GetMainController()
{
    return m_pMainCtrl;
}

ICommunication* ControllerManage::GetCommunication()
{
	return m_pCmmnt;
}

IMessageHandle* ControllerManage::GetMessageHandle()
{
	return m_pMsgHandle;
}
