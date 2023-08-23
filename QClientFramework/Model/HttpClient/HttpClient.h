/********************************************************************
Copyright (C), 2017-2020, Chongqing Ambition Polytron Technologies Inc.
��������:2017/9/8
�� �� ��:uis_engine.h
�� �� ��:wsj

��    ��:uisϵͳ���������ã��������ݷַ�,�źŵ���,��Դ�������ȣ�
�޸���ʷ��
*********************************************************************/
#pragma once
#include <functional>
#include <map>
#include "../http_service_moudle/http_service_interface.h"


//receive_proc�ص�����
//�ѽ��յ����ݽ��������

typedef std::function<void(int req_moudle_id, const std::string& data, bool result)> RECEIVE_CALLBACK;

class HttpClient : public http_service_callback
{
public:
	HttpClient();
	~HttpClient();
public:

	//��������
	BOOL start();

	//ֹͣ����
	BOOL stop();
	//����http����ͷ�û�ID
	void set_user_id(unsigned user_id);

	//*ע���ע���첽�������ݵĻص�����
	BOOL advise_receive_proc(std::string cmd_name, RECEIVE_CALLBACK pfn_receive);

	/*************************************************************************
	* function��http_service_callback�ӿڵ�ʵ��
	* param cmd_name:  in����������
	* param strData:  in������
	*************************************************************************/
	virtual void http_service_receive_callback(int req_moudle_id, const std::string cmd_name, std::string strData, bool success);

	/*************************************************************************
	* function��			�����첽����
	* param url:			�����ַ
	* param cmd_name:		��������
	* param strRequest:		����
	* param method:			����
	* param req_moudle_id:	����ģ��ID
	*************************************************************************/
	int http_asyn_send(const char* url, const char* cmd_name, std::string strRequest,
        int req_moudle_id = 0, HTTP_METHOD method = HTTP_METHOD::HTTP_METHOD_POST);

	// ͬ���������ݽӿ�
	std::string syn_send(const char* url,const char* cmd_name,std::string strRequest,
		HTTP_METHOD method = HTTP_METHOD::HTTP_METHOD_POST);

	std::string get_file_data(const char* url, const char* cmd_name, std::string strRequest);

	/*************************************************************************
	* function��	ͬ���ϴ��ļ�
	* param strUrl: �����url��ַ
	* param strFileName:���ص��ļ���
	* return:			����Ͷ�ݵĽ������
	*************************************************************************/
	int syn_upload_file(const std::string &strUrl, const std::string &strFilePath);

	/*************************************************************************
	* function��	ͬ�������ļ�
	* param strUrl: �����url��ַ
	* param strFileName:���ص��ļ���
	* return:			����Ͷ�ݵĽ������
	*************************************************************************/
	int syn_download_file(const std::string &strUrl, const std::string &strFilePath);

private: // noncopyable
	HttpClient(HttpClient const&);
	HttpClient& operator=(HttpClient const&);

private:
	http_service_out_ptr m_http_service_ptr;//http����
	std::map<std::string, RECEIVE_CALLBACK> m_mapCallback;
};
