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

#ifndef LOGS
#define LOGS

#define MLOG(format, ...)  do{  \
	printf("FKLog msg: function:%s line:%d",__FUNCTION__,__LINE__);  \
	printf(format, ##__VA_ARGS__); printf("\n"); }while(0)
#define MAssert(express) {if(!express){char a[1];a[-0xffff];} }

#define Mcout std::cout<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"
#define Mcerr std::cerr<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"

#endif // !LOGS

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
#define RELEASE_SOCKET(x) {if(x !=INVALID_SOCKET) { ::shutdown(x,2);::closesocket(x);x = INVALID_SOCKET;}}
#endif
