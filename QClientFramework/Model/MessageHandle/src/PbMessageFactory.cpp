#include "PbMessageFactory.h"
#include "../../Framework/include/LogFile.h"
#include "../../3rdParty/protobuf/include/Message.pb.h"

SDataExchange* PbMessageFactory::CreateMsgStruct(unsigned eMsgType, const char* data, unsigned length)
{
	SDataExchange *pData = nullptr;

	switch (eMsgType)
	{
	case pbmsg::ELoginRs:
		pData = new SRespondMsg;
		break;
	case pbmsg::ELogoutRq:
		break;
	default:
		loge() << "���ܴ������Ϣ����:" << (int)eMsgType;
		break;
	}
	
	if (nullptr == pData) return pData;

	if (false == pData->ParseFromPb(data, length))
	{
		loge() << "����eMsgType:" << eMsgType << " ��protobuf��Ϣʧ�ܣ�";
		delete pData;
		pData = nullptr;
	}
	return pData;
}
