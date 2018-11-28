#include "stdafx.h"
#include "utils.h"

std::string WCharToAnsi(LPCWSTR pwszSrc)
{
	int length = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (length <= 0)	return std::string("");

	char *pszDst = new char[length];
	if (NULL == pszDst)	return std::string("");

	length = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, length, NULL, NULL);
	if (length <= 0)
	{
		Mcout << "";
	}
	std::string strTemp(pszDst);
	delete [] pszDst;
	return strTemp;
}

std::wstring AnsiToWChar(LPCSTR pszSrc)
{
	int length = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (length <= 0) return NULL;

	WCHAR *pwszDst = new WCHAR[length];
	if (NULL == pwszDst) return NULL;
	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, length);

	std::wstring strTemp(pwszDst);
	delete pwszDst;
	return strTemp;

}

std::wstring GetErrorMessage(DWORD dw)
{
	LPTSTR lpMsgBuf = NULL;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		lpMsgBuf,
		0, NULL);
	std::wstring strError(lpMsgBuf);
	LocalFree(lpMsgBuf);
	return strError;
}
