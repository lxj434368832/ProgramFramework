#pragma once
/************************************************************************
* function：使用服务的程序接口
* author :	明巧文
* datetime：2018-04-12
* company:  安碧捷科技股份有限公司
/************************************************************************/

#include <tchar.h>
#include <windows.h>

__interface IService
{
	void Start(DWORD argc, TCHAR* argv[]);
	void Stop();
};
