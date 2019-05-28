#pragma once
/*******************************************************************
* @版权信息：
* @文件名称：LogFile.h
* @摘    要：日志操作定义头文件
* @作    者：明巧文
* @当前版本：1.0.0
* @日    期：2019年5月21日
* @备    注：线程安全日志类
*******************************************************************/

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <wtypes.h>
#include <sstream>

class SingleLog
{
public:
	SingleLog();
	~SingleLog();
	void AddLog(char* format, ...);
	template <typename T>
	inline SingleLog& operator<<(const T& log)
	{
		m_strStream << log;
		return *this;
	}

	inline SingleLog& operator<<(std::ostream& (*log)(std::ostream&))
	{
		m_strStream << log;
		return *this;
	}

private:
	char	m_szLineLog[256];
	std::stringstream m_strStream;
};

class LogFile
{
public:
	LogFile();
	~LogFile();
	static LogFile* GetInstance()
	{
		return s_instance;
	}
	void AddLog(std::string &strLog);

private:
	void CheckFileName();
	void WriteLogThread();

	void WriteLog(std::string &strLog);

	static void CALLBACK TimeCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

private:
	static LogFile* s_instance;
	bool			m_bStart;			//是否开始的标识
	std::thread		*m_pThread;
	std::queue<std::string>	m_logList;
	std::mutex				m_mutexLogList;
	std::condition_variable	m_cvConsumer;
	char					m_szFileName[MAX_PATH];
	int						m_iDayOfYear;
	UINT					m_unTimerId;
};


#define Log() (SingleLog())<< __FUNCTION__<<":"<<__LINE__
#define logm() Log()<<"]<Info> "
#define logd() Log()<<"]<Debug> "
#define logw() Log()<<"]<Warn> "
#define loge() Log()<<"]<Error> "

//以下调用方式的消息长度不能超过256
#define LOGM(format, ...)  do{  SingleLog log; \
    log.AddLog("%s:%d]<Info> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGD(format, ...)  do{  SingleLog log; \
    log.AddLog("%s:%d]<Debug> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGW(format, ...)  do{  SingleLog log; \
    log.AddLog("%s:%d]<Warn> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGE(format, ...)  do{  SingleLog log; \
    log.AddLog("%s:%d]<Error> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
