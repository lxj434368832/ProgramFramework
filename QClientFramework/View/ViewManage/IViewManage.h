#pragma once

class IMainClient;

class IViewManage
{
public:
    IViewManage(IMainClient *_main = nullptr) :m_pMain(_main) {}
    virtual ~IViewManage() { m_pMain = nullptr; }

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;

	virtual bool LoginWebServer() = 0;

protected:
	IMainClient *m_pMain = nullptr;
};
