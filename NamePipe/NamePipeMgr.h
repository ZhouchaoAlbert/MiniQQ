#pragma once

class CNamePipeSerMgr
{
public:
	CNamePipeSerMgr();
	~CNamePipeSerMgr();
public:
	DWORD Create(WCHAR *name, DWORD dwMode);				//	����/�����ܵ�
	DWORD Recv(HANDLE *hFrom, BYTE *bBuf, DWORD *dwLen);	//	��������
	DWORD Send(HANDLE hTo, BYTE *bBuf, DWORD dwLen);		//	��������
	DWORD Close();										    //	�رչܵ�
};

//////////////////////////////////////////////
//////////////////////////////////////////////


