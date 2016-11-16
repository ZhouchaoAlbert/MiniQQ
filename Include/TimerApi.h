#pragma once
#include "HookEvent.h"
using namespace Util::Event;

class BASELIB_API CTimerSink  :  public CHookEvent
{
public:
	CTimerSink();
	~CTimerSink();

	void Start(UINT32 uSecond);
	void Stop();

	void IsWork();
};

