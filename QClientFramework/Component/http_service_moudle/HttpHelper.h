#pragma once


typedef unsigned long DWORD;
#define TRUE    1
#define FALSE   0

class http_lock
{
public:
	http_lock() { InitializeCriticalSection(&m_cs); }
	~http_lock() { DeleteCriticalSection(&m_cs); }

	void Lock() { EnterCriticalSection(&m_cs); }
	void UnLock() { LeaveCriticalSection(&m_cs); }
private:
	CRITICAL_SECTION m_cs;
};

class DoHttpLock
{
public:
	DoHttpLock(std::shared_ptr<http_lock> & lock)
		: m_lock(lock)
	{
		m_lock->Lock();
	}

	~DoHttpLock()
	{
		m_lock->UnLock();
	}

private:
	std::shared_ptr<http_lock> m_lock;
};

class HttpHelper
{
protected:
	HttpHelper()
	{
		CURLcode ret = curl_global_init(CURL_GLOBAL_DEFAULT);
		if (ret != CURLE_OK)
		{
			http_log("<error> curl_global_init error :%d \n", ret);
		}
		s_share_handle = curl_share_init();
		curl_share_setopt(s_share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
	}

public:
	~HttpHelper()
	{
		http_log("<message> curl_share_cleanup \n");
		curl_share_cleanup(s_share_handle);
		curl_global_cleanup();

		s_async_downloads.clear();
	}

	static HttpHelper& Instance()
	{
		static HttpHelper the_single_instance;
		s_id++;
		return the_single_instance;
	}

	static void set_share_handle(CURL* curl_handle)
	{
		curl_easy_setopt(curl_handle, CURLOPT_SHARE, s_share_handle);
		curl_easy_setopt(curl_handle, CURLOPT_DNS_CACHE_TIMEOUT, 60 * 5);
	}

	static std::list< std::shared_ptr<HttpDownloader::DownloadHelper> > s_async_downloads;

	static int s_id;
	static std::shared_ptr<http_lock>       s_download_lock;
	static CURLSH*        s_share_handle;

	static size_t RetriveHeaderFunction(char *buffer, size_t size, size_t nitems, void *userdata)
	{
		std::string* receive_header = reinterpret_cast<std::string*>(userdata);
		if (receive_header && buffer)
		{
			receive_header->append(reinterpret_cast<const char*>(buffer), size * nitems);
		}

		return nitems * size;
	}

	static size_t RetriveContentFunction(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		std::string data((const char*)ptr, (size_t)size * nmemb);

		*((std::stringstream*)stream) << data;

		return size * nmemb;
	}


	static DWORD WINAPI DownloadThread(LPVOID param)
	{
		Sleep(10);
		std::shared_ptr<HttpDownloader::DownloadHelper>* request = reinterpret_cast<std::shared_ptr<HttpDownloader::DownloadHelper>*>(param);
		if (request)
		{
			(*request)->Perform();

			if ((*request)->SelfClose())
			{
				DoHttpLock http_lock(s_download_lock);
				HttpHelper::s_async_downloads.remove(*request);
			}

		}
		return 1;
	}

	static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
	{
		HttpDownloader::DownloadHelper::ThreadChunk* thread_chunk = reinterpret_cast<HttpDownloader::DownloadHelper::ThreadChunk*>(userdata);

		if (thread_chunk->_download->m_is_cancel)
		{
			return 0;
		}

		DoHttpLock http_lock(thread_chunk->_download->m_httplock);
		size_t written = 0;
		int real_size = size * nmemb;
		if (thread_chunk->_endidx > 0)
		{
			if (thread_chunk->_startidx <= thread_chunk->_endidx)
			{
				if (thread_chunk->_startidx + real_size > thread_chunk->_endidx)
				{
					real_size = thread_chunk->_endidx - thread_chunk->_startidx + 1;
				}
			}
		}

		int seek_error = fseek(thread_chunk->_fp, thread_chunk->_startidx, SEEK_SET);
		if (seek_error != 0)
		{
			perror("fseek");
		}
		else
		{
			written = fwrite(ptr, 1, real_size, thread_chunk->_fp);
		}
		thread_chunk->_download->m_downloaded_size += written;
		thread_chunk->_startidx += written;

		return written;
	}

	static int progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
	{
		HttpDownloader::DownloadHelper::ThreadChunk* thread_chunk = reinterpret_cast<HttpDownloader::DownloadHelper::ThreadChunk*>(clientp);

		DoHttpLock http_lock(thread_chunk->_download->m_httplock);

		double total_size = thread_chunk->_download->m_total_size;
		double downloaded_size = thread_chunk->_download->m_downloaded_size;
		void* userdata = thread_chunk->_download->m_userdata;
		int callback_result = thread_chunk->_download->m_download_callback(total_size, downloaded_size, userdata);

		return callback_result;
	}

	static DWORD WINAPI DownloadWork(LPVOID param)
	{
		HttpDownloader::DownloadHelper::ThreadChunk* thread_chunk = reinterpret_cast<HttpDownloader::DownloadHelper::ThreadChunk*>(param);
		return thread_chunk->_download->DoDownload(thread_chunk);
	}

	static size_t read_file(char *buffer, size_t size, size_t nitems, void *userdata)
	{
		if (nullptr == userdata || nullptr == buffer)	return 0;
		return fread(buffer, size, nitems, (FILE*)userdata);
	}

	static size_t write_file(char *buffer, size_t size, size_t nitems, void *userdata)
	{
		if (nullptr == userdata || nullptr == buffer)	return 0;
		return fwrite(buffer, size, nitems, (FILE*)userdata);
	}
};

std::list< std::shared_ptr<HttpDownloader::DownloadHelper> > HttpHelper::s_async_downloads;
int HttpHelper::s_id = 1;
std::shared_ptr<http_lock> HttpHelper::s_download_lock(new http_lock);
CURLSH* HttpHelper::s_share_handle = nullptr;
