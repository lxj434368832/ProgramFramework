#ifndef SERVICE_INSTALLER_H_  
#define SERVICE_INSTALLER_H_  
/************************************************************************
* function：服务相关操作，包括安装、卸载、开始、停止功能
* author :	明巧文
* datetime：2018-04-12
* company:  安碧捷科技股份有限公司
/************************************************************************/

#include "ServiceBase.h"  

class ServiceOperate 
{
public:
	static bool Install(LPCTSTR lpServiceName, LPCTSTR lpSrvPath, DWORD dwStartType, DWORD dwErrorControlType);
	static bool Uninstall(LPCTSTR lpServiceName);
	static BOOL IsInstalled(LPCTSTR lpServiceName);
	static bool Start(LPCTSTR lpServiceName);
	static bool Stop(LPCTSTR lpServiceName);
	static void ReconfigureService(LPSTR lpServiceName, LPSTR lpDesc);

private:
	ServiceOperate() {}
	static BOOL StopDependentServices(SC_HANDLE schSCManager, SC_HANDLE schService);
};

#endif // SERVICE_INSTALLER_H_