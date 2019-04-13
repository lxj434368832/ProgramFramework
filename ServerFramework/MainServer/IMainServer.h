#pragma once

class IMessageBusiness;
class IModelManage;
class ICommunication;
struct ServerConfig;

class IMainServer
{
public:
	virtual ~IMainServer(){}
	virtual IMessageBusiness* GetMessageBusiness() = 0;
	virtual IModelManage* GetManageBusiness() = 0;
	virtual ICommunication* GetCommunication() = 0;
	virtual ServerConfig* GetServerConfig() = 0;

	virtual bool Start() = 0;
	virtual void Stop() = 0;
private:

};
