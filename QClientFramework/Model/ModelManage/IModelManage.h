#pragma once

class IMainClient;

class IModelManage
{
public:
    IModelManage(IMainClient *_main = nullptr) :m_main(_main) {}
    virtual ~IModelManage() { m_main = nullptr; }

	virtual bool Start() = 0;
	virtual void Stop() = 0;

private:

	IMainClient *m_main = nullptr;
};
