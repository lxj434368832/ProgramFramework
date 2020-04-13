#include <stdio.h>
#include <windows.h>
#include <string>
#include "..\include\IOCPServer.h"
#include "..\include\IOCPClient.h"

#ifdef _DEBUG
#pragma comment(lib, "Framework/lib/Frameworkd.lib")
#else
#pragma comment(lib, "Framework/lib/Framework.lib")
#endif


int main()
{
	LogFile log;
	//IOCPServer srv;
	//srv.StartServer(6666, 10, 2);

	//getchar();
	//system("pause");
	//srv.StopServer();

	IOCPClient clt(nullptr);
	clt.StartClient();

	getchar();
	system("pause");
	clt.StopClient();

	return 0;
}
