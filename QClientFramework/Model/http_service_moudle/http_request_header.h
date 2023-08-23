#pragma once
#include "base_helper.h"


//Request HeadersԼ��
#define  HEADER_ACCEPT           "Accept"           //������ɽ��ܵ�MIME����
#define  HEADER_ACCEPT_ENCODING  "Accept-Encoding"  //���ݱ��뷽ʽ
#define  HEADER_ACCEPT_LANGUAGE  "Accept-Language"  //��������
#define  HEADER_AUTHORIZATION    "Authorization"    //ǩ��
#define  HEADER_SIGN_DATE        "Sign-Date"        //��Ϣ�����ʱ���
#define  HEADER_HOST             "Host"             //URL�е������Ͷ˿�
#define  HEADER_CONTENT_LENGTH   "Content-Length"   //��Ϣ���ĵĳ���
#define  HEADER_CONTENT_TYPE     "Content-Type"     //˵���������������������α���
#define  HEADER_MSG_IDENT        "Msg-Ident"        //��ϢΨһ��ʶ
#define  HEADER_OPERATOR         "Operator"         //�ӿڵ�����uId

//value
#define  HEADER_ACCEPT_VALUE           "*/*"
#define  HEADER_ACCEPT_ENCODING_VALUE  "deflate"//"gzip, deflate"
#define  HEADER_ACCEPT_LANGUAGE_VALUE  "zh-CN"
#define  HEADER_CONTENT_TYPE_VALUE     "application/json"

#define  TAB_ITEM      "\n"

#define HTTP_METHOD_POST_STR					"POST"
#define HTTP_METHOD_PUT_STR						"PUT"
#define HTTP_METHOD_PATCH_STR					"PATCH"
#define HTTP_METHOD_GET_STR					    "GET"
#define HTTP_METHOD_DELETE_STR					"DELETE"
#define HTTP_METHOD_HEAD_STR					"HEAD"
#define HTTP_METHOD_TRACE_STR					"TRACE"
#define HTTP_METHOD_OPTIONS_STR					"OPTIONS"
#define HTTP_METHOD_CONNECT_STR					"CONNECT"


struct THttpHeader
{
	string m_name;
	string m_value;
	THttpHeader(string name, string value)
		: m_name(name)
		, m_value(value)
	{
	}

	struct hash
	{
		size_t operator() (const string& str) const
		{
			size_t s = std::hash_value(str.c_str());
			return s;
		}
	};

	struct equal_to
	{
		bool operator () (const string& strA, const string& strB) const
		{
			return strA == strB;
		}
	};
};

typedef unordered_multimap<string, string, THttpHeader::hash, THttpHeader::equal_to>	THttpHeaderMap;
typedef THttpHeaderMap::const_iterator	THttpHeaderMapCI;
typedef THttpHeaderMap::iterator		THttpHeaderMapI;


class http_request_header
{
public:
    http_request_header(HTTP_METHOD method, unsigned int content_length, const string& access_key_id, const string& access_key_secret, const string& user_id);
	~http_request_header() {};
public:
	void auto_build_header();
	string get_verb(HTTP_METHOD method);
	//�����
	void add_header_item(string name, string value);
	THttpHeaderMap& get_headr_map()
	{
		return m_headers;
	}
private:
	//base64(hmac-sha1(AccessKeySecret,VERB  + "\n" + Content-Type + "\n" + SignDate+��\n��+ msg_ident)
	string make_authorization();
	//guid����:%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X
	string make_guid();
	string make_sign_date();
	string make_content_length(unsigned int content_length);
private:
	THttpHeaderMap m_headers;
	string m_verb;               //��ʾ��������:POST��GET�ȵ�
	string m_content_type;       //˵���������������������α���
	string m_accept;             //MIME����
	string m_accept_encoding;    //���ݱ��뷽ʽ
	string m_accept_language;    //��������
	string m_msg_ident;		     //��ϢΨһ��ʶ
	string m_sign_date;			 //��Ϣ�����ʱ���
	string m_access_key_secret;  //��ԿID
	string m_access_key_id;      //��Կ����
	string m_authorization;      //����ǩ��
	string m_content_length;     //�����峤��
    string m_user_id;            //�ӿڵ�����uId
};