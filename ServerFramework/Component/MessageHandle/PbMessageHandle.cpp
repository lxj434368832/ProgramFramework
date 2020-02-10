#include "PbMessageHandle.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../CommonFile/EnumDefine.h"
#include "Message.pb.h"
#include "PbMessageFactory.h"
#include <thread>

struct SDataExchange;

struct SMessageData
{
	unsigned		uUserKey;
	unsigned		uMsgType;
	SDataExchange*	pDataExg;
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

void PbMessageHandle::RegisterMessageFunction(unsigned msgType, funMessageHandle handle)
{
	if (handle)	
		m_mapMsgHandle[msgType] = handle;
	else
	{
		auto iter = m_mapMsgHandle.find(msgType);
		if (iter != m_mapMsgHandle.end())
		{
			m_mapMsgHandle.erase(iter);
		}
	}

}

bool PbMessageHandle::Initialize(unsigned uThreadCount)
{
	if (uThreadCount == 0)
		uThreadCount = 2;

	m_bStart = true;

	// 处理用户数据的线程
	for (unsigned i = 0; i < uThreadCount; i++)
	{
		m_threadList.push_back(new std::thread(std::bind(&PbMessageHandle::MessageHandleThread, this)));
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
	msgData->pDataExg = PbMessageFactory::GetMsgStruct(uMsgType, data, length);
	if (nullptr == msgData->pDataExg)
	{
		m_rscMessage.put(msgData);//消息解析失败，直接返回
		return;
	}

	m_mutexMsgList.lock();
	m_msgList.push(msgData);
	m_cvConsumer.notify_one();
	m_mutexMsgList.unlock();
}

void PbMessageHandle::MessageHandleThread()
{
	logm() << "MessageHandleThread start.ThreadID:" << std::this_thread::get_id() << std::endl;
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
			LOGE("消息:%d没有对应的处理方法。", msgData->uMsgType);
		}

		m_rscMessage.put(msgData);
	}

	logm() << "ThreadID:" << std::this_thread::get_id() << "退出";
}
