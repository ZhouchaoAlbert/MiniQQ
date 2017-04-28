#include "stdafx.h"
#include "NamePipeSeriveMgr.h"
#include <process.h>  
#include <string>
#include "NamePipeHelper.h"
using namespace std;
#include "PipeBuffer.h"
#include "Singleton.h"
#include "PackageMgr.h"

int WaitFinish(HANDLE hPipe, OVERLAPPED &tagOver)
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


CNamePipeSeriveMgr::CNamePipeSeriveMgr():
m_hPipe(NULL)
{

}


CNamePipeSeriveMgr::~CNamePipeSeriveMgr()
{
	if (DisconnectNamedPipe(m_hPipe) == 0)
	{
	   //printf("DisconnectNamedPipe  error!\n");
		return;
	}
	CloseHandle(m_hPipe);
}


UINT32 CNamePipeSeriveMgr::Create(LPCSTR lpszName)
{

// 	while (1)
// 	{
		m_hPipe = CreateNamedPipeA((LPCSTR)lpszName,
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED重叠IO  
			//PIPE_TYPE_BYTE|PIPE_READMODE_BYTE,  
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,//不对数量进行限制  
			0, 0, 1000, NULL);
		if (m_hPipe == INVALID_HANDLE_VALUE)
		{
			//printf("CreateNamedPipe  failed!\n");
			m_hPipe = NULL;
			return -1;
		}

		//printf("服务器通道启动\nCreateNamedPipe  Success!\n");

		INT32 nRet = 0;

		OVERLAPPED tagOver;
		memset(&tagOver, 0x0, sizeof(tagOver));

		tagOver.hEvent = CreateEvent(NULL,//默认属性  
			TRUE,//手工reset  
			TRUE,//初始状态signaled  
			NULL);//未命名  

		nRet = ConnectNamedPipe(m_hPipe, &tagOver);
		if (nRet)
		{
			//printf("ConnectNamedPipe failed!  %d\n", GetLastError());
			return -2;
		}

		if (WaitFinish(m_hPipe, tagOver) != 0)
			return -1;

		//读  
		
		HANDLE hHandle = (HANDLE)_beginthreadex(0, 0, &WorkThreadProc, m_hPipe, 0, 0);
		CloseHandle(hHandle);
/*	}*/

	return 0;
}


//服务发送数据接口
UINT32 CNamePipeSeriveMgr::Send(UINT16 cmd, UINT8* buf, UINT16 len)
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

	if (WaitFinish(m_hPipe, tagOver) != 0)
		return -1;
	return 0;
}





UINT32 WINAPI CNamePipeSeriveMgr::WorkThreadProc(LPVOID pv)
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
		INT32 nRet = ReadFile(hPipe, &szReadBuf, sizeof(szReadBuf), &dwReadSize, &tagOver);	//读  
		if (nRet && dwReadSize == 0)
		{
			CloseHandle(hPipe);
			break;
		}
		if (WaitFinish(hPipe, tagOver) != 0)
			return -1;
		if (tagOver.InternalHigh)
		{
			Singleton<CPipeBuffer>::Instance().Append((UINT8*)&szReadBuf, tagOver.InternalHigh);

			Singleton<CPackageMgr>::Instance().OnDataCome();
		}
		

		//string strBuf = szReadBuf;
		//printf("Client Msg: %s\n", strBuf.c_str());
	}
	return 0;
}