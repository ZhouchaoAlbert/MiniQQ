#include "stdafx.h"
#include "ClientHandler.h"
#include "include/cef_runnable.h"
#include "Singleton.h"
#include "UtilString.h"
#include <ShellAPI.h>

#include "ShlObj.h"

#define REQUIRE_UI_THREAD()   ATLASSERT(CefCurrentlyOn(TID_UI));
#define REQUIRE_IO_THREAD()   ATLASSERT(CefCurrentlyOn(TID_IO));
#define REQUIRE_FILE_THREAD() ATLASSERT(CefCurrentlyOn(TID_FILE));

const TCHAR CEF_LOAD_TIMEOUT[] = _T("CefLoadTimeout");
const UINT32 LOAD_TIMEOUT = 20 * 1000;

const TCHAR CEF_START_TIMEOUT[] = _T("CefStartTimeout");
const UINT32 START_TIMEOUT = 5 * 1000;


enum client_menu_ids 
{
	CLIENT_ID_SHOW_DEVTOOLS = MENU_ID_USER_FIRST,
	CLIENT_ID_CLOSE_DEVTOOLS,
	CLIENT_ID_INSPECT_ELEMENT,
};

CClientHandler::CClientHandler()
{
	m_bClosing = FALSE;
}

CClientHandler::~CClientHandler()
{

}

//CefDisplayHandler methods:
void CClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) 
{

}

void CClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& url) 
{
	m_szURL = url.ToWString();
	if (m_Browser && browser->GetIdentifier() == m_Browser->GetIdentifier())
	{
		//Singleton<CAsyncMsg>::Instance().PostHandlerMsg(m_Hwnd, bwt_change_url, m_id, m_szURL.c_str());
	}

	CefDisplayHandler::OnAddressChange(browser, frame, url);
}

// CefLifeSpanHandler methods:
bool CClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& target_url,
	const CefString& target_frame_name,
	cef_window_open_disposition_t target_disposition,
	bool user_gesture,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings,
	bool* no_javascript_access)
{
	//这里使用默认浏览器打开网页，避免CEF重新创建窗口  
	ShellExecute(NULL, L"open", target_url.c_str(), NULL, NULL, SW_SHOW);
	//return false;//创建新窗口  
	return true;   //禁止创建新的窗口  
}

void CClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	if (!m_Browser.get())
	{
		m_Browser = browser;
	}
	//开始不显示，防止一开始显示空白界面
	CefRefPtr<CefBrowserHost> host = browser->GetHost();
	HWND hWnd = host->GetWindowHandle();
	if (hWnd && IsWindow(hWnd))
	{
		BOOL bIsVisible = ::IsWindowVisible(hWnd);
		::ShowWindow(hWnd, SW_HIDE);
	}

	if (!m_szURL.empty())
	{
		if (browser.get()){
			browser->GetMainFrame()->LoadURL(m_szURL.c_str());
		}
	}

	if (browser->IsPopup())
		browser->GetHost()->CloseBrowser(false);
}

bool CClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	if (m_bClosing)
	{
// 		CloseDevTools(browser);
// 		CString strArgsList;
// 		strArgsList.Format(_T("Download-CloseByPVClose:"));
// 		Singleton<CAsyncMsg>::Instance().PostHandlerMsg(m_Hwnd, bwt_show_ui, m_id, strArgsList);
		return false;//默认处理
	}
	else
	{//页面内部触发的关闭请求此时应该上上层知道而不是私下处理

		//Singleton<CAsyncMsg>::Instance().PostHandlerMsg(m_Hwnd, bwt_cef_request_close, m_id, _T(""));
		return true;//默认处理
	}

}

void CClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	//通知上层网页按钮调用关闭窗口
	if (!m_bClosing && m_Browser && browser->GetIdentifier() == m_Browser->GetIdentifier())
	{
		m_bClosing = TRUE;

		//Singleton<CAsyncMsg>::Instance().PostHandlerMsg(m_Hwnd, bwt_close_ec, m_id, _T(""));
	}

	CefLifeSpanHandler::OnBeforeClose(browser);
}


void CClientHandler::OnLoadStart(CefRefPtr< CefBrowser > browser, CefRefPtr< CefFrame > frame)
{
	//MessageBox(NULL, _T("OnLoadStart"), _T("OnLoadStart"), MB_OK);
	m_bHaveLoad = TRUE;
	CString strContent;
	strContent.Format(_T("LoadStart-%s"), m_szURL.c_str());

	CString szRet = frame->GetURL().ToWString().c_str();

	//Singleton<CAsyncMsg>::Instance().PostHandlerMsg(m_Hwnd, bwt_load_finish, m_id, _T("1"));

}

void CClientHandler::OnLoadEnd(CefRefPtr< CefBrowser > browser, CefRefPtr< CefFrame > frame, int httpStatusCode)
{
	//MessageBox(NULL, _T("OnLoadEnd"), _T("OnLoadEnd"), MB_OK);
	m_bHaveLoad = FALSE;

	m_TimeOutCount = 0;
	CString strContent;
	strContent.Format(_T("LoadEnd-%s"), m_szURL.c_str());

	CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
	CString szRet = frame->GetURL().ToWString().c_str();
	
	if (szRet.Compare(L"cef-error:") == 0 || szRet.Compare(L"data:text/html,chromewebdata") == 0)//下载的文件名含有逗号无法处理提示错误
	{

		//Singleton<CAsyncMsg>::Instance().PostHandlerMsg(m_Hwnd, bwt_load_timeout_or_error, m_id, _T("2")); //失败
	
		return;
	}

	if (szRet.Compare(L"about:blank") != 0 && szRet.GetLength()>0)
	{
		//通知上层网页加载完成
	//	Singleton<CAsyncMsg>::Instance().PostHandlerMsg(m_Hwnd, bwt_load_finish, m_id, _T("2"));
	}
}

void CClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	ErrorCode errorCode,
	const CefString& errorText,
	const CefString& failedUrl)
{
	m_bHaveLoad = FALSE;

	m_TimeOutCount = 0;

	if (browser)
	{
		//通知上层页面加载失败

		if (ERR_ABORTED != errorCode/* || -349 == errorCode*/)
		{
			//屏蔽showec引起的错误
			//			if (NULL != m_pIPageEvent)
			//			{
			//				m_pIPageEvent->PvLoadStatus(PV_LOADERROR);
			//			}

		}

	}
	return;
}


bool CClientHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
	const CefKeyEvent& event,
	CefEventHandle os_event,
	bool* is_keyboard_shortcut)
{
	if (m_bClosing)
	{
		//出现过进入这里，但是浏览器关闭了，所以这里排除这种情况
		return false;
	}

	CefString strURL = GetBrowser()->GetMainFrame()->GetURL();
	if (event.windows_key_code == VK_F5 && os_event != NULL && os_event->message == WM_KEYFIRST)
	{
		browser->ReloadIgnoreCache();
		return true;
	}


	if ((event.windows_key_code == VK_ESCAPE  || event.windows_key_code == VK_DOWN || event.windows_key_code  == VK_UP || event.windows_key_code  == VK_RETURN )&& os_event != NULL && os_event->message == WM_KEYFIRST)
	{
		if (GetBrowser()->GetMainFrame())
		{
			 CefRefPtr<CefBrowser> browserMain = NULL;
			 browserMain = GetBrowser()->GetMainFrame()->GetBrowser();
			 if (browserMain.get())
			 {
				if (browserMain->GetHost())
				{
					HWND hWnd = browserMain->GetHost()->GetWindowHandle();

					if (hWnd != NULL)
					{
						HWND hParent = ::GetParent(hWnd);

						if (::IsWindow(hParent))
						{
							::PostMessage(hParent, WM_KEYDOWN, event.windows_key_code, 0);
						}
						
					}
				}
			 }
		}
	}

	return false;
}



void CClientHandler::OnBeforeDownload(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	const CefString& suggested_name,
	CefRefPtr<CefBeforeDownloadCallback> callback)
{
	REQUIRE_UI_THREAD();

	CefRefPtr<CefBrowserHost> host = browser->GetHost();
	if (host.get() == NULL) return;
	HWND hWnd = host->GetWindowHandle();
	if (!IsWindow(hWnd)) return;
	CString strFileName = suggested_name.ToWString().c_str();




	//callback->Continue((CefString)strPathName, false);
}

void CClientHandler::OnDownloadUpdated(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	CefRefPtr<CefDownloadItemCallback> callback)
{
	REQUIRE_UI_THREAD();

	if (!download_item->IsValid())
	{
		return;
	}
	if (download_item->IsCanceled())  //取消下载
	{
		return;
	}

	INT32   iFileID = download_item->GetId();
	CString strUrl = download_item->GetURL().c_str();
	CString strFilePath = download_item->GetFullPath().ToWString().c_str();

}


bool CClientHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	bool is_redirect)
{
	std::string szUrl = request->GetURL();

	if (szUrl.find("showec:") != std::string::npos)
	{
		//Singleton<CAsyncMsg>::Instance().PostHandlerMsg(m_Hwnd, bwt_show_ec, m_id, request->GetURL().ToWString().c_str());
		return true;
	}
	if (szUrl.find("tencent:") != std::string::npos)
	{
		//讲道理客户端不应该这样拦截处理。但是由于急于体测。出此下策。后续找web一起定位解决   by:xzl 2016/8/29
		CString strCommand;
		Util::String::A_2_W(szUrl.c_str(), strCommand);
		ShellExecute(NULL, _T("open"), strCommand, NULL, NULL, SW_HIDE);
		return true;
	}
	return false;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CClientHandler::CloseAllBrowsers(bool force_close) {
	if (!CefCurrentlyOn(TID_UI)) {
		// Execute on the UI thread.
		CefPostTask(TID_UI,	NewCefRunnableMethod(this, &CClientHandler::CloseAllBrowsers,force_close));

		return;
	}

	if (!m_bClosing && m_Browser)
	{
		m_bClosing = TRUE;

		//Singleton<CAsyncMsg>::Instance().PostHandlerMsg(m_Hwnd, bwt_close_ec, m_id, _T(""));
	}
}



void CClientHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	CefRefPtr<CefMenuModel> model)
{
    if (params->GetTypeFlags() & CM_TYPEFLAG_EDITABLE)
    {
        return;
    }

    if (params->GetTypeFlags() & CM_TYPEFLAG_SELECTION)
    {
        CefString strCopy = model->GetLabel(MENU_ID_COPY);
        model->Clear();
        model->AddItem(MENU_ID_COPY, strCopy);
        return;
    }
   
    model->Clear();
}

bool CClientHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	int command_id,
	EventFlags event_flags)
{
	//switch (command_id) {
	//default:  // Allow default handling, if any.
		return false;
	//}
}

void CClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
	bool isLoading,
	bool canGoBack,
	bool canGoForward)
{
	//MessageBox(NULL, _T("OnLoadingStateChange"), _T("OnLoadingStateChange"), MB_OK);
}

bool CClientHandler::OnProcessMessageReceived(
	CefRefPtr<CefBrowser> browser,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message)
{

	// Check for messages from the client renderer.
	CString message_name = message->GetName().c_str();
	if (message_name == _T("show_waiting"))
	{
		return true;
	}
	if (message_name == _T("pv_json_call"))
	{

		CefRefPtr<CefListValue> argList = message->GetArgumentList();
		CefString strFuncName = argList->GetString(0);

		INT32 cmd  = argList->GetInt(1);
		CefString strJsonData = argList->GetString(2);

		CString str;
		str.Format(_T("%s-%d-%s"), strFuncName.c_str(), cmd, strJsonData.c_str());
	

		//Singleton<CAsyncMsg>::Instance().PostPVJson(m_Hwnd, bwt_pv_json_call, m_id, cmd, strJsonData.c_str());

		return true;
	}
	return false;
}


bool CClientHandler::OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser,
	const CefString& message_text,
	bool is_reload,
	CefRefPtr<CefJSDialogCallback> callback)
{
	if (m_bInputCheck)
	{
		callback->Continue(false, _T(""));
		m_bInputCheck = FALSE;
		///Singleton<CAsyncMsg>::Instance().PostHandlerMsg(m_Hwnd, bwt_show_closetip, m_id, _T("1"));
	}
	else
	{
		if (m_bClosing)
		{
			
			callback->Continue(true, _T(""));
			//通知更新进度
			CString strArgsList;
			strArgsList.Format(_T("Download-CloseByPVClose:"));
			//Singleton<CAsyncMsg>::Instance().PostHandlerMsg(m_Hwnd, bwt_show_ui, m_id, strArgsList);
		}
		else
		{//页面内部触发的关闭请求此时应该上上层知道而不是私下处理
			callback->Continue(false, _T(""));
			//Singleton<CAsyncMsg>::Instance().PostHandlerMsg(m_Hwnd, bwt_cef_request_close, m_id, _T(""));
		}
	}
	return true;
}

void  CClientHandler::Init(Aux::Browser::BrwsID id,HWND Hwnd)
{
	m_id        = id;
	m_Hwnd      = Hwnd;
}

void CClientHandler::UnInit(BOOL bCheckInput)
{
	if (!m_bClosing && m_id.IsValid() && m_Browser.get())
	{
		CefRefPtr<CefBrowser> Browser = m_Browser;
		m_bInputCheck = bCheckInput;
		if (FALSE == m_bInputCheck)
		{
			m_Browser = NULL;
			m_bClosing = TRUE;
		}
		else
		{
			m_bClosing = FALSE;
		}
		Browser->GetHost()->CloseBrowser(false);
	}
	else
	{
		m_bClosing = TRUE;
	}
}







void CClientHandler::RefreshPage()
{
	m_TimeOutCount++;
	m_bTimeOut = FALSE;
}

