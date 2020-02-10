//**********************************************************************
//*		说明：此工具箱包含了文件处理、socket初始化、临界区锁初始化、
//*			  钩子、字符集转换、某些系统公共函数的包装函数、IAT、
//*			  TCP/UDP连接函数等
//*		作者：PotalXu、CheLi
//*		日期：2012.01.01（修改整合于2016.01.11 by PotalXu）
//**********************************************************************
#pragma once

#include <windows.h>
#include <string>

namespace mqwUtils
{
//***********系统包装函数**************//
#ifdef WIN32
    std::string GetModulePath(HMODULE module, const char *f_name);        // 获取文件路径名
	HMODULE GetCurrentModuleHandle();                                       // 获得当前模块句柄包括dll模块
	bool IsModuleNameMatch(HMODULE module, char *pName);                         // 判断模块名
	int GetProcessorCount();					// 获取PC的CPU数量
	//guid类型:%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X
	std::string MakeGuid();

	std::string GetErrorMessage(DWORD dw);

	bool GetMacByGetAdaptersInfo(std::string& macOUT);
	bool GetMacByGetAdaptersAddresses(std::string& macOUT);
#endif

}
