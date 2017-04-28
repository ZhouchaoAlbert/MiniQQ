#include "stdafx.h"
#include "AuxBrowser.h"
#include "BrowserMgr.h"
#include "Singleton.h"

BOOL Aux::Browser::OpenPV()
{
	return Singleton<CBrowserMgr>::Instance().OpenPV();
}
