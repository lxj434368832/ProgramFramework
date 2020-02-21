#include "..\include\IOCPDef.h"
#include "..\include\IOCPClient.h"
#include "..\include\IOCPModule.h"
#include "..\include\INetInterface.h"
#include "..\include\stdafx.h"


IOCPClient::IOCPClient(INetInterface *pNet):
	IOCPBase(pNet)
{
}

IOCPClient::~IOCPClient()
{
}

bool IOCPClient::StartClient(unsigned uThreadCount)
{
	if (false == InitIOCP(uThreadCount)) return false;
	return true;
}

bool IOCPClient::AddConnect(unsigned uUserKey, std::string ip, u_short port, int iRecnnt)
{
	bool bRet = false;
	PER_SOCKET_CONTEXT *pSkContext = d->rscSocketContext.get();
	pSkContext->Reset();
	pSkContext->m_uUserKey = uUserKey;

	do
	{
		if (INVALID_SOCKET == pSkContext->m_socket)
		{
			if (INVALID_SOCKET == (pSkContext->m_socket = IOCPModule::Instance()->Socket()))
			{
				LOGM("创建socket失败，错误码：%d", WSAGetLastError());
				break;
			}
			if (IOCPModule::Instance()->BindIoCompletionPort(pSkContext, d->hIOCompletionPort)) break;

			SOCKADDR_IN addrLocal;
			addrLocal.sin_addr.s_addr = ADDR_ANY;
			addrLocal.sin_family = AF_INET;
			addrLocal.sin_port = 0;
			IOCPModule::Instance()->Bind(pSkContext->m_socket, (LPSOCKADDR)&addrLocal);
		}
		bRet = true;

	} while (0);

	if (false == bRet)
	{
		RELEASE_SOCKET(pSkContext->m_socket);
		d->rscSocketContext.put(pSkContext);
		return bRet;
	}
	SOCKADDR_IN *pSrvAddr = &pSkContext->m_clientAddr;
	pSrvAddr->sin_family = AF_INET;
	IOCPModule::Instance()->ParseIPAddress(ip, &pSrvAddr->sin_addr);
	pSrvAddr->sin_port = htons(port);

	bRet = PostConnectEx(pSkContext);

	return bRet;
}

void IOCPClient::StopClient()
{
	//关闭所有socket句柄以清除所有挂起的重叠IO操作
	logm() << "关闭所有socket连接。";
	d->lckConnectList.lock(); 
	auto iter = d->mapConnectList.begin();
	while ( iter != d->mapConnectList.end())
	{
		PER_SOCKET_CONTEXT *pSkContext = iter->second;
		if (INVALID_SOCKET != pSkContext->m_socket)
		{
			if (EOP_CONNECT == pSkContext->m_ReceiveContext.m_oprateType)
			{
				if (0 == ::CancelIoEx((HANDLE)pSkContext->m_socket, NULL))
					logm() << "CancelIoEx失败, code:" << ::GetLastError();
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
				iter = d->mapConnectList.erase(iter);
			}
		}
		else
		{
			LOGM("连接列表中的资源不正确,不应该存在INVALID_SOCKET！");
		}
	}
	d->lckConnectList.unlock();

	IOCPBase::UninitIOCP();
}
