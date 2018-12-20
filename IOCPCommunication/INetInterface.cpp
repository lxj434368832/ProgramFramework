#include "stdafx.h"
#include "INetInterface.h"


INetInterface::INetInterface()
{
	m_funSendData = nullptr;		//发送数据回调
	m_fuDisconnect = nullptr;		//主动断开连接回调
}


INetInterface::~INetInterface()
{
	m_mapIdKey.clear();
	m_mapUserList.clear();
	m_funSendData = nullptr;		//发送数据回调
	m_fuDisconnect = nullptr;		//主动断开连接回调
}

void INetInterface::SetNetworkCallback(SendDataCallback funSendData, DisconnectCallback funDisconnect)
{
	m_funSendData = funSendData;
	m_fuDisconnect = funDisconnect;
}

void INetInterface::AddUser(UserKey uUserKey)
{

}

void INetInterface::HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length)
{

}

void INetInterface::DeleteUser(UserKey uUserKey)
{

}
