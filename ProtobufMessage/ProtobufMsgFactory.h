#pragma once

#include <thread>
#include <queue>
#include <condition_variable>
#include "Message.pb.h"
#include "../ResourceManage.h"

typedef std::function<void(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr)> funMessageHandle;

class ProtobufMsgFactory
{
public:
	ProtobufMsgFactory();
	~ProtobufMsgFactory();
	static ProtobufMsgFactory* instance();
	void RegisterMessageFunction(pbmsg::MSG, funMessageHandle);
	void RemoveMessageFunction(pbmsg::MSG msgType);
	bool Start(unsigned uThreadCount);
	void Stop();

	//处理服务数据
	void AddMessageData(unsigned uUserKey, const char* data, unsigned length);

private:
	void MessageHandleThread();

private:
	struct SMessageData 
	{
		unsigned		m_uUserKey;
		pbmsg::Message	m_msg;
	};

	bool						m_bStart;			//是否开始的标识
	std::vector<std::thread*>	m_threadList;		//线程池列表
	mqw::ResourceManage<SMessageData> m_rscMessage;	//消息资源管理列表
	std::queue<SMessageData*>	m_msgList;			//待处理的消息队列
	std::mutex					m_mutexMsgList;
	std::condition_variable		m_cvConsumer;

	std::map<pbmsg::MSG, funMessageHandle> m_mapMsgHandle;
};

