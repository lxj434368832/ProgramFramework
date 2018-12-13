#pragma once
/*************************************************************************
* function��iocpͨѶ�����ļ�
* author :	������
* datetime��2017-12-14
* company:  
*************************************************************************/
#include "IOCPBase.h"

class IOCPClient :public IOCPBase
{
  public:
	  IOCPClient(INetInterface *pNet);
	  virtual ~IOCPClient();

	  /*************************************************************************
	  * function������һ��������Ҫ�����Ƿ���Ҫ����
	  * param ip port: ��Ҫ���ӵ�ip�Ͷ˿ں�
	  * param pIO:	 �������ʱҪ�������ݣ���Ҫ����pIO
	  * param iRecnnt: �Ƿ�������ʶ,С��0��������Ҫ����
	  * return:		 ���ش����Ӷ�Ӧ��id,�����������ӳɹ���Ϊ0�������ӳ����˴���
	  *************************************************************************/
	  bool StartConnect(unsigned uUserKey, std::string ip, u_short port, int iRecnnt = -1);

};
