#pragma once
/************************************************************************
* filename:MainController.h
* function:主控制器，用于程序启动以及用户登陆业务的处理。
* author :	明巧文
* datetime:2018-12-14
* company:
/************************************************************************/

#include <QThread>
#include <thread>
#include "ControllerColleague.h"
#include "..\..\CommonFile\TypeDefine.h"

class MainModel;
struct SDataExchange;
class ServerConnect;
namespace std{ class thread; }

class MainController : public ControllerColleague
{
    Q_OBJECT
public:
    explicit MainController(ControllerManage*);
    ~MainController() override;

    bool Initialize();
	void Uninitialize();

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

private slots:
	/********************以下为外部异步执行请求的实现槽********************/
	//用户登陆
	void slotUserLogin(std::string strUserName, std::string strPassword);

	//执行系统
	void slotExecuteSystem();


private slots:
	/*************************以下为本类自定槽函数*************************/
	/*************************************************************************
	* function：		 Tcp连接状态通知
	* param uServerType: 服务器类型
	* param state:		 连接成功标识
	* param once:		 第一次连接标识
	*************************************************************************/
	void slotTcpConnectNotify(unsigned uServerType, bool bSuccess);
	void slotTcpDisconnectNotify(unsigned uServerType);

private:
	/*************************以下为消息回调函数*************************/
	void HandleHeartbeat();

	//处理登录响应
	void HandleLoginRs(const unsigned uUserKey, SDataExchange* pMsg);

private:
	ServerConnect* m_pSrvCnnt = nullptr;		//服务端连接
    QThread			m_thread;                   //异步线程
	std::thread		 *m_pHeartbeatThread = nullptr;	//心跳线程
	HANDLE 			 m_hHeartbeatEvent = nullptr;	//心跳事件

	SUserInfo		m_user;
	QSet<unsigned>	m_setSrvUser;
	MLock			m_lckSrvUser;
};

