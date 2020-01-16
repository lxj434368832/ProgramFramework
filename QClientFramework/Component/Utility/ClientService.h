#pragma once

#include <string>
#include <QString>

class ClientService
{
public:
	ClientService();
	~ClientService();

	static void InstallAppCrashHandler();
	static bool IsApplicationRun(std::string strAppName);
	static QString GetAppVersion();
};

