#pragma once

class IMainClient;

class IViewManage
{
public:
    IViewManage(IMainClient *_main = nullptr) :m_main(_main) {}
    virtual ~IViewManage() { m_main = nullptr; }

	virtual bool Start() = 0;
	virtual void Stop() = 0;

protected:
	IMainClient *m_main = nullptr;
};
