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

bool IOCPClient::AddConnect(unsigned uSrvKey, std::string ip, u_short port, int iRecnnt)
{
	bool bRet = false;
	PER_SOCKET_CONTEXT *pSkContext = d->rscSocketContext.get();
	pSkContext->Reset();
	pSkContext->m_uUserKey = uSrvKey;

	do
	{
		if (INVALID_SOCKET == pSkContext->m_socket)
		{
			if (INVALID_SOCKET == (pSkContext->m_socket = IOCPModule::Instance()->Socket()))
			{
				LOGM("����socketʧ�ܣ������룺%d", WSAGetLastError());
				break;
			}
			if (IOCPModule::Instance()->BindIoCompletionPort(pSkContext, d->hIOCompletionPort)) break;

			//int nOpt = 1;
			//setsockopt(pSkContext->m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&nOpt, sizeof(nOpt));

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
	//�ر�����socket�����������й�����ص�IO����
	logm() << "�ر�����socket���ӡ�"; 
	d->iExitUserCount = 0;
	d->lckConnectList.lock(); 
	auto iter = d->mapConnectList.begin();
	while ( iter != d->mapConnectList.end())
	{
		PER_SOCKET_CONTEXT *pSkContext = iter->second;
		if (INVALID_SOCKET != pSkContext->m_socket)
		{
			if (EOP_ACCEPT >= pSkContext->m_ReceiveContext.m_oprateType)
			{
				if (0 == ::CancelIoEx((HANDLE)pSkContext->m_socket, NULL))
					logm() << "CancelIoExʧ��, code:" << ::GetLastError();
				iter++;
			}
			else
			{
				ReCycleSocketRsc(pSkContext, nullptr);
				PER_IO_CONTEXT* pIO = d->rscIoContext.get();
				pIO->m_socket = pSkContext->m_socket;
				if (IOCPModule::Instance()->DisconnectEx(pIO))
				{
					//Ͷ��ʧ�ܣ�ֱ�ӻ�����Դ
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
		else
		{
			LOGM("�����б��е���Դ����ȷ,��Ӧ�ô���INVALID_SOCKET��");
		}
	}
	d->lckConnectList.unlock();

	for(int i = 0; i < 5 && d->iExitUserCount > 5; i++)
	{
		::Sleep(500);
		logm() << "�ȴ��Ͽ����Ӵ�����ϡ�";
	}

	IOCPBase::UninitIOCP();
}

void IOCPClient::SendData(unsigned uSrvKey, unsigned uMsgType, const char* data, unsigned uLength)
{
	IOCPBase::Send(uSrvKey, uMsgType, data, uLength);
}

void IOCPClient::Disconnect(unsigned uSrvKey)
{
	IOCPBase::Disconnect(uSrvKey);
}
