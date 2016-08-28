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


	CUIHeadIcon* m_pHeadIcon;
};

