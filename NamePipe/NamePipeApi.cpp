#include "stdafx.h"
#include "NamePipeApi.h"
#include "Singleton.h"
#include "NamePipeMgr.h"
#include "NamePipeSeriveMgr.h"
#include "NamePipeClientMgr.h"
#include "PackageMgr.h"

BOOL Aux::NameService::CS2CSink::Send(UINT16 uCmd, UINT8 *buf, UINT16 len)
{
	return Singleton<CPackageMgr>::Instance().Send(uCmd, buf, len,this);
}

UINT32 Aux::NameService::Create(LPCSTR lpszName)
{
	return Singleton<CNamePipeSeriveMgr>::Instance().Create(lpszName);
}

UINT32 Aux::NameService::Send(UINT16 cmd, UINT8* buf, UINT16 len)
{
	return Singleton<CPackageMgr>::Instance().Send(cmd, buf, len, NULL);
}


BOOL Aux::NameClient::CC2SSink::Send(UINT16 uCmd, UINT8 *buf, UINT16 len)
{
	return Singleton<CPackageMgr>::Instance().Send(uCmd, buf, len, this);
}

UINT32 Aux::NameClient::Create(LPCSTR lpszName)
{
	return Singleton<CNamePipeClientMgr>::Instance().Create(lpszName);
}

UINT32 Aux::NameClient::Send(UINT16 cmd, UINT8* buf, UINT16 len)
{
	return Singleton<CNamePipeClientMgr>::Instance().Send(cmd, buf, len);
}


