#pragma once

class CNamePipeSerMgr
{
public:
	CNamePipeSerMgr();
	~CNamePipeSerMgr();
public:
	DWORD Create(WCHAR *name, DWORD dwMode);				//	创建/开启管道
	DWORD Recv(HANDLE *hFrom, BYTE *bBuf, DWORD *dwLen);	//	接收数据
	DWORD Send(HANDLE hTo, BYTE *bBuf, DWORD dwLen);		//	发送数据
	DWORD Close();										    //	关闭管道
};

//////////////////////////////////////////////
//////////////////////////////////////////////


