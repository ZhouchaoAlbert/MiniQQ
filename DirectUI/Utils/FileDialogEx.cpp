#include "StdAfx.h"
#include "FileDialogEx.h"
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

namespace DuiLib{

CFileDialogEx::CFileDialogEx(void)
{
	memset(&m_szDefExt, 0, sizeof(m_szDefExt));
	SetFilter(_T("All Files(*.*)|*.*||"));
	memset(&m_szFileName, 0, sizeof(m_szFileName));
	memset(&m_stOFN, 0, sizeof(OPENFILENAME));

	m_lpszFileName = NULL;

	m_stOFN.lStructSize = sizeof(OPENFILENAME);
	m_stOFN.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT;
	m_stOFN.lpstrDefExt = m_szDefExt;
	m_stOFN.lpstrFilter = m_szFilter;
	m_stOFN.lpstrFile = m_szFileName;
	m_stOFN.nMaxFile = MAX_PATH;
}

CFileDialogEx::~CFileDialogEx(void)
{
	if (m_lpszFileName != NULL)
	{
		delete []m_lpszFileName;
		m_lpszFileName = NULL;
	}
}

void CFileDialogEx::SetDefExt(LPCTSTR lpszDefExt)
{
	_tcscpy_s(m_szDefExt, _countof(m_szDefExt), lpszDefExt);
}

void CFileDialogEx::SetFileName(LPCTSTR lpszFileName)
{
	_tcscpy_s(m_szFileName, _countof(m_szFileName), lpszFileName);
}

void CFileDialogEx::SetFlags(DWORD dwFlags)
{
	m_stOFN.Flags = dwFlags;
}

// Ê¾Àý_T("Text Files(*.txt)|*.txt|ÍøÒ³ÎÄ¼þ|*.htm;*.html|All Files(*.*)|*.*||")
void CFileDialogEx::SetFilter(LPCTSTR lpszFilter)
{
	if (lpszFilter != NULL)
	{
		_tcscpy_s(m_szFilter, _countof(m_szFilter) - 1, lpszFilter);
		LPTSTR pch = m_szFilter; // modify the buffer in place
		// MFC delimits with '|' not '\0'
		while ((pch = _tcschr(pch, '|')) != NULL)
			*pch++ = '\0';
	}
}

void CFileDialogEx::SetMultiSel(BOOL bMultiSel/* = TRUE*/)
{
	if (bMultiSel)
		m_stOFN.Flags |= OFN_ALLOWMULTISELECT;
	else
		m_stOFN.Flags &= ~OFN_ALLOWMULTISELECT;
}

void CFileDialogEx::SetParentWnd(HWND hParentWnd)
{
	m_stOFN.hwndOwner = hParentWnd;
}

void CFileDialogEx::SetTitle(LPCTSTR lpszTitle)
{
	m_stOFN.lpstrTitle = lpszTitle;
}

void CFileDialogEx::SetFileNameBufferSize(DWORD dwSize)
{
	if (m_lpszFileName != NULL)
	{
		delete []m_lpszFileName;
		m_lpszFileName = NULL;
	}

	if (dwSize <= (sizeof(m_szFileName) / sizeof(TCHAR)))
	{
		m_stOFN.lpstrFile = m_szFileName;
		m_stOFN.nMaxFile = MAX_PATH;
	}
	else
	{
		m_lpszFileName = new TCHAR[dwSize];
		if (m_lpszFileName != NULL)
		{
			memset(m_lpszFileName, 0, dwSize * sizeof(TCHAR));
			m_stOFN.lpstrFile = m_lpszFileName;
			m_stOFN.nMaxFile = dwSize;
		}
		else
		{
			m_stOFN.lpstrFile = m_szFileName;
			m_stOFN.nMaxFile = MAX_PATH;
		}
	}
}

BOOL CFileDialogEx::ShowOpenFileDlg()
{
	return ::GetOpenFileName(&m_stOFN);
}

BOOL CFileDialogEx::ShowSaveFileDlg()
{
	return ::GetSaveFileName(&m_stOFN);
}

CDuiString CFileDialogEx::GetPathName()
{
	return m_stOFN.lpstrFile;
}

CDuiString CFileDialogEx::GetFileName()
{
	return ::PathFindFileName(m_stOFN.lpstrFile);
}

CDuiString CFileDialogEx::GetFileExt()
{
	return ::PathFindExtension(m_stOFN.lpstrFile);
}

CDuiString CFileDialogEx::GetFileTitle()
{
	return _T("");
}

CDuiString CFileDialogEx::GetFolderPath()
{
	return _T("");
}


POSITION CFileDialogEx::GetStartPosition()
{
	return (POSITION)m_stOFN.lpstrFile;
}

CDuiString CFileDialogEx::GetNextPathName(POSITION& pos)
{
	LPTSTR lpsz = (LPTSTR)pos;
	if (lpsz == m_stOFN.lpstrFile) // first time
	{
		if ((m_stOFN.Flags & OFN_ALLOWMULTISELECT) == 0)
		{
			pos = NULL;
			return m_stOFN.lpstrFile;
		}

		// find char pos after first Delimiter
		while(*lpsz != '\0')
			lpsz = _tcsinc(lpsz);
		lpsz = _tcsinc(lpsz);

		// if single selection then return only selection
		if (*lpsz == '\0')
		{
			pos = NULL;
			return m_stOFN.lpstrFile;
		}
	}

	CDuiString strBasePath = m_stOFN.lpstrFile;
	CDuiString strFileName = lpsz;
	if (strBasePath.GetAt(strBasePath.GetLength() - 1) != '\\')
		strBasePath += _T("\\");
	CDuiString strPath = strBasePath + strFileName;

	// find char pos at next Delimiter
	while(*lpsz != '\0')
		lpsz = _tcsinc(lpsz);
	lpsz = _tcsinc(lpsz);
	if (*lpsz == '\0') // if double terminated then done
		pos = NULL;
	else
		pos = (POSITION)lpsz;

	return strPath;
}

}