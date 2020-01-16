#include "IOCPDef.h"
#include "IOCPClient.h"
#include "IOCPModule.h"
#include "INetInterface.h"
#include "stdafx.h"


IOCPClient::IOCPClient(INetInterface *pNet):
	IOCPBase(pNet)
{
}

IOCPClient::~IOCPClient()
{
	RELEASE(d);
}

bool IOCPClient::StartClient(unsigned uThreadCount)
{
	return InitIOCP(uThreadCount);
}

bool IOCPClient::AddConnect(unsigned uUserKey, std::string ip, u_short port, int iRecnnt)
{
	bool bRet = false;
	PER_SOCKET_CONTEXT *pSkContext = d->rscSocketContext.get();
	pSkContext->Reset();
	pSkContext->m_uUserKey = uUserKey;

	do
	{
		PER_IO_CONTEXT *pIO = &pSkContext->m_ReceiveContext;
		pIO->m_wParam = iRecnnt;

		if (INVALID_SOCKET == pSkContext->m_socket)
		{
			if (INVALID_SOCKET == (pSkContext->m_socket = IOCPModule::Instance()->Socket()))
			{
				LOGM("创建socket失败，错误码：%d", WSAGetLastError());
				break;
			}
		}

		if (IOCPModule::Instance()->BindIoCompletionPort(pSkContext, d->hIOCompletionPort)) break;
		pIO->m_socket = pSkContext->m_socket;

		SOCKADDR_IN addrLocal;
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_port = 0;
		if (IOCPModule::Instance()->Bind(pIO->m_socket, (LPSOCKADDR)&addrLocal)) break;

		SOCKADDR_IN *pSrvAddr = &pSkContext->m_clientAddr;
		pSrvAddr->sin_family = AF_INET;
		IOCPModule::Instance()->ParseIPAddress(ip, &pSrvAddr->sin_addr);
		pSrvAddr->sin_port = htons(port);

		bRet = PostConnectEx(pSkContext);

	} while (0);

	return bRet;
}

void IOCPClient::HandConnectFailed(PER_SOCKET_CONTEXT *pSkContext)
{
	d->lckConnectList.lock();
	d->mapConnectList.erase(pSkContext->m_uUserKey);
	d->lckConnectList.unlock();

	if (1 == pSkContext->m_iDisconnectFlag)
	{
		LOGM("检测到连接断开标识，将直接回收资源！");
		//直接回收资源
		RELEASE_SOCKET(pSkContext->m_socket);
		d->rscSocketContext.put(pSkContext);
		return;
	}

	PER_IO_CONTEXT *pIO = &pSkContext->m_ReceiveContext;
	if ((int)pIO->m_wParam <= 0) //表示不需要重连，直接回收资源
	{
		d->pNetInterface->ConnectNotify(pSkContext->m_uUserKey, false);

		RELEASE_SOCKET(pSkContext->m_socket);
		d->rscSocketContext.put(pSkContext);
	}
	else //	表示需要重连，放入待连接列表中
	{
		d->lckStayConnect.lock();
		d->mapStayConnect[pSkContext->m_uUserKey] = pSkContext;
		d->lckStayConnect.unlock();
	}
}

void IOCPClient::HeartbeatHandle()
{
	d->hHeartbeatEvent = ::CreateEvent(NULL, false, false, NULL);
	if (NULL == d->hHeartbeatEvent)
	{
		loge() << "创建心跳事件失败！";
		return;
	}
	
	std::string strMsg = "心跳数据。";

	while (true)
	{
		DWORD dwRet = ::WaitForSingleObject(d->hHeartbeatEvent, 5000);
		if (WAIT_OBJECT_0 == dwRet)
		{
			logm() << "正常退出心跳线程。";
			break;
		}
		else if (WAIT_FAILED == dwRet)
		{
			loge() << "心跳线程发生错误：" << ::GetLastError();
		}

		//处理待连接列表
		d->lckStayConnect.lock();
		for (auto iter = d->mapStayConnect.begin(); iter != d->mapStayConnect.end(); )
		{
			if (false == PostConnectEx(iter->second))
			{
				d->pNetInterface->ConnectNotify(iter->first, false);
				iter = d->mapStayConnect.erase(iter);
			}
			else
				iter++;
		}
		d->lckStayConnect.unlock();

		d->lckConnectList.lock();
		for (auto iter = d->mapConnectList.begin(); iter != d->mapConnectList.end(); iter++)
		{
			Send(iter->first, 0, strMsg.data(), strMsg.size());
		}
		d->lckConnectList.unlock();
	}
}

void IOCPClient::StopClient()
{
	//关闭所有socket句柄以清除所有挂起的重叠IO操作
	logm() << "关闭所有socket连接。";
	d->lckConnectList.lock();
	for (auto iter = d->mapConnectList.begin(); iter != d->mapConnectList.end(); iter++)
	{
		PER_SOCKET_CONTEXT *pSkContext = iter->second;
		if (INVALID_SOCKET != pSkContext->m_socket)
		{
			if (EOP_CONNECT == pSkContext->m_ReceiveContext.m_oprateType)
				pSkContext->m_iDisconnectFlag = 1;
			//else
			//	m_pNetInterface->DeleteUser(pSkContext->m_uUserKey);

			::shutdown(pSkContext->m_socket, SD_BOTH);
			//RELEASE_SOCKET(pSkContext->m_socket);
		}
		else
		{
			LOGM("连接列表中的资源不正确,不应该存在INVALID_SOCKET！");
		}

		//m_mapConnectList.erase(iter++);
		//m_rscSocketContext.put(pSkContext);
	}
	d->lckConnectList.unlock();

	IOCPBase::UninitIOCP();
}
