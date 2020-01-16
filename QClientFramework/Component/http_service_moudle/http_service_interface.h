/********************************************************************
Copyright (C), 2017-2020, Chongqing Ambition Polytron Technologies Inc.
创建日期:2017/9/11
文 件 名:http_service_interface.h
创 建 人:wsj

描    述:http服务器接口
修改历史：
*********************************************************************/
#pragma once

#include <string>
#include <windows.h>

#define UISCENTER_API extern "C"

//http函数类型
enum class HTTP_METHOD
{
	HTTP_METHOD_POST,
	HTTP_METHOD_PUT,
	HTTP_METHOD_PATCH,
	HTTP_METHOD_GET,
	HTTP_METHOD_DELETE,
	HTTP_METHOD_HEAD,
	HTTP_METHOD_TRACE,
	HTTP_METHOD_OPTIONS,
	HTTP_METHOD_CONNECT,
};


/************************************************************************
名称：抽象基类
描述：定义某些事件的默认处理方法
************************************************************************/

class http_service_callback
{
public:
	/*
	*http_service接收数据回调接口
	*参数说明：id       唯一实例ID (和http_send返回值对应)
	*          req_moudle_id 请求模块ID （由业务层传入，底层只做记录）
	*		   url      接口地址
	*		   cmd_name 命令名称
	*		   p_data   数据buffer
	*          len      数据buffer大小
	*          success   数据是否请求成功
	*/
	virtual void http_service_receive_callback(int req_moudle_id, const std::string cmd_name, std::string strData,bool success) = 0;
};

class http_service_interface
{
public:
	/*
	*启动服务
	*/
	virtual BOOL start() = 0;
	/*
	*停止服务
	*/
	virtual BOOL stop()=0;
	/*
	*说明：发送数据
	*参数 url            完整的url地址   参考：https://127.0.0.1:8080
	*     cmd_name       命令名称(数据接口）
	*     data           数据
	*     len            数据大小
	*     method         方法
	*     thread_mode    调用线程模式 
	*     req_moudle_id 请求模块ID （由业务层传入，底层只做记录）
	*返回值：-1           发送失败
	*        大于0       唯一实例ID
	*/
	virtual int http_asyn_send(
		const char* url,
		const char* cmd_name,
		const char* data,
		unsigned int len, 
		HTTP_METHOD method = HTTP_METHOD::HTTP_METHOD_POST,
		int req_moudle_id = 0) = 0;


	/*
	*说明：同函数同步发送数据，发送和接收同函数
	*参数 
	*     ret            返回值
	*	  url            完整的url地址   参考：https://127.0.0.1:8080
	*     cmd_name       命令名称(数据接口）
	*     data           数据
	*     len            数据大小
	*     method         方法
	*返回值： 接收数据      
	*/
	virtual std::string http_syn_send(
		const char* url,
		const char* cmd_name,
		const char* data,
		unsigned int len,
		HTTP_METHOD method = HTTP_METHOD::HTTP_METHOD_POST) = 0;


	/*
	*说明：设置ssl的证书，密钥
	*参数 ssl_cert_path   证书文件路径
	*     ssl_cert_type   证书类型 
	*     ssl_cert_type   证书密码
	*     ssl_key_path    私钥文件路径
	*     ssl_key_type    私钥类型
	*     ssl_key_passwd  私钥密码
	*/
	virtual void set_setup_ssl(const  char* ssl_cert_path,
		const  char* ssl_cert_type,
		const  char* ssl_cert_passwd,
		const  char* ssl_key_path,
		const  char* ssl_key_type,
		const  char* ssl_key_passwd)=0;
	/*
	*说明：设置授权信息
	*参数：access_key_id     秘钥ID
	*	   access_key_secret 秘钥密文
	*/
	virtual void set_access_key(const  char* access_key_id,const char* access_key_secret) = 0;

    /*
    *说明：设置接口调用者uId
    *参数：user_id     接口调用者uId
    */
    virtual void set_operator(const  char* user_id) = 0;


	virtual int syn_upload_file(const std::string &strUrl, const std::string &strFilePath) = 0;

	virtual int syn_download_file(const std::string &strUrl, const std::string &strFilePath) = 0;
};

/**************************************************/
/**************** 导出函数 ****************/
// 创建 i_uis_server 对象
extern "C" http_service_interface* http_service_create(http_service_callback* listener);
// 销毁 i_uis_server 对象
extern "C" void http_service_destroy(http_service_interface* p_server);

/**************对象智能指针 **************/
template<class T, class _listener, class _creator> class http_service_ptr
{
public:
	http_service_ptr(_listener* pListener)
	{
		if (pListener)
			m_pObj = _creator::create(pListener);
		else
			m_pObj = nullptr;
	}

	~http_service_ptr()
	{
		reset();
	}

public:
	http_service_ptr& reset(T* pObj = nullptr)
	{
		if (pObj != m_pObj)
		{
			if (m_pObj)
				_creator::destroy(m_pObj);

			m_pObj = pObj;
		}

		return *this;
	}

	http_service_ptr& attach(T* pObj)
	{
		return reset(pObj);
	}

	T* detach()
	{
		T* pObj = m_pObj;
		m_pObj = nullptr;

		return pObj;
	}

	BOOL isvalid()	const { return m_pObj != nullptr; }
	T* get()	const { return m_pObj; }
	T* operator ->	()	const { return m_pObj; }
	operator T*		()	const { return m_pObj; }
	http_service_ptr& operator = (T* pObj) { return reset(pObj); }
private:
	http_service_ptr(const http_service_ptr&);
	http_service_ptr& operator = (const http_service_ptr&);
private:
	T* m_pObj;
};

// http_service_interface 对象创建器
struct http_service_creator
{
	static http_service_interface* create(http_service_callback* listener)
	{
		return http_service_create(listener);
	}

	static void destroy(http_service_interface* p_server)
	{
		http_service_destroy(p_server);
	}
};

// http_service_interface 对象智能指针
typedef http_service_ptr<http_service_interface, http_service_callback, http_service_creator> http_service_out_ptr;

