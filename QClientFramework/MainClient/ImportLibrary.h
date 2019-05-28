#pragma once

#ifdef _DEBUG
#pragma comment(lib, "../3rdParty/Framework/lib/Frameworkd.lib")
#pragma comment(lib, "../3rdParty/IOCPCommunication/lib/IOCPCommunicationd.lib")
#pragma comment(lib, "../3rdParty/protobuf/lib/libprotobufd.lib")
#pragma comment(lib, "../3rdParty/MConfigManage/lib/MConfigManaged.lib")
#pragma comment(lib, "../3rdParty/MFramelessWidget/lib/MFramelessWidgetd.lib")

#else
#pragma comment(lib, "../3rdParty/Framework/lib/Framework.lib")
#pragma comment(lib, "../3rdParty/protobuf/lib/libprotobuf.lib")
#pragma comment(lib, "../3rdParty/IOCPCommunication/lib/IOCPCommunication.lib")
#pragma comment(lib, "../3rdParty/MConfigManage/lib/MConfigManage.lib")
#pragma comment(lib, "../3rdParty/MFramelessWidget/lib/MFramelessWidget.lib")
#endif
