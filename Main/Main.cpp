#include<Windows.h>
#include "UtilPath.h"

#include <atlstr.h>
using namespace ATL;

int APIENTRY  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	

	//CString strPath = Util::Path::GetAppPath();


	::CoUninitialize();
	return 0;
}
