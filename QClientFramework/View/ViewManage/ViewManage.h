#pragma once

#include <QObject>
#include "IViewManage.h"

class ViewMediator;
class MMainWindow;
class QSplashScreen;

class ViewManage : public QObject, public IViewManage
{
	Q_OBJECT
public:
	ViewManage(IMainClient *_main = nullptr);
    ~ViewManage() override;

	bool Start() override;
	void Stop() override;
	bool ExecuteLogin() override;

	/*************************************************************************
	* function: 显示弹窗消息
	* param uType:消息类型
	* param strMsg:显示内容
	* return:   1=YES 0=NO
	*************************************************************************/
	unsigned PopupShowMessage(unsigned uType, std::string strTitle, std::string strMsg);

private slots:
	void slotSplashMessage(std::string strMsg);
	void slotShowMainWindow(unsigned uUserType);
	void slotPopupShowMessage(unsigned uType, std::string strTitle, std::string strMsg);

private:
    ViewMediator    *m_pViewMdt;        //视图中介者
	MMainWindow     *m_pMainWnd;        //主窗体
	QSplashScreen	*m_pSplash;			//闪屏窗体
};

