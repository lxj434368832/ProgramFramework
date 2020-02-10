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

bool ModelManage::Initialize()
{
    if(false == m_mainModel->Initialize()) return false;

	return true;
}

void ModelManage::Uninitialize()
{
    m_mainModel->Uninitialize();
}

