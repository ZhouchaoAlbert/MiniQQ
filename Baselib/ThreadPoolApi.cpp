#include "ThreadPoolApi.h"
#include "ThreadObjMgr.h"
#include "Singleton.h"
void Util::ThreadPool::IThreadWork::SendMsg(UINT32 cmd, LPVOID msg)
{
	Singleton<CThreadObjMgr>::Instance().WorkThreadSendMsg(cmd,this, msg);
}

UINT32 Util::ThreadPool::StartThread(IThreadWork* pIThreadWork, IThreadSink* pIThreadSink, LPVOID lParam)
{
	return Singleton<CThreadObjMgr>::Instance().StartThread(pIThreadWork, pIThreadSink, lParam);
}

void   Util::ThreadPool::StopThread(UINT32 uTaskID)
{
	Singleton<CThreadObjMgr>::Instance().StopThread(uTaskID);
}