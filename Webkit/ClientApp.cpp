#include "stdafx.h"
#include "ClientApp.h"
#include "atlstr.h"


using namespace ATL;
CClientApp::CClientApp()
{
}

bool CClientApp::OnBeforeNavigation(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	NavigationType navigation_type,
	bool is_redirect)
{
	m_u16Port = 0;

	return false;
}

//BROWSER---->RENDER
bool CClientApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, 
	CefProcessId source_process, 
	CefRefPtr<CefProcessMessage> message)
{
	std::string message_name = message->GetName();
	if (message_name == "execute_js")
	{
		return true;
	}
	return false;
}

const char kProcessType[] = "type";
void CClientApp::OnBeforeCommandLineProcessing(const CefString& process_type, 
	CefRefPtr<CefCommandLine> command_line)
{
	if (!command_line->HasSwitch("disable-gpu"))
	{
		command_line->AppendSwitch("disable-gpu");
	}
	if (!command_line->HasSwitch("in-process-plugins"))
	{
		command_line->AppendSwitch("in-process-plugins");
	}
	if (!command_line->HasSwitch("process-per-site"))
	{
		command_line->AppendSwitch("process-per-site");
	}

	//°²×°flash²å¼þ PepperFlash/24.0.0.221/
	TCHAR szPath[MAX_PATH * 2] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, _T("\\pepflashplayer.dll;application/x-shockwave-flash"));
	command_line->AppendSwitchWithValue("ppapi-flash-version", "24.0.0.221");
	command_line->AppendSwitchWithValue("register-pepper-plugins", szPath);

	if (!command_line->HasSwitch("proxy-server"))
	{
		if (!m_strIP.IsEmpty() && m_u16Port)
		{
			CString strProxy;
			strProxy.Format(_T("socks5://%s:%u"), m_strIP, m_u16Port);
			CefString	cefProxy(strProxy);
			command_line->AppendSwitchWithValue("proxy-server", cefProxy);
//			command_line->AppendSwitchWithValue("proxy-bypass-list", "127.*,192.168.*,10.10.*,193.9.162.*");

		}
	}

	if (!command_line->HasSwitch("enable-npapi"))
		command_line->AppendSwitch(_T("enable-npapi"));
	
	CString strProcessType = _T("browser");
	if (command_line->HasSwitch(kProcessType))
	{
		CefString strType = command_line->GetSwitchValue(kProcessType);
		strProcessType = strType.ToWString().c_str();
	}


}

void CClientApp::SetProxy(LPCTSTR pszIP, UINT16 port, LPCTSTR pszUser, LPCTSTR pszPwd)
{
	m_strIP = pszIP;
	m_u16Port = port;
	m_strUser = pszUser;
	m_strPwd = pszPwd;
}

////////////////////////////////////////////////////////////////////////////////////


CefRefPtr<CefBrowserProcessHandler> CClientApp::GetBrowserProcessHandler() 
{
	return this;
}

void CClientApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) 
{
	context->Enter();

	context->Exit();
}


void CClientApp::OnWebKitInitialized()
{

}

