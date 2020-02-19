// ServerFramework.cpp : 定义控制台应用程序的入口点。
//
#include <vld.h>
#include "MainServer.h"
#include "ImportLibrary.h"
#include "../CommonFile/CommonDefine.h"

#define SZAPPNAME "ServerFramework"

int main(int argc, char *argv[])
{
	LogFile log;
	IMainServer *pMain = new MainServer;
	do 
	{
		char strCmd[256];
		//printf("%s -install /-i          to install the service\n", SZAPPNAME);
		//printf("%s -remove /-r           to remove the service\n", SZAPPNAME);
		//printf("%s -debug /-d <params>   to run as a console app for debugging\n", SZAPPNAME);
		printf("%s -start /-s            to startup the service\n", SZAPPNAME);
		printf("%s -stop /-p             to stop the service\n", SZAPPNAME);
		printf("%s -quit /-q             to quit the service\n", SZAPPNAME);
		
		scanf_s("%s", strCmd, 256);
		if (_stricmp("-start", strCmd) == 0 || _stricmp("-s", strCmd) == 0)
		{
			if (false == pMain->StartServer())
			{
				loge() << "服务开启失败，请查看日志！";
			}
		}
		else if (_stricmp("-stop", strCmd) == 0 || _stricmp("-p", strCmd) == 0)
		{
			pMain->StopServer();
		}
		else if (_stricmp("-quit", strCmd) == 0 || _stricmp("-q", strCmd) == 0)
		{
			break;
		}

	} while (true);
	delete pMain;
	system("pause");
    return 0;
}

