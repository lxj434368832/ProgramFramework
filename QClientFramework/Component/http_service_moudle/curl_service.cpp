#include "stdafx.h"
#include "curl_service.h"
#include "http_request_header.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#pragma comment(lib,"libcurld.lib")
#pragma comment(lib,"curlpp.lib")

curl_service::curl_service()
{
}


curl_service::~curl_service()
{
}

BOOL curl_service::send(const char* url, const char* data, unsigned int len,const char* method)
{
	try {
		curlpp::Cleanup cleaner;
		curlpp::Easy request;
		request.setOpt(new curlpp::options::Url(url));
		request.setOpt(new curlpp::options::Verbose(true));

		//打包request_header
		http_request_header request_header(method, len);
		request_header.auto_build_header();
		std::list<std::string> header;
		for (THttpHeaderMapCI it = request_header.get_headr_map().begin(), end = request_header.get_headr_map().end(); it != end; ++it)
		{
			header.push_back(it->first + ":" + it->second);
		}
		
		request.setOpt(new curlpp::options::HttpHeader(header));
		request.setOpt(new curlpp::options::PostFields(data));
		request.setOpt(new curlpp::options::PostFieldSize(len));
		//不认证服务器CA
		request.setOpt(new curlpp::options::SslVerifyPeer(false));
		request.setOpt(new curlpp::options::SslVerifyHost(0));
		//ssl双向认证
		request.setOpt(new curlpp::options::SslCert("E:\\client.pem"));
		request.setOpt(new curlpp::options::SslCertPasswd("123456"));
		request.setOpt(new curlpp::options::SslCertType("PEM"));
		request.setOpt(new curlpp::options::SslKey("E:\\private.pem"));
		request.setOpt(new curlpp::options::SslKeyType("PEM"));
		request.setOpt(new curlpp::options::SslKeyPasswd("123456"));

		request.perform();
	}
	catch (curlpp::LogicError & e) {
		std::cout << e.what() << std::endl;
	}
	catch (curlpp::RuntimeError & e) {
		std::cout << e.what() << std::endl;
	}
	return TRUE;
}
