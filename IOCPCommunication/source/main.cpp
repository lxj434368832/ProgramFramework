#include <stdio.h>
#include <windows.h>
#include <string>
#include "..\include\IOCPServer.h"

#ifdef _DEBUG
#pragma comment(lib, "Framework/lib/Frameworkd.lib")
#else
#pragma comment(lib, "Framework/lib/Framework.lib")
#endif


int main()
{
	IOCPServer srv;
	srv.StartServer(6666, 10, 2);

	getchar();
	system("pause");
	srv.StopServer();
	return 0;
}
