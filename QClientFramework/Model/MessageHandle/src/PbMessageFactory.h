#pragma once

#include "../../CommonFile/TypeDefine.h"

class PbMessageFactory
{
public:
	static SDataExchange* CreateMsgStruct(unsigned eMsgType, const char* data, unsigned length);
};

