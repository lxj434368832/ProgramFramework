#pragma once

#ifdef _DEBUG
#pragma comment(lib, "../3rdParty/Framework/lib/Frameworkd.lib")
#pragma comment(lib, "../3rdParty/protobuf/lib/libprotobufd.lib")
#pragma comment(lib, "../3rdParty/IOCPCommunication/lib/IOCPCommunicationd.lib")
#else
#pragma comment(lib, "../3rdParty/protobuf/lib/libprotobuf.lib")
#pragma comment(lib, "../3rdParty/Framework/lib/Framework.lib")
#pragma comment(lib, "../3rdParty/IOCPCommunication/lib/IOCPCommunication.lib")
#endif
