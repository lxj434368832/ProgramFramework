#pragma once

#include <QObject>

class IMessageHandle;
struct SPbMsg;

class IServerConnect : public QObject
{
	Q_OBJECT
public:
	static IServerConnect* Create();
	static void Delete(IServerConnect*& p);

	IServerConnect() = default;
	virtual ~IServerConnect() = default;

	virtual bool Initialize(IMessageHandle* pMsgHdl, unsigned uThreadCount) = 0;
	virtual void Uninitialize() = 0;

	/*************************************************************************
	* function������һ��������Ҫ�����Ƿ���Ҫ����
	* param ip port: ��Ҫ���ӵ�ip�Ͷ˿ں�
	* param pIO:	 �������ʱҪ�������ݣ���Ҫ����pIO
	* param iRecnnt: �Ƿ�������ʶ,С��0������Ҫ����
	* return:		 ���ش����Ӷ�Ӧ��id,�����������ӳɹ���Ϊ0�������ӳ����˴���
	*************************************************************************/
	virtual bool AddConnect(unsigned uSrvKey, std::string ip, ushort port, int iRecnnt = -1) = 0;

	/*************************************************************************
	* function�� ��������
	* param uMsgType: ��Ϣ����
	* param msg:	 ��Ҫ���͵�����
	*************************************************************************/
	virtual void SendData(unsigned uSrvKey, SPbMsg &msg) = 0;


	/*************************************************************************
	* function�� �Ͽ����ӣ��ײ��ص�DeleteUser����
	* param key: �û�id
	* return:	 ��
	*************************************************************************/
	virtual void Disconnect(unsigned uSrvKey) = 0;

Q_SIGNALS:
	void signalTcpConnectNotify(unsigned uSrvKey, bool bSuccess);
	void signalTcpDisconnectNotify(unsigned uSrvKey);

};
