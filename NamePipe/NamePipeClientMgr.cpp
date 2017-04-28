#include "stdafx.h"
#include "NamePipeClientMgr.h"
#include "NamePipeApi.h"
#include "ByteIOS.h"
#include <memory>
#include "NamePipeHelper.h"

int WaitFinish2(HANDLE hPipe, OVERLAPPED &tagOver)
{
	bool bPendingIO = false;
	switch (GetLastError())
	{
		//正在连接中  
	case ERROR_IO_PENDING:
		bPendingIO = true;
		break;
		//已经连接  
	case ERROR_PIPE_CONNECTED:
		SetEvent(tagOver.hEvent);
		break;
	}

	DWORD dwWait = -1;
	DWORD dwTransBytes = -1;

	//等待读写操作完成  
	dwWait = WaitForSingleObject(tagOver.hEvent, INFINITE);
	switch (dwWait)
	{
	case 0:
		if (bPendingIO)
		{
			//获取Overlapped结果  
			if (GetOverlappedResult(hPipe, &tagOver, &dwTransBytes, FALSE) == FALSE)
			{
				printf("ConnectNamedPipe  failed   %d\n", GetLastError());
				return -1;
			}
		}
		break;
		//  读写完成  
	case WAIT_IO_COMPLETION:
		break;

	}

	return 0;
}

CNamePipeClientMgr::CNamePipeClientMgr() :
m_hPipe(NULL)
{
}


CNamePipeClientMgr::~CNamePipeClientMgr()
{
}

UINT32 CNamePipeClientMgr::Create(LPCSTR lpszName)
{
	if (WaitNamedPipeA(lpszName, NMPWAIT_WAIT_FOREVER) == 0)
	{
		printf("WaitNamedPipeA  error\n");
		return -1;
	}

	m_hPipe = CreateFileA(lpszName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);


	if (m_hPipe == INVALID_HANDLE_VALUE)
	{
		printf("CreateFileA error\n");
		return -2;
	}

	//读  
	HANDLE hHandle = (HANDLE)_beginthreadex(0, 0, &ThreadReadFile, m_hPipe, 0, 0);
	CloseHandle(hHandle);
	return 0;
}

UINT32 WINAPI CNamePipeClientMgr::ThreadReadFile(LPVOID pv)
{
	HANDLE hPipe = (HANDLE)pv;
	char szReadBuf[256] = { 0 };
	DWORD dwReadSize = 0;
	OVERLAPPED tagOver;
	memset(&tagOver, 0x0, sizeof(tagOver));

	tagOver.hEvent = CreateEvent(NULL,//默认属性  
		TRUE,//手工reset  
		TRUE,//初始状态signaled  
		NULL);//未命名  

	while (1)
	{
		//读  
		INT32 nRet = ReadFile(hPipe, szReadBuf, sizeof(szReadBuf), &dwReadSize, &tagOver);


 		Util::Buf::CByteIOS buf;
 		if (szReadBuf && tagOver.InternalHigh)
 		{
 			buf.Write((UINT8*)&szReadBuf, tagOver.InternalHigh);
 			UINT32 Falg = 0;
 			UINT16 cmd = 0;
			UINT16 total = 0;
			UINT64 seq = 0;
 			CStringW strTestA;
 			CStringW strTestB;
 
			buf >> Falg >> total >> seq >> cmd >> strTestA >> strTestB;
			MessageBox(NULL, strTestA, strTestB,MB_OK);
			shared_ptr<Util::Buf::CByteIOS> buf;
			buf = shared_ptr<Util::Buf::CByteIOS>(new Util::Buf::CByteIOS);
			 total = 0;
			(*buf) << PRO_FLAG << total << seq << cmd << (LPCWSTR)strTestB;
		
			total = buf->GetLen();
			buf->Set(4, (UINT8*)&total, sizeof(UINT16));
			Aux::NameClient::Send(cmd, buf->GetBuf(), buf->GetLen());
 		}


		if (WaitFinish2(hPipe, tagOver) != 0)
			return -1;

		//printf("Server  Msg: %s\n", szReadBuf);
	}
	return 0;
}


UINT32 CNamePipeClientMgr::Send(UINT16 cmd, UINT8* buf, UINT16 len)
{
	DWORD dwReadSize = 0;
	OVERLAPPED tagOver;
	memset(&tagOver, 0x0, sizeof(tagOver));

	tagOver.hEvent = CreateEvent(NULL,//默认属性  
		TRUE,//手工reset  
		TRUE,//初始状态signaled  
		NULL);//未命名  

	//写  
	DWORD dwHadWrite = 0;
	int nRet = WriteFile(m_hPipe, buf, len, &dwHadWrite, &tagOver);

	if (WaitFinish2(m_hPipe, tagOver) != 0)
		return -1;
	return 0;
}