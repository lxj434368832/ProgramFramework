#pragma once

#ifdef _DEBUG
#pragma comment(lib, "../../Framework/lib/Frameworkd.lib")
#pragma comment(lib, "../../Framework/lib/Log/Logd.lib")
#pragma comment(lib, "../../Debug/IOCPCommunication.lib")
#pragma comment(lib, "../../protobuf/lib/libprotobufd.lib")
#pragma comment(lib, "../3rdParty/MLog/lib/MLogd.lib")
#pragma comment(lib, "../3rdParty/MConfigManage/lib/MConfigManaged.lib")
#pragma comment(lib, "../3rdParty/MFramelessWidget/lib/MFramelessWidgetd.lib")
#pragma comment(lib, "../3rdParty/ReadWriteExcel/lib/ReadWriteExceld.lib")

#else
#pragma comment(lib, "../../Framework/lib/Framework.lib")
#pragma comment(lib, "../../protobuf/lib/libprotobuf.lib")
#pragma comment(lib, "../../Framework/lib/Log/Log.lib")
#pragma comment(lib, "../../Release/IOCPCommunication.lib")
#pragma comment(lib, "../3rdParty/MLog/lib/MLog.lib")
#pragma comment(lib, "../3rdParty/MConfigManage/lib/MConfigManage.lib")
#pragma comment(lib, "../3rdParty/MFramelessWidget/lib/MFramelessWidget.lib")
#pragma comment(lib, "../3rdParty/ReadWriteExcel/lib/ReadWriteExcel.lib")
#endif
