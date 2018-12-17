#pragma once

#include <afxstr.h>

class CXMLReader
{
public:

	CXMLReader();

	~CXMLReader();

public:

	//初始化
	BOOL Init();

	//打开XML文件
	DWORD OpenXMLFile(LPCTSTR lpFileName);

	//关闭文件
	void CloseXMLFile();

	//找根节点
	BOOL FindRoot();

	//获取根节点标签名称
	BOOL GetTagName(CString& strName);

	//获取当前节点中子节点个数
	LONG GetSubNodeNumber();

	//根据索引进入子节点
	BOOL EnterSubNode(long lIndex);

	//根据节点标签和索引进入子节点
	BOOL EnterSubNode(LPCTSTR lpTagName, long lIndex = 0);

	//退出子节点，返回到父节点上
	BOOL ExitSubNode();

	//根据属性名称获取相应的属性值, int版本
	BOOL GetAttributeValue(LPCTSTR lpAttr, int& iValue);

	//根据属性名称获取相应的属性值, long版本
	BOOL GetAttributeValue(LPCTSTR lpAttr, long& lValue);

	//根据属性名称获取相应的属性值,CString版本
	BOOL GetAttributeValue(LPCTSTR lpAttr, CString& strValue);

	//获取值
	BOOL GetText(CString& strValue);

private:

	CXMLReader(const CXMLReader& obj);

	CXMLReader& operator=(const CXMLReader& obj);

private:

	IXMLDocument*				m_pIXMLDoc;		//!< XML文件接口对象	

	IXMLElement*				m_pIXMLCurEmt;	//!< XML当前节点元素接口对象
};