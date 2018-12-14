#include "ProtobufMsgFactory.h"
#include "../CommonDefine.h"
#include <thread>



ProtobufMsgFactory::ProtobufMsgFactory() 
	:m_rscMessage(100)
{
	m_bStart = false;
}

ProtobufMsgFactory::~ProtobufMsgFactory()
{
}

ProtobufMsgFactory* ProtobufMsgFactory::instance()
{
	static ProtobufMsgFactory s_instance;
	return &s_instance;
}

void ProtobufMsgFactory::RegisterMessageFunction(pbmsg::MSG msg_type, funMessageHandle handle)
{
	m_mapMsgHandle[msg_type] = handle;
}

void ProtobufMsgFactory::RemoveMessageFunction(pbmsg::MSG msgType)
{
	auto iter = m_mapMsgHandle.find(msgType);
	if (iter != m_mapMsgHandle.end())
	{
		m_mapMsgHandle.erase(iter);
	}
}

bool ProtobufMsgFactory::Start(unsigned uThreadCount)
{
	if (uThreadCount == 0)
		uThreadCount = 1;

	m_bStart = true;

	// 处理用户数据的线程
	for (unsigned i = 0; i < uThreadCount; i++)
	{
		m_threadList.push_back(new std::thread(std::bind(&ProtobufMsgFactory::MessageHandleThread, this)));
	}

	return true;
}

void ProtobufMsgFactory::Stop()
{
	if (false == m_bStart) return;
	m_bStart = false;

	m_cvConsumer.notify_all();
	for (auto value : m_threadList)
	{
		value->detach();
	}
	m_threadList.clear();

	m_mapMsgHandle.clear();
}

void ProtobufMsgFactory::AddMessageData(unsigned uUserKey, const char* data, unsigned length)
{
	if (false == m_bStart) return;
	SMessageData *msgData = m_rscMessage.get();
	msgData->m_uUserKey = uUserKey;
	msgData->m_msg.ParseFromArray(data, length);

	m_mutexMsgList.lock();
	m_msgList.push(msgData);
	m_mutexMsgList.unlock();

	m_cvConsumer.notify_one();
}

void ProtobufMsgFactory::MessageHandleThread()
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

		auto it = m_mapMsgHandle.find(msgData->m_msg.msg_type());
		if (it != m_mapMsgHandle.end())
		{
			(it->second)(msgData->m_uUserKey, msgData->m_msg, nullptr);
		}
		else
		{
			loge() << "消息 " << msgData->m_msg.msg_type() << " 没有对应的处理方法。";
		}

		m_rscMessage.put(msgData);
	}
}
