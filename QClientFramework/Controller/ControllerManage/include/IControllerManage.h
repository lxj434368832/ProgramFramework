#pragma once

class IMainClient;
class IMainController;

class IControllerManage
{
public:
	static IControllerManage* Create(IMainClient *pMain);
	static void Delete(IControllerManage*& p);
    IControllerManage(IMainClient *_main = nullptr):m_pMain(_main){}
	virtual ~IControllerManage() { m_pMain = nullptr; }
	IMainClient* GetMainClient() { return m_pMain; }
	virtual IMainController* GetMainController() = 0;

    virtual bool Initialize() = 0;
    virtual void Uninitialize() = 0;

protected:
    IMainClient             *m_pMain = nullptr;
};

