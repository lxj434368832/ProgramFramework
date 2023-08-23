#pragma once
class curl_service
{
public:
	curl_service();
	~curl_service();
public:
	BOOL send(const char* url, const char* data, unsigned int len,const char* method);
};
