#pragma once


#include <string>
#include <wtypes.h>
#include <stdint.h>

namespace mqwUtils
{
	//以下是字符转换函数，由于返回的内存是malloc申请的，所以需要外部free
	std::wstring ANSIToUnicode(const char* strANSI);
	std::string UnicodeToANSI(const wchar_t* strUnicode);
	std::wstring UTF8ToUnicode(const char* strUTF8);
	std::string UnicodeToUTF8(const wchar_t* strUnicode);

	std::string ANSIToUTF8(const char* strANSI);

	std::string UTF8ToANSI(const char* strUTF8);


	/*多字符转换为宽字符ANSI - Unicode*/
	wchar_t* Acsi2WideByte(const char* mbs);
	/*宽字符转换为多字符Unicode - ANSI*/
	char* w2m(const wchar_t* wcs);
	std::wstring ANSI2Unicode(const std::string & str);
	std::string Unicode2ANSI(const std::wstring & wstr);
	std::wstring UTF82Unicode(const std::string & str);

	//字符替换函数
	void replace(LPSTR source, char look, char dest);
	
	// 与下函数相反
	ULONG bin_to_hex(const char *src, ULONG len, char * out, bool mode, char *ktext, int hlen); 
	// 内存数据格式转换为字符串，如内存数据0x33 0x61 0x41-> 3aA
	ULONG hex_to_bin(const char *src, ULONG len, char *out);       


	uint64_t bswap_64(uint64_t value);    // 数据位转换，下同
	ULONG bswap_32(ULONG value);
	USHORT bswap_16(ULONG value);
}
