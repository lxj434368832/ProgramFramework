#include <assert.h>
#include <iostream>
#include "HttpClient.h"
#include "../http_service_moudle/http_service.h"
#include "../../3rdParty/Framework/include/CharacterSet.h"
#include "../http_service_moudle/stdafx.h"

#ifdef _DEBUG
#pragma comment(lib,"http_service_moudled.lib")
#else 
#pragma comment(lib,"http_service_moudle.lib")
#endif // _DEBUG

HttpClient::HttpClient() 
	:m_http_service_ptr(this)
{
}

HttpClient::~HttpClient()
{
	m_http_service_ptr->stop();
}

BOOL HttpClient::start()
{
	//启动http服务,初始化配置
	m_http_service_ptr->set_access_key("44CF9590006BF252F707", "OtxrzxIsfpFjA7SwPzILwy8Bw21TLhquhboDYROV");
	m_http_service_ptr->set_setup_ssl("C:\\client.pem", "pem", "123456", "C:\\private.pem", "pem", "123456");
	return m_http_service_ptr->start();
}

BOOL HttpClient::stop()
{
	return true;
}

void HttpClient::http_service_receive_callback(int req_moudle_id, const std::string cmd_name, std::string strData, bool success)
{
	std::string strRespond = mqwUtils::UTF8ToANSI(strData.c_str());
	http_log("<message> http asyn respond %s\n", strRespond);

	auto iter = m_mapCallback.find(cmd_name);
	if (m_mapCallback.end() == iter)
	{
		printf("不存在当前命令:%s对应的处理函数。\n", cmd_name.c_str());
	}
	else
	{
		(iter->second)(req_moudle_id, strData, success);
	}
}

//处理发送数据
int HttpClient::http_asyn_send(const char* url,const char* cmd_name, std::string strRequest,
    const int req_moudle_id,HTTP_METHOD method)
{
	http_log("<message> http_asyn_send url:%s\n cmd:%s data%s\n", url, cmd_name, strRequest);
	//调用底层通信接口
	std::string str_utf8 = mqwUtils::ANSIToUTF8(strRequest.c_str());
    return m_http_service_ptr->http_asyn_send(url, cmd_name, str_utf8.c_str(), str_utf8.length(), method,req_moudle_id);
}

std::string HttpClient::syn_send(const char* url, const char* cmd_name, std::string strRequest, HTTP_METHOD method)
{
	http_log("<message> http_sync_send url:%s\n cmd:%s data%s\n", url, cmd_name, strRequest.c_str());
	//调用底层通信接口
	strRequest = mqwUtils::ANSIToUTF8(strRequest.c_str());
	std::string strUtf8Data = m_http_service_ptr->http_syn_send(url, cmd_name, strRequest.c_str(), strRequest.length(), method);
	std::string strRespond = mqwUtils::UTF8ToANSI(strUtf8Data.c_str());
	http_log("<message> http syn respond %s\n", strRespond.data());
	return strRespond;
}

std::string HttpClient::get_file_data(const char* url, const char* cmd_name, std::string strRequest)
{
	http_log("<message> http_sync_send url:%s\n cmd:%s data%s\n", url, cmd_name, strRequest.c_str());
	strRequest = mqwUtils::ANSIToUTF8(strRequest.c_str());
	std::string strUtf8Data = m_http_service_ptr->http_syn_send(url, cmd_name, strRequest.c_str(),
		strRequest.length(), HTTP_METHOD::HTTP_METHOD_POST);
	std::string strRespond = mqwUtils::UTF8ToANSI(strUtf8Data.c_str());
	http_log("<message> http file data: %s\n", strRespond.data());
	return strRespond;;
}

int HttpClient::syn_upload_file(const std::string &strUrl, const std::string &strFilePath)
{
	return m_http_service_ptr->syn_upload_file(strUrl, strFilePath);
}

int HttpClient::syn_download_file(const std::string &strUrl, const std::string &strFilePath)
{
	http_log("<message> http upload url:%s\n", strUrl);
	return m_http_service_ptr->syn_download_file(strUrl, strFilePath);
}

//注册或注销组件接收数据的回调函数
BOOL  HttpClient::advise_receive_proc(std::string cmd_name, RECEIVE_CALLBACK pfn_receive)
{
	if (cmd_name.empty())
	{
		return FALSE;
	}

	BOOL bRet(FALSE);
	DWORD dwLastError(ERROR_SUCCESS);
	printf("%s, pfn_receive:%p \n", cmd_name.c_str(), pfn_receive);
	if (NULL != pfn_receive)
	{
		m_mapCallback[cmd_name] = pfn_receive;
		bRet = TRUE;
	}
	else
	{
		auto iter = m_mapCallback.find(cmd_name);
		if (m_mapCallback.end() != iter)
		{
			m_mapCallback.erase(iter);
			bRet = TRUE;
		}
		else
		{
			dwLastError = ERROR_FILE_NOT_FOUND;
		}
	}

	SetLastError(dwLastError);
	return bRet;
}

void HttpClient::set_user_id(unsigned user_id)
{
    m_http_service_ptr->set_operator(std::to_string(user_id).data());
}
