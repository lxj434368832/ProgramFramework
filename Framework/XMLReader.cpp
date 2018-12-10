#include "stdafx.h"
#include ".\..\Inc\XMLReader.h"
#include <comdef.h>

CXMLReader::CXMLReader()
	:m_pIXMLDoc(NULL),
	m_pIXMLCurEmt(NULL)
{

}

CXMLReader::~CXMLReader()
{
	if (NULL != m_pIXMLCurEmt)
	{
		m_pIXMLCurEmt->Release();
	}

	if (NULL != m_pIXMLDoc)
	{
		m_pIXMLDoc->Release();
	}

	::CoUninitialize();
}

BOOL CXMLReader::Init()
{
	BOOL bRet = FALSE;

	do 
	{
		HRESULT hResult = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);

		if (S_OK != hResult && S_FALSE != hResult)
		{
			break;
		}

		if (S_OK != ::CoCreateInstance(CLSID_XMLDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDocument, (void**)&m_pIXMLDoc))
		{
			break;
		}
		
		//保存到本类对象结束后释放
		if (NULL != m_pIXMLDoc)
		{
			bRet = TRUE;
		}

	} while (0);

	return bRet;
}

DWORD CXMLReader::OpenXMLFile(LPCTSTR lpFileName)
{	
	DWORD dwRet = -1;

	if (PathFileExists(lpFileName) && NULL != m_pIXMLDoc)
	{	
		try
		{
			_variant_t vtVar(lpFileName);			

			if(S_OK == m_pIXMLDoc->put_URL(vtVar.bstrVal))
			{
				dwRet = ERROR_SUCCESS;
			}
		}
		catch (...)
		{
			EXCEPTION_TRACE(_T("Open xml file exception"));
		}
	}

	return dwRet;
}

void CXMLReader::CloseXMLFile()
{
	if (NULL != m_pIXMLCurEmt)
	{
		m_pIXMLCurEmt->Release();

		m_pIXMLCurEmt = NULL;
	}
}

BOOL CXMLReader::FindRoot()
{
	if (NULL != m_pIXMLCurEmt)
	{
		m_pIXMLCurEmt->Release();

		m_pIXMLCurEmt = NULL;
	}

	if (NULL != m_pIXMLDoc)
	{
		m_pIXMLDoc->get_root(&m_pIXMLCurEmt);		
	}

	return (NULL != m_pIXMLCurEmt);
}

BOOL CXMLReader::GetTagName(CString& strName)
{
	BOOL bRet = FALSE;

	if (NULL != m_pIXMLCurEmt)
	{
		BSTR bstrTmp = NULL;

		bRet = (S_OK == m_pIXMLCurEmt->get_tagName(&bstrTmp));

		if (bRet)
		{
			strName = bstrTmp;
		}

		if (NULL != bstrTmp)
		{
			//释放字符串
			::SysFreeString(bstrTmp);
		}
	}

	return bRet;
}

LONG CXMLReader::GetSubNodeNumber()
{
	LONG lRet = 0;

	if (NULL != m_pIXMLCurEmt)
	{
		IXMLElementCollection* pIXMLEmtClt = NULL;

		m_pIXMLCurEmt->get_children(&pIXMLEmtClt);

		if (NULL != pIXMLEmtClt)
		{
			pIXMLEmtClt->get_length(&lRet);

			pIXMLEmtClt->Release();
		}
	}

	return lRet;
}

BOOL CXMLReader::EnterSubNode(long lIndex)
{
	BOOL bRet = FALSE;

	do 
	{
		if (NULL == m_pIXMLCurEmt)
		{
			break;
		}

		IXMLElementCollection* pIXMLEmtClt = NULL;

		m_pIXMLCurEmt->get_children(&pIXMLEmtClt);

		if (NULL == pIXMLEmtClt)
		{
			break;
		}

		//备份当前元素指针
		IXMLElement* pITmpEmt = m_pIXMLCurEmt;

		pIXMLEmtClt->item(_variant_t(lIndex), _variant_t(VT_EMPTY), (IDispatch**)&m_pIXMLCurEmt);

		pIXMLEmtClt->Release();

		if (NULL == m_pIXMLCurEmt)
		{
			m_pIXMLCurEmt = pITmpEmt;
		}
		else
		{			
			pITmpEmt->Release();

			bRet = TRUE;
		}

	} while (0);

	return bRet;
}

BOOL CXMLReader::EnterSubNode(LPCTSTR lpTagName, long lIndex)
{
	BOOL bRet = FALSE;

	do 
	{
		if (NULL == m_pIXMLCurEmt)
		{
			break;
		}

		IXMLElementCollection* pIXMLEmtClt = NULL;

		m_pIXMLCurEmt->get_children(&pIXMLEmtClt);

		if (NULL == pIXMLEmtClt)
		{
			break;
		}

		//备份当前元素指针
		IXMLElement* pITmpEmt = m_pIXMLCurEmt;

		pIXMLEmtClt->item(_variant_t(lpTagName), _variant_t(lIndex), (IDispatch**)&m_pIXMLCurEmt);

		pIXMLEmtClt->Release();

		if (NULL == m_pIXMLCurEmt)
		{
			m_pIXMLCurEmt = pITmpEmt;
		}
		else
		{			
			pITmpEmt->Release();

			bRet = TRUE;
		}

	} while (0);

	return bRet;
}

BOOL CXMLReader::ExitSubNode()
{
	if (NULL != m_pIXMLCurEmt)
	{
		IXMLElement* pIXMLTmp = NULL;

		if (S_OK == m_pIXMLCurEmt->get_parent(&pIXMLTmp))
		{
			//释放原来的
			m_pIXMLCurEmt->Release();

			m_pIXMLCurEmt = pIXMLTmp;
		}
	}

	return (NULL != m_pIXMLCurEmt);
}

BOOL CXMLReader::GetAttributeValue(LPCTSTR lpAttr, int& iValue)
{
	long lValue = 0;

	BOOL bRet = GetAttributeValue(lpAttr, lValue);

	if (bRet)
	{
		iValue = lValue;
	}

	return bRet;
}

BOOL CXMLReader::GetAttributeValue(LPCTSTR lpAttr, long& lValue)
{
	BOOL bRet = FALSE;

	try
	{
		if (NULL != m_pIXMLCurEmt)
		{
			_variant_t varVal;

			bRet = (S_OK == m_pIXMLCurEmt->getAttribute(_variant_t(lpAttr).bstrVal, &varVal));
			
			varVal.ChangeType(VT_INT);

			if (bRet)
			{
				lValue = varVal.intVal;	
			}
		}
	}
	catch (...)
	{
		EXCEPTION_TRACE(_T("Get attribute value exception"));
	}

	return bRet;
}

BOOL CXMLReader::GetAttributeValue(LPCTSTR lpAttr, CString& strValue)
{
	BOOL bRet = FALSE;	
	
	try
	{
		if (NULL != m_pIXMLCurEmt)
		{
			_variant_t varVal;

			bRet = (S_OK == m_pIXMLCurEmt->getAttribute(_variant_t(lpAttr).bstrVal, &varVal));

			if (bRet)
			{
				strValue = varVal.bstrVal;
			}
		}
	}
	catch (...)
	{
		EXCEPTION_TRACE(_T("Get attribute value exception"));
	}

	return bRet;
}

BOOL CXMLReader::GetText(CString& strValue)
{
	BOOL bRet = FALSE;

	if (NULL != m_pIXMLCurEmt)
	{
		BSTR bstrTmp = NULL;

		bRet = (S_OK == m_pIXMLCurEmt->get_text(&bstrTmp));

		if (bRet)
		{
			strValue = bstrTmp;

			::SysFreeString(bstrTmp);
		}
	}

	return bRet;
}