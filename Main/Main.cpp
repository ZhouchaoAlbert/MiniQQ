#include "UtilPath.h"
#include "AuxMainUI.h"
#include "UIlib.h"
using namespace DuiLib;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	
	//test
	//CString strPath;
	//strPath = Util::Path::GetAppPath();
	//strPath = Util::Path::GetResFolder();
	//Util::Path::GetCurrentDir();
	//Util::Path::GetTempPath();
	//Util::Path::CreateDirectory(_T("C:\\A\\B\\C\\D\\E"), NULL);

	CPaintManagerUI::SetInstance(hInstance);
	CString s = Util::Path::GetResFolder();
	CPaintManagerUI::SetResourcePath(s);

	if (FALSE != Aux::MainUI::LoginFrame())
	{
		Aux::MainUI::ShowMain();
	}
	
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}
