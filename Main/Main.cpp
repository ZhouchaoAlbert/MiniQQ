#include "UtilPath.h"
#include "AuxMainUI.h"
#include "UIlib.h"
#include "ByteIOS.h"
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

	Util::Buf::CByteIOS buf2;

	CString strTestA = _T("sfvwsggegerhhjjkjl");
	buf2 << strTestA;

	Util::Buf::CByteIOS buf;
	UINT32 i = 1;
	UINT16 j = 2;


	CString strA = _T("12345"), strB = _T("56789");

	CStringA strAA = _T("12345"), strBB = _T("56789");


	buf << i << j << (LPCWSTR)strA << (LPCWSTR)strB;

	UINT32 k = 0;
	UINT16 n = 0;
	CStringW strC, strD;
	CStringA strCC, strDD;
	buf >> k >> n >> strC >> strD;

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
