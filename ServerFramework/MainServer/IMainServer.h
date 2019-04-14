#pragma once

struct ServerConfig;
class IModelManage;
class IControllerManage;
class IMessageHandle;
class ITCPCommunication;

class IMainServer
{
public:
	virtual ~IMainServer(){}
	virtual ServerConfig* GetServerConfig() = 0;
	virtual IModelManage* GetModelManage() = 0;
	virtual IControllerManage* GetControllerManage() = 0;
	virtual IMessageHandle* GetMessageBusiness() = 0;
	virtual ITCPCommunication* GetCommunication() = 0;

	virtual bool Start() = 0;
	virtual void Stop() = 0;
private:

};
