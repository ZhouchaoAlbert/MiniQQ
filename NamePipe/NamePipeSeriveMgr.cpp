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
		//����������  
	case ERROR_IO_PENDING:
		bPendingIO = true;
		break;
		//�Ѿ�����  
	case ERROR_PIPE_CONNECTED:
		SetEvent(tagOver.hEvent);
		break;
	}

	DWORD dwWait = -1;
	DWORD dwTransBytes = -1;

	//�ȴ���д�������  
	dwWait = WaitForSingleObject(tagOver.hEvent, INFINITE);
	switch (dwWait)
	{
	case 0:
		if (bPendingIO)
		{
			//��ȡOverlapped���  
			if (GetOverlappedResult(hPipe, &tagOver, &dwTransBytes, FALSE) == FALSE)
			{
				printf("ConnectNamedPipe  failed   %d\n", GetLastError());
				return -1;
			}
		}
		break;
		//  ��д���  
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
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED�ص�IO  
			//PIPE_TYPE_BYTE|PIPE_READMODE_BYTE,  
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,//����������������  
			0, 0, 1000, NULL);
		if (m_hPipe == INVALID_HANDLE_VALUE)
		{
			//printf("CreateNamedPipe  failed!\n");
			m_hPipe = NULL;
			return -1;
		}

		//printf("������ͨ������\nCreateNamedPipe  Success!\n");

		INT32 nRet = 0;

		OVERLAPPED tagOver;
		memset(&tagOver, 0x0, sizeof(tagOver));

		tagOver.hEvent = CreateEvent(NULL,//Ĭ������  
			TRUE,//�ֹ�reset  
			TRUE,//��ʼ״̬signaled  
			NULL);//δ����  

		nRet = ConnectNamedPipe(m_hPipe, &tagOver);
		if (nRet)
		{
			//printf("ConnectNamedPipe failed!  %d\n", GetLastError());
			return -2;
		}

		if (WaitFinish(m_hPipe, tagOver) != 0)
			return -1;

		//��  
		
		HANDLE hHandle = (HANDLE)_beginthreadex(0, 0, &WorkThreadProc, m_hPipe, 0, 0);
		CloseHandle(hHandle);
/*	}*/

	return 0;
}


//���������ݽӿ�
UINT32 CNamePipeSeriveMgr::Send(UINT16 cmd, UINT8* buf, UINT16 len)
{
	DWORD dwReadSize = 0;
	OVERLAPPED tagOver;
	memset(&tagOver, 0x0, sizeof(tagOver));

	tagOver.hEvent = CreateEvent(NULL,//Ĭ������  
		TRUE,//�ֹ�reset  
		TRUE,//��ʼ״̬signaled  
		NULL);//δ����  

	//д  
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

	tagOver.hEvent = CreateEvent(NULL,//Ĭ������  
		TRUE,//�ֹ�reset  
		TRUE,//��ʼ״̬signaled  
		NULL);//δ����  

	while (1)
	{
		INT32 nRet = ReadFile(hPipe, &szReadBuf, sizeof(szReadBuf), &dwReadSize, &tagOver);	//��  
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