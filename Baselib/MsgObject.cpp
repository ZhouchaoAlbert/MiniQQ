#include "MsgObject.h"
#include "Singleton.h"
#include "MsgObjectMgr.h"

CMsgObject::CMsgObject():
m_bStop(FALSE)
{
	m_hWnd = Create(NULL);
	ATLASSERT(m_hWnd);
}


CMsgObject::~CMsgObject()
{

}

void   CMsgObject::Start()
{
	m_bStop = TRUE;
}

void   CMsgObject::Stop()
{
	m_bStop = FALSE;
}

HWND CMsgObject::GetMsgWnd()
{
	return m_hWnd;
}

void CMsgObject::AddMsg(UINT32 uMsgID)
{
	m_mapMsgID[uMsgID] = uMsgID;
}

void CMsgObject::DeleteMsg(UINT32 uMsgID)
{
	m_mapMsgID.erase(uMsgID);
}


void CMsgObject::Destroy()
{
	Stop();
	::DestroyWindow(m_hWnd);
}

BOOL CMsgObject::ProcessWindowMessage(
	_In_ HWND hWnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam,
	_Inout_ LRESULT& lResult,
	_In_ DWORD dwMsgMapID)
{
	BOOL bHandled = FALSE;
	map<UINT32, UINT32>::iterator iter = m_mapMsgID.find(uMsg);

	if (uMsg == WM_TIMER)
	{
		UINT32 i = 0;
	}
	if (m_bStop && iter != m_mapMsgID.end())  //通知接受的管理类
	{
		Singleton<CMsgObjectMgr>::Instance().OnMessage(this, uMsg, wParam, lParam, bHandled);
	}
	return bHandled;
}