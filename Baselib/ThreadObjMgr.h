#pragma once
#include <map>
#include <vector>
using namespace std;
#include "ThreadPoolApi.h"
using namespace Util::ThreadPool;
#include "ThreadObject.h"
#include "KernelObjApi.h"
#include "UtilMsg.h"

enum eThreadType
{
	E_THREAD_NODE    = 0,
	E_THREAD_SUSPEND =1,
	E_THREAD_RESUME  =2,
};
struct ThreadNode
{
	IThreadSink* pIThreadSink;
	IThreadWork* pIThreadWork;
	LPVOID       plParam;
	CThreadObject* pThreadObject;
	eThreadType   eType;
};
struct WorkSendMsg
{
	UINT32 cmd;
	IThreadWork* pIThreadWork;
};

class CThreadObjMgr : Util::Msg::CMsgBase
{
public:
	CThreadObjMgr();
	~CThreadObjMgr();
public:
	UINT32 StartThread(IThreadWork* pIThreadWork, IThreadSink* pIThreadSink, LPVOID plParam);
	void   StopThread(UINT32 uTaskID);

	BOOL   GetAddTask(UINT32& uTaskID, ThreadNode& node, CThreadObject* pThreadObject);
	void   OnTaskFinish(UINT32 uTaskID);

public:
	void WorkThreadSendMsg(UINT32 cmd, IThreadWork* pIThreadWork, LPVOID msg);

	virtual void OnMessage(UINT32 uMsgID, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
protected:
	BOOL HaveTask(IThreadWork* pIThreadWork, IThreadSink* pIThreadSink);	
	BOOL AddWorkTask();
private:
	map<UINT32, ThreadNode>m_mapTaskNode;
	map<UINT32, ThreadNode>m_mapWorkNode;
	vector<ThreadNode> m_vecThreadObj;

	CCriticalSection m_scLock;
	UINT32 m_uTaskID;
};

