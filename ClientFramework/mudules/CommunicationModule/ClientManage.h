#pragma once
/*************************************************************************
* function：连接管理定义文件
* author :	明巧文
* datetime：2017-12-14
* company:  
*************************************************************************/
#include "../IOCPCommunication/INetInterface.h"

class MessageModule;
struct UserInfo;

//客户端的UserKey就使用此枚举变量
enum class EServerType
{
	EST_UNKNOWN,
	EST_CMD_SERVER,
	EST_DATA_SERVER
};

class ClientManage : public INetInterface
{
public:
	ClientManage();
	virtual ~ClientManage();
	void Start();
	void Stop();
	

private:
	//新服务用户成功连接通知
	virtual void AddUser(unsigned uUserKey) override;

	//处理服务数据
	virtual void HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

	// 如果成功删除用户返回删除的地址,如果用户不存在则返回NULL
	virtual void DeleteUser(unsigned uUserKey) override;

};

