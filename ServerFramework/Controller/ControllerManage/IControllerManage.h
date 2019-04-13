#pragma once

class IMainServer;
class MainController;

class IControllerManage
{
public:
    IControllerManage(IMainServer *_main = nullptr):m_main(_main){}
    virtual ~IControllerManage() { m_main = nullptr; }
    virtual MainController* GetMainController() = 0;

    virtual bool Start() = 0;
    virtual void Stop() = 0;

protected:
    IMainServer             *m_main = nullptr;
};

