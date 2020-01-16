
#include "HttpClient.h"
#include <iostream>
#include <stdio.h>
#include <tchar.h>

using namespace std;

class test_moudle
{
public:
	test_moudle() {};
	~test_moudle() {};
public:
	void  on_receive(int req_moudle_id,const string& data, bool success);
};

void test_moudle::on_receive(int req_moudle_id, const string& data, bool success)
{
	std::cout << "req_moudle_id" << req_moudle_id << "---------->data" << data << "ret:" << success << std::endl;
}

int main()
{
	HttpClient* engine = new HttpClient();
	if (!engine->start())
	{
		return 0;
	}
	string cmd_name("/message/getHeartBeat");
	test_moudle test;
	string data("{\n   \"clientMac\" : \"string\",\n   \"clientTime\" : 0,\n   \"serverTime\" : 0\n}\n");
	//////////////////////////////////////////////////////////////////////////
	//命令模式
	//注册数据回调
	engine->advise_receive_proc(cmd_name, std::bind(&test_moudle::on_receive, &test, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	//while (true)
	//{
	//	//无命令模式
	//	engine->http_asyn_send("http://192.168.2.49:80", cmd_name.c_str(),
	//		data, 0, HTTP_METHOD::HTTP_METHOD_POST);
	//	Sleep(2000);
	//}

	getchar();
	////////////////////////////////////////////////////////////////////////////
	////命令模式
	//string data2("1111111123344");
	//string cmd_name2("/logout");
	//test_moudle test2;
	////注册数据回调
	//uis_engine::advise_receive_proc(cmd_name2, std::bind(&test_moudle::on_receive, &test2, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
	//engine->send("https://192.168.2.50", cmd_name.c_str(),data.c_str(), data.size(),0, HTTP_METHOD::HTTP_METHOD_POST, HTTP_THREAD_MODE::THREAD_MODE_ASYNC_MULTI_THREAD);

	////命令模式
	////压力测试
	//for(int i = 0; i < 1000;i++)
	//{
	//	string data("");
	//	string cmd_name("/logout");
	//	Sleep(50);
	//	engine->send("https://192.168.2.50", cmd_name.c_str(),
	//		data.c_str(), data.size(),0, HTTP_METHOD::HTTP_METHOD_POST,HTTP_THREAD_MODE::THREAD_MODE_ASYNC_MULTI_THREAD);
	//}
	//getchar();
	engine->stop();
	delete engine;
	engine = nullptr;
    return 0;
}