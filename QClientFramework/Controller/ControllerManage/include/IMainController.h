#pragma once

#include <QObject>

class IControllerManage;

class IMainController : public QObject
{
	Q_OBJECT
public:
	static IMainController* Create(IControllerManage*);
	static void Delete(IMainController*&);
	explicit IMainController() = default;
	virtual ~IMainController() = default;

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;

signals:
	/*************************以下为对外的通知消息*************************/
	//登陆提示消息
	void signalLoginTipMsg(std::string strMsg);
	//登陆消息通知
	void signalLoginMessageNt(bool bSuccess, std::string strMsg);
	//发送闪屏消息
	void signalSplashMessage(std::string strMsg);
	//通知显示主窗体
	void signalShowMainWindow(unsigned uUserType);

	//弹窗显示消息
	void signalPopupShowMessage(unsigned uType, std::string strTitle, std::string strMsg);

signals:
	/************************以下为外部异步执行请求************************/
	//用户登陆
	void signalUserLogin(std::string strUserName, std::string strPassword);
	//执行系统
	void signalExecuteSystem();

};
