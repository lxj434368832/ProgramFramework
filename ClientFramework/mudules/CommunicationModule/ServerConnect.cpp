#include "..\..\..\IOCPCommunication\IOCPClient.h"
#include "ServerConnect.h"
#include "MainCommunication.h"
#include "..\..\MainClient\IMainClient.h"
#include "..\MessageModule\IMessage.h"
#include "..\MessageModule\ProtobufMsgFactory.h"
#include "..\..\include\TypeDefine.h"


ServerConnect::ServerConnect(ICommunication *pCmmnt)
	:INetInterface(),
	m_rscUser(SERVER_COUNT)
{
	m_pCommunication = pCmmnt;
	m_pIOCPClient = new IOCPClient(this);
}

ServerConnect::~ServerConnect()
{
	m_pCommunication = nullptr;
	m_pMsgFctry = nullptr;
	RELEASE(m_pIOCPClient);
}

bool ServerConnect::Start()
{
	m_pMsgFctry = m_pCommunication->GetMainClient()->GetMessageModule()->GetProtobufMsgFactory();
	if (nullptr == m_pMsgFctry)
	{
		loge() << "获取消息工厂函数失败！";
		return false;
	}

	ClientConfig &cfg = *m_pCommunication->GetMainClient()->GetClientConfig();
	if (false == m_pIOCPClient->StartClient(cfg.uIOCPThreadCount)) return;

	if (false == m_pIOCPClient->AddConnect(EST_CMD_SERVER, cfg.strServerIP, cfg.usServerPort, 1))
	{
		loge() << "添加对命令服务器的连接失败！";
		return false;
	}
	return true;

}

void ServerConnect::Stop()
{
	m_pIOCPClient->StopClient();
	m_pMsgFctry = nullptr;
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
	m_lckUserList.lock();
	auto iter = m_mapUserList.find(uUserKey);
	if (iter != m_mapUserList.end() && iter->second)
	{
		m_lckUserList.unlock();
		loge() << "已经存在当前用户key:" << uUserKey << "id:" << iter->second->m_uUserId << "无需添加！";
		return;
	}

	UserInfo *pUser = m_rscUser.get();
	pUser->m_uUserKey = uUserKey;
	pUser->m_uUserId = 0;
	m_mapUserList[uUserKey] = pUser;
	m_lckUserList.unlock();

	logm() << "连接服务:" << uUserKey<<"成功！";
}

void ServerConnect::HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length)
{
	if (m_pMsgFctry)
	{
		m_pMsgFctry->AddMessageData(uUserKey, data, length);
	}
}

void ServerConnect::DeleteUser(UserKey uUserKey)
{
	UserInfo *pUser = nullptr;
	m_lckUserList.lock();
	auto iter = m_mapUserList.find(uUserKey);
	if (m_mapUserList.end() != iter)
	{
		pUser = iter->second;
	}
	m_mapUserList.erase(uUserKey);
	m_lckUserList.unlock();

	if (pUser)
	{
		m_rscUser.put(pUser);
		logm() << "连接断开userKey:" << uUserKey;
	}
}
