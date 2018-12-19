#include "ServerConnect.h"


ServerConnect::ServerConnect(ICommunication *pCmmnt)
{
	m_pCommunication = pCmmnt;
}

ServerConnect::~ServerConnect()
{

}

bool ServerConnect::Start()
{

}

void ServerConnect::Stop()
{
	m_funSendData = nullptr;		//发送数据回调
	m_fuDisconnect = nullptr;		//主动断开连接回调
	m_mapIdKey.clear();				//用户id和key的映射关系
	m_mapUserList.clear();			//用户key和UserInfo的映射
}

void ServerConnect::Send(UserKey uUserKey, const char * data, unsigned uLength)
{
	if (m_funSendData)
	{
		m_funSendData(uUserKey, 0, data, uLength);
	}
}

void ServerConnect::Disconnect(UserKey uUserKey)
{
	if (m_fuDisconnect)	m_fuDisconnect(uUserKey);
}

void ServerConnect::AddUser(UserKey uUserKey)
{

}

void ServerConnect::HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length)
{

}

void ServerConnect::DeleteUser(UserKey uUserKey)
{

}
