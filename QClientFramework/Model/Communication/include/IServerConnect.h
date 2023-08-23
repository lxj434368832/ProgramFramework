#pragma once

#include <QObject>

class IMessageHandle;
struct SPbMsg;

class IServerConnect : public QObject
{
	Q_OBJECT
public:
	static IServerConnect* Create();
	static void Delete(IServerConnect*& p);

	IServerConnect() = default;
	virtual ~IServerConnect() = default;

	virtual bool Initialize(IMessageHandle* pMsgHdl, unsigned uThreadCount) = 0;
	virtual void Uninitialize() = 0;

	/*************************************************************************
	* function：开启一个连接需要考虑是否需要重连
	* param ip port: 需要连接的ip和端口号
	* param pIO:	 如果连接时要发送数据，需要传递pIO
	* param iRecnnt: 是否重连标识,小于0代表不需要重连
	* return:		 返回此连接对应的id,但不代表连接成功，为0代表连接出现了错误
	*************************************************************************/
	virtual bool AddConnect(unsigned uSrvKey, std::string ip, ushort port, int iRecnnt = -1) = 0;

	/*************************************************************************
	* function： 发送数据
	* param uMsgType: 消息类型
	* param msg:	 需要发送的数据
	*************************************************************************/
	virtual void SendData(unsigned uSrvKey, SPbMsg &msg) = 0;


	/*************************************************************************
	* function： 断开连接，底层会回调DeleteUser函数
	* param key: 用户id
	* return:	 无
	*************************************************************************/
	virtual void Disconnect(unsigned uSrvKey) = 0;

Q_SIGNALS:
	void signalTcpConnectNotify(unsigned uSrvKey, bool bSuccess);
	void signalTcpDisconnectNotify(unsigned uSrvKey);

};
