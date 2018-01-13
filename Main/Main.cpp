#include "UtilPath.h"
#include "AuxMainUI.h"
#include "UIlib.h"
#include "ByteIOS.h"
#include "UtilPath.h"
#include "NamePipeApi.h"
#include "TimerApi.h"

using namespace DuiLib;

class AA
{
public:
	AA();
	~AA();
	void OnTimer(void *pParam1, void *pParam2, void *pParam3);
private:
	CTimerSink m_TimeSink;
};

AA::AA()
{
	m_TimeSink.Register(_T("StartPipe"), this, &AA::OnTimer);
	m_TimeSink.Start(1000);
}

AA::~AA()
{
}

void AA::OnTimer(void *pParam1, void *pParam2, void *pParam3)
{
	m_TimeSink.Stop();
	//创建服务端
	//Aux::NameService::Create(STR_NAMEPIPE);
}
#include "Singleton.h"
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

	Singleton<AA>::Instance();
 	//Test拉起Browser进程
 #ifdef DEBUG
 	CString strPath = _T("BrowserD.exe");
 #else
 	CString strPath = _T("Browser.exe");
 #endif
 	CString  strBrowser = Util::Path::GetAppPath() + _T("\\") + strPath;
 	TCHAR szCmd[1024];
 	_sntprintf_s(szCmd, sizeof(szCmd), _T("\"%s\""),strBrowser);
 
 	STARTUPINFO si;
 	PROCESS_INFORMATION pi;
 
 	ZeroMemory(&si, sizeof(si));
 	si.cb = sizeof(si);
 	ZeroMemory(&pi, sizeof(pi));
 
 	BOOL bRetVal = TRUE;
 	if (CreateProcess(
 		NULL,
 		szCmd,
 		NULL,
 		NULL,
 		FALSE,
 		0,
 		NULL,
 		NULL,
 		&si,
 		&pi))
 	{
 		CloseHandle(pi.hThread);
 		CloseHandle(pi.hProcess);
 	}
 	else
 	{
 		HRESULT hr = GetLastError();
 		bRetVal = FALSE;
 	}
 

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}
