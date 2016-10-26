#include "AuxMainUI.h"
#include "LoginFrame.h"
#include "Singleton.h"
#include "MainFrame.h"

BOOL Aux::MainUI::LoginFrame()
{
	CLoginFrame*	pLoginFrame = &Singleton<CLoginFrame>::Instance();
	pLoginFrame->Create(NULL, _T("MiniQQ"), WS_POPUPWINDOW, 0, 0, 0, 0, 0);
	pLoginFrame->CenterWindow();
	UINT32 uRet = pLoginFrame->ShowModal();
	if (uRet == IDOK)
	{
		Singleton<CLoginFrame>::UnInstance();
		return TRUE;
	}
	return FALSE;
}


void Aux::MainUI::ShowMain()
{
	CMainFrame* pMainFrame = &Singleton<CMainFrame>::Instance();
	pMainFrame->Create(NULL, _T("MinQQ"), WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, 0);
	pMainFrame->CenterWindow();
	pMainFrame->ShowWindow(true, true);
}