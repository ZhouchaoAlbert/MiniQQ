#pragma once
#include <map>
#include "UtilMsg.h"
#include "HookEvent.h"
using namespace std;
using namespace Util::Event;

const UINT32 DEFINE_TIME_MSGID = WM_USER + 1005;

class CTimeMgr : Util::Msg::CMsgBase
{
public:
	CTimeMgr();
	~CTimeMgr();

	void Start(UINT32 uSecond,IHookEvent* pSink);
	void Stop(IHookEvent* pSink);
	BOOL IsWork(IHookEvent* pSink);
protected:
	virtual void OnMessage(UINT32 uMsgID, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	UINT32 GetTimerID();
	map<UINT32, IHookEvent*>m_mapForward;
	map<IHookEvent*, UINT32>m_mapInverte;
};

