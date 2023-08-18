#include "ModelManage.h"
#include "MainModel.h"
#include "../../CommonFile/CommonDefine.h"


ModelManage::ModelManage(IMainClient *_main)
	:IModelManage(_main)
{
    m_pMainModel = new MainModel(this);
}

ModelManage::~ModelManage()
{
    RELEASE(m_pMainModel);
}

MainModel *ModelManage::GetMainModel()
{
    return  m_pMainModel;
}

bool ModelManage::Initialize()
{
    if(false == m_pMainModel->Initialize()) return false;

	return true;
}

void ModelManage::Uninitialize()
{
	m_pMainModel->Uninitialize();
}
