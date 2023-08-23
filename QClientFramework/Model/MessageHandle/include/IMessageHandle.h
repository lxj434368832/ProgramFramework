#pragma once

#include <functional>

struct SDataExchange;

typedef std::function<void(const unsigned uUserKey, SDataExchange *msg)> funMessageHandle;

class IMessageHandle
{
public:
	static IMessageHandle* CreatePb();
	static void Delete(IMessageHandle*& p);
	IMessageHandle(void *pMain = nullptr):m_pMain(pMain){}
	virtual ~IMessageHandle() { m_pMain = nullptr; }
	//inline void* GetMainClient() { return m_pMain; }

	virtual bool Initialize(unsigned uThreadCount) = 0;
	virtual void Uninitialize() = 0;

	virtual void RegisterMsgFunction(unsigned, funMessageHandle) = 0;
	virtual void RemoveMsgFunction(unsigned msgType) = 0;

	//处理protobuf数据
	virtual void AddMessageData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) = 0;

protected:
	void*	m_pMain;
};
