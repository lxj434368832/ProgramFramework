#include "NetworkHelp.h"
#include <QList>
#include <QNetworkInterface>
#include <QStandardPaths>
#include <QHostInfo>


NetworkHelp::NetworkHelp()
{
}


NetworkHelp::~NetworkHelp()
{
}

QString NetworkHelp::GetIpAdress()
{
    QString localIPAddress = "";
	QList<QHostAddress> listAddress = QNetworkInterface::allAddresses();
	for (auto &addr : listAddress)
	{
		if (!addr.isNull() &&
			addr.protocol() == QAbstractSocket::IPv4Protocol &&
			addr != QHostAddress::LocalHost)
		{
			localIPAddress = addr.toString();
			return localIPAddress;
		}
	}
    return localIPAddress;
}

QString NetworkHelp::GetUserName()
{
    QString userName =QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    userName = userName.section("/", -1, -1);
    return QString();
}

QString NetworkHelp::GetMachineName()
{
    return  QHostInfo::localHostName();
}

QString NetworkHelp::GetMACAdress()
{
	QString	qstrMac;
	QList<QNetworkInterface>  listNet = QNetworkInterface::allInterfaces();//获取所有网卡信息
	for (auto &net : listNet)
	{
		QNetworkInterface::InterfaceFlags flags = net.flags();
		//判断该网卡是否是合法
		if (net.isValid() && flags.testFlag(QNetworkInterface::IsUp)
			&& flags.testFlag(QNetworkInterface::IsRunning)
			&& !flags.testFlag(QNetworkInterface::IsLoopBack))
		{
			qstrMac = net.hardwareAddress();
			//qstrMac.replace(':', '-');
			break;
		}
	}
	return qstrMac;   //获取该网卡的MAC
}
