#ifdef WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#endif

#include "sockets.h"


#if defined(_MSC_VER)
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE (1)
#endif
#pragma warning(disable : 4996)
#endif


//*****************************************************************************
//	函数说明：	根据s获取socket名
//	函数原型：	int get_sock_name(SOCKET s, sockaddr_in *sock)
//  参数：      s查询的socket句柄、sock获取的数据
//  返回值：    len
//  用法：		
//*****************************************************************************
int sockets::get_sock_name(sock_fd s, sockaddr_in *sock)
{
	sockaddr sockdata;
	int len = sizeof(sockdata);
	len = getsockname(s, &sockdata, &len);
	memcpy(sock, &sockdata.sa_data, sizeof(sockdata.sa_data));
	return len;
}


//*****************************************************************************
//	函数说明：	TCP连接初始化
//	函数原型：	SOCKET tcp_link(char * ip, int port)
//  参数：      ip地址、port端口
//  返回值：    成功返回socket句柄值，否则返回0
//  用法：		
//*****************************************************************************
sock_fd sockets::tcp_link(const std::string &ip, int port)
{
	struct	sockaddr_in	sock;
	struct	hostent	*host = 0;

	sock_fd hsocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (hsocket == INVALID_SOCKET) return 0;
	memset(&sock, 0, sizeof(sock));
	sock.sin_addr.s_addr = inet_addr(ip.c_str());
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);

	if (sock.sin_addr.s_addr == INADDR_NONE && (host = gethostbyname(ip.c_str())))
		memcpy(&sock.sin_addr, host->h_addr_list[0], host->h_length);

	if (connect(hsocket, (struct sockaddr *)&sock, sizeof(sock)) != SOCKET_ERROR)
		return hsocket;
	closesocket(hsocket);
	return 0;
}

//*****************************************************************************
//	函数说明：	TCP监听初始化
//	函数原型：	SOCKET tcp_listen(u_short port)
//  参数：      port端口
//  返回值：    正确hsocket、 错误0
//  用法：		同上
//*****************************************************************************
sock_fd sockets::tcp_listen(const std::string &ip, u_short port)
{
	sock_fd hsocket;
	struct sockaddr_in sock;
	struct hostent *host = 0;
	hsocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == hsocket)
	{
		return 0;
	}
	ZeroMemory((char *)&sock, sizeof(sock));
	sock.sin_family = AF_INET;
	if (!ip.empty())
		sock.sin_addr.s_addr = inet_addr(ip.c_str());
	else
		sock.sin_addr.s_addr = htonl(INADDR_ANY);
	sock.sin_port = htons(port);
	// 绑定地址和端口
	if (SOCKET_ERROR == bind(hsocket, (struct sockaddr *) &sock, sizeof(sock)))
	{
		closesocket(hsocket);
		return 0;
	}
	// 开始进行监听
	if (SOCKET_ERROR == listen(hsocket, SOMAXCONN))
	{
		closesocket(hsocket);
		return 0;
	}
	return hsocket;
}

//*****************************************************************************
//	函数说明：	UDP连接初始化
//	函数原型：	SOCKET udp_link(char * ip, int port, sockaddr_in &sock)
//  参数：      ip地址、port端口、sock为SOCKADDR_IN结构
//  返回值：    同上
//  用法：		同上
//*****************************************************************************
sock_fd sockets::udp_link(const std::string &ip, int port, sockaddr_in &sock)
{
	struct hostent *host = 0;

	sock_fd hsocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (hsocket == INVALID_SOCKET) return 0;
	memset(&sock, 0, sizeof(sock));
	sock.sin_addr.s_addr = inet_addr(ip.c_str());
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);

	if (sock.sin_addr.s_addr == INADDR_NONE && (host = gethostbyname(ip.c_str())))
		memcpy(&sock.sin_addr, host->h_addr_list[0], host->h_length);

	return hsocket;
}

//*****************************************************************************
//	函数说明：	UDP监听初始化
//	函数原型：	SOCKET udp_listen(u_short port)
//  参数：      port端口
//  返回值：    正确hsocket、 错误0
//  用法：		同上
//*****************************************************************************
sock_fd sockets::udp_listen(const std::string &ip, u_short port)
{
	sock_fd hsocket;
	struct sockaddr_in sock;
	struct hostent *host = 0;
	hsocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == hsocket)
	{
		return 0;
	}
	ZeroMemory((char *)&sock, sizeof(sock));
	sock.sin_family = AF_INET;
	if (!ip.empty())
		sock.sin_addr.s_addr = inet_addr(ip.c_str());
	else
		sock.sin_addr.s_addr = htonl(INADDR_ANY);
	sock.sin_port = htons(port);
	// 绑定地址和端口
	if (SOCKET_ERROR == bind(hsocket, (struct sockaddr *) &sock, sizeof(sock)))
	{
		closesocket(hsocket);
		return 0;
	}
	return hsocket;
}


//*****************************************************************************
//	函数说明：	IP地址转换，由DWORD转为网络字符串类型
//	函数原型：	const char* inet_ntoa(unsigned long ip_addr)
//  参数：      （ip_addr）DWORD类型的IP地址
//  返回值：    正确IP地址、 错误XXX.XXX.XXX.XXX
//  用法：		
//*****************************************************************************
const char* sockets::inet_ntoa(unsigned long ip_addr)
{
	static char* lpErrorAddr = "XXX.XXX.XXX.XXX";

	//使用系统函数转换
	char*	lpAddress = ::inet_ntoa(*(in_addr*)(&ip_addr));
	if (NULL == lpAddress)
	{
		return lpErrorAddr;
	}
	return lpAddress;
}

//*****************************************************************************
//	函数说明：	根据length接收数据
//	函数原型：	int recv_to_end(sock_fd s,char *buffer,int length,int flag)
//  参数：      如recv一致
//  返回值：    如果接收完length个数据，返回length，否则错误
//  用法：		和recv一致
//*****************************************************************************
int sockets::recv_to_end(sock_fd s, char *buffer, int length, int flag)
{
	char* ptr = buffer;
	int byteLeft = length;
	int l = 0;
	do {
		l = recv(s, ptr, byteLeft, flag);
		if (l == 0)
		{
			break;
		}
		ptr += l;
		byteLeft -= l;
	} while (byteLeft > 0 && l > 0);
	return length - byteLeft;
}

//*****************************************************************************
//	函数说明：	根据length发送数据
//	函数原型：	int send_to_end(sock_fd s,char *buffer,int length,int flag)
//  参数：      如recv一致
//  返回值：    如果发送完length个数据，返回length，否则错误
//  用法：		和上方法一致
//*****************************************************************************
int sockets::send_to_end(sock_fd s, char *buffer, int length, int flag)
{	// 总共需要发送的，已经发送的长度
	int total_bytes = length, bytes_sent = 0;
	do
	{
		int send_bytes = total_bytes - bytes_sent;
		int ret_bytes = send(s, buffer + bytes_sent, send_bytes, flag);
		if (ret_bytes < 0)
			break;
		bytes_sent += ret_bytes;
	} while (bytes_sent != total_bytes);
	return bytes_sent;
}
