#pragma once

#ifdef _DEBUG
#pragma comment(lib, "../Framework/lib/Debug/Framework.lib")
#pragma comment(lib, "../Framework/lib/Log/Debug/Log.lib")
#else
#pragma comment(lib,"../Framework/lib/Release/Framework.lib")
#pragma comment(lib, "../Framework/lib/Log/Release/Log.lib")
#endif
