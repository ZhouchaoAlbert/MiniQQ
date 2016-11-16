#pragma once
#include <map>
#include "UtilMsg.h"
#include "BaseSink.h"
using namespace std;

const UINT32 DEFINE_TIME_MSGID = WM_USER + 1005;

class CTimeMgr : Util::Msg::CMsgBase
{
public:
	CTimeMgr();
	~CTimeMgr();

	void Start(UINT32 uSecond,IBaseSink* pSink);
	void Stop(IBaseSink* pSink);
	BOOL IsWork(IBaseSink* pSink);
protected:
	virtual void OnMessage(UINT32 uMsgID, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	UINT32 GetTimerID();
	map<UINT32, IBaseSink*>m_mapForward;
	map<IBaseSink*, UINT32>m_mapInverte;
};

