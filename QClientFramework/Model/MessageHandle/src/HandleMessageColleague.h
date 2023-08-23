#pragma once
/*************************************************************************
* function����Ϣ����ͬ���࣬������Ҫ������Ϣ���඼�̳д���,��Ҫ���ڴ洢��Ҫ���ⲿ��
* author :	������
* datetime��2018-12-14
* company:
*************************************************************************/

class IMessageHandle;
class ICommunication;
class MainController;

class HandleMessageColleague
{
public:
	HandleMessageColleague(IMessageHandle *pHandle = nullptr);
	virtual ~HandleMessageColleague();

	bool Initialize();
	void Uninitialize();

protected:
	IMessageHandle		*m_pMsgHandle = nullptr;
	ICommunication	*m_pTcpCmmnt = nullptr;
	MainController		*m_pMainCtrl = nullptr;		//��������
};