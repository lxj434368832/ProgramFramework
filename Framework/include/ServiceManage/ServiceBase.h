#ifndef SERVICE_BASE_H_
#define SERVICE_BASE_H_  
/************************************************************************
* function：服务所有任务的实现，控制程序的运行、停止、暂停、恢复等
* author :	明巧文
* datetime：2018-04-12
* company:  安碧捷科技股份有限公司
/************************************************************************/

#include <windows.h>
#include <atlstr.h>
#include "CopyDisable.h"

// Base Service class used to create windows services.  
class ServiceBase
{
public:
	//DISABLE_COPY_MOVE(ServiceBase); 
	ServiceBase(const CString& name,
		const CString& displayName,
		DWORD dwStartType,
		DWORD dwErrCtrlType = SERVICE_ERROR_NORMAL,
		DWORD dwAcceptedCmds = SERVICE_ACCEPT_STOP,
		const CString& depends = _T(""),
		const CString& account = _T(""),
		const CString& password = _T(""));

	virtual ~ServiceBase() {}

	//cmdline main enter
	int ProcessMain(int argc, _TCHAR* argv[]);

	const CString& GetName() const { return m_name; }
	const CString& GetDisplayName() const { return m_displayName; }
	const DWORD GetStartType() const { return m_dwStartType; }
	const DWORD GetErrorControlType() const { return m_dwErrorCtrlType; }
	const CString& GetDependencies() const { return m_depends; }

	// Account info service runs under.  
	const CString& GetAccount() const { return m_account; }
	const CString& GetPassword() const { return m_password; }

protected:

	void SetStatus(DWORD dwState, DWORD dwErrCode = NO_ERROR, DWORD dwWait = 0);

	// TODO(Olster): Move out of class/make static.  
	// Writes |msg| to Windows event log.  
	void WriteToEventLog(const CString& msg, WORD type = EVENTLOG_INFORMATION_TYPE);
	void LogEvent(WORD type, LPCTSTR pszFormat, ...);

	virtual void OnStart(DWORD argc, TCHAR* argv[])= 0;
	virtual void OnStop() {}
	virtual void OnPause() {}
	virtual void OnContinue() {}
	virtual void OnShutdown() {}

	virtual void OnSessionChange(DWORD /*evtType*/,
		WTSSESSION_NOTIFICATION* /*notification*/) {}

private:
	void DisplayMainUsage();
	bool CmdRun();
	bool DebugRun();
	static BOOL WINAPI ControlCtrlHandler(DWORD dwCtrlType);

	bool ServiceRun();
	// Registers handle and starts the service.  
	static void WINAPI SvcMain(DWORD argc, TCHAR* argv[]);

	// Called whenever service control manager updates service status.  
	static DWORD WINAPI ServiceCtrlHandler(DWORD ctrlCode, DWORD evtType,
		void* evtData, void* context);

	//execute sevice relation command
	void Start(DWORD argc, TCHAR* argv[]);
	void ContinueRun();
	void Stop();
	void Pause();
	void Continue();
	void Shutdown();

	DISABLE_COPY_MOVE(ServiceBase);

private:
	CString m_name;
	CString m_displayName;
	DWORD m_dwStartType;
	DWORD m_dwErrorCtrlType;
	CString m_depends;
	CString m_account;
	CString m_password;

	// Info about service dependencies and user account.  
	bool m_hasDepends;/* = false*/;
	bool m_hasAcc;/* = false*/;
	bool m_hasPass;/* = false*/;

	SERVICE_STATUS m_svcStatus;
	SERVICE_STATUS_HANDLE m_svcStatusHandle;

	bool	m_bRunning;
	static ServiceBase* m_service;
};

#endif // SERVICE_BASE_H_</span>  