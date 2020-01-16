#pragma once

class IMainServer;
class MainController;

class IControllerManage
{
public:
    IControllerManage(IMainServer *_main = nullptr):m_pMain(_main){}
	virtual ~IControllerManage() { m_pMain = nullptr; }
	IMainServer* GetMainClient() { return m_pMain; }

    virtual MainController* GetMainController() = 0;

    virtual bool Start() = 0;
    virtual void Stop() = 0;

protected:
    IMainServer             *m_pMain = nullptr;
};

