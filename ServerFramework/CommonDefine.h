#pragma once

#include "../IOCPCommunication/stdafx.h"

#ifdef _DEBUG
#pragma comment(lib,"../protobuf/lib/libprotobufd.lib")
#else
#pragma comment(lib,"../protobuf/lib/libprotobuf.lib")
#endif
