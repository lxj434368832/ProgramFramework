#pragma once

struct ClientConfig;
class IControllerManage;
class IViewManage;
class IModelManage;
class ITCPCommunication;
class IMessage;

class IMainClient
{
public:
	virtual ~IMainClient(){}
	virtual ClientConfig* GetClientConfig() = 0;
	virtual IViewManage* GetViewInterface() = 0;
	virtual IControllerManage* GetLogicInterface() = 0;
	virtual IModelManage* GetModelInterface() = 0;

	virtual ITCPCommunication* GetMainCommunication() = 0;
	virtual IMessage*	GetMessageModule() = 0;

	virtual bool Start() = 0;
	virtual void Stop() = 0;

private:

};
