#pragma once
/*************************************************************************
* function��iocpͨѶ�����ļ�
* author :	������
* datetime��2017-12-14
* company: 
*************************************************************************/

#include "IOCPBase.h"

struct IOCPServerData;

class IOCPServer : public IOCPBase
{
public:
	IOCPServer();
	~IOCPServer();
	void InitData(INetInterface *pNet);

	bool StartServer(USHORT nPort, unsigned dwMaxConnection = 10, unsigned uThreadCount = 0);
	void StopServer();

	//������⺯���������̶߳�ʱ���á�
	void StartHeartbeatCheck();

	/*************************************************************************
	* function�� �������ݣ��ⲿ�ص�
	* param key: �û�id
	* param data:��Ҫ���͵�����
	* return:	 ��
	*************************************************************************/
	void SendData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength);

	/*************************************************************************
	* function�� �Ͽ����ӣ��ⲿ�ص�
	* param key: �û�id
	*************************************************************************/
	void Disconnect(unsigned uUserKey) override;

private:
	/*************************************************************************
	* function��  ������Է���˵ļ���
	* param port: ���ؼ����Ķ˿ں�
	* param iMaxServerCount:�������Ӹ���
	* return:	  �ɹ�����true,ʧ�ܷ���false.
	*************************************************************************/
	bool StartServerListen(u_short port, unsigned iMaxConnectCount) override;

private:
	IOCPServerData *d;
};
