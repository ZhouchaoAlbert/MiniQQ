#pragma once
#include <map>
#include "UtilMsg.h"
#include "SinkApi.h"
using namespace std;


const UINT32 DEFINE_TIME_MSGID = WM_USER + 1005;

class CTimeMgr : Util::Msg::CMsgBase
{
public:
	CTimeMgr();
	~CTimeMgr();

	void Start(UINT32 uSecond, Util::Sink::ISink* pSink);
	void Stop(Util::Sink::ISink* pSink);
	BOOL IsWork(Util::Sink::ISink* pSink);
protected:
	virtual void OnMessage(UINT32 uMsgID, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	UINT32 GetTimerID();
	map<UINT32, Util::Sink::ISink*>m_mapForward;
	map<Util::Sink::ISink*, UINT32>m_mapInverte;
};

