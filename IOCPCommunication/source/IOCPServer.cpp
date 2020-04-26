#include "..\include\IOCPDef.h"
#include "..\include\IOCPServer.h"
#include "..\include\IOCPModule.h"
#include "..\include\INetInterface.h"
#include "..\include\stdafx.h"
#include <assert.h>

IOCPServer::IOCPServer(INetInterface *pNet) : 
	d(new IOCPServerData(pNet)),
	IOCPBase(d)
{
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
	d->bStart = true;
	return true;
}

bool IOCPServer::StartServerListen(u_short port, unsigned iMaxConnectCount)
{
	bool bRet = false;
	do
	{
		if (nullptr == d->pListenSocketContext)
		{
			d->pListenSocketContext = d->rscSocketContext.get();
		}

		if (INVALID_SOCKET == d->pListenSocketContext->m_socket)
		{
			d->pListenSocketContext->m_socket = IOCPModule::Instance()->Socket();
			if (INVALID_SOCKET == d->pListenSocketContext->m_socket)
			{
				LOGM("创建Server监听socket失败，错误码：%d", WSAGetLastError());
				break;
			}	
		}

		SOCKADDR_IN addrLocal;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_port = htons(port);
		if (IOCPModule::Instance()->Bind(d->pListenSocketContext->m_socket, (LPSOCKADDR)&addrLocal)) break;

		if (IOCPModule::Instance()->Listen(d->pListenSocketContext->m_socket, SOMAXCONN)) break;

		if (IOCPModule::Instance()->BindIoCompletionPort(d->pListenSocketContext, d->hIOCompletionPort)) break;

		//投递接受操作
		for (unsigned i = 0; i < iMaxConnectCount; i++)
		{
			if (false == (bRet = PostAcceptEx(d->pListenSocketContext->m_socket)))
			{
				break;
			}
		}

	} while (0);

	if (false == bRet)
	{
		RELEASE_SOCKET(d->pListenSocketContext->m_socket);
		d->rscSocketContext.put(d->pListenSocketContext);
		d->pListenSocketContext = nullptr;
	}
	return bRet;
}

void IOCPServer::StopServer()
{
	if (false == d->bStart) return;
	d->bStart = false;

	logm() << "关闭监听端口。";
	RELEASE_SOCKET(d->pListenSocketContext->m_socket);
	d->rscSocketContext.put(d->pListenSocketContext);
	d->pListenSocketContext = nullptr;

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

void IOCPServer::SendData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength)
{
	IOCPBase::Send(uUserKey, uMsgType, data, uLength);
}

void IOCPServer::Disconnect(unsigned uUserKey)
{
	IOCPBase::Disconnect(uUserKey);
}
