#include "stdafx.h"
#include <list>
#include <regex>
#include <sstream>
#include "HttpRequest.h"  


#define ZLIB_WINAPI
#define CURL_STATICLIB
#include "../../3rdParty/libcurl/curl.h"
#include "../../3rdParty/libcurl/zlib/zlib.h"

#include "HttpHelper.h"
#include <io.h>

#if _DEBUG
#pragma comment( lib, "../3rdParty/libcurl/lib/zlibd.lib")
#pragma comment( lib, "../3rdParty/libcurl/lib/libcurld.lib")
#pragma comment( lib, "../3rdParty/libcurl/lib/libeay32d.lib" )
#pragma comment( lib, "../3rdParty/libcurl/lib/ssleay32d.lib" )
#else
#pragma comment( lib, "../3rdParty/libcurl/lib/zlib.lib")
#pragma comment( lib, "../3rdParty/libcurl/lib/libcurl.lib")
#pragma comment( lib, "../3rdParty/libcurl/lib/libeay32.lib" )
#pragma comment( lib, "../3rdParty/libcurl/lib/ssleay32.lib" )
#endif

#pragma comment ( lib, "ws2_32.lib" )
#pragma comment ( lib, "wldap32.lib" )

HttpRequest::HttpRequest()
{
    HttpHelper::Instance();
	m_request_handle = new HttpRequest::RequestHelper;
}

HttpRequest::~HttpRequest()
{
	if (m_request_handle)
	{
		delete m_request_handle;
		m_request_handle = nullptr;
	}
}

int HttpRequest::SetRetryTimes(int retry_times)
{
    if (m_request_handle)
    {
        m_request_handle->SetRetryTimes(retry_times);
        return REQUEST_OK;
    }

    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetRequestId(int id)
{
    if (m_request_handle)
    {
        m_request_handle->m_id = id;
        return REQUEST_OK;
    }

    return REQUEST_INIT_ERROR;
}
int HttpRequest::GetRequestId()
{
	if (m_request_handle)
	{
		return m_request_handle->m_id;
	}

	return -1;
}

HANDLE HttpRequest::GetHandle()
{
	if (m_request_handle)
	{
		return &m_request_handle;
	}

	return nullptr;
}


int HttpRequest::SetRequestTimeout(long time_out)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetRequestTimeout(time_out) == CURLE_OK)
        {
            return REQUEST_OK;
        }
        else
        {
            return REQUEST_INVALID_OPT;
        }
    }

    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetRequestUrl(const std::string& url)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetRequestUrl(url) == CURLE_OK)
        {
            return REQUEST_OK;
        }
        else
        {
            return REQUEST_INVALID_OPT;
        }
    }

    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetMovedUrl(bool get_moved_url)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetMovedUrl(get_moved_url) == CURLE_OK)
        {
            return REQUEST_OK;
        }
        else
        {
            return REQUEST_INVALID_OPT;
        }
    }

    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetSetupSSL(const std::string& ssl_cert, 
	const std::string& ssl_cert_type,
	const std::string& ssl_cert_passwd,
	const std::string& ssl_key, 
	const std::string& ssl_key_type, 
	const std::string& ssl_key_passwd)
{
	if (m_request_handle)
	{
		if (m_request_handle->SetSetupSSL(ssl_cert,
			ssl_cert_type,
			ssl_cert_passwd,
			ssl_key,
			ssl_key_type,
			ssl_key_passwd) == CURLE_OK)
		{
			return REQUEST_OK;
		}
		else
		{
			return REQUEST_INVALID_OPT;
		}
	}

	return REQUEST_INIT_ERROR;
}

int HttpRequest::SetPostData(const std::string& message)
{
    return SetPostData(message.c_str(), message.size());
}

int HttpRequest::SetPostData(const void* data, unsigned int size)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetPostData(data, size) == CURLE_OK)
        {
            return REQUEST_OK;
        }
        else
        {
            return REQUEST_INVALID_OPT;
        }
    }
    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetRequestHeader(const std::map<std::string, std::string>& headers)
{
    if (m_request_handle)
    {
        for (auto it = headers.begin(); it != headers.end(); ++it)
        {
            std::string header = it->first;
            header += ": ";
            header += it->second;
            if (m_request_handle->SetRequestHeader(header) != CURLE_OK)
            {
                return REQUEST_INVALID_OPT;
            }
        }
        return REQUEST_OK;
	}
	http_log("<error> SetRequestHeader error\n");
    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetRequestHeader(const std::string& header)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetRequestHeader(header) == CURLE_OK)
        {
            return REQUEST_OK;
        }
        else
        {
            return REQUEST_INVALID_OPT;
        }
    }
    return REQUEST_INIT_ERROR;
}

int HttpRequest::SetRequestProxy(const std::string& proxy, long proxy_port)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetRequestProxy(proxy, proxy_port) == CURLE_OK)
        {
            return REQUEST_OK;
        }
        else
        {
            return REQUEST_INVALID_OPT;
        }
    }

    return REQUEST_INIT_ERROR;
}

bool HttpRequest::PerformRequest(std::string& out_data)
{
	if (m_request_handle)
	{
		return m_request_handle->Perform(out_data);
	}
	return false;
}

bool HttpRequest::GetHttpCode(HANDLE request_handle, long* http_code)
{
    std::shared_ptr<RequestHelper>* request = reinterpret_cast<std::shared_ptr<RequestHelper>*>(request_handle);
    if (request && http_code)
    {
        *http_code = (*request)->GetHttpCode();
        return true;
    }

    return false;
}

bool HttpRequest::GetReceiveHeader(HANDLE request_handle, std::string* header)
{
    std::shared_ptr<RequestHelper>* request = reinterpret_cast<std::shared_ptr<RequestHelper>*>(request_handle);
    if (request)
    {
        return (*request)->GetHeader(header);
    }

    return false;
}

bool HttpRequest::GetReceiveContent(HANDLE request_handle, std::string* receive)
{
    std::shared_ptr<RequestHelper>* request = reinterpret_cast<std::shared_ptr<RequestHelper>*>(request_handle);
    if (request)
    {
        return (*request)->GetContent(receive);
    }

    return false;
}

bool HttpRequest::GetErrorString(HANDLE request_handle, std::string* error_string)
{
    std::shared_ptr<RequestHelper>* request = reinterpret_cast<std::shared_ptr<RequestHelper>*>(request_handle);
    if (request)
    {
        return (*request)->GetErrorString(error_string);
    }

    return false;
}

HttpRequest::RequestHelper::RequestHelper()
    : m_curl_handle(nullptr)
    , m_http_headers(nullptr)
    , m_retry_times(HttpRequest::s_kRetryCount)
    , m_http_code(0)
    , m_post_data(nullptr)
{
	m_id = HttpHelper::s_id;
    m_curl_handle = curl_easy_init();
}

HttpRequest::RequestHelper::~RequestHelper()
{
    if (m_curl_handle)
    {
        curl_easy_cleanup(m_curl_handle);
    }
    if (m_http_headers)
    {
        curl_slist_free_all(reinterpret_cast<curl_slist*>(m_http_headers));
    }
    if (m_post_data)
    {
        delete m_post_data;
        m_post_data = nullptr;
    }
}

int HttpRequest::RequestHelper::SetRequestTimeout(long time_out)
{
    if (m_curl_handle)
    {
        return curl_easy_setopt(m_curl_handle, CURLOPT_TIMEOUT, 0);
    }

    return CURLE_FAILED_INIT;
}

int HttpRequest::RequestHelper::SetRequestUrl(const std::string& url)
{
    if (m_curl_handle)
    {
        if (url.substr(0, 5) == "https")
        {
            curl_easy_setopt(m_curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(m_curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
        }

        return curl_easy_setopt(m_curl_handle, CURLOPT_URL, url.c_str());
    }

    return CURLE_FAILED_INIT;
}

int HttpRequest::RequestHelper::SetSetupSSL(const std::string& ssl_cert, 
	const std::string& ssl_cert_type, 
	const std::string& ssl_cert_passwd, 
	const std::string& ssl_key,
	const std::string& ssl_key_type, 
	const std::string& ssl_key_passwd)
{
	if (m_curl_handle)
	{
		curl_easy_setopt(m_curl_handle, CURLOPT_SSLCERT,ssl_cert.c_str());
		curl_easy_setopt(m_curl_handle, CURLOPT_SSLCERTTYPE, ssl_cert_type.c_str());
		curl_easy_setopt(m_curl_handle, CURLOPT_SSLCERTPASSWD, ssl_cert_passwd.c_str());
		curl_easy_setopt(m_curl_handle, CURLOPT_SSLKEY, ssl_key.c_str());
		curl_easy_setopt(m_curl_handle, CURLOPT_SSLKEYTYPE, ssl_key_type.c_str());
		return curl_easy_setopt(m_curl_handle, CURLOPT_SSLKEYPASSWD, ssl_key_passwd.c_str());
	}

	return CURLE_FAILED_INIT;
}

int HttpRequest::RequestHelper::SetMovedUrl(bool get_moved_url)
{
    if (m_curl_handle)
    {
        if (get_moved_url)
        {
            curl_easy_setopt(m_curl_handle, CURLOPT_MAXREDIRS, 5);
            return curl_easy_setopt(m_curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
        }
        else
        {
            return curl_easy_setopt(m_curl_handle, CURLOPT_FOLLOWLOCATION, 0L);
        }
    }

    return CURLE_FAILED_INIT;
}

int HttpRequest::RequestHelper::SetPostData(const void* data, unsigned int size)
{
    if (m_curl_handle )//&& data && size > 0)
    {
        CURLcode curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_POST, 1);
        if (curl_code == CURLE_OK)
        {
            if (m_post_data)
            {
                delete m_post_data;
                m_post_data = nullptr;
            }

            if (size == 0)
            {
                curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_POSTFIELDS, "");
            }
            else
            {
                m_post_data = new char[size];
                memcpy(m_post_data, data, size);
                curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_POSTFIELDS, m_post_data);
            }
        }

        if (curl_code == CURLE_OK)
        {
            curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_POSTFIELDSIZE, size);
        }

        return curl_code;
    }

    return CURLE_FAILED_INIT;
}

int HttpRequest::RequestHelper::SetRequestHeader(const std::string& header)
{
    if (m_curl_handle && header.empty() == false)
    {
        m_http_headers = curl_slist_append(reinterpret_cast<curl_slist*>(m_http_headers), header.c_str());

        return m_http_headers ? CURLE_OK : CURLE_FAILED_INIT;
    }

    return CURLE_FAILED_INIT;
}

int HttpRequest::RequestHelper::SetRequestProxy(const std::string& proxy, long proxy_port)
{
    //CURLOPT_PROXY
    if (m_curl_handle)
    {
        CURLcode curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_PROXYPORT, proxy_port);

        curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_PROXY, proxy.c_str());

        return curl_code;
    }

    return CURLE_FAILED_INIT;
}

bool HttpRequest::RequestHelper::Perform(std::string &receive_content)
{
	if (m_curl_handle)
	{
		CURLcode curl_code;
		if (m_http_headers)
		{
			curl_code = curl_easy_setopt(m_curl_handle, CURLOPT_HTTPHEADER, reinterpret_cast<curl_slist*>(m_http_headers));
			if (curl_code != CURLE_OK)
			{
				http_log("<error> RequestHelper::Perform curl_easy_setopt header error %d\n", curl_code);
				return false;
			}
		}

		m_receive_header.clear();
		
		//set force http redirect
		SetMovedUrl(true);
		std::stringstream out;
		curl_easy_setopt(m_curl_handle, CURLOPT_HEADERFUNCTION, HttpHelper::RetriveHeaderFunction);
		curl_easy_setopt(m_curl_handle, CURLOPT_HEADERDATA, &m_receive_header);
		curl_easy_setopt(m_curl_handle, CURLOPT_WRITEFUNCTION, HttpHelper::RetriveContentFunction);
		curl_easy_setopt(m_curl_handle, CURLOPT_WRITEDATA, &out);
		curl_easy_setopt(m_curl_handle, CURLOPT_NOPROGRESS, 1);
		curl_easy_setopt(m_curl_handle, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(m_curl_handle, CURLOPT_TIMEOUT, 5);  //接收数据时超时设置，如果指定秒内数据未接收完，直接退出
		curl_easy_setopt(m_curl_handle, CURLOPT_CONNECTTIMEOUT, 1);//连接超时，这个数值如果设置太短可能导致数据请求不到就断开了
		curl_code = curl_easy_perform(m_curl_handle);
		
		/*超时了，重试多少次都超时mingel
		if (curl_code == CURLE_OPERATION_TIMEDOUT)
		{
			int retry_count = m_retry_times;
			while (retry_count > 0)
			{
				http_log("<message> RequestHelper::Perform error IMEDOUT retry_count header  %d\n", retry_count);
				curl_code = curl_easy_perform(m_curl_handle);
				if (curl_code != CURLE_OPERATION_TIMEDOUT) break;
				retry_count--;
			}
		}*/
		receive_content = out.str();
		curl_easy_getinfo(m_curl_handle, CURLINFO_RESPONSE_CODE, &m_http_code);
		if (curl_code == CURLE_OK )
		{
			//&& m_http_code == 200
			http_log("<message> %s http code: %d\n", __FUNCTION__, m_http_code);
			return true;
		}
		else
		{
			const char* err_string = curl_easy_strerror(curl_code);
			m_error_string = err_string;
			http_log("<error> %s error code:%d error msg: %s\n", __FUNCTION__, curl_code, err_string);
		}
	}

	return false;
}

bool HttpRequest::RequestHelper::GetHeader(std::string* header)
{
    if (m_receive_header.empty()) return false;
    else if (header) *header = m_receive_header;

    return true;
}

bool HttpRequest::RequestHelper::GetContent(std::string* receive)
{
    if (m_receive_content.empty()) return false;
    else if (receive) *receive = m_receive_content;

    return true;
}

bool HttpRequest::RequestHelper::GetErrorString(std::string* error_string)
{
    if (m_error_string.empty()) return false;
    else if (error_string) *error_string = m_error_string;

    return true;
}


////////////////////////////http文件下载//////////////////////////////////////////////
HttpDownloader::HttpDownloader()
{
	HttpHelper::Instance();
	m_request_handle.reset(new HttpDownloader::DownloadHelper);
}

HttpDownloader::~HttpDownloader()
{

}

int HttpDownloader::SetRequestProxy(const std::string& proxy, long proxy_port)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetRequestProxy(proxy, proxy_port) == CURLE_OK)
        {
            return 0;
        }
        else
        {
            return HttpRequest::REQUEST_INVALID_OPT;
        }
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetRetryTimes(int retry_times /* = s_kRetryCount */)
{
    if (m_request_handle)
    {
        m_request_handle->SetRetryTimes(retry_times);
        return HttpRequest::REQUEST_OK;
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetTimeout(long time_out /* = 0 */)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetTimeout(time_out) == CURLE_OK)
        {
            return HttpRequest::REQUEST_OK;
        }
        else
        {
            return HttpRequest::REQUEST_INVALID_OPT;
        }
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetDownloadUrl(const std::string& url)
{
    if (m_request_handle)
    {
        if (m_request_handle->SetRequestUrl(url) == CURLE_OK)
        {
            return HttpRequest::REQUEST_OK;
        }
        else
        {
            return HttpRequest::REQUEST_INVALID_OPT;
        }
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetRequestHeader(const std::map<std::string, std::string>& headers)
{
	for (auto it = headers.begin(); it != headers.end(); ++it)
	{
		std::string header = it->first;
		header += ": ";
		header += it->second;
		if (m_request_handle->SetRequestHeader(header) != CURLE_OK)
		{
			return HttpRequest::REQUEST_INVALID_OPT;
		}
	}
	return HttpRequest::REQUEST_OK;
}

int HttpDownloader::SetUserData(void* userdata)
{
    if (m_request_handle)
    {
        m_request_handle->SetUserData(userdata);

        return HttpRequest::REQUEST_OK;
    }
    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetRequestId(int id)
{
    if (m_request_handle)
    {
        m_request_handle->SetRequestId(id);
        return HttpRequest::REQUEST_OK;
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetProgressCallback(ProgressCallback pc)
{
    if (m_request_handle)
    {
        m_request_handle->SetProgressCallback(pc);

        return HttpRequest::REQUEST_OK;
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SetResultCallback(ResultCallback rc)
{
    if (m_request_handle)
    {
        m_request_handle->SetResultCallback(rc);

        return HttpRequest::REQUEST_OK;
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::DownloadFile(const std::string& file_name, int thread_count /* = 5 */)
{
    if (m_request_handle)
    {
        m_request_handle->SetDownloadFile(file_name);
        m_request_handle->SetDownloadThreadCount(thread_count);
    }

    return HttpRequest::REQUEST_INIT_ERROR;
}

bool HttpDownloader::StartDownloadSYNC()
{
	if (m_request_handle)
	{
		if (m_request_handle->m_is_running)
		{
			return false;
		}

		m_request_handle->Reset();
		return m_request_handle->Perform();
	}

	return false;
}

HANDLE HttpDownloader::StartDownloadASYNC()
{
	if (m_request_handle)
	{
		if (m_request_handle->m_is_running)
		{
			return nullptr;
		}

		m_request_handle->Reset();
		DoHttpLock http_lock(HttpHelper::s_download_lock);
		HttpHelper::s_async_downloads.push_back(m_request_handle);
		std::shared_ptr<DownloadHelper>& request = HttpHelper::s_async_downloads.back();
		DWORD thread_id;
		HANDLE async_thread = CreateThread(NULL, 0, HttpHelper::DownloadThread, &request, 0, &thread_id);
		request->m_perform_thread = async_thread;
		return &request;
	}

	return nullptr;
}

int HttpDownloader::SynDownloadFile(const std::string &strUrl, const std::string &strFilePath)
{
	if (m_request_handle)
	{
		return m_request_handle->DownloadFileSyn(strUrl, strFilePath);
	}
	return HttpRequest::REQUEST_INIT_ERROR;
}

int HttpDownloader::SynUploadFile(const std::string &strUrl, const std::string &strFilePath)
{
	if (m_request_handle)
	{
		return m_request_handle->UploadFileSyn(strUrl, strFilePath);
	}
	return HttpRequest::REQUEST_INIT_ERROR;
}

void HttpDownloader::Close(HANDLE handle)
{
    std::shared_ptr<DownloadHelper>* request = (reinterpret_cast<std::shared_ptr<DownloadHelper> *>(handle));
    if (request == INVALID_HANDLE_VALUE || request == nullptr)
    {
        return;
    }

    bool basync = false;

    DoHttpLock http_lock(HttpHelper::s_download_lock);
    for (auto it = HttpHelper::s_async_downloads.begin(); it != HttpHelper::s_async_downloads.end(); ++it)
    {
        if ((*request) == *it)
        {
            if (WaitForSingleObject((*request)->m_perform_thread, 10) == WAIT_OBJECT_0)
            {
                HttpHelper::s_async_downloads.remove(*request);
            }
            else
            {
                (*request)->m_close_self = true;
            }
            basync = true;
            break;
        }
    }

    if (basync == false)
    {
        (*request)->m_is_cancel = true;
        //request->reset();
    }
}

bool HttpDownloader::CancelDownload(HANDLE handle)
{
    std::shared_ptr<DownloadHelper>* request = (reinterpret_cast<std::shared_ptr<DownloadHelper> *>(handle));
    if (request == INVALID_HANDLE_VALUE || request == nullptr)
    {
        return false;
    }

    (*request)->m_is_cancel = true;

    return true;
}

bool HttpDownloader::GetHttpCode(HANDLE handle, long* http_code)
{
    std::shared_ptr<DownloadHelper>* request = reinterpret_cast<std::shared_ptr<DownloadHelper>*>(handle);
    if (request && http_code)
    {
        *http_code = (*request)->GetHttpCode();
        return true;
    }

    return false;
}

bool HttpDownloader::GetErrorString(HANDLE handle, std::string* error_string)
{
    std::shared_ptr<DownloadHelper>* request = reinterpret_cast<std::shared_ptr<DownloadHelper>*>(handle);
    if (request)
    {
        return (*request)->GetErrorString(error_string);
    }

    return false;
}

bool HttpDownloader::GetReceiveHeader(HANDLE handle, std::string* header)
{
    std::shared_ptr<DownloadHelper>* request = reinterpret_cast<std::shared_ptr<DownloadHelper>*>(handle);
    if (request)
    {
        return (*request)->GetHeader(header);
    }

    return false;
}

void* HttpDownloader::GetUserData(HANDLE handle)
{

    std::shared_ptr<DownloadHelper>* request = reinterpret_cast<std::shared_ptr<DownloadHelper>*>(handle);
    if (request)
    {
        return (*request)->GetUserData();
    }

    return nullptr;
}

HttpDownloader::DownloadHelper::DownloadHelper()
    : m_perform_thread(nullptr)
    , m_close_self(false)
    , m_retry_times(HttpDownloader::s_kRetryCount)
    , m_thread_count(HttpDownloader::s_kThreadCount)
    , m_http_code(0)
    , m_time_out(0)
    , m_proxy_port(0)
    , m_total_size(0.0)
    , m_downloaded_size(0.0)
    , m_multi_download(false)
    , m_download_fail(true)
    , m_is_running(false)
    , m_httplock(new http_lock)
    , m_userdata(NULL)
{
    m_download_callback = std::bind(&DownloadHelper::DownloadDefaultCallback, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    m_result_callback = std::bind(&DownloadHelper::ResultDefaultCallback, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    m_id = HttpHelper::s_id;
}

HttpDownloader::DownloadHelper::~DownloadHelper()
{
    if (m_perform_thread)
    {
        CloseHandle(m_perform_thread);
        m_perform_thread = nullptr;
    }

	if (m_head_list)
	{
		curl_slist_free_all(m_head_list);
		m_head_list = nullptr;
	}
}

int HttpDownloader::DownloadHelper::SetTimeout(long time_out /* = 0 */)
{
    m_time_out = time_out;

    return CURLE_OK;
}

int HttpDownloader::DownloadHelper::SetRequestUrl(const std::string& url)
{
    m_url = url;
    return CURLE_OK;
}

int HttpDownloader::DownloadHelper::SetRequestProxy(const std::string& proxy, long proxy_port)
{
    m_http_proxy = proxy;
    m_proxy_port = proxy_port;
    return CURLE_OK;
}

int HttpDownloader::DownloadHelper::SetRequestHeader(const std::string& header)
{
	if (false == header.empty())
	{
		m_head_list = curl_slist_append(reinterpret_cast<curl_slist*>(m_head_list), header.c_str());

		return m_head_list ? CURLE_OK : CURLE_FAILED_INIT;
	}

	return CURLE_FAILED_INIT;
}

int HttpDownloader::DownloadHelper::SetProgressCallback(ProgressCallback pc)
{
    m_download_callback = pc;
    return CURLE_OK;
}

int HttpDownloader::DownloadHelper::SetResultCallback(ResultCallback rc)
{
    m_result_callback = rc;

    return CURLE_OK;
}

int HttpDownloader::DownloadHelper::SetDownloadFile(const std::string& file_name)
{
    m_file_path = file_name;
    return CURLE_OK;
}

int HttpDownloader::DownloadHelper::SetDownloadThreadCount(int thread_count)
{
    m_thread_count = thread_count;
    return CURLE_OK;
}

bool HttpDownloader::DownloadHelper::Perform()
{
    m_total_size = GetDownloadFileSize();
    if (m_total_size < 0)
    {
        return false;
    }

    std::string out_file_name = m_file_path;
    std::string src_file_name = out_file_name;
    out_file_name += ".dl";

    FILE *fp = nullptr;
    DeleteFileA(out_file_name.c_str());
    fopen_s(&fp, out_file_name.c_str(), "wb");
    if (!fp)
	{
		http_log("<error> open file failed,error code:%d\n", GetLastError());
        return false;
    }

    int down_code = HttpRequest::REQUEST_PERFORM_ERROR;
    int thread_count = SplitDownloadCount(m_total_size);

    m_thread_count = thread_count > m_thread_count ? m_thread_count : thread_count;
    //文件大小有分开下载的必要并且服务器支持多线程下载时，启用多线程下载
    if (m_multi_download && m_thread_count > 1)
    {
        long gap = static_cast<long>(m_total_size) / m_thread_count;
        std::vector<HANDLE> threads;
        for (int i = 0; i < m_thread_count; i++)
        {
            ThreadChunk* thread_chunk = new ThreadChunk;
            thread_chunk->_fp = fp;
            thread_chunk->_download = this;

            if (i < m_thread_count - 1)
            {
                thread_chunk->_startidx = i * gap;
                thread_chunk->_endidx = thread_chunk->_startidx + gap - 1;
            }
            else
            {
                thread_chunk->_startidx = i * gap;
                thread_chunk->_endidx = -1;
            }

            DWORD thread_id;
            HANDLE hThread = CreateThread(NULL, 0, HttpHelper::DownloadWork, thread_chunk, 0, &(thread_id));
            threads.push_back(hThread);
        }

        WaitForMultipleObjects(threads.size(), &threads[0], TRUE, INFINITE);
        for (HANDLE handle : threads)
        {
            CloseHandle(handle);
        }
    }
    else
    {
        ThreadChunk* thread_chunk = new ThreadChunk;
        thread_chunk->_fp = fp;
        thread_chunk->_download = this;
        thread_chunk->_startidx = 0;
        thread_chunk->_endidx = 0;
        down_code = DoDownload(thread_chunk);
    }

    fclose(fp);

    if (m_download_fail == false)
    {
        MoveFileExA(out_file_name.c_str(), src_file_name.c_str(), MOVEFILE_REPLACE_EXISTING);
    }
    else
    {
        DeleteFileA(out_file_name.c_str());
    }

    m_result_callback(m_id, m_download_fail ? false : true, m_error_string);

    m_is_running = false;

    return !m_download_fail;
}

int HttpDownloader::DownloadHelper::DownloadFileSyn(const std::string &strUrl, const std::string &strFilePath)
{
	int iRet = 0;

	CURL *handle = curl_easy_init();

	if (iRet) return iRet;

	FILE *file;
	if (fopen_s(&file, strFilePath.c_str(), "wb"))
	{
		http_log("<error> 新建下载文件失败！文件路径：%s", strFilePath.c_str());
		printf("新建下载文件失败！文件路径：%s", strFilePath.c_str());
		return CURLE_FAILED_INIT;
	}

	curl_easy_setopt(handle, CURLOPT_URL, m_url.c_str());
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, m_head_list);

	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, HttpHelper::write_file);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, file);
	curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 1);
	curl_easy_setopt(handle, CURLOPT_TIMEOUT, 30);	//下载限时30s

	iRet = curl_easy_perform(handle);
	fclose(file);

	if (handle)
	{
		curl_easy_cleanup(handle);
	}
	return iRet;
}

int HttpDownloader::DownloadHelper::UploadFileSyn(const std::string &strUrl, const std::string &strFilePath)
{
	int iRet = 0;

	CURL *handle = curl_easy_init();

	if (iRet) return iRet;

	FILE *file;
	if (fopen_s(&file, strFilePath.c_str(), "rb"))
	{
		http_log("<error> 打开上传文件失败！文件路径：%s", strFilePath.c_str());
		printf("打开上传文件失败！文件路径：%s", strFilePath.c_str());
		return CURLE_FAILED_INIT;
	}

	ULONG64 iFileSize = _filelength(_fileno(file));
	curl_easy_setopt(handle, CURLOPT_URL, m_url.c_str());
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, m_head_list);

	curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(handle, CURLOPT_READFUNCTION, HttpHelper::read_file);
	curl_easy_setopt(handle, CURLOPT_READDATA, file);
	curl_easy_setopt(handle, CURLOPT_INFILESIZE_LARGE, iFileSize);
	curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT_MS, 1000);
	curl_easy_setopt(handle, CURLOPT_TIMEOUT, 30);	//上传限时30s

	iRet = curl_easy_perform(handle);
	fclose(file);

	if (handle)
	{
		curl_easy_cleanup(handle);
	}
	return iRet;
}

bool HttpDownloader::DownloadHelper::GetHeader(std::string* header)
{
    if (m_receive_header.empty()) return false;
    else if (header) *header = m_receive_header;

    return true;
}

bool HttpDownloader::DownloadHelper::GetErrorString(std::string* error_string)
{
    if (m_error_string.empty()) return false;
    else if (error_string) *error_string = m_error_string;

    return true;
}

int HttpDownloader::DownloadHelper::DownloadDefaultCallback(double total_size, double downloaded_size, void* userdata)
{
    return 0;
}

void HttpDownloader::DownloadHelper::ResultDefaultCallback(int id, bool success, const std::string& data)
{
}

double HttpDownloader::DownloadHelper::GetDownloadFileSize()
{
    if (m_url.empty())
    {
        return -1.0;
    }
    else
    {
        double down_file_length = -1.0;
        CURL *handle = curl_easy_init();
        HttpHelper::set_share_handle(handle);

        if (handle)
        {
			std::stringstream out;
            curl_easy_setopt(handle, CURLOPT_URL, m_url.c_str());
            curl_easy_setopt(handle, CURLOPT_HEADER, 1);
            curl_easy_setopt(handle, CURLOPT_NOBODY, 1);
            curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
            curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 5);
            curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, HttpHelper::RetriveHeaderFunction);
            curl_easy_setopt(handle, CURLOPT_HEADERDATA, &m_receive_header);
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, HttpHelper::RetriveContentFunction);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, &out);
            //curl_easy_setopt(handle, CURLOPT_WRITEDATA, NULL);
            curl_easy_setopt(handle, CURLOPT_RANGE, "2-");

            CURLcode curl_code = curl_easy_perform(handle);

            if (curl_code == CURLE_OPERATION_TIMEDOUT)
            {
                int retry_count = m_retry_times;
                while (retry_count > 0)
                {
                    curl_code = curl_easy_perform(handle);
                    if (curl_code != CURLE_OPERATION_TIMEDOUT) break;
                    retry_count--;
                }
            }

            curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &m_http_code);

            if (curl_code == CURLE_OK)
            {
                curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &down_file_length);

                //匹配"Content-Range: bytes 2-1449/26620" 则证明支持多线程下载
                std::regex pattern("CONTENT-RANGE\\s*:\\s*\\w+\\s*(\\d+)-(\\d*)/(\\d+)", std::regex::icase);
                m_multi_download = std::regex_search(m_receive_header, pattern);
            }
            else
            {
               const char* err_string = curl_easy_strerror(curl_code);
               m_error_string = err_string;
            }            

            curl_easy_cleanup(handle);
        }

        return down_file_length;
    }
}

int HttpDownloader::DownloadHelper::DoDownload(ThreadChunk* thread_chunk)
{
    CURL* curl_handle = curl_easy_init();
    HttpHelper::set_share_handle(curl_handle);

    if (thread_chunk->_download->m_url.substr(0, 5) == "https")
    {
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
    }

    curl_easy_setopt(curl_handle, CURLOPT_URL, thread_chunk->_download->m_url.c_str());

    const char* user_agent = ("Mozilla/5.0 (Windows NT 5.1; rv:5.0) Gecko/20100101 Firefox/5.0");
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, user_agent);

    curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_POST, 0L);

    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT_MS, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, thread_chunk->_download->m_time_out);   //0 means block always

    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, HttpHelper::write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, thread_chunk);
    curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, HttpHelper::RetriveHeaderFunction);
    curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, NULL);

    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_XFERINFOFUNCTION, HttpHelper::progress_callback);
    curl_easy_setopt(curl_handle, CURLOPT_XFERINFODATA, thread_chunk);

    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_LIMIT, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_TIME, 5L);
	curl_easy_setopt(curl_handle, CURLOPT_ACCEPT_ENCODING, "gzip");//采用gzip压缩

    if (thread_chunk->_endidx != 0)
    {
        std::string down_range;
        std::ostringstream ostr;
        if (thread_chunk->_endidx > 0)
        {
            ostr << thread_chunk->_startidx << "-" << thread_chunk->_endidx;
        }
        else
        {
            ostr << thread_chunk->_startidx << "-";
        }
        
        down_range = ostr.str();
        curl_easy_setopt(curl_handle, CURLOPT_RANGE, down_range.c_str());
    }

    CURLcode curl_code = curl_easy_perform(curl_handle);
    if (curl_code == CURLE_OPERATION_TIMEDOUT)
    {
        int retry_count = m_retry_times;
        while (retry_count > 0)
        {
            curl_code = curl_easy_perform(curl_handle);
            if (curl_code != CURLE_OPERATION_TIMEDOUT) break;
            retry_count--;
        }
    }

    long http_code;
	curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code);
	if (curl_code == CURLE_OK && (http_code >= 200 && http_code <= 300))
	{
		m_http_code = http_code;
		thread_chunk->_download->m_download_fail = false;
	}
	else
	{
		const char* err_string = curl_easy_strerror(curl_code);
		m_error_string = err_string;
		thread_chunk->_download->m_download_fail = true;
		m_http_code = http_code;
	}

    curl_easy_cleanup(curl_handle);

    delete thread_chunk;

    return curl_code;
}

int HttpDownloader::DownloadHelper::SplitDownloadCount(double down_size)
{
    const double size_2mb = 2.0 * 1024 * 1024;
    const double size_10mb = 10.0 * 1024 * 1024;
    const double size_50mb = 50.0 * 1024 * 1024;

    if (down_size <= size_2mb)
    {
        return 1;
    }
    else if (down_size > size_2mb && down_size <= size_10mb)
    {
        return static_cast<int>(down_size / (size_2mb));
    }
    else if (down_size > size_10mb && down_size <= size_50mb)
    {
        return HttpDownloader::s_kThreadCount + 1;
    }
    else
    {
        int down_count = static_cast<int>(down_size / size_10mb);
        return down_count > 10 ? 10 : down_count;
    }

    return 1;
}

void HttpDownloader::DownloadHelper::Reset()
{
    if (m_is_running)
    {
        return;
    }

    if (m_perform_thread)   //thread run over because if m_is_running set true, Reset wont be invoke
    {
        CloseHandle(m_perform_thread);
        m_perform_thread = nullptr;
    }

    m_close_self = false;
    m_multi_download = false;
    m_download_fail = true;
    m_is_running = false;
    m_is_cancel = false;
    m_http_code = 0;
    m_total_size = 0.0;
    m_downloaded_size = 0.0;
    m_receive_header = "";
    m_error_string = "";
}
