#pragma once
#include <UIlib.h>
#include "UIHeadIcon.h"
using namespace DuiLib;

class CLoginFrame : public WindowImplBase
{
public:
	CLoginFrame();
	~CLoginFrame();

	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);

private:
	CUIHeadIcon* m_pHeadIcon;
};

