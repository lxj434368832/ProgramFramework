#include "IOCPServer.h"
#include "IOCPModule.h"
#include "INetInterface.h"
#include <assert.h>

IOCPServer::IOCPServer(INetInterface *pNet) : 
	IOCPBase(pNet)
{
}

IOCPServer::~IOCPServer()
{
}

bool IOCPServer::StartServer(USHORT nPort, unsigned dwMaxConnection, unsigned uThreadCount)
{
	if (false == InitIOCP(uThreadCount))
		return false;

	if (StartServerListen(nPort, dwMaxConnection))
	{
		//3、安全地让线程退出
		for (unsigned i = 0; i < m_uThreadCount; i++)
		{
			IOCPModule::Instance()->PostQueuedCompletionStatus(m_hIOCompletionPort, 0, NULL, NULL);
		}

		::WaitForMultipleObjects(m_uThreadCount, m_aThreadList, TRUE, INFINITE);

		for (unsigned i = 0; i < m_uThreadCount; i++)
		{
			::CloseHandle(m_aThreadList[i]);
		}
		delete[] m_aThreadList;
		m_aThreadList = nullptr;

		RELEASE_HANDLE(m_hIOCompletionPort);
		return false;
	}
	return true;
}

bool IOCPServer::StopServer()
{
	//1、关闭监听套接字
	::shutdown(m_pListenSocketContext->m_socket, SD_BOTH);
	::closesocket(m_pListenSocketContext->m_socket);
	m_pListenSocketContext->m_socket = INVALID_SOCKET;
	m_rscSocketContext.put(m_pListenSocketContext);
	m_pListenSocketContext = nullptr;

	//2、等待AcceptEx IO操作完成
	while (true)
	{
		bool bFind = false;
		for (auto value : m_mapConnectList)
		{
			if (EOP_ACCEPT == value.second->m_ReceiveContext.m_oprateType)
			{
				bFind = true;
			}
		}
		if (false == bFind) break;

		::Sleep(10);
	}

	//3、关闭客户端连接并取消所有IO
	m_lckConnectList.lock();
	auto iter = m_mapConnectList.begin();
	while (iter != m_mapConnectList.end())
	{
		PER_SOCKET_CONTEXT *pSkContext = iter->second;
		if (INVALID_SOCKET != pSkContext->m_socket &&
			EOP_RECEIVE == pSkContext->m_ReceiveContext.m_oprateType)
		{
			m_pNetInterface->DeleteUser(pSkContext->m_uUserKey);
			::shutdown(pSkContext->m_socket, SD_BOTH);
			RELEASE_SOCKET(pSkContext->m_socket);
			m_mapConnectList.erase(iter++);
			m_rscSocketContext.put(pSkContext);
		}
		else
		{
			MLOG("连接列表中的资源不正确！");
			iter++;
		}
	}
	m_lckConnectList.unlock();
	
	//3、安全地让线程退出
	for (unsigned i = 0; i < m_uThreadCount; i++)
	{
		IOCPModule::Instance()->PostQueuedCompletionStatus(m_hIOCompletionPort, 0, NULL, NULL);
	}

	::WaitForMultipleObjects(m_uThreadCount, m_aThreadList, TRUE, INFINITE);

	for (unsigned i = 0; i < m_uThreadCount; i++)
	{
		::CloseHandle(m_aThreadList[i]);
	}
	delete[] m_aThreadList;
	m_aThreadList = nullptr;

	RELEASE_HANDLE(m_hIOCompletionPort);
	return true;
}

void IOCPServer::HeartbeatCheck()
{

}

bool IOCPServer::StartServerListen(u_short port, unsigned iMaxConnectCount)
{
	bool bRet = false;
	do
	{
		if (nullptr == m_pListenSocketContext)
		{
			m_pListenSocketContext = m_rscSocketContext.get();
		}

		if (INVALID_SOCKET == m_pListenSocketContext->m_socket)
		{
			m_pListenSocketContext->m_socket = IOCPModule::Instance()->Socket();
			if (INVALID_SOCKET == m_pListenSocketContext->m_socket)
			{
				MLOG("创建Server监听socket失败，错误码：%d", WSAGetLastError());
				break;
			}
		}

		//SO_REUSEADDR允许同一port上启动同一服务器的多个实例(多个进程)。但每个实例绑定的IP地址是不能相同的。
		//int nOpt = 1;
		//setsockopt(m_pListenSocketContext->m_Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&nOpt, sizeof(nOpt));	//重用地址

		SOCKADDR_IN addrLocal;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_port = htons(port);
		if (IOCPModule::Instance()->Bind(m_pListenSocketContext->m_socket, (LPSOCKADDR)&addrLocal)) break;

		if (IOCPModule::Instance()->Listen(m_pListenSocketContext->m_socket, SOMAXCONN)) break;

		if (IOCPModule::Instance()->BindIoCompletionPort(m_pListenSocketContext, m_hIOCompletionPort)) break;

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
		m_rscSocketContext.put(m_pListenSocketContext);
		m_pListenSocketContext = nullptr;
	}
	return bRet;
}