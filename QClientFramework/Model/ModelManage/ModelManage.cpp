#include "ModelManage.h"
#include "MainModel.h"
#include "../../CommonFile/CommonDefine.h"



ModelManage::ModelManage(IMainClient *_main)
	:IModelManage(_main)
{
    m_mainModel = new MainModel();
}

ModelManage::~ModelManage()
{
    RELEASE(m_mainModel);
}

MainModel *ModelManage::GetMainModel()
{
    return  m_mainModel;
}

bool ModelManage::Start()
{
    if(false == m_mainModel->Start()) return false;

	return true;
}

void ModelManage::Stop()
{
    m_mainModel->Stop();
}

