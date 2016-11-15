#include "MsgObjectMgr.h"


CMsgObjectMgr::CMsgObjectMgr()
{
}


CMsgObjectMgr::~CMsgObjectMgr()
{
}


void CMsgObjectMgr::Register(CMsgBase* pMsgBase)
{
	map<CMsgBase*, CMsgObject*>::iterator iter = m_mapMsgObject.find(pMsgBase);
	if (iter == m_mapMsgObject.end())
	{
		CMsgObject* pMsgObject = new CMsgObject;
		ATLASSERT(pMsgObject);
		m_mapMsgObject[pMsgBase] = pMsgObject;
	}
}

void CMsgObjectMgr::UnRegister(CMsgBase* pMsgBase)
{
	map<CMsgBase*, CMsgObject*>::iterator iter = m_mapMsgObject.find(pMsgBase);
	if (iter != m_mapMsgObject.end())
	{
		CMsgObject* pMsgObject = iter->second;
		m_mapMsgObject.erase(iter);
		pMsgObject->Destroy();
	}
}

void CMsgObjectMgr::AddMsg(CMsgBase* pMsgBase, UINT32 uMsgID)
{
	CMsgObject* pMsgObject = FindMsgBase(pMsgBase);
	if (pMsgObject)
	{
		pMsgObject->AddMsg(uMsgID);
	}
}

void CMsgObjectMgr::DeleteMsg(CMsgBase* pMsgBase, UINT32 uMsgID)
{
	CMsgObject* pMsgObject = FindMsgBase(pMsgBase);
	if (pMsgObject)
	{
		pMsgObject->DeleteMsg(uMsgID);
	}
}

void CMsgObjectMgr::Start(CMsgBase* pMsgBase)
{
	CMsgObject* pMsgObject = FindMsgBase(pMsgBase);
	if (pMsgObject)
	{
		pMsgObject->Start();
	}
}

void CMsgObjectMgr::Stop(CMsgBase* pMsgBase)
{
	CMsgObject* pMsgObject = FindMsgBase(pMsgBase);
	if (pMsgObject)
	{
		pMsgObject->Stop();
	}
}

HWND CMsgObjectMgr::GetMsgWnd(CMsgBase* pMsgBase)
{
	CMsgObject* pMsgObject = FindMsgBase(pMsgBase);
	if (pMsgObject)
	{
		return pMsgObject->GetMsgWnd();
	}
	return NULL;
}

CMsgObject* CMsgObjectMgr::FindMsgBase(CMsgBase* pMsgBase)
{
	map<CMsgBase*, CMsgObject*>::iterator iter = m_mapMsgObject.find(pMsgBase);
	if (iter != m_mapMsgObject.end())
	{
		return  iter->second;
	}
	return NULL;
}



void CMsgObjectMgr::OnMessage(CMsgObject* pMsgObject, UINT32 uMsgID, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	for (map<CMsgBase*, CMsgObject*>::iterator iter = m_mapMsgObject.begin(); iter != m_mapMsgObject.end();iter++)
	{
		if (iter->second == pMsgObject)
		{
			CMsgBase* pMsgBase = iter->first;
			pMsgBase->OnMessage(uMsgID, wParam, lParam, bHandled);
			break;
		}
	}
}