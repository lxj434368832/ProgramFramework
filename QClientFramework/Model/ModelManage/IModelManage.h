#pragma once

class IMainClient;
class StatisticModel;
class MainModel;

class IModelManage
{
public:
    IModelManage(IMainClient *_main = nullptr) :m_main(_main) {}
    virtual ~IModelManage() { m_main = nullptr; }
    virtual StatisticModel* GetStatisticModel() = 0;
    virtual MainModel* GetMainModel() = 0;

	virtual bool Start() = 0;
	virtual void Stop() = 0;

protected:
	IMainClient *m_main = nullptr;
};
