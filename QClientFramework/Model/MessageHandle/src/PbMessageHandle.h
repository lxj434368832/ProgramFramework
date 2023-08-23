#pragma once
/*************************************************************************
* function：处理protobuf消息
* author :	明巧文
* datetime：2017-12-14
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

	//处理服务数据
	void AddMessageData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

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

	//可能要加锁
	std::map<unsigned, funMessageHandle> m_mapMsgHandle;
};

