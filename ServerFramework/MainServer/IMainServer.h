#pragma once

struct SServerConfig;
class IModelManage;
class IControllerManage;
class IMessageHandle;
class ICommunication;

class IMainServer
{
public:
	virtual ~IMainServer(){}
	virtual SServerConfig* GetServerConfig() = 0;
	virtual IModelManage* GetModelManage() = 0;
	virtual IControllerManage* GetControllerManage() = 0;
	virtual IMessageHandle* GetMessageHandle() = 0;
	virtual ICommunication* GetCommunication() = 0;

	virtual bool StartServer() = 0;
	virtual void StopServer() = 0;
private:

};
