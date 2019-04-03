#pragma once

#ifdef _DEBUG
//#pragma comment(lib, "../../Framework/lib/Debug/Framework.lib")
//#pragma comment(lib, "../../Framework/lib/Log/Debug/Log.lib")
//#pragma comment(lib, "../../Debug/IOCPCommunication.lib")

#pragma comment(lib, "../3rdParty/MLog/lib/MLogd.lib")
#pragma comment(lib, "../3rdParty/MConfigManage/lib/MConfigManaged.lib")
#pragma comment(lib, "../3rdParty/MFramelessWidget/lib/MFramelessWidgetd.lib")
//#pragma comment(lib, "../3rdParty/ReadWriteExcel/lib/ReadWriteExceld.lib")

#else
//#pragma comment(lib, "../../Framework/lib/Release/Framework.lib")
//#pragma comment(lib, "../../Framework/lib/Log/Release/Log.lib")
//#pragma comment(lib, "../../Release/IOCPCommunication.lib")
#pragma comment(lib, "../3rdParty/MLog/lib/MLog.lib")
#pragma comment(lib, "../3rdParty/MConfigManage/lib/MConfigManage.lib")
#pragma comment(lib, "../3rdParty/MFramelessWidget/lib/MFramelessWidget.lib")
//#pragma comment(lib, "../3rdParty/ReadWriteExcel/lib/ReadWriteExcel.lib")
#endif
