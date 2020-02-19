#pragma once
/*************************************************************************
* function��iocpģ�Ͷ����ļ�����װ��ص�����������������ϵͳ���������
* author :	������
* datetime��2017-12-14
* company:  ���̽ݿƼ��ɷ����޹�˾
*************************************************************************/

#include "IOCPDef.h"

struct PER_IO_CONTEXT;
struct PER_SOCKET_CONTEXT;

// ��Ҫ��װiocp��ص����ӡ����ա����ͺ���

class IOCPModule
{
public:
	~IOCPModule();
	static IOCPModule* Instance();

	// ��ñ����д�����������
	int GetProcessorCount();

	SOCKET Socket();

	/*************************************************************************
	* function����socket�����ص�ַname
	* return:	�ɹ�����0��ʧ�ܷ���WSAGetLastError()��ֵ
	*************************************************************************/
	int Bind(SOCKET s, const LPSOCKADDR name);

	/*************************************************************************
	* function��������������
	* return:	�ɹ�����0��ʧ�ܷ���WSAGetLastError()��ֵ
	*************************************************************************/
	int Listen(SOCKET s, int iMaxUserCount);

	/*************************************************************************
	* function��������ɶ˿�
	* return:	�ɹ����ش����ľ����ʧ�ܷ���NULL
	*************************************************************************/
	HANDLE CreateIoCompletionPort();

	/*************************************************************************
	* function����socket�󶨵���ɶ˿�hIOCP��
	* return:	�ɹ�����0��ʧ�ܷ���WSAGetLastError()��ֵ
	*************************************************************************/
	int BindIoCompletionPort(PER_SOCKET_CONTEXT *pSkContext, HANDLE hIOCP);
	
	/*************************************************************************
	* function�������ͻ��˵�����
	* return:	�ɹ�����0��ʧ�ܷ���WSAGetLastError()��ֵ
	*************************************************************************/
	int AcceptEx(SOCKET listenSocket, PER_IO_CONTEXT *pIO);

	void GetAcceptExSockaddrs(PER_IO_CONTEXT *pIO, LPSOCKADDR *lpAddr);

	std::string GetIPAddress(PVOID pAddrBuf);
	bool ParseIPAddress(std::string strIP, PVOID pAddrBuf);

	//�ɹ�����0�����ɹ�����GetLastError()��ֵ
	int ConnectEx(PER_IO_CONTEXT *pIO, const LPSOCKADDR name);

	//�ɹ�����0�����ɹ�����GetLastError()��ֵ
	int DisconnectEx(PER_IO_CONTEXT *pIO);

	//�ɹ�����0�����ɹ�����GetLastError()��ֵ
	int PostQueuedCompletionStatus(HANDLE hCP, DWORD dwTransBytes, ULONG_PTR dwCompletionKey, LPOVERLAPPED lpOL);

	//�ɹ�����0�����ɹ�����GetLastError()��ֵ
	int Send(PER_IO_CONTEXT *pIO);

	//�ɹ�����0�����ɹ�����GetLastError()��ֵ
	int Receive(PER_IO_CONTEXT *pIO);

	//����keeplive����,����˵���
	bool SetKeepLiveParam(PER_IO_CONTEXT *pIO);

private:
	IOCPModule();
	IOCPModule(const IOCPModule&);
	IOCPModule& operator=(const IOCPModule&);

	void LoadAllWSAFunction();

private:
	LPFN_CONNECTEX m_fnConnectEx = NULL;
	LPFN_ACCEPTEX m_fnAcceptEx = NULL;
	LPFN_DISCONNECTEX m_fnDisconnectEx = NULL;
	LPFN_GETACCEPTEXSOCKADDRS m_fnGetAcceptExSockaddrs = NULL;
};