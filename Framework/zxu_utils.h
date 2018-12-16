//**********************************************************************
//*		说明：此工具箱包含了文件处理、socket初始化、临界区锁初始化、
//*			  钩子、字符集转换、某些系统公共函数的包装函数、IAT、
//*			  TCP/UDP连接函数等
//*		作者：PotalXu、CheLi
//*		日期：2012.01.01（修改整合于2016.01.11 by PotalXu）
//**********************************************************************
#pragma once

#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include <vector>
#include <cstdarg>          // 定义了va_list相关

#ifdef WIN32
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#endif

#ifndef SOCKET_ERROR                      
#define SOCKET_ERROR (-1)
#endif


#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
typedef int     sock_fd;
#else
typedef int     sock_fd;
#endif
#else
typedef int     sock_fd;
#endif

namespace zxu
{
	class log_files;    	    // 文件类
	class init_socket;      // socket初始化类
    class winapi;           // 
    class sockets;          // socket相关函数
	class hook;             // HOOK类
	class charset;          // 字符类
    class cfg_reg_reader;   // 配置文件和注册表读取类

//***********系统包装函数**************//
#ifdef WIN32
    std::string get_module_path(HMODULE module, const char *f_name);        // 获取文件路径名
	HMODULE get_self_module_handle();                                       // 获得当前模块句柄包括dll模块
	bool is_process_a(HMODULE module, char *pname);                         // 判断模块名
	int get_num_of_processors();					// 获取PC的CPU数量
	//guid类型:%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X
	std::string make_guid();
#endif
    bool is_time(time_t ms, time_t dms);                                    // 是否大于时间
    bool is_time_v(time_t &ms, time_t dms);                                 // 大于时间修改

}
