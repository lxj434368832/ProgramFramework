#pragma once

struct ClientConfig;
class ILogicInterface;
class IViewInterface;
class IModelInterface;
class ICommunication;
class IMessage;

class IMainClient
{
public:
	virtual ~IMainClient(){}
	virtual ClientConfig* GetClientConfig() = 0;
	virtual IViewInterface* GetViewInterface() = 0;
	virtual ILogicInterface* GetLogicInterface() = 0;
	virtual IModelInterface* GetModelInterface() = 0;

	virtual ICommunication* GetMainCommunication() = 0;
	virtual IMessage*	GetMessageModule() = 0;

	virtual bool Start() = 0;
	virtual void Stop() = 0;

private:

};
