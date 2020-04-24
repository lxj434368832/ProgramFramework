#include "ModelManage.h"
#include "MainModel.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../Component/TCPCommunication/Communication.h"
#include "..\..\Component\MessageHandle\MessageHandle.h"


ModelManage::ModelManage(IMainClient *_main)
	:IModelManage(_main)
{
	m_pCmmnt = new Communication(this);
	m_pMsgHandle = new MessageHandle(this);

    m_pMainModel = new MainModel();
}

ModelManage::~ModelManage()
{
	RELEASE(m_pMsgHandle);
	RELEASE(m_pCmmnt);
    RELEASE(m_pMainModel);
}

MainModel *ModelManage::GetMainModel()
{
    return  m_pMainModel;
}

ICommunication* ModelManage::GetCommunication()
{
	return m_pCmmnt;
}

IMessageHandle* ModelManage::GetMessageHandle()
{
	return m_pMsgHandle;
}

bool ModelManage::Initialize()
{
	if (false == m_pCmmnt->Initialize()) return false;
	if (false == m_pMsgHandle->Initialize()) return false;
    if(false == m_pMainModel->Initialize()) return false;

	return true;
}

void ModelManage::Uninitialize()
{
	m_pMainModel->Uninitialize();
	m_pMsgHandle->Uninitialize();
	m_pCmmnt->Uninitialize();
}

