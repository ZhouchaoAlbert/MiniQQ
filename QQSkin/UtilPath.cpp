#include "UtilPath.h"


CString Util::Path::GetAppPath()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);

	CString strPath(szPath);
	for (INT32 nPos = (INT32)strPath.GetLength() - 1; nPos >= 0; --nPos)
	{
		TCHAR cChar = strPath[nPos];
		if (_T('\\') == cChar || _T('/') == cChar)
		{

		}
			//return strPath.substr(0, nPos + 1);
	}
	return strPath;
}