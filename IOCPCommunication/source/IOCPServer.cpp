#include "..\include\IOCPDef.h"
#include "..\include\IOCPServer.h"
#include "..\include\IOCPModule.h"
#include "..\include\INetInterface.h"
#include "..\include\stdafx.h"
#include <assert.h>

IOCPServer::IOCPServer(INetInterface *pNet) : 
	IOCPBase(pNet)
{
	m_bStart = false;
	m_pListenSocketContext = nullptr;
}

IOCPServer::~IOCPServer()
{
}

bool IOCPServer::StartServer(USHORT nPort, unsigned dwMaxConnection, unsigned uThreadCount)
{
	if (false == InitIOCP(uThreadCount))
		return false;

	if (false == StartServerListen(nPort, dwMaxConnection))
	{
		//安全地让线程退出
		for (unsigned i = 0; i < d->uThreadCount; i++)
		{
			IOCPModule::Instance()->PostQueuedCompletionStatus(d->hIOCompletionPort, 0, NULL, NULL);
		}

		for (unsigned i = 0; i < d->uThreadCount; i++)
		{
			if (d->aThreadList[i].joinable())
			{
				d->aThreadList[i].join();
			}
		}
		delete[] d->aThreadList;
		d->aThreadList = nullptr;

		delete[] d->aThreadList;
		d->aThreadList = nullptr;

		RELEASE_HANDLE(d->hIOCompletionPort);
		return false;
	}
	m_bStart = true;
	return true;
}

bool IOCPServer::StartServerListen(u_short port, unsigned iMaxConnectCount)
{
	bool bRet = false;
	do
	{
		if (nullptr == m_pListenSocketContext)
		{
			m_pListenSocketContext = d->rscSocketContext.get();
		}

		if (INVALID_SOCKET == m_pListenSocketContext->m_socket)
		{
			m_pListenSocketContext->m_socket = IOCPModule::Instance()->Socket();
			if (INVALID_SOCKET == m_pListenSocketContext->m_socket)
			{
				LOGM("创建Server监听socket失败，错误码：%d", WSAGetLastError());
				break;
			}	
		}

		SOCKADDR_IN addrLocal;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_port = htons(port);
		if (IOCPModule::Instance()->Bind(m_pListenSocketContext->m_socket, (LPSOCKADDR)&addrLocal)) break;

		if (IOCPModule::Instance()->Listen(m_pListenSocketContext->m_socket, SOMAXCONN)) break;

		if (IOCPModule::Instance()->BindIoCompletionPort(m_pListenSocketContext, d->hIOCompletionPort)) break;

		//投递接受操作
		for (unsigned i = 0; i < iMaxConnectCount; i++)
		{
			if (false == (bRet = PostAcceptEx(m_pListenSocketContext->m_socket)))
			{
				break;
			}
		}

	} while (0);

	if (false == bRet)
	{
		RELEASE_SOCKET(m_pListenSocketContext->m_socket);
		d->rscSocketContext.put(m_pListenSocketContext);
		m_pListenSocketContext = nullptr;
	}
	return bRet;
}

void IOCPServer::StopServer()
{
	if (false == m_bStart) return;
	m_bStart = false;

	logm() << "关闭监听端口。";
	RELEASE_SOCKET(m_pListenSocketContext->m_socket);
	d->rscSocketContext.put(m_pListenSocketContext);
	m_pListenSocketContext = nullptr;

	logm() << "取消所有连接socket的IO操作。"; 
	d->iExitUserCount = 0;
	d->lckConnectList.lock();
	auto iter = d->mapConnectList.begin();
	while (iter != d->mapConnectList.end())
	{
		PER_SOCKET_CONTEXT *pSkContext = iter->second;
		if (INVALID_SOCKET == pSkContext->m_socket)
		{
			LOGM("连接列表中的资源不正确,不应该存在INVALID_SOCKET！");
			iter++;
		}
		else
		{
			if (EOP_ACCEPT >= pSkContext->m_ReceiveContext.m_oprateType)
			{
				iter++;
			}
			else
			{
				ReCycleSocketRsc(pSkContext, nullptr);
				PER_IO_CONTEXT* pIO = d->rscIoContext.get();
				pIO->m_socket = pSkContext->m_socket;
				if (IOCPModule::Instance()->DisconnectEx(pIO))
				{
					//投递失败，直接回收资源
					RELEASE_SOCKET(pSkContext->m_socket);
					d->rscSocketContext.put(pSkContext);

					pIO->Reset();
					d->rscIoContext.put(pIO);
				}
				else
					d->iExitUserCount++;

				iter = d->mapConnectList.erase(iter);
			}
		}
	}
	d->lckConnectList.unlock();

	for (int i = 0; i < 50 && d->iExitUserCount > 5; i++)
	{
		::Sleep(500);
		logm() << "等待断开连接处理完毕。";
	}

	IOCPBase::UninitIOCP();
}

void IOCPServer::StartHeartbeatCheck()
{

}
