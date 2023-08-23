/********************************************************************
Copyright (C), 2017-2020, Chongqing Ambition Polytron Technologies Inc.
创建日期:2017/9/8
文 件 名:HttpRequest.h
创 建 人:wsj

描    述:提供http/https的Request通信服务,https文件下载服务；
修改历史：
*********************************************************************/

#pragma once
#include <string>
#include <map>
#include <memory>
#include <functional>
#include <vector>

class http_lock;
#ifndef _WIN32
typedef void* HANDLE;
#endif

struct curl_slist;

class HttpRequest
{
public:
	//处理结果
    typedef enum {
        REQUEST_OK,
        REQUEST_INVALID_OPT,
        REQUEST_PERFORM_ERROR,
        REQUEST_OPENFILE_ERROR,
        REQUEST_INIT_ERROR,
    }RequestResult;

    typedef std::function<void(int, bool, const std::string&)> ResultCallback;
    friend class HttpHelper;
    HttpRequest();
    ~HttpRequest();
public:
    int SetRetryTimes(int retry_times = s_kRetryCount);
    int SetRequestId(int id);
	int GetRequestId();
	HANDLE GetHandle();
	int SetRequestTimeout(long time_out = 0);
    int SetRequestUrl(const std::string& url);
    int SetMovedUrl(bool get_moved_url);
	int SetSetupSSL(const std::string& ssl_cert,
		const std::string& ssl_cert_type,
		const std::string& ssl_cert_passwd,
		const std::string& ssl_key,
		const std::string& ssl_key_type,
		const std::string& ssl_key_passwd);

    int SetPostData(const std::string& message);
    int SetPostData(const void* data, unsigned int size);
    int SetRequestHeader(const std::map<std::string, std::string>& headers);
    int SetRequestHeader(const std::string& header);
    int SetRequestProxy(const std::string& proxy, long proxy_port);
	//同步请求
	bool PerformRequest(std::string& out_data);
public:
	
    static bool GetHttpCode(HANDLE request_handle, long* http_code);
    static bool GetReceiveHeader(HANDLE request_handle, std::string* header);
    static bool GetReceiveContent(HANDLE request_handle, std::string* receive);
    static bool GetErrorString(HANDLE request_handle, std::string* error_string);
protected:
    class RequestHelper {
    public:
        RequestHelper();
        ~RequestHelper();

        friend class HttpRequest;
        friend class HttpHelper;
	public:
        int      SetRetryTimes(int retry_times) { m_retry_times = retry_times; return REQUEST_OK; }
        int      SetRequestTimeout(long time_out = 0);
        int      SetRequestUrl(const std::string& url);
		int		 SetSetupSSL(const std::string& ssl_cert,const std::string& ssl_cert_type,const std::string& ssl_cert_passwd,
			const std::string& ssl_key,const std::string& ssl_key_type,const std::string& ssl_key_passwd);

        int      SetMovedUrl(bool get_moved_url);
        int      SetPostData(const void* data, unsigned int size);
        int      SetRequestHeader(const std::string& header);
        int      SetRequestProxy(const std::string& proxy, long proxy_port);
		bool     Perform(std::string &receive_content);
        long     GetHttpCode() { return m_http_code; }
		long     GetCurlCode() { return m_curl_code; }
        bool     GetHeader(std::string* header);
        bool     GetContent(std::string* receive);
        bool     GetErrorString(std::string* error_string);

    private:
        HANDLE      m_curl_handle;
        HANDLE      m_http_headers;
        int         m_retry_times;
        int         m_id;
        long        m_http_code;
		long        m_curl_code;
        std::string     m_receive_content;
        std::string     m_receive_header;
        std::string     m_error_string;
        char*               m_post_data;
    };

private:
    RequestHelper				   *m_request_handle;
    static const int               s_kRetryCount = 1; //失败时，尝试重复请求次数
};

class HttpDownloader
{
public:
    typedef enum {
        DOWN_SYNC,
        DOWN_ASYNC,
    }DownType;

    //double total_size, double downloaded_size, void* userdata
    typedef std::function<int(double, double, void*)> ProgressCallback;
    //int id, bool success, const std::string& data
    typedef std::function<void(int, bool, const std::string&)> ResultCallback;

    friend class HttpHelper;

    HttpDownloader();
    ~HttpDownloader();
public:
    int         SetRequestProxy(const std::string& proxy, long proxy_port);
    int         SetRetryTimes(int retry_times = s_kRetryCount);
    int         SetTimeout(long time_out = 0);
	int         SetDownloadUrl(const std::string& url);
	int			SetRequestHeader(const std::map<std::string, std::string>& headers);
    int         SetUserData(void* userdata);
    int         SetRequestId(int id);
    int         SetProgressCallback(ProgressCallback pc);
    int         SetResultCallback(ResultCallback rc);

    int         DownloadFile(const std::string& file_name, int thread_count = 5);
	//同步下载
	bool           StartDownloadSYNC();
	//异步下载
	HANDLE        StartDownloadASYNC();
	//同步下载文件
	int			  SynDownloadFile(const std::string &strUrl, const std::string &strFilePath);
	//同步上传文件
	int			  SynUploadFile(const std::string &strUrl, const std::string &strFilePath);
    static bool   CancelDownload(HANDLE handle);
    static void   Close(HANDLE handle);
    static bool   GetHttpCode(HANDLE handle, long* http_code);
    static bool   GetReceiveHeader(HANDLE handle, std::string* header);
    static bool   GetErrorString(HANDLE handle, std::string* error_string);
    static void*  GetUserData(HANDLE handle);
protected:
    class DownloadHelper {
    public:
        typedef struct tThreadChunk
        {
            FILE*       _fp;
            long        _startidx;
            long        _endidx;

            DownloadHelper*     _download;
        }ThreadChunk;

        DownloadHelper();
        ~DownloadHelper();

        friend class HttpDownloader;
        friend class HttpHelper;
        friend ThreadChunk;

        void     SetRetryTimes(int retry_times) { m_retry_times = retry_times; }
        void     SetRequestId(int id) { m_id = id;  }
        int      SetTimeout(long time_out = 0);
        int      SetRequestUrl(const std::string& url);
        int      SetRequestProxy(const std::string& proxy, long proxy_port);

		int      SetRequestHeader(const std::string& header);
        void     SetUserData(void *userdata) { m_userdata = userdata; }
        int      SetProgressCallback(ProgressCallback pc);
        int      SetResultCallback(ResultCallback rc);
        int      SetDownloadFile(const std::string& file_name);
        int      SetDownloadThreadCount(int thread_count);

		bool      Perform();
		//同步下载文件
		int			  DownloadFileSyn(const std::string &strUrl, const std::string &strFilePath);
		//同步上传文件
		int			  UploadFileSyn(const std::string &strUrl, const std::string &strFilePath);

        int      GetHttpCode() { return m_http_code; }
        bool     GetHeader(std::string* header);
        bool     GetErrorString(std::string* error_string);
        bool     SelfClose(void) { return m_close_self; }
        void*    GetUserData(void) { return m_userdata; }

    protected:
        int      DownloadDefaultCallback(double total_size, double downloaded_size, void* userdata);
        void     ResultDefaultCallback(int id, bool success, const std::string& data);
        double   GetDownloadFileSize();
        int      DoDownload(ThreadChunk* thread_chunk);
        int      SplitDownloadCount(double down_size);
        void  Reset(void);

    private:
		HANDLE        m_perform_thread;
		curl_slist*  m_head_list;
        int          m_retry_times;
        int          m_thread_count;
        int          m_id;
        long         m_time_out;

        std::string  m_file_path;
        std::string  m_url;
        std::string  m_http_proxy;
        std::string  m_receive_header;
        std::string  m_error_string;

        bool          m_close_self;
        bool          m_multi_download;
        bool         m_download_fail;
        bool          m_is_running;
        bool         m_is_cancel;
        void*        m_userdata;
        long         m_http_code;
        long         m_proxy_port;
        double       m_total_size;
        double       m_downloaded_size;

        std::shared_ptr<http_lock> m_httplock;
        ProgressCallback  m_download_callback;
        ResultCallback    m_result_callback;
    };

private:
    std::shared_ptr<DownloadHelper>    m_request_handle;
    static const int          s_kRetryCount = 3;
    static const int          s_kThreadCount = 4;
};