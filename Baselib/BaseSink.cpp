#include "BaseSink.h"


CBaseSink::CBaseSink()
{

}


CBaseSink::~CBaseSink()
{

}




void CBaseSink::UnRegister()
{
	if (m_pEvent)
	{
		delete m_pEvent;
		m_pEvent = NULL;
	}
}


void CBaseSink::OnSink(void* pLaram1, void* pLaram2, void* pLaram3)
{
	if (m_pEvent)
	{
		m_pEvent->OnEvent(pLaram1, pLaram2, pLaram3);
	}
}