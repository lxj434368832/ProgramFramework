#include "stdafx.h"
#include <stdio.h>
#include <time.h>
#include <string>
#include <tchar.h>
#include <sstream>




std::string get_full_pathex()
{
	char cpath[MAX_PATH] = { 0 };
	char cdrive[MAX_PATH] = { 0 };
	char cdir[MAX_PATH] = { 0 };
	//获得当前exe的路径
	GetModuleFileNameA(NULL, cpath, MAX_PATH);
	_splitpath_s(cpath, cdrive,MAX_PATH, cdir,MAX_PATH, 0, 0,0,0);
	std::stringstream tmp_str;
	tmp_str << cdrive << cdir;
	return tmp_str.str();
}

//日志
void http_log(char const *msg, ...)
{
	static char sPath[256] = { 0 };

	time_t ulNow = ::time(nullptr);
	tm tmNow;
	::localtime_s(&tmNow, &ulNow);

	if (0 == sPath[0])
	{
		sprintf_s(sPath, sizeof(sPath), "%shttp_log/%4d%02d%02d%02d%02d%02d",
			get_full_pathex().c_str(),
			tmNow.tm_year + 1900,
			tmNow.tm_mon + 1,
			tmNow.tm_mday,
			tmNow.tm_hour,
			tmNow.tm_min,
			tmNow.tm_sec);
	}

	FILE *fp;
	if (!msg)
	{
		return;
	}
	fopen_s(&fp, sPath, "a");
	if (fp==nullptr)
	{
		//printf("open file \"%s\" err \n" ,sPath);
		return;
	}

	fprintf(fp, "%4d-%02d-%02d %02d:%02d:%02d \n",
		tmNow.tm_year + 1900,
		tmNow.tm_mon + 1,
		tmNow.tm_mday,
		tmNow.tm_hour,
		tmNow.tm_min,
		tmNow.tm_sec);

	va_list args;
	va_start(args, msg);
	vfprintf(fp, msg, args);
	va_end(args);
	fflush(fp);
	fclose(fp);

}