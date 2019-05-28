#pragma once

struct ClientConfig;
class IControllerManage;
class IViewManage;
class IModelManage;
class ITCPCommunication;
class IMessageHandle;

class IMainClient
{
public:
	virtual ~IMainClient(){}
	virtual ClientConfig* GetClientConfig() = 0;
	virtual IViewManage* GetViewInterface() = 0;
	virtual IControllerManage* GetControllerInterface() = 0;
	virtual IModelManage* GetModelInterface() = 0;

	virtual ITCPCommunication* GetTCPCommunication() = 0;
	virtual IMessageHandle*	GetMessageHandle() = 0;

	virtual bool Start() = 0;
	virtual void Stop() = 0;

private:

};
