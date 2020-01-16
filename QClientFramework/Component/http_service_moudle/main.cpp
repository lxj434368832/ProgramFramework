#include "stdafx.h"
#include <iostream>
#include <thread>
#include "http_service.h"

http_service service;
string data("1234567");
void thread_task() {
	for (int i = 0; i < 10000;i++)
	{
		//std::cout << "no" << i << "thread id:" <<std::thread::id()<<std::endl;
		service.send("/login", data.c_str(), data.size(), "POST", TRUE);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	service.start("192.168.3.132", 8080);
	
	service.send("/login", data.c_str(), data.size(), "POST", TRUE);
	std::thread t1(thread_task);
	std::thread t2(thread_task);
	t1.join();
	t2.join();
	getchar();
	return 0;
}