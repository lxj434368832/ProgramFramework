#pragma once
#include <WinSock2.h>
#include <string>
#include <xhash>
#include <unordered_map>
#include <map>

using namespace std;

//http函数类型
enum HTTP_METHOD
{
	HTTP_METHOD_POST,
	HTTP_METHOD_PUT,
	HTTP_METHOD_PATCH,
	HTTP_METHOD_GET,
	HTTP_METHOD_DELETE,
	HTTP_METHOD_HEAD,
	HTTP_METHOD_TRACE,
	HTTP_METHOD_OPTIONS,
	HTTP_METHOD_CONNECT,
};

//Request Headers约定
#define  HEADER_ACCEPT           "Accept"           //浏览器可接受的MIME类型
#define  HEADER_ACCEPT_ENCODING  "Accept-Encoding"  //数据编码方式
#define  HEADER_ACCEPT_LANGUAGE  "Accept-Language"  //语言种类
#define  HEADER_AUTHORIZATION    "Authorization"    //签名
#define  HEADER_SIGN_DATE        "SignDate"       //消息发起的时间戳
#define  HEADER_HOST             "Host"             //URL中的主机和端口
#define  HEADER_CONTENT_LENGTH   "Content-Length"   //消息正文的长度
#define  HEADER_CONTENT_TYPE     "Content-Type"     //说明了请求主体的内容是如何编码
#define  HEADER_MSG_IDENT        "MsgIdent"        //消息唯一标识
#define  HEADER_OPERATOR         "Operator"         //接口调用者uId

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


bool GetMacByGetAdaptersInfo(std::string& macOUT);
bool GetMacByGetAdaptersAddresses(std::string& macOUT);

class http_request_header
{
public:
	http_request_header(HTTP_METHOD method, unsigned int content_length, const string& access_key_id, const string& access_key_secret, const string& user_id);
	~http_request_header() {};
public:
	void auto_build_header();
	std::map<std::string, std::string>& GetHttpHead();
	string get_verb(HTTP_METHOD method);
	//添加项
	void add_header_item(string name, string value);

private:
	//base64(hmac-sha1(AccessKeySecret,VERB  + "\n" + Content-Type + "\n" + SignDate+“\n”+ msg_ident)
	string make_authorization();
	//guid类型:%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X
	string make_guid();
	string make_sign_date();
	string make_content_length(unsigned int content_length);
private:
	std::map<std::string, std::string>	m_headers;	//http头列表
	string m_verb;               //表示请求类型:POST、GET等等
	string m_content_type;       //说明了请求主体的内容是如何编码
	string m_accept;             //MIME类型
	string m_accept_encoding;    //数据编码方式
	string m_accept_language;    //语言种类
	string m_msg_ident;		     //消息唯一标识
	string m_sign_date;			 //消息发起的时间戳
	string m_access_key_secret;  //秘钥ID
	string m_access_key_id;      //秘钥密文
	string m_authorization;      //数字签名
	string m_content_length;     //数据体长度
    string m_user_id;            //接口调用者uId
};