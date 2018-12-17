#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <string>

namespace mqwUtils
{
	//以下是字符转换函数，由于返回的内存是malloc申请的，所以需要外部free
	wchar_t * ANSIToUnicode(const char* str);
	char * UnicodeToANSI(const wchar_t* str);
	wchar_t * UTF8ToUnicode(const char* str); 
	char * UnicodeToUTF8(const wchar_t* str);

	/*宽字符转换为多字符Unicode - ANSI*/
	char* w2m(const wchar_t* wcs);
	/*多字符转换为宽字符ANSI - Unicode*/
	wchar_t* m2w(const char* mbs);

	char* ANSIToUTF8(const char* str);
	void  ANSIToUTF8(std::string& str);

	char* UTF8ToANSI(const char* str);
	void UTF8ToANSI(std::string& str);

	//字符替换函数
	void replace(LPSTR source, char look, char dest);
}
