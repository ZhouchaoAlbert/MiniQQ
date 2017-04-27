#ifndef __CEF_CLIENT_HANDLER_H__
#define __CEF_CLIENT_HANDLER_H__

#include <include/cef_browser.h>
#include <include/cef_client.h>
#include "include/cef_task.h"
#include <map>
#include <atlstr.h>
#include "BrowserDef.h"
using namespace std;
using namespace ATL;


class CClientHandler :
	public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefKeyboardHandler,
	public CefDownloadHandler,
	public CefRequestHandler,
	public CefContextMenuHandler,
	public CefJSDialogHandler
{
public:
	CClientHandler();
	~CClientHandler();

	// CefClient methods:
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE{ return this;}

	//CefDisplayHandler methods:
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) OVERRIDE;
	virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& url) OVERRIDE;

	// CefLifeSpanHandler methods:
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		CefLifeSpanHandler::WindowOpenDisposition target_disposition,
		bool user_gesture,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access) OVERRIDE;

	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	//CefLoadHandler methods:
	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) OVERRIDE;
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) OVERRIDE;

	//CefKeyboardHandler methods:
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE{ return this; }
	virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event, bool* is_keyboard_shortcut) OVERRIDE;

	// CefDownloadHandler methods:
	virtual void OnBeforeDownload(CefRefPtr<CefBrowser> browser,CefRefPtr<CefDownloadItem> download_item,const CefString& suggested_name,CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;
	virtual void OnDownloadUpdated(CefRefPtr<CefBrowser> browser,CefRefPtr<CefDownloadItem> download_item,CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;
	virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE{ return this; }

	//CefRequestHandler methods:
	virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		bool is_redirect) OVERRIDE;
	bool CClientHandler::OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser,
		const CefString& message_text,
		bool is_reload,
		CefRefPtr<CefJSDialogCallback> callback);


	virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE{ return this; }



	//CefContextMenuHandler  methods:
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		CefRefPtr<CefMenuModel> model) OVERRIDE;
	virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		int command_id,
		EventFlags event_flags) OVERRIDE;



	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message)
		OVERRIDE;


	
	// Request that all existing browser windows close.
	void CloseAllBrowsers(bool force_close);



	virtual bool OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text)
	{
		return false;
	}

	//Self methods:
	CefRefPtr<CefBrowser> GetBrowser() { 
		return m_Browser; 
	}


	virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward)OVERRIDE;

	void RefreshPage();

	void Init(Aux::Browser::BrwsID id, HWND Hwnd);
	void UnInit(BOOL bCheckInput);
	BOOL IsClosing(){ return m_bClosing; }

protected:
	CefRefPtr<CefBrowser> m_Browser;
	std::wstring m_szURL;
	BOOL m_bClosing;
	BOOL m_bInputCheck;
	Aux::Browser::BrwsID m_id;
	HWND                 m_Hwnd;
protected:
	void OnTimer();
	void OnLoadTimer();
protected:

	IMPLEMENT_REFCOUNTING(CClientHandler);


	BOOL m_bHaveLoad;

	BOOL m_bTimeOut;
	UINT32 m_TimeOutCount;
};

#endif

