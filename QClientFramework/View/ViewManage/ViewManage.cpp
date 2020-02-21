#include "ViewManage.h"
#include "ViewMediator.h"
#include "../MainWindow/MMainWindow.h"
#include "../../CommonFile/CommonDefine.h"
#include "../MainWindow/LoginDialog.h"
#include "../../MainClient/IMainClient.h"
#include "../../Controller/ControllerManage/IControllerManage.h"
#include "../../Controller/ControllerManage/MainController.h"
#include "../../CommonFile/EnumDefine.h"
#include <QSplashScreen>
#include <QLabel>
#include <QMovie>
#include <QMessageBox>



ViewManage::ViewManage(IMainClient *_main)
:QObject()
,IViewManage(_main)
{
	m_pViewMdt = new ViewMediator(m_pMain);
	m_pMainWnd = new MMainWindow(m_pViewMdt);
	m_pSplash = new QSplashScreen(QPixmap("://Resource/SplashScreen.gif").scaled(500, 500));
	m_pSplash->setFont(QFont("Microsoft YaHei UI", 14));

	QLabel *lbSplash = new QLabel(m_pSplash);
	QMovie *splashGif = new QMovie("://Resource/SplashScreen.gif", QByteArray(), m_pSplash);
	splashGif->setScaledSize(QSize(500, 500));
	lbSplash->setMovie(splashGif);
	splashGif->start();

	m_pMainCtrl = m_pMain->GetControllerManage()->GetMainController();
	connect(m_pMainCtrl, SIGNAL(signalSplashMessage(std::string)), SLOT(slotSplashMessage(std::string)));
	connect(m_pMainCtrl, SIGNAL(signalShowMainWindow(uint)), SLOT(slotShowMainWindow(uint)));
	connect(m_pMainCtrl, SIGNAL(signalPopupShowMessage(uint, std::string, std::string)), SLOT(slotPopupShowMessage(uint, std::string, std::string)));
}

ViewManage::~ViewManage()
{
	RELEASE(m_pMainWnd);
	RELEASE(m_pViewMdt);
}

bool ViewManage::Initialize()
{
	return true;
}

void ViewManage::Uninitialize()
{
}

bool ViewManage::StartClient()
{
	LoginDialog lgDlg(m_pViewMdt);
	if (QDialog::Accepted == lgDlg.exec())
	{
		m_pSplash->show();
		m_pSplash->showMessage(QString::fromLocal8Bit("正在启动系统..."), Qt::AlignBottom, Qt::white);
		
		emit m_pMainCtrl->signalExecuteSystem();
		return true;
	}
	else
	{
		RELEASE(m_pSplash);
		return false;
	}
}

void ViewManage::slotSplashMessage(std::string strMsg)
{
	if (m_pSplash)
	{
		m_pSplash->showMessage(QString::fromLocal8Bit(strMsg.c_str()), Qt::AlignBottom, Qt::white);
	}
}

unsigned ViewManage::PopupShowMessage(unsigned uType, std::string strTitle, std::string strMsg)
{
	QString qstrMsg = QString("<font size='12' color='red'>%1</font>").arg(QString::fromLocal8Bit(strMsg.c_str()));
	int iRet = 0;
	switch (uType)
	{
	case EPopupMsgType::EPMT_INFORM:
		logm() << strMsg;
		iRet = QMessageBox::information(nullptr, QString::fromLocal8Bit(strTitle.c_str()), qstrMsg
			, QString::fromLocal8Bit("确定"));
		break;
	case EPopupMsgType::EPMT_WARNING:
		logw() << strMsg;
		iRet = QMessageBox::warning(nullptr, QString::fromLocal8Bit(strTitle.c_str()), qstrMsg
			, QString::fromLocal8Bit("确定"));
		break;
	case EPopupMsgType::EPMT_CRITICAL:
		loge() << strMsg;
		iRet = QMessageBox::critical(nullptr, QString::fromLocal8Bit(strTitle.c_str()), qstrMsg
			, QString::fromLocal8Bit("确定"));
		break;
	case EPopupMsgType::EPMT_QUESTION:
		logm() << strMsg;
		iRet = QMessageBox::question(nullptr, QString::fromLocal8Bit(strTitle.c_str()), qstrMsg
			, QString::fromLocal8Bit("确定"), QString::fromLocal8Bit("取消"));
		break;
	default:
		loge() << "未知显示消息类型！";
		break;
	}
	return iRet;
}

void ViewManage::slotShowMainWindow(unsigned uUserType)
{
	if (nullptr == m_pSplash) return;
	/*
	switch (uUserType)
	{
	case EUserType::EUT_WPOAllFunction:
	m_mainWnd->AddTabWidget(nullptr, "");
	break;
	case EUserType::EUT_WPOWorkstation:
	m_mainWnd->AddTabWidget(nullptr, "");
	break;
	case EUserType::EUT_WPOMonitorCenter:
	m_mainWnd->AddTabWidget(nullptr, "");
	break;
	default:
	break;
	}*/

	m_pMainWnd->show();
	m_pSplash->finish(m_pMainWnd);
	RELEASE(m_pSplash);
}

void ViewManage::slotPopupShowMessage(unsigned uType, std::string strTitle, std::string strMsg)
{
	PopupShowMessage(uType, strTitle, strMsg);
}
