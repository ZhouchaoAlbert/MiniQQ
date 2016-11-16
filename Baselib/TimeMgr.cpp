#include "TimeMgr.h"


CTimeMgr::CTimeMgr()
{
	AddMsg(DEFINE_TIME_MSGID);
}


CTimeMgr::~CTimeMgr()
{
	DeleteMsg(DEFINE_TIME_MSGID);

	map<UINT32, IBaseSink*>m_mapTmp = m_mapForward;
	m_mapForward.clear();
	m_mapInverte.clear();
	for (map<UINT32, IBaseSink*>::iterator iter = m_mapTmp.begin(); iter != m_mapTmp.end(); iter++)
	{
		KillTimer(GetMsgWnd(), iter->first);
	}
}


void CTimeMgr::Start(UINT32 uSecond, IBaseSink* pSink)
{
	if (m_mapInverte.find(pSink) !=m_mapInverte.end())
	{
		return;
	}
	UINT32 uTimerID = GetTimerID();
	if (0 == uTimerID)
	{
		return;
	}
	m_mapForward[uTimerID] = pSink;
	m_mapInverte[pSink]    = uTimerID;

	::SetTimer(GetMsgWnd(), uTimerID, uSecond, NULL);
}


void CTimeMgr::Stop(IBaseSink* pSink)
{
	map<IBaseSink*, UINT32>::iterator iter = m_mapInverte.find(pSink);
	if (iter == m_mapInverte.end())
	{
		return;
	}
	map<UINT32, IBaseSink*>::iterator iter2 = m_mapForward.find(iter->second);
	if (iter2 == m_mapForward.end())
	{
		m_mapInverte.erase(iter);
		return;
	}
	if (iter->second != iter2->first)
	{
		ATLASSERT(FALSE);
		return;
	}
	UINT32 uTimerID = iter->second;
	m_mapInverte.erase(iter);
	m_mapForward.erase(iter2);
	KillTimer(GetMsgWnd(), uTimerID);
}


void CTimeMgr::OnMessage(UINT32 uMsgID, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsgID != DEFINE_TIME_MSGID)
	{
		ATLASSERT(FALSE);
	}
	UINT32 uTimerID = (UINT32)wParam;
	map<UINT32, IBaseSink*>::iterator iter = m_mapForward.find(uTimerID);
	if (iter != m_mapForward.end() && NULL != iter->second)
	{
		iter->second->OnSink(NULL, NULL, NULL);
		bHandled = TRUE;
	}
}

BOOL CTimeMgr::IsWork(IBaseSink* pSink)
{
	map<IBaseSink*, UINT32>::iterator iter = m_mapInverte.find(pSink);
	if (iter == m_mapInverte.end())
	{
		return TRUE;
	}
	return FALSE;
}

UINT32 CTimeMgr::GetTimerID()
{
	for (UINT32 uIndex = 1; uIndex < (UINT32)-1;uIndex++)
	{
		if (m_mapForward.find(uIndex) == m_mapForward.end())
		{
			return uIndex;
		}
	}
	return 0;
}