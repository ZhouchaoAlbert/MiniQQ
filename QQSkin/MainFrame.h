#pragma once
#include <UIlib.h>
using namespace DuiLib;

class CMainFrame : public WindowImplBase
{
public:
	CMainFrame();
	~CMainFrame();

	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);

};

