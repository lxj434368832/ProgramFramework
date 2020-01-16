#include "ClientService.h"
#include <windows.h>
#include <DbgHelp.h>
#include <time.h>
#include <QMessageBox>
#include <QApplication>

#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "version.lib")

ClientService::ClientService()
{
}


ClientService::~ClientService()
{
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	char dumpFile[256] = { 0 };
	time_t now;
	struct tm *t_tm;
	time(&now);
	t_tm = localtime(&now);
	sprintf_s(dumpFile, sizeof(dumpFile), "dump\\WPOClient%4d%02d%02d%02d%02d%02d.dmp",
		t_tm->tm_year + 1900,
		t_tm->tm_mon + 1,
		t_tm->tm_mday,
		t_tm->tm_hour,
		t_tm->tm_min,
		t_tm->tm_sec);

	//创建 Dump 文件  
	HANDLE hDumpFile = ::CreateFileA(dumpFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDumpFile != INVALID_HANDLE_VALUE)
	{
		//Dump信息  
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ExceptionPointers = pException;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ClientPointers = TRUE;
		//写入Dump文件内容  
		::MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
		::CloseHandle(hDumpFile);
	}

	//这里弹出一个错误对话框并退出程序  
	EXCEPTION_RECORD* record = pException->ExceptionRecord;
	QString errCode(QString::number(record->ExceptionCode, 16)), errAdr(QString::number((uint)record->ExceptionAddress, 16)), errMod;
	QMessageBox::critical(NULL, QString::fromLocal8Bit("程式崩溃"), QString::fromLocal8Bit("<FONT size=4><div><b>对于发生的错误，表示诚挚的歉意</b><br/></div>") +
		QString::fromLocal8Bit("<div>错误代码：%1</div><div>错误地址：%2</div></FONT>").arg(errCode).arg(errAdr),
		QMessageBox::Ok);
	return EXCEPTION_EXECUTE_HANDLER;
}

void ClientService::InstallAppCrashHandler()
{
#ifndef _DEBUG
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);//注册异常捕获函数  
#endif // !_DEBUG

}

bool ClientService::IsApplicationRun(std::string strAppName)
{
	HANDLE hMutex = CreateMutexA(NULL, true, strAppName.data());
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		::CloseHandle(hMutex);
		QString qstrMsg = QString("<font size='12' color='red'>%1</font>").arg(QString::fromLocal8Bit("已经启动了一个程序,需要先关闭程序？"));
		QMessageBox::information(nullptr, QString::fromLocal8Bit("系统消息"), qstrMsg
				, QString::fromLocal8Bit("确定"));
		return true;
	}
	return false;
}

QString ClientService::GetAppVersion()
{
	QString fullName = QApplication::applicationFilePath();

	QString result = "";
	char* pData = nullptr;
	do
	{
		//获得文件基础信息
		DWORD dwLen = GetFileVersionInfoSize(fullName.toStdWString().c_str(), 0);
		if (0 == dwLen)
		{
			break;
		}
		pData = new char[dwLen + 1];
		BOOL bSuccess = GetFileVersionInfo(fullName.toStdWString().c_str(), 0, dwLen, pData);
		if (!bSuccess)
		{
			break;
		}
		struct LANGANDCODEPAGE
		{
			WORD wLanguage;
			WORD wCodePage;
		} *lpTranslate;
		LPVOID lpBuffer = nullptr;
		UINT uLen = 0;
		//获得语言和代码页(language and code page)
		bSuccess = VerQueryValue(pData, (TEXT("\\VarFileInfo\\Translation")), (LPVOID*)&lpTranslate, &uLen);
		if (!bSuccess)
		{
			break;
		}
		QString str1, str2;
		str1.setNum(lpTranslate->wLanguage, 16);
		str2.setNum(lpTranslate->wCodePage, 16);
		str1 = "000" + str1;
		str2 = "000" + str2;
		//获得文件版本信息
		QString verPath = "\\StringFileInfo\\" + str1.right(4) + str2.right(4) + "\\FileVersion";
		bSuccess = VerQueryValue(pData, (verPath.toStdWString().c_str()), &lpBuffer, &uLen);
		if (!bSuccess)
		{
			break;
		}
		result = QString::fromUtf16((const unsigned short int *)lpBuffer);
	} while (false);
	if (nullptr != pData)
	{
		delete pData;
	}
	return result;
}
