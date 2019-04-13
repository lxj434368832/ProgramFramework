#pragma once

#ifdef _DEBUG
#pragma comment(lib, "../../Framework/lib/Frameworkd.lib")
#pragma comment(lib, "../../Framework/lib/Log/Logd.lib")
#pragma comment(lib, "../../protobuf/lib/libprotobufd.lib")
#pragma comment(lib, "../../Debug/IOCPCommunication.lib")
#else
#pragma comment(lib, "../../protobuf/lib/libprotobuf.lib")
#pragma comment(lib, "../../Framework/lib/Framework.lib")
#pragma comment(lib, "../../Framework/lib/Log/Log.lib")
#pragma comment(lib, "../../Release/IOCPCommunication.lib")
#endif
