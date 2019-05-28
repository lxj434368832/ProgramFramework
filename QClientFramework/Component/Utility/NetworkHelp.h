#pragma once

#include <QString>

class NetworkHelp
{
public:
	NetworkHelp();
	~NetworkHelp();

	// 获得ip地址  
	static QString GetIpAdress();
	// 获得用户名  
	static QString GetUserName();
	// 获得机器名字  
	static QString GetMachineName();
    // 获得MAC地址
	static QString GetMACAdress();
};

