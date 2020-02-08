#pragma once

#include "../../CommonFile/TypeDefine.h"
#include "../../3rdParty/protobuf/include/Message.pb.h"

class PbMessageFactory
{
public:
	PbMessageFactory();
	~PbMessageFactory();
	static SDataExchange* GetMsgStruct(unsigned eMsgType, const char* data, unsigned length);
};

