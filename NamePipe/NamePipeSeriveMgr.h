#pragma once
class CNamePipeSeriveMgr
{
public:
	CNamePipeSeriveMgr();
	~CNamePipeSeriveMgr();

	UINT32 Create(LPCSTR lpszName);

	UINT32 Send(UINT16 cmd, UINT8* buf, UINT16 len);

	static UINT32 WINAPI WorkThreadProc(LPVOID pv);

private:	
	HANDLE m_hPipe;

};

