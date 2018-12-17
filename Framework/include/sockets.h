#pragma once

#include <string>
#include <windows.h>

#ifndef SOCKET_ERROR                      
#define SOCKET_ERROR (-1)
#endif

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
typedef int     sock_fd;
#else
typedef int     sock_fd;
#endif
#else
typedef int     sock_fd;
#endif

class sockets
{
public:
	sockets() = default;
public:
	static int get_sock_name(sock_fd s, struct sockaddr_in *sock);                         // 
	static sock_fd tcp_link(const std::string &ip, int port);                       // TCP连接
	static sock_fd tcp_listen(const std::string &ip, u_short port);                 // TCP监听
	static sock_fd udp_link(const std::string &ip, int port, struct sockaddr_in &sock);    // UDP连接
	static sock_fd udp_listen(const std::string &ip, u_short port);                 // UDP监听
	static const char* inet_ntoa(unsigned long ip_addr);

	static int recv_to_end(sock_fd s, char *buffer, int length, int flag);       // recv函数的进一步包装
	static int send_to_end(sock_fd s, char *buffer, int length, int flag);       // 发送length的数据
};
