#include "LoginFrame.h"

#include "UtilPath.h"

CLoginFrame::CLoginFrame()
{
}


CLoginFrame::~CLoginFrame()
{
}


CDuiString CLoginFrame::GetSkinFolder()
{
	return _T("Resource");
}

CDuiString CLoginFrame::GetSkinFile()
{
	return _T("xml\\LoginFrame.xml");
}

LPCTSTR CLoginFrame::GetWindowClassName(void) const
{
	return _T("LoginFrameClass");
}

CControlUI* CLoginFrame::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("HeadIconUI")) == 0)
	{
		return new CHeadIconUI();
	}
	else if (_tcscmp(pstrClass, _T("EditCombUI")) == 0)
	{
		return new CEditCombUI();
	}
	else if (_tcscmp(pstrClass, _T("IPAddressUI")) == 0)
	{
		return new CIPAddressUI();
	}
	return NULL;
}

void CLoginFrame::InitWindow()
{

	m_pHeadIcon = static_cast<CHeadIconUI*>(m_PaintManager.FindControl(_T("user_icon")));

	CString strIcon = Util::Path::GetResMaterialFolder() + _T("\\DefaultFace.png");
	m_pHeadIcon->SetHeadIcon(strIcon);
}

void CLoginFrame::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		CDuiString szName = msg.pSender->GetName();
		if (szName == _T("btn_close"))
		{
			Close();
		}
		else if (szName == _T("btn_min"))
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if (szName == _T("btn_login"))
		{
			Close(IDOK);
		}
	}

}