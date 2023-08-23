#pragma once
/*************************************************************************
* function��iocpͨѶ�����ļ�
* author :	������
* datetime��2017-12-14
* company:  
*************************************************************************/


struct PER_SOCKET_CONTEXT;
struct PER_IO_CONTEXT;
struct IOCPBaseData;
class INetInterface;

class IOCPBase
{
public:
	IOCPBase() = default;
	IOCPBase(INetInterface *pNet);
	virtual ~IOCPBase();

protected:
	bool InitIOCP(unsigned uThreadCount);

	void UninitIOCP();

	/*************************************************************************
	* function��  ������Է���˵ļ���
	* param port: ���ؼ����Ķ˿ں�
	* param iMaxServerCount:�������Ӹ���
	* return:	  �ɹ�����true,ʧ�ܷ���false.
	*************************************************************************/
	virtual bool StartServerListen(u_short port, unsigned iMaxConnectCount);

	/*************************************************************************
	* function������һ��������Ҫ�����Ƿ���Ҫ����
	* param ip port: ��Ҫ���ӵ�ip�Ͷ˿ں�
	* param pIO:	 �������ʱҪ�������ݣ���Ҫ����pIO
	* param iRecnnt: �Ƿ�������ʶ,С��0������Ҫ����
	* return:		 ���ش����Ӷ�Ӧ��id,�����������ӳɹ���Ϊ0�������ӳ����˴���
	*************************************************************************/
	virtual bool AddConnect(unsigned uUserKey, std::string ip, u_short port, int iRecnnt = -1);

	//�������˲���
	void HandServerOperate(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);
	//Ͷ������
	bool PostConnectEx(PER_SOCKET_CONTEXT *pSkContext);
	//������������
	void HandConnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	//Ͷ�ݽ���
	bool PostAcceptEx(SOCKET listenSocket);
	void HandAccept(int iResult, PER_SOCKET_CONTEXT *pListenSkContext, PER_IO_CONTEXT* pIO);

	//
	void PostReceive(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void HandReceive(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	//������յ�������
	void UnpackReceivedData(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	/*************************************************************************
	* function�� ��������
	* param key: �û�key
	* param data:��Ҫ���͵�����
	* return:	 ��
	*************************************************************************/
	void Send(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength);

	//�������
	void PackSendData(PER_SOCKET_CONTEXT * pSkContext, unsigned uMsgType, const char* data, unsigned length);
	//����
	bool PostSend(PER_SOCKET_CONTEXT *pSkContext);

	void HandSend(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	/*************************************************************************
	* function�� �Ͽ�����
	* param key: �û�Key
	*************************************************************************/
	virtual void Disconnect(unsigned uUserKey);
	//
	void PostDisconnectEx(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void ReCycleSocketRsc(PER_SOCKET_CONTEXT * pSkContext, PER_IO_CONTEXT* pIO);

	void HandDisconnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

private:
	void WorkThread();

protected:
	IOCPBaseData *d;
};
