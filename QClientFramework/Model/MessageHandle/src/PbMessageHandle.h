#pragma once
/*************************************************************************
* function������protobuf��Ϣ
* author :	������
* datetime��2017-12-14
* company:
*************************************************************************/
#include <queue>
#include <thread>
#include <condition_variable>
#include "../../3rdParty/Framework/include/ResourceManage.h"
#include "../include/IMessageHandle.h"

struct SMessageData;

class PbMessageHandle : public IMessageHandle
{
public:
	PbMessageHandle(void * pMain = nullptr);
	~PbMessageHandle();

	void RegisterMsgFunction(unsigned msgType, funMessageHandle handle) override;
	void RemoveMsgFunction(unsigned msgType) override;

	bool Initialize(unsigned uThreadCount) override;
	void Uninitialize() override;

	//�����������
	void AddMessageData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

private:
	void MessageHandleThread();

private:
	void*						m_pMain;			//���ݸ���Ϣ�����ָ��
	bool						m_bStart;			//�Ƿ�ʼ�ı�ʶ
	std::vector<std::thread*>	m_threadList;		//�̳߳��б�

	mqw::ResourceManage<SMessageData> m_rscMessage;	//��Ϣ��Դ�����б�
	std::queue<SMessageData*>	m_msgList;			//���������Ϣ����
	std::mutex					m_mutexMsgList;
	std::condition_variable		m_cvConsumer;

	//����Ҫ����
	std::map<unsigned, funMessageHandle> m_mapMsgHandle;
};

