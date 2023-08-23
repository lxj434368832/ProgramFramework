#include "PbMessageHandle.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../CommonFile/EnumDefine.h"
#include "Message.pb.h"
#include "PbMessageFactory.h"


IMessageHandle* IMessageHandle::CreatePb()
{
	return new PbMessageHandle();
}

void IMessageHandle::Delete(IMessageHandle*& p)
{
	RELEASE(p);
}


struct SMessageData
{
	unsigned		uUserKey;
	unsigned		uMsgType;
	SDataExchange*	pDataExg;
	inline void ResetData(){
		RELEASE(pDataExg);}
};


PbMessageHandle::PbMessageHandle(void * pMain)
	:m_rscMessage(MESSAGE_RESOURCE_COUNT)
{
	m_pMain = pMain;
	m_bStart = false;
}

PbMessageHandle::~PbMessageHandle()
{
	google::protobuf::ShutdownProtobufLibrary();
}

void PbMessageHandle::RegisterMsgFunction(unsigned msgType, funMessageHandle handle)
{
	m_mapMsgHandle[msgType] = handle;
}

void PbMessageHandle::RemoveMsgFunction(unsigned msgType)
{
	auto iter = m_mapMsgHandle.find(msgType);
	if (iter != m_mapMsgHandle.end())
	{
		m_mapMsgHandle.erase(iter);
	}
}

bool PbMessageHandle::Initialize(unsigned uThreadCount)
{
	if (uThreadCount == 0)
		uThreadCount = 2;

	m_bStart = true;

	// �����û����ݵ��߳�
	for (unsigned i = 0; i < uThreadCount; i++)
	{
		m_threadList.push_back(new std::thread(&PbMessageHandle::MessageHandleThread, this));
	}

	return true;
}

void PbMessageHandle::Uninitialize()
{
	m_pMain = nullptr;
	if (false == m_bStart) return;
	m_bStart = false;

	m_cvConsumer.notify_all();
	for (auto value : m_threadList)
	{
		if(value->joinable())
			value->join();
		delete value;
	}
	m_threadList.clear();

	m_mapMsgHandle.clear();
}

void PbMessageHandle::AddMessageData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length)
{
	if (false == m_bStart) return;
	SMessageData *msgData = m_rscMessage.get();

	msgData->uUserKey = uUserKey;
	msgData->uMsgType = uMsgType;
	msgData->pDataExg = PbMessageFactory::CreateMsgStruct(uMsgType, data, length);
	if (nullptr == msgData->pDataExg)
	{
		m_rscMessage.put(msgData);//��Ϣ����ʧ�ܣ�ֱ�ӷ���
		return;
	}

	m_mutexMsgList.lock();
	m_msgList.push(msgData);
	m_cvConsumer.notify_one();
	m_mutexMsgList.unlock();
}

void PbMessageHandle::MessageHandleThread()
{
	logm() << "MessageHandleThread start.ThreadID:" << std::this_thread::get_id();
	while (m_bStart)
	{
		SMessageData *msgData = nullptr;
		{
			std::unique_lock<std::mutex> lck(m_mutexMsgList);
			while (m_msgList.empty() && m_bStart)
			{
				m_cvConsumer.wait(lck);
			}
			if (false == m_bStart) break;
			msgData = m_msgList.front();
			m_msgList.pop();
		}

		auto it = m_mapMsgHandle.find(msgData->uMsgType);
		if (it != m_mapMsgHandle.end())
		{
			(it->second)(msgData->uUserKey, msgData->pDataExg);
		}
		else
		{
			LOGE("��Ϣ:%dû�ж�Ӧ�Ĵ�������", msgData->uMsgType);
		}
		msgData->ResetData();
		m_rscMessage.put(msgData);
	}

	logm() << "MessageHandleThread ThreadID:" << std::this_thread::get_id()<< "�˳�.";
}
