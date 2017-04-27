#pragma once
#include "BaseDefine.h"
namespace Util
{
	namespace String
	{
		//Ansci - Unicode
		BASELIB_API BOOL A_2_W(LPCSTR pszSrc, CString& strDst);
		BASELIB_API BOOL W_2_A(LPCWSTR pszSrc, CStringA& strDst);
		//Utf8  - Unicode
		BASELIB_API BOOL Utf8_2_W(LPCSTR pszSrc, CString& strDst);
		BASELIB_API BOOL W_2_Utf8(LPCWSTR pszSrc, CStringA& strDst);
		//Utf8  - Ansci
		BASELIB_API BOOL A_2_Utf8(LPCSTR pszSrc, CStringA& strDst);
		BASELIB_API BOOL Utf8_2_A(LPCSTR pszSrc, CStringA& strDst);
		//user
		BASELIB_API BOOL A_2_W(LPCSTR pszSrc, CString& strDst, UINT32 uCodePage);
		BASELIB_API BOOL W_2_A(LPCWSTR pszSrc, CStringA& strDst, UINT32 uCodePage);
		//
		BASELIB_API void  StringSplit(CString text, CString sp, std::vector<CString>& vecSplit);

		BASELIB_API INT32 StrCmp(TCHAR* p1, TCHAR* p2);
	}
}

