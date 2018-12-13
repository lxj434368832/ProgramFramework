// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include <stdio.h>
#include <iostream>
#include <WinSock2.h>
#include <windows.h>


// TODO:  在此处引用程序需要的其他头文件
#include "../Framework/zxl_logging.h"

//#ifdef _DEBUG
//#pragma comment(lib,"../../common/lib/protobuf/Debug/libprotobuf.lib")
//#else
//#pragma comment(lib,"../../common/lib/protobuf/Release/libprotobuf.lib")
//#endif

#ifndef LOGS
#define LOGS
#define MLOG(format, ...)  do{  \
	printf("MLog msg: function:%s line:%d",__FUNCTION__,__LINE__);  \
	printf(format, ##__VA_ARGS__); printf("\n"); }while(0)

#define MAssert(express) {if(!express){char a[1];a[-0xffff];} }

#define Mcout std::cout<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"
#define Mcerr std::cerr<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"
#endif // !LOGS

#ifndef ZX_LOGGING_H_
static HANDLE consolehwnd = ::GetStdHandle(STD_OUTPUT_HANDLE);
#define logm() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[O]:"
#define loge() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[E]:"
#define logd() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[D]:"
#define logt() ::SetConsoleTextAttribute(consolehwnd, 4);std::cout<<"[T]:"<<::GetTickCount64()
//#define logt() ::system("color 4");std::cout<<"[T]:"
#else
#define logm() LogInfo()<<"["<<__FUNCTION__<<":"<<__LINE__<<"|<message>]"<<std::endl
#define loge() LogInfo()<<"["<<__FUNCTION__<<":"<<__LINE__<<"|<error>]"<<std::endl
#define logd() LogInfo()<<"["<<__FUNCTION__<<":"<<__LINE__<<"|<debug>]"<<std::endl
#define logt() LogInfo()<<"["<<__FUNCTION__<<":"<<__LINE__<<"|<trace>]"<<std::endl
#endif

// 释放指针宏
#ifndef RELEASE
#define RELEASE(x) {if(x!=NULL){delete x;x=NULL;}}
#endif

// 释放句柄宏
#ifndef RELEASE_HANDLE
#define RELEASE_HANDLE(x) {if(x != NULL && x != INVALID_HANDLE_VALUE){::CloseHandle(x);x = NULL;}}
#endif

// 释放Socket宏
#ifndef RELEASE_SOCKET
#define RELEASE_SOCKET(x) {if(x !=INVALID_SOCKET) { ::closesocket(x);x = INVALID_SOCKET;}}
#endif
