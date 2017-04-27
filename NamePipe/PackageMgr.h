#pragma once
#include <BaseDefine.h>
#include <SinkApi.h>
#include <ByteIOS.h>
#include <TimerApi.h>
#include <memory>
#include <map>
class CPackageMgr
{
public:
	CPackageMgr();
	~CPackageMgr();

	struct C2SNode
	{
		C2SNode()
		{
			memset(this, 0, sizeof(C2SNode));
		}
		UINT16 cmd;
		UINT64 seq;
		UINT64 sendtime;
		UINT8 retrycount;
		shared_ptr<Util::Buf::CByteIOS> buf;
		Util::Sink::ISink *pSink;
	};

public:
	BOOL Send(UINT16 cmd, UINT8 *buf, UINT16 len, Util::Sink::ISink *pSink);
	void Cancel(Util::Sink::ISink *pSink);
	void OnDataCome();	
private:
	map<UINT64, C2SNode> m_mapRequest;

	CTimerSink m_tmTimeout;
	void OnTimer(void *pParam1, void *pParam2, void *pParam3);
};
