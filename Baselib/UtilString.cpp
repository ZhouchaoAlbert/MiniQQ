#include "UtilString.h"


BOOL Util::String::A_2_W(LPCSTR pszSrc, CString& strDst)
{
	try
	{
		CA2W pszW(pszSrc);
		if (NULL == pszW)
		{
			return FALSE;
		}
		strDst = pszW;
	}
	catch (CAtlException* e)
	{
		e;
		return FALSE;
	}

	return TRUE;
}

BOOL Util::String::W_2_A(LPCWSTR pszSrc, CStringA& strDst)
{
	try
	{
		CW2A pszA(pszSrc);
		if (NULL == pszA)
		{
			return FALSE;
		}
		strDst = pszA;
	}
	catch (CAtlException* e)
	{
		e;
		return FALSE;
	}

	return TRUE;
}


//Utf8 Unicode
BOOL Util::String::Utf8_2_W(LPCSTR pszSrc, CString& strDst)
{
	try
	{
		CA2W pszW(pszSrc, CP_UTF8);
		if (NULL == pszW)
		{
			return FALSE;
		}
		strDst = pszW;
	}
	catch (CAtlException* e)
	{
		e;
		return FALSE;
	}

	return TRUE;
}

BOOL Util::String::W_2_Utf8(LPCWSTR pszSrc, CStringA& strDst)
{
	try
	{
		CW2A pszU8(pszSrc, CP_UTF8);
		if (NULL == pszU8)
		{
			return FALSE;
		}
		strDst = pszU8;
	}
	catch (CAtlException* e)
	{
		e;
		return FALSE;
	}

	return TRUE;
}

BOOL A_2_Utf8(LPCSTR pszSrc, CStringA& strDst)
{
	try
	{
		CW2A pszU8(CA2W(pszSrc), CP_UTF8);
		strDst = pszU8;
	}
	catch (CAtlException* e)
	{
		e;
		return FALSE;
	}

	return TRUE;
}

BOOL Utf8_2_A(LPCSTR pszSrc, CStringA& strDst)
{
	try
	{
		CW2A pszA(CA2W(pszSrc, CP_UTF8));
		strDst = pszA;
	}
	catch (CAtlException* e)
	{
		e;
		return FALSE;
	}
	return TRUE;
}

BOOL Util::String::A_2_W(LPCSTR pszSrc, CString& strDst, UINT32 uCodePage)
{
	try
	{
		CA2W pszW(pszSrc, uCodePage);
		if (NULL == pszW)
		{
			return FALSE;
		}
		strDst = pszW;
	}
	catch (CAtlException* e)
	{
		e;
		return FALSE;
	}

	return TRUE;
}

BOOL Util::String::W_2_A(LPCWSTR pszSrc, CStringA& strDst, UINT32 uCodePage)
{
	try
	{
		CW2A pszU8(pszSrc, uCodePage);
		if (NULL == pszU8)
		{
			return FALSE;
		}
		strDst = pszU8;
	}
	catch (CAtlException* e)
	{
		e;
		return FALSE;
	}

	return TRUE;
}

void  Util::String::StringSplit(CString text, CString sp, std::vector<CString>& vecSplit)
{
	vecSplit.clear();
	INT32 pos = text.Find(sp, 0);
	while (pos >= 0)
	{
		CString t = text.Left(pos);
		vecSplit.push_back(t);
		text = text.Right(text.GetLength() - pos - sp.GetLength());
		pos = text.Find(sp);
	}
	vecSplit.push_back(text);
}

INT32 Util::String::StrCmp(TCHAR* p1, TCHAR* p2)
{
	if (!p1)
	{
		if (!p2)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		if (!p2)
		{
			return -1;
		}
		else
		{
			return _tcscmp(p1, p2);
		}
	}
}