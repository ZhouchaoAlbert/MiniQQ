#pragma once
#include "SinkApi.h"


class BASELIB_API CTimerSink : public Util::Sink::CSinkBase
{
public:
	CTimerSink();
	~CTimerSink();

	void Start(UINT32 uSecond);
	void Stop();

	void IsWork();
};

