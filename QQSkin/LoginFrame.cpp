#include "LoginFrame.h"


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