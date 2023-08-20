//#include <vld.h>
#include "MainClient.h"
#include "../CommonFile/CommonDefine.h"
#include "../Component/Utility/ClientService.h"
#include "ImportLibrary.h"
#include <string>
#include <QApplication>
//#include <QTextCodec>

//#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    LogFile log;

	ClientService::InstallAppCrashHandler();
	if (ClientService::IsApplicationRun("QClientFramework"))
		return 0;

    IMainClient *pMain = new MainClient;
	if (pMain->StartClient())
		a.exec();
	else loge() << "�������ʧ�ܣ���鿴��־��";

    pMain->StopClient();
    RELEASE(pMain);
	return 0;
}

