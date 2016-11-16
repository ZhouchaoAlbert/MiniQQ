#include "ThreadObjMgr.h"


const UINT32 INIT_THREADOBJ_MAX = 5;
const UINT32 DEFINE_THREAD_MSGID = WM_USER + 11111;
CThreadObjMgr::CThreadObjMgr():
m_uTaskID(0)
{
	AddMsg(DEFINE_THREAD_MSGID);
	//首先启动5个挂起的线程
	for (UINT32 uIndex = 0; uIndex < INIT_THREADOBJ_MAX;uIndex++)
	{
		CThreadObject* pThreadObj = new CThreadObject;
		if (pThreadObj)
		{
			ThreadNode node;
			node.eType = E_THREAD_SUSPEND;
			node.pThreadObject = pThreadObj;
			m_vecThreadObj.push_back(node);
		}
	}
}


CThreadObjMgr::~CThreadObjMgr()
{
	DeleteMsg(DEFINE_THREAD_MSGID);
}


UINT32 CThreadObjMgr::StartThread(IThreadWork* pIThreadWork, IThreadSink* pIThreadSink, LPVOID plParam)
{
	//1.有效性判断
	ATLASSERT(pIThreadWork);
	ATLASSERT(pIThreadSink);
	if (NULL == pIThreadWork || NULL == pIThreadSink)
	{
		ATLASSERT(FALSE);
		return 0;
	}
	//2.加锁
	Util::KObj::CLock lock(&m_scLock, TRUE, TRUE);
	//3.是否重复
	if (!HaveTask(pIThreadWork,pIThreadSink))
	{
		return 0;
	}
	//4.保存
	m_uTaskID++;
	ThreadNode node;
	node.pIThreadSink = pIThreadSink;
	node.pIThreadWork = pIThreadWork;
	node.plParam = plParam;
	node.pThreadObject = NULL;
	m_mapTaskNode[m_uTaskID] = node;

	//5.添加到工作线程
	if (AddWorkTask())
	{
		return m_uTaskID;
	}
	return 0;
}

void   CThreadObjMgr::StopThread(UINT32 uTaskID)
{
	map<UINT32, ThreadNode>::iterator iter = m_mapWorkNode.find(uTaskID); 
	if (iter != m_mapWorkNode.end())
	{
		iter->second.pThreadObject->Stop();
	}
}


BOOL CThreadObjMgr::HaveTask(IThreadWork* pIThreadWork, IThreadSink* pIThreadSink)
{
	BOOL bExsit = FALSE;
	for (map<UINT32, ThreadNode>::iterator iter = m_mapTaskNode.begin(); iter != m_mapTaskNode.end(); iter++)
	{
		if (iter->second.pIThreadWork == pIThreadWork && iter->second.pIThreadSink == pIThreadSink)
		{
			bExsit = TRUE;
			break;
		}
	}
	return bExsit;
}

BOOL   CThreadObjMgr::AddWorkTask()
{
	BOOL bRet = FALSE;
	for (vector<ThreadNode>::iterator iter = m_vecThreadObj.begin(); iter != m_vecThreadObj.end();iter++)
	{
		if (iter->eType == E_THREAD_SUSPEND)
		{
			iter->pThreadObject->Start();	
			iter->eType = E_THREAD_RESUME;
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}


BOOL   CThreadObjMgr::GetAddTask(UINT32& uTaskID, ThreadNode& node,CThreadObject* pThreadObject)
{
	Util::KObj::CLock lock(&m_scLock, TRUE, TRUE);
	map<UINT32, ThreadNode>mapTmpNode = m_mapTaskNode;
	BOOL bRet = FALSE;
	for (map<UINT32, ThreadNode>::iterator iter = mapTmpNode.begin(); iter != m_mapTaskNode.end();iter++)
	{
		uTaskID = iter->first;
		node    = iter->second;
		m_mapWorkNode[iter->first] = node;
		m_mapWorkNode[iter->first].pThreadObject = pThreadObject;
		m_mapTaskNode.erase(iter);
		bRet = TRUE;
		break;
	}
	return bRet;
}

void   CThreadObjMgr::OnTaskFinish(UINT32 uTaskID)
{
	//加锁
	Util::KObj::CLock lock(&m_scLock, TRUE, TRUE);
	map<UINT32, ThreadNode>::iterator iter = m_mapWorkNode.find(uTaskID);
	if (iter == m_mapWorkNode.end())
	{
		ATLASSERT(FALSE);
		return;
	}
	BOOL bFind = FALSE;
	for (vector < ThreadNode >::iterator it = m_vecThreadObj.begin(); it != m_vecThreadObj.end(); iter++)
	{
		if (iter->second.pThreadObject == it->pThreadObject)
		{
			iter->second.pIThreadSink->OnTaskFinish(uTaskID);
			
			bFind     = TRUE;
			it->eType = E_THREAD_SUSPEND;
			
			//把任务线程挂起
			break;
		}
		
	}
	if (bFind)
	{
		m_mapWorkNode.erase(iter);
	}
	else
	{
		ATLASSERT(FALSE);
	}
}


void CThreadObjMgr::WorkThreadSendMsg(UINT32 cmd, IThreadWork* pIThreadWork, LPVOID msg)
{
	WorkSendMsg* pWorkSendMsg = new WorkSendMsg;
	pWorkSendMsg->cmd = cmd;
	pWorkSendMsg->pIThreadWork = pIThreadWork;
	::PostMessage(GetMsgWnd(), DEFINE_THREAD_MSGID, (WPARAM)pWorkSendMsg, (LPARAM)msg);
}

void CThreadObjMgr::OnMessage(UINT32 uMsgID, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsgID != DEFINE_THREAD_MSGID)
	{
		ATLASSERT(FALSE);
		return;
	}

	WorkSendMsg* pWorkSendMsg = (WorkSendMsg*)wParam;
	LPVOID pMsg = (LPVOID)lParam;
	Util::KObj::CLock lock(&m_scLock, TRUE, TRUE);
	for (map<UINT32, ThreadNode>::iterator iter = m_mapWorkNode.begin(); iter != m_mapWorkNode.end(); iter++)
	{
		if (iter->second.pIThreadWork == pWorkSendMsg->pIThreadWork)
		{
			iter->second.pIThreadSink->OnRevMsg(pWorkSendMsg->cmd, iter->first, pMsg);
			break;
		}
	}
	delete pWorkSendMsg;
	pWorkSendMsg = NULL;
}