#pragma once
/*************************************************************************
* function：线程定时器自定义
* author :	明巧文
* datetime：2018-01-05
* company:  安碧捷科技股份有限公司
*************************************************************************/

#include <thread>
#include <functional>

typedef std::function<void()> funTimerProc;

class MTimer
{
public:
	MTimer();
	~MTimer();
	/*************************************************************************
	* function：		开启一个定时器，定时器会在每次处理完毕之后再重新定时
	* param uInterval:	定时时间
	* param lpTimerFunc:回调函数
	* param isStartCall:是否一启动就调用
	* return bool:		成功返回true,失败返回false
	*************************************************************************/
	bool Start(unsigned uInterval, funTimerProc lpTimerFunc, bool bStartCall = false);

	/*************************************************************************
	* function：		停止前面创建的定时器
	* return void:		无
	*************************************************************************/
	void Stop();

private:
	void TimerCallback(unsigned uInterval, bool bStartCall);

private:
	std::thread		*m_pTimerThread = nullptr;	//定时器线程
	HANDLE 			m_hTimerEvent = nullptr;	//定时器事件
	funTimerProc	m_funTimerProc = nullptr;	//回调函数
};

