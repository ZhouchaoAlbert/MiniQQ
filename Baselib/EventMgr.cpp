#include "EventMgr.h"


CEventMgr::CEventMgr()
{
}


CEventMgr::~CEventMgr()
{
	m_mapEventMgr.clear();
}

void CEventMgr::Register(LPCTSTR lpszName, IEvent* pEvent)
{
	map<CString, list<IEvent*>>::iterator iter = m_mapEventMgr.find(lpszName);
	if (iter != m_mapEventMgr.end())
	{
		for (list<IEvent*>::iterator it = iter->second.begin(); it != iter->second.end();it++)
		{
			if (*it == pEvent)
			{
				return;
			}
		}
	}
	m_mapEventMgr[CString(lpszName)].push_back(pEvent);
}

void CEventMgr::UnRegister(LPCTSTR lpszName, IEvent* pEvent)
{
	map<CString, list<IEvent*>>::iterator iter = m_mapEventMgr.find(lpszName);
	if (iter != m_mapEventMgr.end())
	{
		for (list<IEvent*>::iterator it = iter->second.begin(); it != iter->second.end(); it++)
		{
			if (*it == pEvent)
			{
				m_mapEventMgr[CString(lpszName)].erase(it);;
			}
		}
	}
}

void CEventMgr::OnHookEvent(LPCTSTR lpszName, void*pLaram1, void*pLaram2, void*pLaram3)
{
	map<CString, list<IEvent*>>::iterator iter = m_mapEventMgr.find(lpszName);
	if (iter != m_mapEventMgr.end())
	{
		for (list<IEvent*>::iterator it = iter->second.begin(); it != iter->second.end(); it++)
		{
			if (*it)
			{		
				(*it)->OnEvent(pLaram1, pLaram2, pLaram3);
			}
		}
	}
}