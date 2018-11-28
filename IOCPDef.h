#pragma once
/*************************************************************************
* function：iocp公共定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  安碧捷科技股份有限公司
*************************************************************************/

#include "stdafx.h"

#define MAX_BUF_LEN 1024 //1K

enum EOperateType
{
	EOP_UNKNOWN,
	EOP_CNNT_CONNECT,
	EOP_CNNT_DISCONNECT,
	EOP_CNNT_RECEIVE,
	EOP_CNNT_SEND,
	EOP_CNNT_OPERATE,
	EOP_CLIENT_ACCEPT,
	EOP_CLIENT_DISCONNECT,
	EOP_CLIENT_RECEIVE,
	EOP_CLIENT_SEND,
	EOP_CLIENT_OPRATE,
	EOP_SRV_ACCEPT,
	EOP_SRV_DISCONNECT,
	EOP_SRV_RECEIVE,
	EOP_SRV_SEND,
	EOP_SRV_OPERATE
};

#pragma pack(push,1)
struct PackHeader
{
	unsigned long ulBodyLength;
	int			  msgType;
};
#pragma pack(pop)

struct IOContext
{

	OVERLAPPED overlapped;
	SOCKET socket;
	EOperateType oprateType;
	unsigned uUserId;
	WSABUF wsabuf;
	char* buffer;
	unsigned bufLength;
	unsigned dataLength;
	WPARAM wParam;			// 扩展参数
	LPARAM lParam;			// 扩展参数2
	IOContext()
	{
		socket = INVALID_SOCKET;
		buffer = new char[MAX_BUF_LEN];
		bufLength = MAX_BUF_LEN;
		reset();
	}
	~IOContext()
	{
		delete[] buffer;
		RELEASE_SOCKET(socket);		//释放socket资源，可能不必要
	}
	void reset()
	{
		oprateType = EOP_UNKNOWN;
		uUserId = 0;
		wsabuf.buf = buffer;
		wsabuf.len = bufLength;
		dataLength = 0;
		wParam = 0;
		lParam = 0;
	}
};

