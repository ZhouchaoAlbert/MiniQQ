#include "stdafx.h"
#include "NamePipeMgr.h"


CNamePipeSerMgr::CNamePipeSerMgr()
{
}


CNamePipeSerMgr::~CNamePipeSerMgr()
{
}

DWORD CNamePipeSerMgr::Create(WCHAR *name, DWORD dwMode)
{
	return 0;
}
DWORD CNamePipeSerMgr::Recv(HANDLE *hFrom, BYTE *bBuf, DWORD *dwLen)
{
	return 0;
}
DWORD CNamePipeSerMgr::Send(HANDLE hTo, BYTE *bBuf, DWORD dwLen)
{
	return 0;
}
DWORD CNamePipeSerMgr::Close()
{
	return 0;
}
