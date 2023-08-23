#pragma once

#include <QObject>
#include "IViewManage.h"

class ViewMediator;
class MMainWindow;
class QSplashScreen;
class IMainController;

class ViewManage : public QObject, public IViewManage
{
	Q_OBJECT
public:
	ViewManage(IMainClient *_main = nullptr);
    ~ViewManage() override;

	bool Initialize() override;
	void Uninitialize() override;
	bool StartClient() override;

	/*************************************************************************
	* function: ��ʾ������Ϣ
	* param uType:��Ϣ����
	* param strMsg:��ʾ����
	* return:   1=YES 0=NO
	*************************************************************************/
	unsigned PopupShowMessage(unsigned uType, std::string strTitle, std::string strMsg);

private slots:
	void slotSplashMessage(std::string strMsg);
	void slotShowMainWindow(unsigned uUserType);
	void slotPopupShowMessage(unsigned uType, std::string strTitle, std::string strMsg);

private:
	IMainController	*m_pMainCtrl;		//��������
    ViewMediator    *m_pViewMdt;        //��ͼ�н���
	MMainWindow     *m_pMainWnd;        //������
	QSplashScreen	*m_pSplash;			//��������
};
