/********************************************************************
Copyright (C), 2017-2020, Chongqing Ambition Polytron Technologies Inc.
��������:2017/9/8
�� �� ��:http_service.h
�� �� ��:wsj
��    ��:http/https�ͻ��˷���
�޸���ʷ��
*********************************************************************/
#pragma once
#include <memory>
#include "base_helper.h"
#include "..\Utility\ds_thread.h"
#include "..\Utility\ds_lock.h"

//http������������Ϣ
typedef struct tag_http_client_context
{
	int     m_request_id =0;
	string  m_url;         //url��ַ    
	string  m_cmd_name;    //������
	int     m_req_moudle_id; //�����ģ��ID
}http_client_context;


class http_service :public ds_thread, public http_service_interface
{
public:
	http_service(http_service_callback* pcallback);
	~http_service();
	//��ȡΨһGUID
	std::string make_guid();
public:
	/*
	*��������
	*/
	virtual BOOL start();
	/*
	*ֹͣ����
	*/
	virtual BOOL stop();
	/*
	*����ssl��֤�飬��Կ
	*���� ssl_cert_path ֤���ļ�·��
	*     ssl_cert_type ֤������
	*     ssl_cert_type ֤������
	*     ssl_key_path ˽Կ�ļ�·��
	*     ssl_key_type ˽Կ����
	*     ssl_key_passwd ˽Կ����
	*/
	virtual void set_setup_ssl(const  char* ssl_cert_path,
		const  char* ssl_cert_type,
		const  char* ssl_cert_passwd,
		const  char* ssl_key_path,
		const  char* ssl_key_type,
		const  char* ssl_key_passwd);
	/*
	*˵����������Ȩ��Ϣ
	*������access_key_id     ��ԿID
	*	   access_key_secret ��Կ����
	*/
	void set_access_key(const  char* access_key_id, const char* access_key_secret);
    /*
    *˵�������ýӿڵ�����uId
    *������user_id     �ӿڵ�����uId
    */
    void set_operator(const  char* user_id);
	/*
	*http���ݻص�
	*/
	void request_result_callback(int id, bool success, const std::string& data);

	virtual int http_asyn_send(const char* url,
		const char* cmd_name,
		const char* data,
		unsigned int len,
		HTTP_METHOD method,
		int req_moudle_id);

	virtual std::string http_syn_send(
		const char* url,
		const char* cmd_name,
		const char* data,
		unsigned int len,
		HTTP_METHOD method = HTTP_METHOD::HTTP_METHOD_POST);

	int syn_upload_file(const std::string &strUrl, const std::string &strFilePath) override;

	int syn_download_file(const std::string &strUrl, const std::string &strFilePath) override;

public:

	//��������
    void do_send(http_client_context *p_context,
		const string& url,
		const char* data,
		unsigned int len,
		HTTP_METHOD method);

protected:
	/*
	*��ʼ����
	*/
	BOOL initialize();
	/*
	*�����������۳�ʼ���ɹ���������
	*/
	void uninitialize();
	/*
	*������յ�����
	*/
	void on_get(ds_node* pNode);
	/*
	*ִ�ж�ʱ����
	*/
	void on_timer();
private:
	string make_url(const char* remote_address, const char* cmd_name);
	http_client_context* get_http_client_context(const int id);
	void delete_http_client_context(const int id);

private:
	ds_lock  m_http_client_map_lock;
	map<int, http_client_context*> m_http_client_map;//http�����ļ�

	http_service_callback* m_pcallback = nullptr;

private:
	string m_ssl_cert_path;
	string m_ssl_cert_type;
	string m_ssl_cert_passwd;
	string m_ssl_key_path;
	string m_ssl_key_type;
	string m_ssl_key_passwd;
	string m_access_key_id;     //��ԿID
	string m_access_key_secret; //��Կ����
    string m_user_id;
};

template<typename T>
class __declspec(novtable) interface_node : public ds_node
{
public:
	virtual void on_get(T* pOwner) = 0;
	bool equal(ds_node *node) 
	{
		return false;
	}
};
//�������ݽڵ�
template<typename T>
class send_node : public interface_node<T>
{
public:
	send_node(http_client_context* context_ptr,const string& url, const char* data, unsigned int len, HTTP_METHOD method)
		: m_context_ptr(context_ptr)
		, m_url(url)
		, m_data(data,len)
		, m_method(method)
	{
	}
	~send_node(){}
	void on_get(T* p_owner)
	{
		p_owner->do_send(m_context_ptr,m_url.c_str(), m_data.c_str(), m_data.size(), m_method);
	}
private:
	string m_url;
	string m_data;
	HTTP_METHOD m_method;
	http_client_context* m_context_ptr;
};