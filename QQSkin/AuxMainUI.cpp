#include "AuxMainUI.h"
#include "LoginFrame.h"
#include "Singleton.h"

BOOL Aux::MainUI::LoginFrame()
{
	CLoginFrame*	pLoginFrm = &Singleton<CLoginFrame>::Instance();
	pLoginFrm->Create(NULL, _T("MiniQQ"), WS_POPUPWINDOW, 0, 0, 0, 0, 0);
	pLoginFrm->CenterWindow();
	pLoginFrm->ShowModal();
// 	UINT32 uRet = pLoginFrm->GetReturn();
// 	Singleton<DuiLib::CLoginFrame>::UnInstance();
// 	return (uRet == IDOK);
	return TRUE;
}