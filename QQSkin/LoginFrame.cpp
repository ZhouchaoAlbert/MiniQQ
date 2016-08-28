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
		return new CUIHeadIcon();
	}

	return NULL;
}

void CLoginFrame::InitWindow()
{

	m_pHeadIcon = static_cast<CUIHeadIcon*>(m_PaintManager.FindControl(_T("user_icon")));

	CString strIcon = Util::Path::GetResMaterialFolder() + _T("\\DefaultFace.png");
	m_pHeadIcon->SetHeadIcon(strIcon);
}