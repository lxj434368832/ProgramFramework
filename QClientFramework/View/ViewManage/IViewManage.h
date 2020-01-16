#pragma once

class IMainClient;

class IViewManage
{
public:
    IViewManage(IMainClient *_main = nullptr) :m_pMain(_main) {}
    virtual ~IViewManage() { m_pMain = nullptr; }

	virtual bool Start() = 0;
	virtual void Stop() = 0;

	virtual bool ExecuteLogin() = 0;

protected:
	IMainClient *m_pMain = nullptr;
};
