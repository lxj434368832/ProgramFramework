#include "ModelManage.h"
#include "MainModel.h"
#include "StatisticModel.h"
#include "../../CommonFile/CommonDefine.h"



ModelManage::ModelManage(IMainClient *_main)
	:IModelManage(_main)
{
    m_mainModel = new MainModel();
    m_statistic = new StatisticModel;
}

ModelManage::~ModelManage()
{
    RELEASE(m_mainModel);
    RELEASE(m_statistic);
}

MainModel *ModelManage::GetMainModel()
{
    return  m_mainModel;
}

StatisticModel *ModelManage::GetStatisticModel()
{
    return m_statistic;
}

bool ModelManage::Start()
{
    if(false == m_mainModel->Start()) return false;
    if(false == m_statistic->Start()) return false;

	return true;
}

void ModelManage::Stop()
{
    m_mainModel->Stop();
    m_statistic->Stop();
}

