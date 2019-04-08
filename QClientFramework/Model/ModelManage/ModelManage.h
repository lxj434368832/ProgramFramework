#pragma once
/************************************************************************
* filename:ModelManage.h
* function: 模型管理器，用于管理各个模型
* author :	明巧文
* datetime:2018-12-14
* company:  安碧捷科技股份有限公司
/************************************************************************/

#include "IModelManage.h"

class ModelManage : public IModelManage
{
public:
    ModelManage(IMainClient *_main = nullptr);
    ~ModelManage() override;
    StatisticModel* GetStatisticModel() override;
    MainModel* GetMainModel() override;

	bool Start() override;
	void Stop() override;

private:
    MainModel           *m_mainModel;
    StatisticModel      *m_statistic;
};

