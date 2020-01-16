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

namespace Utils
{
//***********系统包装函数**************//
#ifdef WIN32
    std::string get_module_path(HMODULE module, const char *f_name);        // 获取文件路径名
	HMODULE get_self_module_handle();                                       // 获得当前模块句柄包括dll模块
	bool is_process_a(HMODULE module, char *pname);                         // 判断模块名
	int get_num_of_processors();					// 获取PC的CPU数量
	//guid类型:%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X
	std::string make_guid();

	std::string GetErrorMessage(DWORD dw);

	bool GetMacByGetAdaptersInfo(std::string& macOUT);
	bool GetMacByGetAdaptersAddresses(std::string& macOUT);

#endif
    bool is_time(time_t ms, time_t dms);           // 是否大于时间
    bool is_time_v(time_t &ms, time_t dms);        // 大于时间修改

}
