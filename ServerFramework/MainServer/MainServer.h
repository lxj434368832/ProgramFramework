#pragma once
#include <string>
#include "IMainServer.h"

struct ServerConfig
{
	unsigned short  usListenPort		= 6666;         // 监听端口
	unsigned int    uServerThreadCount	= 4;			// 完成端口监听工作线程数量
	unsigned int    uMessageThreadCount = 4;			// 消息处理线程数量
	unsigned int    uInitAcceptCount	= 10;           // 服务端初始接受个数
	unsigned int    uHeartbeatTime		= 6000;			// 定时心跳检测时间ms

	// 需要判断读取出来的配置是否正确
	bool CheckValid()
	{
		if (usListenPort == 0
			|| uInitAcceptCount == 0
			|| uServerThreadCount == 0
			|| uHeartbeatTime == 0)
		{
			return false;
		}
		return true;
	}

};

class MainServer : public IMainServer
{
public:
	MainServer();
	~MainServer();
	IMessageBusiness* GetMessageBusiness() override;
	IModelManage* GetManageBusiness() override;
	ICommunication* GetCommunication() override;
	ServerConfig* GetServerConfig() override;


	bool Start();
	void Stop();

private:
	bool ReadConfigFile();

private:
	ServerConfig		m_srvConfig;		//服务配置项
	IMessageBusiness*	m_pMessage;			//消息模块
	IModelManage*	m_pManage;			//管理模块
	ICommunication*		m_pCommunication;	//通讯模块
};

