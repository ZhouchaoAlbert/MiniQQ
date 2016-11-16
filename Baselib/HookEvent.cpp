#include "HookEvent.h"
#include "EventMgr.h"
#include "Singleton.h"
Util::Event::CHookEvent::CHookEvent():
m_pEvent(NULL)
{
}


Util::Event::CHookEvent::~CHookEvent()
{
	if (m_pEvent)
	{
		UnRegister(m_Name, m_pEvent);
		delete m_pEvent;
		m_pEvent = NULL;
	}
}


void Util::Event::CHookEvent::OnHookEvent(void* pLaram1, void* pLaram2, void* pLaram3)
{
	if (m_pEvent)
	{
		m_pEvent->OnEvent(pLaram1, pLaram2, pLaram3);
	}
}

void Util::Event::CHookEvent::Register(LPCTSTR lpszName, IEvent * pEvent)
{
	Singleton<CEventMgr>::Instance().Register(lpszName, pEvent);
}

void Util::Event::CHookEvent::UnRegister(LPCTSTR lpszName, IEvent* pEvent)
{
	Singleton<CEventMgr>::Instance().UnRegister(lpszName, pEvent);
}


void Util::Event::BrodcaseEvent(LPCTSTR lpazName, void*pLaram1, void*pLaram2, void*pLaram3)
{
	Singleton<CEventMgr>::Instance().OnHookEvent(lpazName, pLaram1,  pLaram2, pLaram3);
}

void Util::Event::DoExit()
{
	Singleton<CEventMgr>::UnInstance();
}