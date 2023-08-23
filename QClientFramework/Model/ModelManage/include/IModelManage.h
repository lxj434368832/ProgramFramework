#pragma once

class IMainClient;
class IServerConnect;
class IMessageHandle;

class IModelManage
{
public:
	static IModelManage* Create(IMainClient *pMain);
	static void Delete(IModelManage*& p);
	IModelManage(IMainClient *_main = nullptr) :m_pMain(_main) {}
	virtual ~IModelManage() { m_pMain = nullptr; }
	IMainClient* GetMainClient() { return m_pMain; }
	virtual IServerConnect* GetServerConnect() = 0;
	virtual IMessageHandle* GetPbMessageHandle() = 0;

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;

protected:
	IMainClient *m_pMain = nullptr;
};
