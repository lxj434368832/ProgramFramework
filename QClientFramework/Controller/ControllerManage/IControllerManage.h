#pragma once

class IMainClient;
class MainController;
class StatisticController;

class IControllerManage
{
public:
    IControllerManage(IMainClient *_main = nullptr):m_main(_main){}
    virtual ~IControllerManage() { m_main = nullptr; }
    virtual MainController* GetMainController() = 0;
    virtual StatisticController* GetStatisticController() = 0;

    virtual bool Start() = 0;
    virtual void Stop() = 0;

protected:
    IMainClient             *m_main = nullptr;
};

