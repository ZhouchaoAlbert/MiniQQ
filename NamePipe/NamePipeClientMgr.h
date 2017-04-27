#pragma once
#include "BaseDefine.h"
class CNamePipeClientMgr
{
public:
	CNamePipeClientMgr();
	~CNamePipeClientMgr();

public:
	UINT32 Create(LPCSTR lpszName);
	UINT32 Send(UINT16 cmd, UINT8* buf, UINT16 len);


private:	
	static UINT32 WINAPI ThreadReadFile(LPVOID pv);

	HANDLE  m_hPipe;
};

