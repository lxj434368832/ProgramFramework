#pragma once

#define MESSAGE_RESOURCE_COUNT 500
#define USER_SHARE_LOCK_COUNT 10

//服务器类型，客户端的UserKey就使用此枚举变量
enum EServerType
{
	EST_UNKNOWN,
	EST_CMD_SERVER,
	EST_DATA_SERVER,
	EST_SERVER_COUNT = 60000
};

//显示消息类型
enum EPopupMsgType
{
	EPMT_UNKNOWN,
	EPMT_INFORM,			//信息
	EPMT_WARNING,           //告警
	EPMT_CRITICAL,          //严重
	EPMT_QUESTION			//问询
};
