/************************************************************************
* function:进程辅助函数
* author :	明巧文
* datetime:2018-04-03
* company:  安碧捷科技股份有限公司
/************************************************************************/
#pragma once

#include <windows.h>
#include <string>
#include <list>
#include <map>

class ProcessHandle
{
public:
	/*************************************************************************
	* function:		获得模块路径
	* param:		module模块句柄，如果为空，则获取当前进程的路径
	* return:		当前模块对应的路径
	*************************************************************************/
	static std::string GetModulePath(HMODULE module);
	/*************************************************************************
	* function:		通过进程名字获取PID
	* param pName:	in,进程exe名字
	* return:		当前名对应的进程PID
	*************************************************************************/
	static DWORD GetProcessIDByName(LPCSTR pName);

	/*************************************************************************
	* function:			通过进程ID检测进程是否存在
	* param dwProcessID:in,进程PID
	* return:			进程句柄
	*************************************************************************/
	static bool CheckProcessRunByPID(DWORD dwProcessID);

	/*************************************************************************
	* function:		通过进程名字获取PID列表，适用于启动多个相同的程序
	* param pName:	in,进程exe名字
	* return:		当前名对应的进程PID列表
	*************************************************************************/
	static std::list<DWORD> GetProcessIDListByName(LPCSTR pProcName);

	/*************************************************************************
	* function:			通过进程ID获取进程路径
	* param dwProcessID:in,进程的PID
	* param buffer:		out,进程的路径
	*************************************************************************/
	static void GetProcessPathByPID(DWORD dwProcessID, LPSTR buffer);

	/*************************************************************************
	* function:		通过进程名字获取进程路径
	* param pName:	in,进程名字
	* param buffer:	out,进程的路径
	*************************************************************************/
	static void GetProcessPathByName(LPCSTR pName, LPSTR buffer);

	/*************************************************************************
	* function:		通过进程名获取进程的目录
	* param pName:	in,进程名字
	* param buffer:	out,进程的目录
	*************************************************************************/
	static void GetProcessDirByName(LPCSTR pName, LPSTR buffer);

	/*************************************************************************
	* function:		通过进程名获取进程的目录列表
	* param pName:	in,进程名字
	* param DirList:out,进程的目录列表
	*************************************************************************/
	static void GetProcessDirListByName(LPCSTR pProcName, std::map<DWORD, std::string> &DirList);

	/*************************************************************************
	* function:			通过进程ID获取进程句柄
	* param dwProcessID:in,进程PID
	* return:			进程句柄
	*************************************************************************/
	static HANDLE GetProcessHandleByPID(DWORD dwProcessID);

	/*************************************************************************
	* function:			通过进程名获取进程句柄
	* param pName:		in,进程名
	* return:			进程句柄
	*************************************************************************/
	static HANDLE GetProcessHandleByName(LPCSTR pName);

	/*************************************************************************
	* function:			终止进程
	* param pName:		in,进程名
	* return:			成功返回true，失败false
	*************************************************************************/
	static bool  TerminateProcess(LPCSTR pName);

	/*************************************************************************
	* function:			通过进程的PID终止进程
	* param pName:		in,进程名
	* return:			成功返回true，失败false
	*************************************************************************/
	static bool  TerminateProcByPID(DWORD dwPID);

	/*************************************************************************
	* function:			终止进程，使用nt内核函数终止
	* param pName:		in,进程名
	* return:			成功返回true，失败false
	*************************************************************************/
	static bool NTTerminateProcess(LPCSTR lpName);

	/*************************************************************************
	* function:				创建进程
	* param strAppName:		in,应用程序名字
	* param strCmdline:		in,传递给应用程序的命令行参数
	* param strCurDir:		in,应用程序的当前工作目录
	* return:				成功返回true,失败false
	*************************************************************************/
	static bool CreateProcess(LPCSTR strAppName,LPSTR strCmdline,LPCSTR strCurDir);
	static bool CreateProcessByCmd(LPCSTR strAppName, LPSTR strCmdline, LPCSTR strCurDir);
	static BOOL CreateProcessTUser(LPCSTR strAppName, LPSTR strCmdline, LPCSTR strCurDir);
	static BOOL CreateProcessTKernel(LPCSTR strAppName, LPSTR strCmdline, LPCSTR strCurDir);

	static BOOL GetTokenByName(LPTSTR lpName, HANDLE &hToken);

	static BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpPrivilegeName, BOOL bEnablePrivilege);

protected:

	class MHandle
	{
	public:
		MHandle() :m_handle(NULL){}
		MHandle(HANDLE handle) : m_handle(handle) {}

		~MHandle()
		{
			if (m_handle)
			{
				::CloseHandle(m_handle);
			}
		}

		operator HANDLE()
		{
			return m_handle;
		}

		HANDLE* operator &(){ return &m_handle; }
		HANDLE& operator *() = delete;

	private:
		HANDLE m_handle;
	};
};

