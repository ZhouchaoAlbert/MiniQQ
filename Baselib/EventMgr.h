#pragma once
#include "BaseDefine.h"
#include "EventDefine.h"
#include <map>
using namespace std;
#include <list>

class CEventMgr
{
public:
	CEventMgr();
	~CEventMgr();

	void Register(LPCTSTR lpszName, IEvent* pEvent);
	void UnRegister(LPCTSTR lpszName, IEvent* pEvent);
	void OnHookEvent(LPCTSTR lpszName, void*pLaram1, void*pLaram2, void*pLaram3);
private:
	map<CString, list<IEvent*>> m_mapEventMgr;
};

