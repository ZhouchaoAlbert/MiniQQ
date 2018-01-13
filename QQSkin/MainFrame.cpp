#include "MainFrame.h"



CMainFrame::CMainFrame()
{
}


CMainFrame::~CMainFrame()
{
}


CDuiString CMainFrame::GetSkinFolder()
{
	return _T("Resource");
}

CDuiString CMainFrame::GetSkinFile()
{
	return _T("xml\\MainFrame.xml");
}

LPCTSTR CMainFrame::GetWindowClassName(void) const
{
	return _T("MainFrameClass");
}

CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{
	
	return NULL;
}

void CMainFrame::InitWindow()
{

	
}

#include "AuxBrowser.h"

void CMainFrame::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		CDuiString szName = msg.pSender->GetName();
		if (szName == _T("Test"))
		{
			//Aux::Browser::OpenPV();
		}
	}

}