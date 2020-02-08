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

    virtual bool Initialize() = 0;
    virtual void Uninitialize() = 0;

protected:
    IMainServer             *m_pMain = nullptr;
};

