#include "stdafx.h"
#include "ClientManage.h"
#include "UserInfo.h"

ClientManage::ClientManage()
{

}

ClientManage::~ClientManage()
{

}

void ClientManage::Start()
{

}

void ClientManage::Stop()
{
	m_funSendData = nullptr;		//发送数据回调
	m_fuDisconnect = nullptr;		//主动断开连接回调
	m_mapIdKey.clear();				//用户id和key的映射关系
	m_mapUserList.clear();			//用户key和UserInfo的映射
}

void ClientManage::AddUser(unsigned uUserKey)
{

}

void ClientManage::HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length)
{

}

void ClientManage::DeleteUser(unsigned uUserKey)
{

}
