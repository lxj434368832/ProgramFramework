#pragma once
/*************************************************************************
* function：服务端连接管理
* author :	明巧文
* datetime：2017-12-14
* company:  
*************************************************************************/
#include "../../../IOCPCommunication/INetInterface.h"
#include "../../../Framework/include/ResourceManage.h"

class MessageModule;
class IOCPClient;
class ICommunication;
class ProtobufMsgFactory;

//服务器类型，客户端的UserKey就使用此枚举变量
enum EServerType
{
	EST_UNKNOWN,
	EST_CMD_SERVER,
	EST_DATA_SERVER,
	SERVER_COUNT
};

#define CONNECT_SHARE_LOCK_COUNT 

class ServerConnect : public INetInterface
{
public:
	ServerConnect(ICommunication *pCmmnt = nullptr);
	virtual ~ServerConnect();
	bool Start();
	void Stop();

	/*************************************************************************
	* function： 发送数据
	* param key: 用户id
	* param data:需要发送的数据
	* return:	 无
	*************************************************************************/
	void Send(UserKey uUserKey, const char* data, unsigned uLength);

	void Disconnect(UserKey uUserKey);

private:
	//新服务用户成功连接通知
	virtual void AddUser(UserKey uUserKey) override;

	//处理服务数据
	virtual void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

	// 如果成功删除用户返回删除的地址,如果用户不存在则返回NULL
	virtual void DeleteUser(UserKey uUserKey) override;

private:
	ICommunication*		m_pCommunication;
	ProtobufMsgFactory* m_pMsgFctry;
	IOCPClient			*m_pIOCPClient;	//IOCP 客户端

	mqw::ResourceManage<UserInfo>	m_rscUser;
	MLock				m_shareLock[SERVER_COUNT];
};

