#pragma once
#include "ByteIOS.h"
#include "Singleton.h"
#include "NamePipeApi.h"

class CBrowserMgr
{
public:
	CBrowserMgr();
	~CBrowserMgr();

	BOOL OpenPV();

private:
	Aux::NameService::CS2CSink m_pSink;
	void OnSink(void* pLaram1, void* pLaram2, void* pLaram3);
};

