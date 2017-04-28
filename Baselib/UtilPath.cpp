#include "UtilPath.h"


CString Util::Path::GetAppPath()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath(szPath);
	INT32 iFind = strPath.ReverseFind(_T('\\'));
	if (iFind < 0)
	{
		return _T("");
	}
	strPath = strPath.Left(iFind);
	return strPath;
}

CString Util::Path::GetCurrentDir()
{
	TCHAR szCurDir[MAX_PATH] = { 0 };
	::GetCurrentDirectory(MAX_PATH, szCurDir);

	UINT32 uLen = _tcslen(szCurDir);
	if (uLen <= 0)
	{
		return _T("");
	}
	TCHAR cLastChar = szCurDir[uLen - 1];

	if (cLastChar != _T('\\') && cLastChar != _T('/'))
	{
		_tcscat(szCurDir, _T("\\"));
	}
	return szCurDir;
}

CString  Util::Path::GetTempPath()
{
	TCHAR szTempPath[MAX_PATH] = { 0 };
	::GetTempPath(MAX_PATH, szTempPath);

	UINT32 uLen = _tcslen(szTempPath);
	if (uLen <= 0)
	{
		return _T("");
	}
	
	TCHAR cLastChar = szTempPath[uLen - 1];
	if (cLastChar != _T('\\') && cLastChar != _T('/'))
	{
		_tcscat(szTempPath, _T("\\"));
	}
	return szTempPath;
}


CString Util::Path::GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName)
{
	CString strPath, strFileName, strExtFileName, strFullPath;
	TCHAR szBuf[MAX_PATH] = { 0 };

	if (IsDirectoryExist(lpszPath))
	{
		strPath = lpszPath;
	}	
	else
	{
		strPath = GetAppPath();	
	}

	strFileName	   = GetFileNameWithoutExtension(lpszFileName);
	strExtFileName = GetExtension(lpszFileName);

	for (INT32 i = 2; i < 10000; i++)
	{
		if (strExtFileName.IsEmpty())
		{
			strFullPath = strPath;
			strFullPath += strFileName;
			wsprintf(szBuf, _T("%d"), i);
			strFullPath += szBuf;
		}
		else
		{
			strFullPath = strPath;
			strFullPath += strFileName;
			wsprintf(szBuf, _T("%d."), i);
			strFullPath += szBuf;
			strFullPath += strExtFileName;
		}

		if (!IsFileExist(strFullPath))
		{
			return strFullPath;
		}
	}
	return _T("");
}

CString Util::Path::GetTempFileName(LPCTSTR lpszFileName)
{
	return GetRandomFileName(GetTempPath(), lpszFileName);
}

CString  Util::Path::GetInstallFolder()    //获取安装目录
{
	CString strPath = GetAppPath();
	ATLASSERT(!strPath.IsEmpty());

	INT32 ix = strPath.ReverseFind(_T('\\'));
	if (ix != -1)
	{
		return strPath.Mid(0, ix);
	}
	ATLASSERT(FALSE);
	return _T("");
}



CString Util::Path::GetResFolder()
{
	return GetInstallFolder() + _T("\\Resource");
}

CString Util::Path::GetResMaterialFolder()
{
	return GetResFolder() + _T("\\material");
}

CString Util::Path::GetResXmlFolder()
{
	return GetResFolder() + _T("\\xml");
}

BOOL Util::Path::IsDirectory(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
	{
		return FALSE;
	}

	DWORD dwAttr = ::GetFileAttributes(lpszPath);
	return (((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)) ? TRUE : FALSE);
}

BOOL Util::Path::IsFileExist(LPCTSTR lpszFileName)
{
	if (NULL == lpszFileName || NULL == *lpszFileName)
	{
		return FALSE;
	}
	
	DWORD dwAttr = ::GetFileAttributes(lpszFileName);
	return (((dwAttr != 0xFFFFFFFF) && (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))) ? TRUE : FALSE);
}

BOOL Util::Path::IsDirectoryExist(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
	{
		return FALSE;
	}
		
	DWORD dwAttr = ::GetFileAttributes(lpszPath);
	return (((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)) ? TRUE : FALSE);
}

BOOL Util::Path::CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	if (NULL == lpPathName || NULL == *lpPathName)
	{
		return FALSE;
	}
	TCHAR cPath[MAX_PATH]	 = { 0 };
	TCHAR cTmpPath[MAX_PATH] = { 0 };
	TCHAR *lpPos = NULL;
	TCHAR  cTmp  = _T('\0');

	_tcsncpy(cPath, lpPathName, MAX_PATH);

	for (INT32 i = 0; i < (INT32)_tcslen(cPath); i++)
	{
		if (_T('\\') == cPath[i])
		{
			cPath[i] = _T('/');
		}
	}

	lpPos = _tcschr(cPath, _T('/'));
	while (NULL != lpPos)
	{
		if (lpPos == cPath)
		{
			lpPos++;
		}
		else
		{
			cTmp = *lpPos;
			*lpPos = _T('\0');
			_tcsncpy(cTmpPath, cPath, MAX_PATH);
			::CreateDirectory(cTmpPath, lpSecurityAttributes);
			*lpPos = cTmp;
			lpPos++;
		}
		lpPos = _tcschr(lpPos, _T('/'));
	}
	return TRUE;
}

CString Util::Path::GetPathRoot(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
	{
		return _T("");
	}

	CString strPath(lpszPath);


	return strPath;
}

CString Util::Path::GetExtension(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
	{
		return _T("");
	}
	CString strPath(lpszPath);
	INT32 iFind = strPath.ReverseFind('\\');
	if (iFind < 0)
	{
		iFind = strPath.ReverseFind('/');
	}
	CString strFileName = strPath.Right(strPath.GetLength() - 1 - iFind);

	iFind = strFileName.ReverseFind('.');
	if (iFind < 0)
	{
		return _T("");
	}
	strFileName = strFileName.Right(strFileName.GetLength() - 1- iFind);
	return strFileName;
}

CString Util::Path::GetFileNameWithoutExtension(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
	{
		return _T("");
	}
	CString strPath(lpszPath);
	INT32 iFind = strPath.ReverseFind('\\');
	if (iFind < 0)
	{
		iFind = strPath.ReverseFind('/');
	}
	CString strFileName = strPath.Right(strPath.GetLength() - 1 - iFind);
	
	iFind = strFileName.ReverseFind('.');
	if (iFind < 0)
	{
		return _T("");
	}
	strFileName = strFileName.Left(iFind);
	return strFileName;
}

CString Util::Path::GetCefResPath()
{
	return GetInstallFolder() + _T("\\CefRes");
}