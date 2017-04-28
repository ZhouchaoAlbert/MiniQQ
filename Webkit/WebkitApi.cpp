#include "stdafx.h"
#include "WebkitApi.h"
#include "ClientApp.h"
#include "UtilPath.h"

INT32 Aux::WebkitApi::Init(HINSTANCE hInstance)
{
	CefMainArgs main_args(hInstance);
	CefRefPtr<CClientApp> app(new CClientApp);

	INT32 exitCode = CefExecuteProcess(main_args, app.get(), NULL);
	if (exitCode >= 0) return -1;

	CefSettings settings;
	settings.multi_threaded_message_loop = true;
	settings.single_process =  false;
	settings.background_color = 822;
	settings.no_sandbox = true;
// 	CString strLogFile = Util::Path::GetLogFolder(TRUE) + _T("\\ECBrwsCef.log");
// 	cef_string_wide_to_utf16(strLogFile, strLogFile.GetLength(), &settings.log_file);

	CString strCefRes =  Util::Path::GetCefResPath();
	CString strLocal = strCefRes + _T("\\locales");
	CefString(&settings.resources_dir_path) = strCefRes;
	CefString(&settings.locales_dir_path) = strLocal;
/*	CefString(&settings.cache_path) = Util::Path::GetCefCachePath(TRUE);*/

// #ifdef DEBUG
// 	settings.log_severity = LOGSEVERITY_VERBOSE;//tbd
// 	CString strSubProcessFilePath = Util::Path::GetExeFolder() + _T("\\ECBrwsSubD.exe");
// #else
// 	settings.log_severity = LOGSEVERITY_VERBOSE;
// 	CString strSubProcessFilePath = Util::Path::GetExeFolder() + _T("\\ECBrwsSub.exe");
// #endif
// 	cef_string_wide_to_utf16(strSubProcessFilePath, strSubProcessFilePath.GetLength(), &settings.browser_subprocess_path);
	cef_string_ascii_to_utf16("zh-CN", strlen("zh-CN"), &settings.locale);

	bool bRet = CefInitialize(main_args, settings, app.get(), NULL);
	if (!bRet)
	{
		ATLASSERT(FALSE);
		return -2;
	}
	return 0;
}

void Aux::WebkitApi::DoExit()
{


}