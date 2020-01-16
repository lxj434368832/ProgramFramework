/********************************************************************
Copyright (C), 2017-2020, Chongqing Ambition Polytron Technologies Inc.
创建日期:2017/9/8
文 件 名:uis_engine.h
创 建 人:wsj

描    述:uis系统的引擎作用，负责数据分发,信号调度,资源调配管理等；
修改历史：
*********************************************************************/
#pragma once
#include <functional>
#include <map>
#include "../http_service_moudle/http_service_interface.h"


//receive_proc回调函数
//把接收的数据交组件处理。

typedef std::function<void(int req_moudle_id, const std::string& data, bool result)> RECEIVE_CALLBACK;

class HttpClient : public http_service_callback
{
public:
	HttpClient();
	~HttpClient();
public:

	//开启引擎
	BOOL start();

	//停止服务
	BOOL stop();
	//设置http请求头用户ID
	void set_user_id(unsigned user_id);

	//*注册或注销异步接收数据的回调函数
	BOOL advise_receive_proc(std::string cmd_name, RECEIVE_CALLBACK pfn_receive);

	/*************************************************************************
	* function：http_service_callback接口的实现
	* param cmd_name:  in，命令名称
	* param strData:  in，数据
	*************************************************************************/
	virtual void http_service_receive_callback(int req_moudle_id, const std::string cmd_name, std::string strData, bool success);

	/*************************************************************************
	* function：			发送异步数据
	* param url:			服务地址
	* param cmd_name:		命令名称
	* param strRequest:		数据
	* param method:			方法
	* param req_moudle_id:	请求模块ID
	*************************************************************************/
	int http_asyn_send(const char* url, const char* cmd_name, std::string strRequest,
        int req_moudle_id = 0, HTTP_METHOD method = HTTP_METHOD::HTTP_METHOD_POST);

	// 同步发送数据接口
	std::string syn_send(const char* url,const char* cmd_name,std::string strRequest,
		HTTP_METHOD method = HTTP_METHOD::HTTP_METHOD_POST);

	std::string get_file_data(const char* url, const char* cmd_name, std::string strRequest);

	/*************************************************************************
	* function：	同步上传文件
	* param strUrl: 请求的url地址
	* param strFileName:下载的文件名
	* return:			返回投递的结果命令
	*************************************************************************/
	int syn_upload_file(const std::string &strUrl, const std::string &strFilePath);

	/*************************************************************************
	* function：	同步下载文件
	* param strUrl: 请求的url地址
	* param strFileName:下载的文件名
	* return:			返回投递的结果命令
	*************************************************************************/
	int syn_download_file(const std::string &strUrl, const std::string &strFilePath);

private: // noncopyable
	HttpClient(HttpClient const&);
	HttpClient& operator=(HttpClient const&);

private:
	http_service_out_ptr m_http_service_ptr;//http服务
	std::map<std::string, RECEIVE_CALLBACK> m_mapCallback;
};
