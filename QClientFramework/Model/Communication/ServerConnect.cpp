#include "ICommunication.h"
#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\CommonFile\CommonDefine.h"
#include "ServerConnect.h"
#include "../../3rdParty/IOCPCommunication/include/IOCPClient.h"
#include "..\MessageHandle\include\IMessageHandle.h"
#include <windows.h>


IServerConnect* IServerConnect::Create()
{
	return new ServerConnect();
}

void IServerConnect::Delete(IServerConnect*& p)
{
	RELEASE(p);
}


ServerConnect::ServerConnect(void *pMain)
{
	m_pMain = pMain;
	m_pIOCPClient = new IOCPClient(this);
}

ServerConnect::~ServerConnect()
{
	m_pMain = nullptr;
	RELEASE(m_pIOCPClient);
}

bool ServerConnect::Initialize(IMessageHandle* pMsgHdl, unsigned uThreadCount)
{
	m_pMsgModule = pMsgHdl;

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

bool ServerConnect::AddConnect(unsigned uSrvKey, std::string ip, ushort port, int iRecnnt)
{
	SServerInfo info;
	info.uSrvKey = uSrvKey;
	info.strIp = ip;
	info.usPort = port;
	info.iRecnnt = iRecnnt;
	m_lckSrvInfo.lock();
	m_mapSrvInfo[uSrvKey] = info;
	m_lckSrvInfo.unlock();
	if (m_pIOCPClient->AddConnect(uSrvKey, ip, port, iRecnnt))
	{
		LOGM("开始连接服务器key:%d ip:%s port:%d。", uSrvKey, ip.data(), port);
		return true;
	}
	else
	{
		LOGE("连接服务器key:%d ip:%s port:%d失败！", uSrvKey, ip.data(), port);
		return false;
	}
}

void ServerConnect::SendData(UserKey uSrvKey, SPbMsg &msg)
{
	this->Send(uSrvKey, msg.uMsgType, msg.strMsg.data(), msg.strMsg.length());
}

void ServerConnect::Send(unsigned uSrvKey, unsigned uMsgType, const char * data, unsigned uLength)
{
	if (m_mapSrvInfo.find(uSrvKey) == m_mapSrvInfo.end())
	{
		LOGM("SrvKey:%d离线，停止发送数据。", uSrvKey);
	}
	else if (m_pIOCPClient)
	{
		m_pIOCPClient->SendData(uSrvKey, uMsgType, data, uLength);
	}
}

void ServerConnect::Disconnect(unsigned uSrvKey)
{
	m_lckSrvInfo.lock();
	m_mapSrvInfo.erase(uSrvKey);
	m_lckSrvInfo.unlock();
	if (m_pIOCPClient)	m_pIOCPClient->Disconnect(uSrvKey);
}

void ServerConnect::ConnectNotify(unsigned uKey, bool bSuccess)
{
	emit signalTcpConnectNotify(uKey, bSuccess);
	if (false == bSuccess)
	{
		MAutoLock lck(m_lckSrvInfo);
		auto iter = m_mapSrvInfo.find(uKey);
		if (m_mapSrvInfo.end() != iter)
		{
			iter->second.bNeedConnect = true;
		}
	}
}

void ServerConnect::HandData(unsigned uKey, unsigned uMsgType, const char* data, unsigned length)
{
	if (m_pMsgModule)
	{
		m_pMsgModule->AddMessageData(uKey, uMsgType, data, length);
	}
}

void ServerConnect::DisConnectNotify(unsigned uKey)
{
	emit signalTcpDisconnectNotify(uKey);

	MAutoLock lck(m_lckSrvInfo);
	auto iter = m_mapSrvInfo.find(uKey);
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
				if (m_pIOCPClient->AddConnect(info.uSrvKey, info.strIp, info.usPort, info.iRecnnt))
				{
					LOGM("开始连接服务器key:%d ip:%s port:%d。",
						info.uSrvKey, info.strIp.data(), info.usPort);
					info.bNeedConnect = false;
				}
				else
				{
					LOGE("添加对服务器key:%d ip:%s port:%d的连接失败！", 
						info.uSrvKey, info.strIp.data(), info.usPort);
				}
			}
		}

		m_lckSrvInfo.unlock();
	}
}
