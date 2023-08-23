#include "stdafx.h"
#include <iostream>
#include <sstream>
#include "http_service.h"
#include "http_request_header.h"
#include "HttpRequest.h"

http_service::http_service(http_service_callback* pcallback)
	:m_pcallback(pcallback)
{
}

http_service::~http_service()
{
	m_pcallback = nullptr;
}

std::string http_service::make_guid()
{
	char buf[64] = { 0 };
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		sprintf_s(buf, sizeof(buf)
			, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]
		);
	}
	return string(buf);
}

BOOL http_service::start()
{
	return open_thread(make_guid().c_str());
}

string http_service::make_url(const char* remote_address,const char* cmd_name)
{
	std::ostringstream ostr;
	ostr << remote_address
		 << cmd_name;

	return ostr.str();
}

BOOL http_service::stop()
{
	if (get_handle() != nullptr)
	{
		return close_thread();
	}

	return TRUE;
}

BOOL http_service::initialize()
{
	return TRUE;
}

void http_service::uninitialize()
{
	
}

void http_service::on_get(ds_node* pNode)
{
	dynamic_cast<interface_node<http_service>*>(pNode)->on_get(this);
}

void http_service::on_timer()
{

}

http_client_context* http_service::get_http_client_context(const int id)
{
	ds_auto_lock lock(&m_http_client_map_lock);
	auto item = m_http_client_map.find(id);
	if (item != m_http_client_map.end())
	{
		return item->second;
	}
	return nullptr;
}

void http_service::delete_http_client_context(const int id)
{
	ds_auto_lock lock(&m_http_client_map_lock);
	auto item = m_http_client_map.find(id);
	if (item != m_http_client_map.end())
	{
		delete item->second;
		item->second = nullptr;
		m_http_client_map.erase(item);
	}
}

void http_service::request_result_callback(int id, bool success, const std::string& data)
{
	http_client_context* p_context = get_http_client_context(id);
	if (p_context)
	{
		delete_http_client_context(id);
	}
}

void http_service::set_setup_ssl(const char* ssl_cert_path, const char* ssl_cert_type, const char* ssl_cert_passwd, const char* ssl_key_path, const char* ssl_key_type, const char* ssl_key_passwd)
{
	assert(ssl_cert_path
		&&ssl_cert_type
		&&ssl_cert_passwd
		&&ssl_key_path
		&&ssl_key_type
		&&ssl_key_passwd);
	m_ssl_cert_path = ssl_cert_path;
	m_ssl_cert_type = ssl_cert_type;
	m_ssl_cert_passwd = ssl_cert_passwd;
	m_ssl_key_path = ssl_key_path;
	m_ssl_key_type = ssl_key_type;
	m_ssl_key_passwd = ssl_key_passwd;
}

void http_service::set_access_key(const char* access_key_id, const char* access_key_secret)
{
	assert(access_key_id && access_key_secret);
	m_access_key_id = access_key_id;
	m_access_key_secret = access_key_secret;
}

void http_service::set_operator(const  char* user_id)
{
    assert(user_id);
    m_user_id = user_id;
}

int http_service::http_asyn_send(const char* url, const char* cmd_name, const char* data,
	unsigned int len, HTTP_METHOD method,int req_moudle_id)
{
	string full_url = make_url(url, cmd_name);
	http_client_context *context_ptr = new http_client_context;
	assert(context_ptr);
	context_ptr->m_url = url;
	context_ptr->m_cmd_name = cmd_name;
	context_ptr->m_req_moudle_id = req_moudle_id;

	//单线程队列，会在线程中调用do_send()
	return set(new send_node<http_service>(context_ptr, full_url, data, len, method));
}

void http_service::do_send(http_client_context *p_context,
	const string& url,
	const char* data,
	unsigned int len,
	HTTP_METHOD method)
{
	assert(p_context);
	assert(data);
	//打包request_header
	http_request_header request_header(method, len, m_access_key_id, m_access_key_secret, m_user_id);
	request_header.auto_build_header();
	std::map<std::string, std::string> headers;
	for (THttpHeaderMapCI it = request_header.get_headr_map().begin(), end = request_header.get_headr_map().end(); it != end; ++it)
	{
		headers[it->first] = it->second;
	}
	//发起数据请求
	HttpRequest request;
	request.SetRequestHeader(headers);
	request.SetRequestUrl(url);
	if (url.substr(0, 5) == "https")
	{
		request.SetSetupSSL(m_ssl_cert_path,
			m_ssl_cert_type,
			m_ssl_cert_passwd,
			m_ssl_key_path,
			m_ssl_key_type,
			m_ssl_key_passwd);
	}

	//post方式处理
	if (method == HTTP_METHOD::HTTP_METHOD_POST)
	{
		request.SetPostData(data, len);
	}

	//添加上下文管理
	p_context->m_request_id = request.GetRequestId();

	//执行HttpRequest
	std::string  out_data;
	bool ret = request.PerformRequest(out_data);
	if (m_pcallback)
	{
		m_pcallback->http_service_receive_callback(
			p_context->m_req_moudle_id, p_context->m_cmd_name, data, ret);
	}
	delete p_context;
}

std::string http_service::http_syn_send(const char* url, const char* cmd_name,
	const char* data, unsigned int len, HTTP_METHOD method)
{
	assert(data);
	string full_url = make_url(url, cmd_name);

	//打包request_header
	http_request_header request_header(method, len, m_access_key_id, m_access_key_secret, m_user_id);
	request_header.auto_build_header();
	std::map<std::string, std::string> headers;
	for (THttpHeaderMapCI it = request_header.get_headr_map().begin(), end = request_header.get_headr_map().end(); it != end; ++it)
	{
		headers[it->first] = it->second;
	}
	//发起数据请求
	HttpRequest request;
	request.SetRequestHeader(headers);
	request.SetRequestUrl(full_url);
	if (full_url.substr(0, 5) == "https")
	{
		request.SetSetupSSL(m_ssl_cert_path,
			m_ssl_cert_type,
			m_ssl_cert_passwd,
			m_ssl_key_path,
			m_ssl_key_type,
			m_ssl_key_passwd);
	}
	//非post方式处理
	if (method == HTTP_METHOD::HTTP_METHOD_POST)
	{
		request.SetPostData(data, len);
	}

	//执行HttpRequest
	std::string  out_data;
	bool ret = request.PerformRequest(out_data);
	return out_data;
}

int http_service::syn_upload_file(const std::string &strUrl, const std::string &strFileName)
{
	//打包request_header
	http_request_header request_header(HTTP_METHOD::HTTP_METHOD_POST, 0, m_access_key_id, m_access_key_secret, m_user_id);
	request_header.auto_build_header();
	std::map<std::string, std::string> headers;
	for (THttpHeaderMapCI it = request_header.get_headr_map().begin(), end = request_header.get_headr_map().end(); it != end; ++it)
	{
		headers[it->first] = it->second;
	}
	//发起数据请求
	HttpDownloader request;
	request.SetRequestHeader(headers);
	return request.SynUploadFile(strUrl, strFileName);
}

int http_service::syn_download_file(const std::string &strUrl, const std::string &strFilePath)
{
	//打包request_header
	http_request_header request_header(HTTP_METHOD::HTTP_METHOD_GET, 0, m_access_key_id, m_access_key_secret, m_user_id);
	request_header.auto_build_header();
	std::map<std::string, std::string> headers;
	for (THttpHeaderMapCI it = request_header.get_headr_map().begin(), end = request_header.get_headr_map().end(); it != end; ++it)
	{
		headers[it->first] = it->second;
	}
	//发起数据请求
	HttpDownloader request;
	request.SetRequestHeader(headers);
	return request.SynDownloadFile(strUrl, strFilePath);
	return 0;
}

