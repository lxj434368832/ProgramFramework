#ifndef ZXU_DOMAIN_NAME_H_
#define ZXU_DOMAIN_NAME_H_
/************************************************************************
*function: 域名相关的文件,目前只支持将域名解析成IP地址
*author：  明巧文
*company： ambition tech
*datetime：2017-11-29
************************************************************************/

#include <list>
#include <ws2tcpip.h>

namespace zxu
{
	enum e_domain_name_code
	{
		ednc_no_error = 0,				//获取成功，没有错误
		ednc_af_error = 1,				//获取的地址家族错误
		ednc_host_not_found = WSAHOST_NOT_FOUND,	//主机未找到
		ednc_host_name_error = WSANO_DATA			//主机名字错误
							//其余为通过WSAGetLastError()获取的错误码
	};
	
	/************************************************************************
	*function:域名解析,只能获取IPV4地址。前提是要先调用WSAStartup(）初始化winsock
	*后续需要使用InetPton，InetNtop进行相关的转换
	*param host_name:空字符结束的域名标识，如www.baidu.com
	*param add_list: 获取到的IP地址列表，一般只有一个
	*return int：   具体含义参见eDomainNameCode
	************************************************************************/
	int get_host_ipv4(char *host_name, std::list<std::wstring> &list);

	/************************************************************************
	*function:域名解析,只能获取IPV6地址。前提是要先调用WSAStartup(）初始化winsock
	*后续需要使用InetPton，InetNtop进行相关的转换
	*param host_name:空字符结束的域名标识，如www.baidu.com
	*param add_list: 获取到的IP地址列表，一般只有一个
	*return int：   具体含义参见eDomainNameCode
	************************************************************************/
	int get_host_ipv6(char *host_name, std::list<std::wstring> &list);
}


/* 以下是调用示例,仅供参考
int main(int argc, char **argv)
{
WSADATA wsaData;
int iResult;

// 此处应添加的程序如下
iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
if (iResult != 0)
{
std::cout << "初始化WSAStartup失败!" << std::endl;
return 1;
}

std::list<std::wstring> strIP_list;
char *host_name = "www.cnambition.com";
std::cout << "解析的名字是 :" << host_name << std::endl;
int retval = zxu::get_host_ipv4(host_name, strIP_list);

switch (retval)
{
case zxu::ednc_no_error:
std::cout << "获取主机IP成功，列表为：\n";
for each (auto strIP in strIP_list)
{
std::wcout << "IP address：" << strIP.c_str() << std::endl;
}
break;
case zxu::ednc_af_error:
std::cout << "获取的地址家族错误!" << std::endl;
break;
case zxu::ednc_host_not_found:
std::cout << "获取IP地址错误，主机未找到！" << std::endl;
break;
case zxu::ednc_host_name_error:
std::cout << "获取IP地址错误，主机域名错误，请检查" << std::endl;
break;
default:
std::cout << "获取IP地址错误，错误码是：" << retval << std::endl;
break;
}

system("pause");
WSACleanup();
return 0;
}
*/

#endif
