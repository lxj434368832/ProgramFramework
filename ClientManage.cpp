#include "stdafx.h"
#include "ClientManage.h"


ClientManage::ClientManage()
{
}


ClientManage::~ClientManage()
{
}

void ClientManage::SetClientListenSocket(SOCKET listenSocket)
{
	m_sClientLstnSocket = listenSocket;
}

SOCKET ClientManage::GetClientListenSocket()
{
	return m_sClientLstnSocket;
}

unsigned ClientManage::GetClientUserId()
{
	return m_uClientUidGenerator++;
}

void ClientManage::ClientAcceptNotify(unsigned uUserId, SOCKET s)
{

}

void ClientManage::HandClientData(unsigned uUserId, char* data, unsigned length)
{

}

UserInfo* ClientManage::DeleteClientUser(unsigned userId)
{
	return NULL;
}
