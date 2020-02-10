#pragma once

struct ClientConfig;
class IControllerManage;
class IViewManage;
class IModelManage;
class ICommunication;
class IMessageHandle;
class MConfigManage;

class IMainClient
{
public:
	virtual ~IMainClient(){}
	virtual ClientConfig* GetClientConfig() = 0;
	virtual MConfigManage* GetConfigManage() = 0;
	virtual IModelManage* GetModelManage() = 0;
	virtual IControllerManage* GetControllerManage() = 0;
	virtual IViewManage* GetViewManage() = 0;

	virtual ICommunication* GetCommunication() = 0;
	virtual IMessageHandle*	GetMessageHandle() = 0;

	virtual bool StartClient() = 0;
	virtual void StopClient() = 0;

private:

};
