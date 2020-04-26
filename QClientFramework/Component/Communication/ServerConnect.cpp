#include "ICommunication.h"
#include "..\..\MainClient\IMainClient.h"
#include "..\MessageHandle\IMessageHandle.h"
#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\CommonFile\CommonDefine.h"
#include "..\..\CommonFile\EnumDefine.h"
#include "ServerConnect.h"
#include "../../3rdParty/IOCPCommunication/include/IOCPClient.h"
#include "..\..\Model\ModelManage\IModelManage.h"
#include <windows.h>

ServerConnect::ServerConnect(IMainClient *pMain) :
	QObject(),
	INetInterface()
{
	m_pMain = pMain;
	m_pIOCPClient = new IOCPClient(this);
}

ServerConnect::~ServerConnect()
{
	m_pMain = nullptr;
	RELEASE(m_pIOCPClient);
}

bool ServerConnect::Initialize(unsigned uThreadCount)
{
	m_pMsgModule = m_pMain->GetModelManage()->GetMessageHandle();
	if (nullptr == m_pMsgModule)
	{
		loge() << "获取消息处理模块失败！";
		return false;
	}

	if (false == m_pIOCPClient->StartClient(uThreadCount)) return false;

	m_pThDeamenConnect = new std::thread(&ServerConnect::DeamonConnectCallback, this, 5000);
	if (nullptr == m_pThDeamenConnect)
	{
		loge() << "开启守护连接线程失败。";
		return false;
	}

	return true;
}

void ServerConnect::Uninitialize()
{
	if (m_hDeamenConnect)
	{
		::SetEvent(m_hDeamenConnect);
	}
	if (m_pThDeamenConnect && m_pThDeamenConnect->joinable())
	{
		m_pThDeamenConnect->join();
	}
	m_pIOCPClient->StopClient();

	RELEASE(m_pThDeamenConnect);
	RELEASE_HANDLE(m_hDeamenConnect);
	m_pMsgModule = nullptr;
}

bool ServerConnect::AddConnect(unsigned uUserKey, std::string ip, ushort port, int iRecnnt)
{
	SServerInfo info;
	info.uUserKey = uUserKey;
	info.strIp = ip;
	info.usPort = port;
	info.iRecnnt = iRecnnt;
	m_lckSrvInfo.lock();
	m_mapSrvInfo[uUserKey] = info;
	m_lckSrvInfo.unlock();
	if (m_pIOCPClient->AddConnect(uUserKey, ip, port, iRecnnt))
	{
		LOGM("开始连接服务器key:%d ip:%s port:%d。", uUserKey, ip.data(), port);
		return true;
	}
	else
	{
		LOGE("连接服务器key:%d ip:%s port:%d失败！", uUserKey, ip.data(), port);
		return false;
	}
}

void ServerConnect::SendData(UserKey uUserKey, SPbMsg &msg)
{
	this->Send(uUserKey, msg.uMsgType, msg.strMsg.data(), msg.strMsg.length());
}

void ServerConnect::Send(unsigned uUserKey, unsigned uMsgType, const char * data, unsigned uLength)
{
	if (m_mapSrvInfo.find(uUserKey) == m_mapSrvInfo.end())
	{
		LOGM("SrvKey:%d离线，停止发送数据。", uUserKey);
	}
	else if (m_pIOCPClient)
	{
		m_pIOCPClient->SendData(uUserKey, uMsgType, data, uLength);
	}
}

void ServerConnect::Disconnect(unsigned uUserKey)
{
	m_lckSrvInfo.lock();
	m_mapSrvInfo.erase(uUserKey);
	m_lckSrvInfo.unlock();
	if (m_pIOCPClient)	m_pIOCPClient->Disconnect(uUserKey);
}

void ServerConnect::ConnectNotify(UserKey uUserKey, bool bSuccess)
{
	emit signalTcpConnectNotify(uUserKey, bSuccess);
	if (false == bSuccess)
	{
		MAutoLock lck(m_lckSrvInfo);
		auto iter = m_mapSrvInfo.find(uUserKey);
		if (m_mapSrvInfo.end() != iter)
		{
			iter->second.bNeedConnect = true;
		}
	}
}

void ServerConnect::HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length)
{
	if (m_pMsgModule)
	{
		m_pMsgModule->HandleProtobufMessage(uUserKey, uMsgType, data, length);
	}
}

void ServerConnect::DeleteUser(unsigned uUserKey)
{
	emit signalTcpDisconnectNotify(uUserKey);

	MAutoLock lck(m_lckSrvInfo);
	auto iter = m_mapSrvInfo.find(uUserKey);
	if (m_mapSrvInfo.end() != iter)
	{
		SServerInfo &info = iter->second;
		if (info.iRecnnt)
		{
			info.bNeedConnect = true;
		}
		else
		{
			m_mapSrvInfo.erase(iter);
		}
	}
}

void ServerConnect::DeamonConnectCallback(unsigned uInterval)
{
	m_hDeamenConnect = ::CreateEvent(NULL, false, false, NULL);
	if (NULL == m_hDeamenConnect)
	{
		loge() << "创建守护连接事件失败！";
		return;
	}

	while (true)
	{
		DWORD dwRet = ::WaitForSingleObject(m_hDeamenConnect, uInterval);
		if (WAIT_OBJECT_0 == dwRet)
		{
			logm() << "正常退出守护连接线程。";
			break;
		}
		else if (WAIT_FAILED == dwRet)
		{
			loge() << "守护连接线程发生错误：" << ::GetLastError();
			break;
		}
		
		m_lckSrvInfo.lock();
		for (auto iter = m_mapSrvInfo.begin(); iter != m_mapSrvInfo.end(); iter++)
		{
			SServerInfo &info = iter->second;
			if (info.bNeedConnect)
			{
				if (m_pIOCPClient->AddConnect(info.uUserKey, info.strIp, info.usPort, info.iRecnnt))
				{
					LOGM("开始连接服务器key:%d ip:%s port:%d。",
						info.uUserKey, info.strIp.data(), info.usPort);
					info.bNeedConnect = false;
				}
				else
				{
					LOGE("添加对服务器key:%d ip:%s port:%d的连接失败！", 
						info.uUserKey, info.strIp.data(), info.usPort);
				}
			}
		}

		m_lckSrvInfo.unlock();
	}
}
