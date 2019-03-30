#pragma once

#include <string>

typedef unsigned UserKey;

struct ClientConfig
{
	std::string		strServerIP = "";				//连接的IP
	unsigned short  usServerPort = 6666;			//连接端口
	unsigned int    uIOCPThreadCount = 4;			// 完成端口监听工作线程数量
	unsigned int    uMessageThreadCount = 4;		// 消息处理线程数量
	unsigned int    uHeartbeatTime = 6000;			// 定时心跳发送时间ms

	// 需要判断读取出来的配置是否正确
    bool CheckValid();
};
