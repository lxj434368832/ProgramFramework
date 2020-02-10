#pragma once

class IMainClient;
class MainModel;

class IModelManage
{
public:
    IModelManage(IMainClient *_main = nullptr) :m_main(_main) {}
    virtual ~IModelManage() { m_main = nullptr; }
    virtual MainModel* GetMainModel() = 0;

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;

protected:
	IMainClient *m_main = nullptr;
};
