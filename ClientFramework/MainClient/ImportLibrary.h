#pragma once

#ifdef _DEBUG
#pragma comment(lib, "../../Framework/lib/Debug/Framework.lib")
#pragma comment(lib, "../../Framework/lib/Log/Debug/Log.lib")
#pragma comment(lib,"../../protobuf/lib/libprotobufd.lib")
#pragma comment(lib, "../../Debug/IOCPCommunication.lib")
#else
#pragma comment(lib, "../../Framework/lib/Release/Framework.lib")
#pragma comment(lib, "../../Framework/lib/Log/Release/Log.lib")
#pragma comment(lib, "../../protobuf/lib/libprotobuf.lib")
#pragma comment(lib, "../../Release/IOCPCommunication.lib")
#endif
