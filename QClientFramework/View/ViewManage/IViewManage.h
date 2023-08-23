#pragma once

class IMainClient;

class IViewManage
{
public:
	static IViewManage* Create(IMainClient *pMain);
	static void Delete(IViewManage*& p);
    IViewManage(IMainClient *_main = nullptr) :m_pMain(_main) {}
    virtual ~IViewManage() { m_pMain = nullptr; }

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;

	virtual bool StartClient() = 0;

protected:
	IMainClient *m_pMain = nullptr;
};
