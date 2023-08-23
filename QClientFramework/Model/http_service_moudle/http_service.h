/********************************************************************
Copyright (C), 2017-2020, Chongqing Ambition Polytron Technologies Inc.
创建日期:2017/9/8
文 件 名:http_service.h
创 建 人:wsj
描    述:http/https客户端服务；
修改历史：
*********************************************************************/
#pragma once
#include <memory>
#include "base_helper.h"
#include "..\Utility\ds_thread.h"
#include "..\Utility\ds_lock.h"

//http请求上下文信息
typedef struct tag_http_client_context
{
	int     m_request_id =0;
	string  m_url;         //url地址    
	string  m_cmd_name;    //命令码
	int     m_req_moudle_id; //请求的模块ID
}http_client_context;


class http_service :public ds_thread, public http_service_interface
{
public:
	http_service(http_service_callback* pcallback);
	~http_service();
	//获取唯一GUID
	std::string make_guid();
public:
	/*
	*启动服务
	*/
	virtual BOOL start();
	/*
	*停止服务
	*/
	virtual BOOL stop();
	/*
	*设置ssl的证书，密钥
	*参数 ssl_cert_path 证书文件路径
	*     ssl_cert_type 证书类型
	*     ssl_cert_type 证书密码
	*     ssl_key_path 私钥文件路径
	*     ssl_key_type 私钥类型
	*     ssl_key_passwd 私钥密码
	*/
	virtual void set_setup_ssl(const  char* ssl_cert_path,
		const  char* ssl_cert_type,
		const  char* ssl_cert_passwd,
		const  char* ssl_key_path,
		const  char* ssl_key_type,
		const  char* ssl_key_passwd);
	/*
	*说明：设置授权信息
	*参数：access_key_id     秘钥ID
	*	   access_key_secret 秘钥密文
	*/
	void set_access_key(const  char* access_key_id, const char* access_key_secret);
    /*
    *说明：设置接口调用者uId
    *参数：user_id     接口调用者uId
    */
    void set_operator(const  char* user_id);
	/*
	*http数据回调
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

	//发送数据
    void do_send(http_client_context *p_context,
		const string& url,
		const char* data,
		unsigned int len,
		HTTP_METHOD method);

protected:
	/*
	*初始工作
	*/
	BOOL initialize();
	/*
	*清理工作，不论初始化成功与否均调用
	*/
	void uninitialize();
	/*
	*处理接收的数据
	*/
	void on_get(ds_node* pNode);
	/*
	*执行定时任务
	*/
	void on_timer();
private:
	string make_url(const char* remote_address, const char* cmd_name);
	http_client_context* get_http_client_context(const int id);
	void delete_http_client_context(const int id);

private:
	ds_lock  m_http_client_map_lock;
	map<int, http_client_context*> m_http_client_map;//http上下文集

	http_service_callback* m_pcallback = nullptr;

private:
	string m_ssl_cert_path;
	string m_ssl_cert_type;
	string m_ssl_cert_passwd;
	string m_ssl_key_path;
	string m_ssl_key_type;
	string m_ssl_key_passwd;
	string m_access_key_id;     //秘钥ID
	string m_access_key_secret; //秘钥密文
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
//发生数据节点
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