#pragma once

#include "../3rdParty/IOCPCommunication/include/INetInterface.h"

class IMainServer;
class IOCPServer;
class UserInfoManage;
class PbMessageHandle;
struct SPbMsg;

class TCPServer : public INetInterface
{
public:
	TCPServer(IMainServer*);
	~TCPServer();

	bool Initialize();
	void Uninitialize();

	bool StartServer();
	void StopServer();

	/*************************************************************************
	* function�� ��������
	* param uMsgType: ��Ϣ����
	* param key:	  �û�id
	* param data:	 ��Ҫ���͵�����
	*************************************************************************/
	void SendData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned uLength);

	/*************************************************************************
	* function�� ��������
	* param uMsgType: ��Ϣ����
	* param msg:	 ��Ҫ���͵�����
	*************************************************************************/
	void SendData(UserKey uUserKey, SPbMsg &msg);

	void Disconnect(UserKey uUserKey);

private:
	//ʵ��INetInterface�ӿ�
	// ���û�����֪ͨ
	void ConnectNotify(UserKey uUserKey, bool bSuccess) override;

	// �û��Ͽ�����֪ͨ
	void DisConnectNotify(UserKey uUserKey) override;

	// �����������
	void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

private:
	IMainServer			*m_pMain;
	IOCPServer			*m_pIOCPServer;	//IOCP �����
	PbMessageHandle		*m_pPbMsgHandle;
	UserInfoManage		*m_pUserMng;
};
