#pragma once
/*************************************************************************
* function����������ӹ���
* author :	������
* datetime��2017-12-14
* company:  
*************************************************************************/
#include <QObject>
#include <thread>
#include "../../3rdParty/IOCPCommunication/include/INetInterface.h"
#include "..\..\3rdParty\Framework\include\MLock.h"
#include "include/IServerConnect.h"

class IOCPClient;
class IMessageHandle;
//#define CONNECT_SHARE_LOCK_COUNT 16

class ServerConnect : public IServerConnect, public INetInterface
{
public:
	ServerConnect(void *pMain = nullptr);
	virtual ~ServerConnect();
	bool Initialize(IMessageHandle* pMsgHdl, unsigned uThreadCount) override;
	void Uninitialize() override;

	/*************************************************************************
	* function������һ��������Ҫ�����Ƿ���Ҫ����
	* param ip port: ��Ҫ���ӵ�ip�Ͷ˿ں�
	* param pIO:	 �������ʱҪ�������ݣ���Ҫ����pIO
	* param iRecnnt: �Ƿ�������ʶ,С��0������Ҫ����
	* return:		 ���ش����Ӷ�Ӧ��id,�����������ӳɹ���Ϊ0�������ӳ����˴���
	*************************************************************************/
	bool AddConnect(unsigned uSrvKey, std::string ip, ushort port, int iRecnnt = -1) override;

	/*************************************************************************
	* function�� ��������
	* param uMsgType: ��Ϣ����
	* param msg:	 ��Ҫ���͵�����
	*************************************************************************/
	void SendData(unsigned uSrvKey, SPbMsg &msg) override;

	/*************************************************************************
	* function�� �Ͽ����ӣ��ײ��ص�DeleteUser����
	* param key: �û�id
	* return:	 ��
	*************************************************************************/
	void Disconnect(unsigned uSrvKey) override;

private:
	/*************************************************************************
	* function�� ��������
	* param key: �û�id
	* param data:��Ҫ���͵�����
	* return:	 ��
	*************************************************************************/
	void Send(unsigned uSrvKey, unsigned uMsgType, const char* data, unsigned uLength);

	//ʵ��INetInterface�ӿ�
	// ���ӽ��֪ͨ
	void ConnectNotify(unsigned uKey, bool bSuccess) override;

	// �����������
	void HandData(unsigned uKey, unsigned uMsgType, const char* data, unsigned length) override;

	// �û��Ͽ�����֪ͨ
	void DisConnectNotify(unsigned uKey) override;

private:
	void DeamonConnectCallback(unsigned uInterval);

private:
	struct SServerInfo
	{
		unsigned	uSrvKey;
		std::string	strIp;
		ushort		usPort;
		int			iRecnnt = 0;
		bool		bNeedConnect = false;
	};

	void				*m_pMain;
	IMessageHandle		*m_pMsgModule;
	IOCPClient			*m_pIOCPClient;				//IOCP �ͻ���
	std::map<unsigned, SServerInfo> m_mapSrvInfo;	//������Ϣ�б�
	MLock							m_lckSrvInfo;	//������Ϣ��
	std::thread						*m_pThDeamenConnect;	//�ػ������߳�
	HANDLE							m_hDeamenConnect;		//�ػ������¼�
};
