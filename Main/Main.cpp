#include <windows.h>
#include "UtilPath.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	
	CString strPath;

	strPath = Util::Path::GetAppPath();
	Util::Path::GetCurrentDir();
	Util::Path::GetTempPath();

	Util::Path::CreateDirectory(_T("C:\\A\\B\\C\\D\\E"), NULL);

	::CoUninitialize();
	return 0;
}
