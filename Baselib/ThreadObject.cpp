#include "ThreadObject.h"
#include "Singleton.h"
#include "ThreadObjMgr.h"

CThreadObject::CThreadObject():
m_handle(NULL),
m_uTaskID(0),
m_bExit(TRUE)
{
	m_handle = (HANDLE)_beginthreadex(NULL, 0, _ThreadFunc, this, CREATE_SUSPENDED, &m_uTaskID);
}


CThreadObject::~CThreadObject()
{


}

void CThreadObject::Start()
{
	m_bExit = FALSE;
	::ResumeThread(m_handle);
}

void CThreadObject::Stop()
{
    if (m_uTaskID)
    {
		m_bExit = TRUE;
		_endthreadex(m_uTaskID);
		m_uTaskID = 0;
    }
}


UINT32 CThreadObject::_ThreadFunc(LPVOID obj)
{
	if (obj)
	{
		((CThreadObject*)obj)->ThreadFunc();
	}

	return 0;
}

void CThreadObject::ThreadFunc()
{
	while (!m_bExit)
	{
		UINT32 uTaskID = 0;
		ThreadNode node;
		if (Singleton<CThreadObjMgr>::Instance().GetAddTask(uTaskID, node,this))
		{
			//执行工作任务
			node.pIThreadWork->DoWorkTask(node.plParam);
			//工作结束
			Singleton<CThreadObjMgr>::Instance().OnTaskFinish(uTaskID);
		}
		else
		{
			::SuspendThread(m_handle);
		}

	}
}