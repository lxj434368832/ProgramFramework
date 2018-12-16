#include "IOCPClient.h"
#include "IOCPModule.h"
#include "INetInterface.h"


IOCPClient::IOCPClient(INetInterface *pNet):
	IOCPBase(pNet)
{
}

IOCPClient::~IOCPClient()
{
}

bool IOCPClient::StartConnect(unsigned uUserKey, std::string ip, u_short port, int iRecnnt)
{
	bool bRet = false;
	PER_SOCKET_CONTEXT *pSkContext = m_rscSocketContext.get();
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
				MLOG("创建socket失败，错误码：%d", WSAGetLastError());
				break;
			}
		}

		if (IOCPModule::Instance()->BindIoCompletionPort(pSkContext, m_hIOCompletionPort)) break;
		pIO->m_socket = pSkContext->m_socket;

		SOCKADDR_IN addrLocal;
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_port = 0;
		if (IOCPModule::Instance()->Bind(pIO->m_socket, (LPSOCKADDR)&addrLocal)) break;

		SOCKADDR_IN *pSrvAddr = &pSkContext->m_clientAddr;
		pSrvAddr->sin_family = AF_INET;
		IOCPModule::Instance()->GetIPAddress((LPSOCKADDR)&pSrvAddr->sin_addr);
		pSrvAddr->sin_port = htons(port);

		bRet = PostConnectEx(pSkContext);

	} while (0);

	return bRet;
}

void IOCPClient::StopClient()
{
	//关闭所有socket句柄以清除所有挂起的重叠IO操作
	m_lckConnectList.lock();
	auto iter = m_mapConnectList.begin();
	while (iter != m_mapConnectList.end())
	{
		PER_SOCKET_CONTEXT *pSkContext = iter->second;
		if (INVALID_SOCKET != pSkContext->m_socket)
		{
			if (EOP_CONNECT != pSkContext->m_ReceiveContext.m_oprateType)
				m_pNetInterface->DeleteUser(pSkContext->m_uUserKey);

			::shutdown(pSkContext->m_socket, SD_BOTH);
			RELEASE_SOCKET(pSkContext->m_socket);
		}
		else
		{
			MLOG("连接列表中的资源不正确,不应该存在INVALID_SOCKET！");
		}
		m_mapConnectList.erase(iter++);
		m_rscSocketContext.put(pSkContext);
	}
	m_lckConnectList.unlock();

	IOCPBase::UninitIOCP();
}

void IOCPClient::StartHeartbeatSend()
{

}
