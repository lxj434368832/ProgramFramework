/********************************************************************
Copyright (C), 2017-2020, Chongqing Ambition Polytron Technologies Inc.
��������:2017/9/11
�� �� ��:http_service_interface.h
�� �� ��:wsj

��    ��:http�������ӿ�
�޸���ʷ��
*********************************************************************/
#pragma once

#include <string>
#include <windows.h>

#define UISCENTER_API extern "C"

//http��������
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
���ƣ��������
����������ĳЩ�¼���Ĭ�ϴ�����
************************************************************************/

class http_service_callback
{
public:
	/*
	*http_service�������ݻص��ӿ�
	*����˵����id       Ψһʵ��ID (��http_send����ֵ��Ӧ)
	*          req_moudle_id ����ģ��ID ����ҵ��㴫�룬�ײ�ֻ����¼��
	*		   url      �ӿڵ�ַ
	*		   cmd_name ��������
	*		   p_data   ����buffer
	*          len      ����buffer��С
	*          success   �����Ƿ�����ɹ�
	*/
	virtual void http_service_receive_callback(int req_moudle_id, const std::string cmd_name, std::string strData,bool success) = 0;
};

class http_service_interface
{
public:
	/*
	*��������
	*/
	virtual BOOL start() = 0;
	/*
	*ֹͣ����
	*/
	virtual BOOL stop()=0;
	/*
	*˵������������
	*���� url            ������url��ַ   �ο���https://127.0.0.1:8080
	*     cmd_name       ��������(���ݽӿڣ�
	*     data           ����
	*     len            ���ݴ�С
	*     method         ����
	*     thread_mode    �����߳�ģʽ 
	*     req_moudle_id ����ģ��ID ����ҵ��㴫�룬�ײ�ֻ����¼��
	*����ֵ��-1           ����ʧ��
	*        ����0       Ψһʵ��ID
	*/
	virtual int http_asyn_send(
		const char* url,
		const char* cmd_name,
		const char* data,
		unsigned int len, 
		HTTP_METHOD method = HTTP_METHOD::HTTP_METHOD_POST,
		int req_moudle_id = 0) = 0;


	/*
	*˵����ͬ����ͬ���������ݣ����ͺͽ���ͬ����
	*���� 
	*     ret            ����ֵ
	*	  url            ������url��ַ   �ο���https://127.0.0.1:8080
	*     cmd_name       ��������(���ݽӿڣ�
	*     data           ����
	*     len            ���ݴ�С
	*     method         ����
	*����ֵ�� ��������      
	*/
	virtual std::string http_syn_send(
		const char* url,
		const char* cmd_name,
		const char* data,
		unsigned int len,
		HTTP_METHOD method = HTTP_METHOD::HTTP_METHOD_POST) = 0;


	/*
	*˵��������ssl��֤�飬��Կ
	*���� ssl_cert_path   ֤���ļ�·��
	*     ssl_cert_type   ֤������ 
	*     ssl_cert_type   ֤������
	*     ssl_key_path    ˽Կ�ļ�·��
	*     ssl_key_type    ˽Կ����
	*     ssl_key_passwd  ˽Կ����
	*/
	virtual void set_setup_ssl(const  char* ssl_cert_path,
		const  char* ssl_cert_type,
		const  char* ssl_cert_passwd,
		const  char* ssl_key_path,
		const  char* ssl_key_type,
		const  char* ssl_key_passwd)=0;
	/*
	*˵����������Ȩ��Ϣ
	*������access_key_id     ��ԿID
	*	   access_key_secret ��Կ����
	*/
	virtual void set_access_key(const  char* access_key_id,const char* access_key_secret) = 0;

    /*
    *˵�������ýӿڵ�����uId
    *������user_id     �ӿڵ�����uId
    */
    virtual void set_operator(const  char* user_id) = 0;


	virtual int syn_upload_file(const std::string &strUrl, const std::string &strFilePath) = 0;

	virtual int syn_download_file(const std::string &strUrl, const std::string &strFilePath) = 0;
};

/**************************************************/
/**************** �������� ****************/
// ���� i_uis_server ����
extern "C" http_service_interface* http_service_create(http_service_callback* listener);
// ���� i_uis_server ����
extern "C" void http_service_destroy(http_service_interface* p_server);

/**************��������ָ�� **************/
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

// http_service_interface ���󴴽���
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

// http_service_interface ��������ָ��
typedef http_service_ptr<http_service_interface, http_service_callback, http_service_creator> http_service_out_ptr;

