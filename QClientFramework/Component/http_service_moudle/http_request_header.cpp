#include "stdafx.h"
#include <combaseapi.h>
#include <sstream>
#include <time.h>
#include "http_request_header.h"
#include "base64.h"
#include "hmac_sha1.h"
#include <iostream>
#include <sys/timeb.h>

#pragma comment(lib,"Ole32.lib")

http_request_header::http_request_header(HTTP_METHOD method, unsigned int content_length, const string& access_key_id, const string& access_key_secret, const string& user_id)
	:m_verb(get_verb(method))
	,m_accept(HEADER_ACCEPT_VALUE)
	,m_accept_encoding(HEADER_ACCEPT_ENCODING_VALUE)
	,m_content_type(HEADER_CONTENT_TYPE_VALUE)
	,m_accept_language(HEADER_ACCEPT_LANGUAGE_VALUE)
	,m_access_key_id(access_key_id)
	,m_access_key_secret(access_key_secret)
    ,m_user_id(user_id)
{
	m_msg_ident = make_guid();
	m_sign_date = make_sign_date();
	m_authorization  = make_authorization();
	m_content_length = make_content_length(content_length);
}


void http_request_header::auto_build_header()
{
	add_header_item(HEADER_ACCEPT, m_accept);
	add_header_item(HEADER_ACCEPT_ENCODING, m_accept_encoding);
	add_header_item(HEADER_ACCEPT_LANGUAGE, m_accept_language);
	add_header_item(HEADER_AUTHORIZATION, m_authorization);
	add_header_item(HEADER_SIGN_DATE, m_sign_date);
    add_header_item(HEADER_OPERATOR, m_user_id);
	add_header_item(HEADER_CONTENT_LENGTH, m_content_length);
	add_header_item(HEADER_CONTENT_TYPE, m_content_type);
	add_header_item(HEADER_MSG_IDENT, m_msg_ident);
}

string http_request_header::get_verb(HTTP_METHOD method)
{
	switch (method)
	{
	case  HTTP_METHOD::HTTP_METHOD_POST:return HTTP_METHOD_POST_STR;
	case  HTTP_METHOD::HTTP_METHOD_PUT:return HTTP_METHOD_PUT_STR;
	case  HTTP_METHOD::HTTP_METHOD_PATCH:return HTTP_METHOD_PATCH_STR;
	case  HTTP_METHOD::HTTP_METHOD_GET:return HTTP_METHOD_GET_STR;
	case  HTTP_METHOD::HTTP_METHOD_DELETE:return HTTP_METHOD_DELETE_STR;
	case  HTTP_METHOD::HTTP_METHOD_HEAD:return HTTP_METHOD_HEAD_STR;
	case  HTTP_METHOD::HTTP_METHOD_TRACE:return HTTP_METHOD_TRACE_STR;
	case  HTTP_METHOD::HTTP_METHOD_OPTIONS:return HTTP_METHOD_OPTIONS_STR;
	case  HTTP_METHOD::HTTP_METHOD_CONNECT:return HTTP_METHOD_CONNECT_STR;
	default:
		break;
	}
	return HTTP_METHOD_POST_STR;
}

void http_request_header::add_header_item(string name, string value)
{
	m_headers.emplace(THttpHeaderMap::value_type(name, value));
}

static std::string byte2hex(const char *str,int len)
{
	assert(str);
	std::string stmp;
	char strTmp[10];
	for (int n = 0; n < len; n++)
	{
		sprintf_s(strTmp, "%02X", str[n] & 0XFF);
		stmp.append(strTmp);
	}
	return stmp;
}

//base64(hmac-sha1(AccessKeySecret,VERB  + "\n" + Content-Type + "\n" + SignDate+“\n”+ msg_ident)
string http_request_header::make_authorization()
{
	//打包加密数据体
	std::ostringstream ostr;
	ostr << m_verb
		<< TAB_ITEM
		<< m_content_type
		<< TAB_ITEM
		<< m_sign_date
		<< TAB_ITEM
		<< m_msg_ident;
	
	string plaintext = ostr.str();
	//生成数字签名Signature
	unsigned char digest[1024] = {0};
	hmac_sha1((unsigned char*)m_access_key_secret.c_str(), m_access_key_secret.size(), (unsigned char*)plaintext.c_str(), plaintext.size(), digest);
	string base64in((char*)digest, 20);
	string signature;
    Base64::Encode(base64in, &signature);
	string authorization;
    authorization.append("RCSSS ");
	authorization.append(m_access_key_id);
	authorization.append(":");
	authorization.append(signature);
	return authorization;
}

std::string http_request_header::make_guid()
{
	char buf[64] = { 0 };
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		sprintf_s(buf, sizeof(buf)
			, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]
		);
	}
	return string(buf);
}

std::string http_request_header::make_sign_date()
{
	std::ostringstream ostr;
	timeb tmb;
	ftime(&tmb);

	ostr << tmb.time * 1000 + tmb.millitm;
	m_sign_date = ostr.str();
	return ostr.str();
}

std::string http_request_header::make_content_length(unsigned int content_length)
{
	std::ostringstream ostr;
	ostr << content_length;
	return ostr.str();
}