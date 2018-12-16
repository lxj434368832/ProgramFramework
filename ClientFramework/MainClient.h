#pragma once

#include <string>

struct ServerConfig
{
	// 需要判断读取出来的配置是否正确
	bool CheckValid()
	{
		if (strServerIP.empty()
			|| usServerPort == 0
			|| uInitAcceptCount == 0
			|| uServerThreadCount == 0
			|| uHeartbeatTime == 0)
		{
			return false;
		}
		return true;
	}

	std::string		strServerIP = "";				//连接的IP
	unsigned short  usServerPort = 6666;			//连接端口
	unsigned int    uServerThreadCount = 4;			// 完成端口监听工作线程数量
	unsigned int    uMessageThreadCount = 4;		// 消息处理线程数量
	unsigned int    uInitAcceptCount = 10;          // 服务端初始接受个数
	unsigned int    uHeartbeatTime = 6000;			// 定时心跳发送时间ms
};

class IOCPClient;
class INetInterface;

class MainClient
{
public:
	MainClient();
	~MainClient();
	bool Start();
	bool Stop();

private:
	bool ReadConfigFile();

private:
	ServerConfig	m_srvConfig;	//服务配置项
	INetInterface	*m_pClientList;	//客户端列表
	IOCPClient		*m_pIOCPClient;	//IOCP 客户端
};

