#pragma once

#include <queue>
#include <thread>
#include <condition_variable>
#include "../../3rdParty/Framework/include/ResourceManage.h"

struct SDataExchange;
struct SMessageData;

typedef std::function<void(const unsigned uUserKey, SDataExchange *msg)> funMessageHandle;

class PbMessageHandle
{
public:
	PbMessageHandle(void * pMain = nullptr);
	~PbMessageHandle();

	void RegisterMessageFunction(unsigned, funMessageHandle);
	void RemoveMessageFunction(unsigned msgType);

	bool Initialize(unsigned uThreadCount);
	void Uninitialize();

	//处理服务数据
	void AddMessageData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length);

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

