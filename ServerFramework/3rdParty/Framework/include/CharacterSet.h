#pragma once


#include <string>
#include <wtypes.h>
#include <stdint.h>

namespace mqwUtils
{
	std::wstring ANSIToUnicode(const char* strANSI);
	std::string UnicodeToANSI(const wchar_t* strUnicode);
	std::wstring UTF8ToUnicode(const char* strUTF8);
	std::string UnicodeToUTF8(const wchar_t* strUnicode);

	std::string ANSIToUTF8(const char* strANSI);

	std::string UTF8ToANSI(const char* strUTF8);

	//字符替换函数
	void replace(LPSTR source, char look, char dest);
	

	// 内存数据格式转换为字符串，如内存数据0x33 0x61 0x41-> 3aA
	ULONG hex_to_bin(const char *src, ULONG len, char *out);
	// 与上函数相反
	ULONG bin_to_hex(const char *src, ULONG len, char * out, bool mode, char *ktext, int hlen);  


	uint64_t bswap_64(uint64_t value);    // 数据位转换，下同
	ULONG bswap_32(ULONG value);
	USHORT bswap_16(ULONG value);
}
