#pragma once
#include "BaseSink.h"


class BASELIB_API CTimerSink  :  public CBaseSink
{
public:
	CTimerSink();
	~CTimerSink();

	void Start(UINT32 uSecond);
	void Stop();

	void IsWork();
};

