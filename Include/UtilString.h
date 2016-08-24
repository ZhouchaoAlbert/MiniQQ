#pragma once
#include "BaseDefine.h"
namespace Util
{
	namespace String
	{
		//Ansci - Unicode
		BASE_API BOOL A_2_W(LPCSTR pszSrc, CString& strDst);
		BASE_API BOOL W_2_A(LPCWSTR pszSrc, CStringA& strDst);
		//Utf8  - Unicode
		BASE_API BOOL Utf8_2_W(LPCSTR pszSrc, CString& strDst);
		BASE_API BOOL W_2_Utf8(LPCWSTR pszSrc, CStringA& strDst);
		//Utf8  - Ansci
		BASE_API BOOL A_2_Utf8(LPCSTR pszSrc, CStringA& strDst);
		BASE_API BOOL Utf8_2_A(LPCSTR pszSrc, CStringA& strDst);
		//user
		BASE_API BOOL A_2_W(LPCSTR pszSrc, CString& strDst, UINT32 uCodePage);
		BASE_API BOOL W_2_A(LPCWSTR pszSrc, CStringA& strDst, UINT32 uCodePage);
		//
		BASE_API void  StringSplit(CString text, CString sp, std::vector<CString>& vecSplit);

		
	}
}

