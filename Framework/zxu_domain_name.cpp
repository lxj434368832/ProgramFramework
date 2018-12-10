#include "../../include/zxu_utils/zxu_domain_name.h"

int get_host_ip(char *host_name, ADDRESS_FAMILY af, std::list<std::wstring> &list);

int zxu::get_host_ipv4(char *host_name, std::list<std::wstring> &list)
{
	return get_host_ip(host_name, AF_INET, list);
}

int zxu::get_host_ipv6(char *host_name, std::list<std::wstring> &list)
{
	return get_host_ip(host_name, AF_INET6, list);
}

int get_host_ip(char *host_name, ADDRESS_FAMILY af, std::list<std::wstring> &list)
{
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	DWORD dwRetval = 0;

	TCHAR ipstringbuffer[200];
	DWORD bufferLength = 200;

	list.clear();

	//--------------------------------
	// Setup the hints address info structure
	// which is passed to the getaddrinfo() function
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = 0;
	hints.ai_protocol = 0;

	//--------------------------------
	// Call getaddrinfo(). If the call succeeds,
	// the result variable will hold a linked list
	// of addrinfo structures containing response
	// information
	dwRetval = getaddrinfo(host_name, NULL, &hints, &result);
	if (dwRetval != 0)
	{
		return dwRetval = WSAGetLastError();
	}

	// Retrieve each address and print out the hex bytes
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		dwRetval = WSAAddressToString(ptr->ai_addr, ptr->ai_addrlen, NULL, ipstringbuffer, &bufferLength);

		if (dwRetval)
		{
			dwRetval = WSAGetLastError();
			continue;
		}

		if (af == ptr->ai_family)
		{
			list.push_back(ipstringbuffer);
			dwRetval = 0;
		}
	}
	freeaddrinfo(result);
	dwRetval = list.size() ? 0 : 1;
	return dwRetval;
}

#ifdef _WINSOCK_DEPRECATED_NO_WARNINGS
/************************************************************************
*function:域名解析,只能获取IPV4地址。前提是要先调用WSAStartup(）初始化winsock
*			这个是使用的老版本，建议使用上面支持ipv6的方式
*param host_name:空字符结束的域名标识，如www.baidu.com
*param add_list: 获取到的IP地址列表，一般只有一个
*return bool：   成功返回true，失败返回false
************************************************************************/
bool get_host_ip(char *host_name, std::list<std::string>& add_list)
{
	bool bRet = true;
	DWORD dwError = 0;
	int i = 0;
	hostent *remoteHost = NULL;
	in_addr addr;
	char **pAlias = NULL;

	if (NULL == host_name)
	{
		std::cout << "字符串为空，请检查！" << std::endl;
		return false;
	}

	remoteHost = gethostbyname(host_name);

	if (remoteHost == NULL)
	{
		dwError = WSAGetLastError();
		if (0 != dwError)
		{
			if (WSAHOST_NOT_FOUND == dwError)
			{
				std::cout << "主机未找到！" << std::endl;
			}
			else if (dwError == WSANO_DATA)
			{
				std::cout << "请求的数据错误，请检查！" << std::endl;
			}
			else
			{
				std::cout << "获取主机IP失败,错误码：" << dwError;
			}
		}
		std::cout << "域名解析出错！" << std::endl;
		bRet = false;
	}
	else
	{
		std::cout << "主机名：" << remoteHost->h_name << std::endl;
		for (pAlias = remoteHost->h_aliases; *pAlias != NULL; pAlias++)
		{
			std::cout << "可选的主机名#" << i << ": " << *pAlias << std::endl;
		}

		i = 0;
		if (AF_INET == remoteHost->h_addrtype)
		{
			while (remoteHost->h_addr_list[i] != 0)
			{
				addr.s_addr = *(u_long*)remoteHost->h_addr_list[i++];
				add_list.push_back(inet_ntoa(addr));
			}
		}
		else
		{
			std::cout << "解析的地址类型错误！" << std::endl;
			bRet = false;
		}
	}

	return bRet;
}

#endif // _WINSOCK_DEPRECATED_NO_WARNINGS

