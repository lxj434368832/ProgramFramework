#pragma once
/*************************************************************************
* function：服务端连接管理
* author :	明巧文
* datetime：2017-12-14
* company:  
*************************************************************************/
#include <thread>
#include "../../3rdParty/IOCPCommunication/include/INetInterface.h"
#include "..\..\3rdParty\Framework\include\MLock.h"

class MessageHandle;
class IOCPClient;
class ICommunication;
class IMessageHandle; 
struct SPbMsg;

//#define CONNECT_SHARE_LOCK_COUNT 16

class ServerConnect : public INetInterface
{
public:
	ServerConnect(ICommunication *pCmmnt = nullptr);
	virtual ~ServerConnect();
	bool Initialize(unsigned uThreadCount);
	void Uninitialize();

	/*************************************************************************
	* function：开启一个连接需要考虑是否需要重连
	* param ip port: 需要连接的ip和端口号
	* param pIO:	 如果连接时要发送数据，需要传递pIO
	* param iRecnnt: 是否重连标识,小于0代表不需要重连
	* return:		 返回此连接对应的id,但不代表连接成功，为0代表连接出现了错误
	*************************************************************************/
	virtual bool AddConnect(unsigned uUserKey, std::string ip, ushort port, int iRecnnt = -1);

	/*************************************************************************
	* function： 发送数据
	* param uMsgType: 消息类型
	* param msg:	 需要发送的数据
	*************************************************************************/
	void SendData(UserKey uUserKey, SPbMsg &msg);

	/*************************************************************************
	* function： 发送数据
	* param key: 用户id
	* param data:需要发送的数据
	* return:	 无
	*************************************************************************/
	void Send(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength);

	/*************************************************************************
	* function： 断开连接，底层会回调DeleteUser函数
	* param key: 用户id
	* return:	 无
	*************************************************************************/
	void Disconnect(unsigned uUserKey);

private:
	//实现INetInterface接口
	//连接结果通知
	void ConnectNotify(UserKey uUserKey, bool bSuccess) override;

	//处理服务数据
	void HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

	// 删除用户
	void DeleteUser(unsigned uUserKey) override;

private:
	void DeamonConnectCallback(unsigned uInterval);

private:
	struct SServerInfo
	{
		unsigned	uUserKey;
		std::string	strIp;
		ushort		usPort;
		int			iRecnnt = 0;
		bool		bNeedConnect = false;
	};

	ICommunication		*m_pCmmnt;
	IMessageHandle		*m_pMsgModule;
	IOCPClient			*m_pIOCPClient;				//IOCP 客户端
	std::map<unsigned, SServerInfo> m_mapSrvInfo;	//服务信息列表
	MLock							m_lckSrvInfo;	//服务信息锁
	std::thread						*m_pThDeamenConnect;	//守护连接线程
	HANDLE							m_hDeamenConnect;		//守护连接事件
};
