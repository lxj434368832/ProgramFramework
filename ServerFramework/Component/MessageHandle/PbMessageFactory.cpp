#include "PbMessageFactory.h"
#include "../../Framework/include/LogFile.h"



PbMessageFactory::PbMessageFactory()
{
}


PbMessageFactory::~PbMessageFactory()
{
}

SDataExchange* PbMessageFactory::GetMsgStruct(unsigned eMsgType, const char* data, unsigned length)
{
	SDataExchange *pData = nullptr;

	switch (eMsgType)
	{
	case pbmsg::EHeartbeatNt:
		pData = new SHeartBeatNt;
		break;
	case pbmsg::ELoginRq:
		pData = new SLoginRq;
		break;
	case pbmsg::ELogoutRq:
		break;
	default:
		loge() << "不能处理的消息类型:" << (int)eMsgType;
		break;
	}
	
	if (nullptr == pData) return pData;

	if (false == pData->ParseFromPb(data, length))
	{
		loge() << "解析eMsgType:" << eMsgType << " 的protobuf消息失败！";
		delete pData;
		pData = nullptr;
	}
	return pData;
}
