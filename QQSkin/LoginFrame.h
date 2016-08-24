#pragma once
#include <UIlib.h>
using namespace DuiLib;

class CLoginFrame : public WindowImplBase
{
public:
	CLoginFrame();
	~CLoginFrame();

	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
};

