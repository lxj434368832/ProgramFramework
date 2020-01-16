/************************************************************************
* function:文件辅助函数
* author :	明巧文
* datetime:2018-04-03
* company:  安碧捷科技股份有限公司
/************************************************************************/
#pragma once
#include <wtypes.h>
#include <string>
#include <vector>
#include <list>

class FileHandle
{
public:
	FileHandle();
	~FileHandle();

	/*************************************************************************
	* function:			获取文件版本号
	* param FilePath:	in,文件的路径
	* return:			当前文件对应的版本号
	*************************************************************************/
	static void GetFileVersion(LPCSTR FilePath, LPSTR buffer);

	/*************************************************************************
	* function:			获取父目录，自动去掉末尾的/或\,
	* param strPath:	in,需要处理的路径名
	* return:			当前路径对应的父目录
	*************************************************************************/
	static std::string GetParentDir(LPCSTR strPath);

	/*************************************************************************
	* function:			创建目录，包括多级目录
	* param strDir:		in,目录名
	* return:			0表示成功，否则为错误代码
	*************************************************************************/
	static int CreateDir(LPCSTR strDir);

	/*************************************************************************
	* function:			重命名加上时间戳，自动去掉末尾的/或\,
	* param lpName:		in,需要重命名的名字
	* return:			成功返回true,失败返回false
	*************************************************************************/
	static std::string RenameWithTimeStamp(LPCSTR lpName);

	/*************************************************************************
	* function:			将文件或目录名加上时间戳
	* param lpName:		in,需要加时间戳的名字
	* return:			成功返回true,失败返回false
	*************************************************************************/
	static std::string AddTimeStapToName(LPCSTR lpName);

	/*************************************************************************
	* function:			拷贝文件，包括目录，使用的是命令行的方式
	* param lpSource:	in,需要拷贝的源文件或源文件夹
	* param lpDest:		in,需要拷贝的目标文件或目标文件夹
	* return:			成功返回true,失败返回false
	*************************************************************************/
	static BOOL CopyFile(LPCTSTR lpSource, LPCTSTR lpDest);

	/*************************************************************************
	* function:			拷贝文件夹
	* param lpSource:	in,需要拷贝的源文件夹
	* param lpDest:		in,需要拷贝的目标文件夹
	* return:			成功返回true,失败返回false
	*************************************************************************/
	static bool CopyDirectory(LPCTSTR lpSource, LPCTSTR lpDest);

	/*************************************************************************
	* function:			删除文件，包括目录，使用的是命令行的方式
	* param lpPath:		in,需要删除的文件或文件夹
	* return:			成功返回true,失败返回false
	*************************************************************************/
	static BOOL DeleteFile(LPCTSTR lpPath);

	/*************************************************************************
	* function:			删除文件夹
	* param lpSource:	in,需要删除的源文件夹
	* return:			成功返回true,失败返回false
	*************************************************************************/
	static bool DeleteDirectory(LPCTSTR lpSource);

	/*************************************************************************
	* function:			获取目录下的所有文件
	* param lpDir:		in,目录路径
	* param filter:		in,过滤条件
	* return:			文件列表
	*************************************************************************/
	static std::list<std::string> GetAllFile(LPCTSTR lpDir,LPCTSTR filter);
};

