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
#include <QSet>
#include "ControllerColleague.h"
#include "..\..\..\CommonFile\TypeDefine.h"
#include "..\..\..\3rdParty\Framework\include\MLock.h"
#include "..\include\IMainController.h"

struct SDataExchange;

class MainController : public IMainController, public ControllerColleague
{
    Q_OBJECT
public:
	explicit MainController(IControllerManage*);
    ~MainController();

    bool Initialize() override;
	void Uninitialize() override;

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
    QThread			m_thread;                   //异步线程
	std::thread		 *m_pHeartbeatThread = nullptr;	//心跳线程
	HANDLE 			 m_hHeartbeatEvent = nullptr;	//心跳事件

	SUserInfo		m_user;
	QSet<unsigned>	m_setSrvUser;
	MLock			m_lckSrvUser;
};

