#ifndef __CEF_CLIENT_APP_H__
#define __CEF_CLIENT_APP_H__

#include <include/cef_app.h>
#include "include/cef_print_handler.h"
#include "atlstr.h"

using namespace ATL;

class CClientApp : 
	public CefApp,
	public CefBrowserProcessHandler,
	public CefRenderProcessHandler
{
public:
	CClientApp();
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
		OVERRIDE{ return this; }

	virtual bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		NavigationType navigation_type,
		bool is_redirect) OVERRIDE;

	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) OVERRIDE;


	virtual void OnBeforeCommandLineProcessing(
		const CefString& process_type,
		CefRefPtr<CefCommandLine> command_line) OVERRIDE;

	void SetProxy(LPCTSTR pszIP, UINT16 port, LPCTSTR pszUser, LPCTSTR pszPwd);


	public:
		virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE;
		virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
		virtual void OnWebKitInitialized() OVERRIDE;


	IMPLEMENT_REFCOUNTING(CClientApp);

private:
	CString	m_strIP;
	UINT16	m_u16Port;
	CString	m_strUser;
	CString	m_strPwd;
};

#endif

