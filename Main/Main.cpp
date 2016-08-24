#include <windows.h>
#include "UtilPath.h"
#include "AuxMainUI.h"
#include <UIlib.h>
using namespace DuiLib;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	
	DuiLib::CPaintManagerUI::SetInstance(hInstance);
	CString s = Util::Path::GetResFolder();
	DuiLib::CPaintManagerUI::SetResourcePath(s);

	//Aux::MainUI::LoginFrame();

 	//CString strPath;
// 
 	///strPath = Util::Path::GetAppPath();
	//strPath = Util::Path::GetResFolder();
// 	Util::Path::GetCurrentDir();
// 	Util::Path::GetTempPath();
// 	Util::Path::CreateDirectory(_T("C:\\A\\B\\C\\D\\E"), NULL);



	::CoUninitialize();
	return 0;
}
