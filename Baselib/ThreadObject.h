#pragma once
#include "BaseDefine.h"
//创建线程 处理工作任务

class CThreadObject
{
public:
	CThreadObject();
	~CThreadObject();

	void Start();
	void Stop();


protected:
	static UINT32 WINAPI _ThreadFunc(LPVOID obj);
	void ThreadFunc();
private:

	HANDLE m_handle;
	UINT32 m_uTaskID;
	BOOL   m_bExit;
};

