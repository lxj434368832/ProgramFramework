#include "ModelManage.h"
#include "../../MessageHandle/include/IMessageHandle.h"
#include "../../Communication/include/IServerConnect.h"
#include "../../../MainClient/IMainClient.h"
#include "../../../CommonFile/CommonDefine.h"
#include "../../../CommonFile/TypeDefine.h"

IModelManage* IModelManage::Create(IMainClient *pMain)
{
	return new ModelManage(pMain);
}

void IModelManage::Delete(IModelManage*& p)
{
	RELEASE(p);
}


ModelManage::ModelManage(IMainClient *_main)
	:IModelManage(_main)
{
	m_pPbMsgHandle = IMessageHandle::CreatePb();
	m_pConnect = IServerConnect::Create();
}

ModelManage::~ModelManage()
{
	IServerConnect::Delete(m_pConnect);
	IMessageHandle::Delete(m_pPbMsgHandle);
}

IServerConnect* ModelManage::GetServerConnect()
{
	return m_pConnect;
}

IMessageHandle* ModelManage::GetPbMessageHandle()
{
	return m_pPbMsgHandle;
}

bool ModelManage::Initialize()
{
	logm() << "************开始消息处理的初始化************";
	auto pCfg = m_pMain->GetClientConfig();
	if (false == m_pPbMsgHandle->Initialize(pCfg->uMessageThreadCount)) return false;
	if (false == m_pConnect->Initialize(m_pPbMsgHandle, pCfg->uServerThreadCount)) return false;

	return true;
}

void ModelManage::Uninitialize()
{
	m_pConnect->Uninitialize();
	m_pPbMsgHandle->Uninitialize();
}
