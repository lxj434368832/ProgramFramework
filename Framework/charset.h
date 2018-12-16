#pragma once

#include <string>
#include <windows.h>

class charset
{
public:
	charset() {};
	~charset() {};
public:
	static ULONG bin_to_hex(const char *src, ULONG len, char * out, bool mode, char *ktext, int hlen);     // 与下函数相反
	static ULONG hex_to_bin(const char *src, ULONG len, char *out);                                        // 内存数据格式转换为字符串，如内存数据0x33 0x61 0x41-> 3aA
	static int uni_to_ans(const wchar_t *src_str, char *dst_str, int len);                                  // UNICODE转ANSI
	static int ans_to_uni(const char *src_str, wchar_t *dst_str, int len);                                  // ANSI转UNICODE
	static int utf8_to_ans(const char *src_str, char *dst_str, int len);                                    // UTF-8转ANSI
	static int ans_to_utf8(const char *src_str, char *dst_str, int len);                                    // ANSI转UTF-8

	static const std::string ans_to_utf8(const std::string &a_str);                                         // ANSI转UTF-8(c++11)
	static const std::string utf8_to_ans(const std::string &u_str);                                         // UTF-8转ANSI(c++11)
	static const std::string ws_to_s(const std::wstring& src);
	static const std::wstring s_to_ws(const std::string& src);
	static const std::string ws_to_utf8(const std::wstring& src);
	static const std::wstring utf8_to_ws(const std::string& src);

	static uint64_t bswap_64(uint64_t value);                                                        // 数据位转换，下同
	static ULONG bswap_32(ULONG value);
	static USHORT bswap_16(ULONG value);
};
