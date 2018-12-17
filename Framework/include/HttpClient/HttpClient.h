#pragma once
/************************************************************************
* function：HTTP通讯的头文件
* author :	明巧文
* datetime：2018-04-03
* company:  安碧捷科技股份有限公司
/************************************************************************/

#include <wtypes.h>
#include <string>
#include <map>

struct curl_slist;

class HttpClient
{
public:
	HttpClient();
	~HttpClient();

	/*************************************************************************
	* function：	设置授权信息
	* param stdId:  in，秘钥ID
	* param strSecret: in,秘钥密码
	*************************************************************************/
	void SetAccessKey(LPCTSTR stdId, LPCTSTR strSecret);

	int SetRequestUrl(const std::string& strUrl);	//设置url地址

	int SetRequestHeader(const std::map<std::string, std::string>& headers);	//设置url头

	/*************************************************************************
	* function：投递一个请求
	* param strUrl:  in，请求的url地址
	* param strPost: in,请求的数据
	* param strResponse: out,返回的数据
	* return:		返回投递的结果命令,0为成功
	*************************************************************************/
	int Post(const std::string &strUrl, const std::string &strPost, std::string& strResponse);

	/*************************************************************************
	* function：下载文件
	* param strUrl:  in，请求的url地址
	* param strFileName: in,下载的文件名
	* return:		返回投递的结果命令
	*************************************************************************/
	int DownloadFile(const std::string &strUrl, const std::string &strFileName);

	std::string MakeUrl(const char* remote_address, const char* cmd_name);

private:
	static size_t HeaderCallback(char *buffer, size_t size, size_t nitems, void *userdata);		//头回调
	static size_t DataCallback(char *buffer, size_t size, size_t nitems, void *userdata);		//请求数据回调
	static size_t DownloadCallback(char *buffer, size_t size, size_t nitems, void *lpVoid);		//下载回调

private:
	std::string		m_strKeyId;			 //秘钥ID
	std::string		m_strKeySecret;		 //秘钥密文

	HANDLE			m_hCurl;			//curl处理的句柄
	curl_slist		*m_pHeadList;		//curl头列表
	std::string     m_strReceiveHead;	//返回的头部数据

	std::string		m_strUserId;		//用户ID
};

