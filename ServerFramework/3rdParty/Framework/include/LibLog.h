#pragma once
/*******************************************************************
* @版权信息：
* @文件名称：Log.h
* @摘    要：日志操作定义头文件
* @作    者：明巧文
* @当前版本：1.0.0
* @日    期：2015年4月2日
* @备    注：线程安全日志类
*******************************************************************/

#include <stdio.h>
#include <string>

#define LOG_BUFFER_SIZE 8 * 1024 * 1024	//8M

std::string GetAppDir();

//写日志函数
void LOG(const char * szLibName, char* format, ...);


#define LOGM(format, ...)  do{  \
	char szFunInfo[256];	\
	sprintf_s(szFunInfo,"[%s:%d]<Info> %s\n",__FUNCTION__,__LINE__, format);  \
	LOG(LIB_NAME, szFunInfo, ##__VA_ARGS__); }while(0)

#define LOGD(format, ...)  do{  \
	char szFunInfo[256];	\
	sprintf_s(szFunInfo,"[%s:%d]<Debug> %s\n",__FUNCTION__,__LINE__, format);  \
	LOG(LIB_NAME, szFunInfo, ##__VA_ARGS__); }while(0)

#define LOGW(format, ...)  do{  \
	char szFunInfo[256];	\
	sprintf_s(szFunInfo,"[%s:%d]<Warn> %s\n",__FUNCTION__,__LINE__, format);  \
	LOG(LIB_NAME, szFunInfo, ##__VA_ARGS__); }while(0)

#define LOGE(format, ...)  do{  \
	char szFunInfo[256];	\
	sprintf_s(szFunInfo,"[%s:%d]<Error> %s\n",__FUNCTION__,__LINE__, format);  \
	LOG(LIB_NAME, szFunInfo, ##__VA_ARGS__); }while(0)


/*以下放入CommonDefine前面,定义LIB_NAME为具体的模块名
#include "LibLog.h"
#define LIB_NAME "LogTest"
*/