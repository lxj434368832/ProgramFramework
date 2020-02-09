#pragma once
/*************************************************************************
* function：定时器自定义
* author :	明巧文
* datetime：2018-01-05
* company:  安碧捷科技股份有限公司
*************************************************************************/

#include <wtypes.h>
#include <map>
#include <mutex>
#include <functional>

typedef std::function<void(DWORD timerID)> funTimerProc;

class MTimer
{
public:
	MTimer();
	~MTimer();
	DWORD GetTimerId();

	/*************************************************************************
	* function：		开启一个定时器，定时器会在每次处理完毕之后再重新定时
	* param uElapse:	定时时间
	* param lpTimerFunc:回调函数
	* param isStartCall:是否一启动就调用
	* return bool:		成功返回true,失败返回false
	*************************************************************************/
	bool start(UINT uElapse, funTimerProc lpTimerFunc,bool isStartCall=false);

	/*************************************************************************
	* function：		停止前面创建的定时器
	* return void:		无
	*************************************************************************/
	void stop();

	/*************************************************************************
	* function：		开启一个定时器，走的是SetTimer的流程
	* param uElapse:	定时时间
	* param lpTimerFunc:回调函数
	* param dwUser:		用户自定义数据
	* return bool:		成功返回true,失败返回false
	*************************************************************************/
	bool start_timer(UINT uElapse, funTimerProc lpTimerFunc);

	void stop_timer();
	
private:
	static DWORD WINAPI onThreadProc(LPVOID lpParameter);
	DWORD ThreadProc();

	static DWORD WINAPI onThreadProc1(LPVOID lpParameter);
	DWORD ThreadProc1();

	static void CALLBACK onTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

private:
	HANDLE			m_hThread = NULL;
	bool			m_bStart = true;
	UINT			m_uElapse = 0;
	funTimerProc	m_funTimerProc;
	bool			m_bIsStartCall = false;

	DWORD_PTR		m_dwUser = 0;

	UINT			m_uTimerID = 0;
	funTimerProc	m_funTimerProc1;
	static	std::map<UINT, funTimerProc>	s_mapUser;	//时间id和用户数据的映射关系
	static std::recursive_mutex			s_mtxMap;	//
};

