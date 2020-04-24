#pragma once

class IMainClient;
class MainController;

class IControllerManage
{
public:
    IControllerManage(IMainClient *_main = nullptr):m_pMain(_main){}
	virtual ~IControllerManage() { m_pMain = nullptr; }
	IMainClient* GetMainClient() { return m_pMain; }
	virtual MainController* GetMainController() = 0;

    virtual bool Initialize() = 0;
    virtual void Uninitialize() = 0;

protected:
    IMainClient             *m_pMain = nullptr;
};

