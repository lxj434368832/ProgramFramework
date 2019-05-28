#pragma once

#include <queue>
#include <thread>
#include <condition_variable>
#include "../../3rdParty/Framework/include/ResourceManage.h"

namespace pbmsg { class Message; }
struct SMessageData;

typedef std::function<void(const unsigned uUserKey, const pbmsg::Message *msg)> funMessageHandle;

class ProtobufMsgFactory
{
public:
	ProtobufMsgFactory(void * pMain = nullptr);
	~ProtobufMsgFactory();

	void RegisterMessageFunction(unsigned, funMessageHandle);
	void RemoveMessageFunction(unsigned msgType);

	bool Start(unsigned uThreadCount);
	void Stop();

	//处理服务数据
	void AddMessageData(unsigned uUserKey, const char* data, unsigned length);

private:
	void MessageHandleThread();

private:
	void*						m_pMain;			//传递给消息处理的指针
	bool						m_bStart;			//是否开始的标识
	std::vector<std::thread*>	m_threadList;		//线程池列表

	mqw::ResourceManage<SMessageData> m_rscMessage;	//消息资源管理列表
	std::queue<SMessageData*>	m_msgList;			//待处理的消息队列
	std::mutex					m_mutexMsgList;
	std::condition_variable		m_cvConsumer;

	std::map<unsigned, funMessageHandle> m_mapMsgHandle;
};

