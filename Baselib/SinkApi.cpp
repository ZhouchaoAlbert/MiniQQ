#include <SinkApi.h>


Util::Sink::CSinkBase::CSinkBase():
m_pEvent(NULL)
{
	memset(m_strSinkName, 0, sizeof(TCHAR)* MAX_PATH);
}

Util::Sink::CSinkBase::~CSinkBase()
{
	UnRegister();
}

void Util::Sink::CSinkBase::OnSink(void *pParam1, void *pParam2, void *pParam3)
{
	if (m_pEvent)
	{
		m_pEvent->OnEvent(pParam1, pParam2, pParam3);
	}
}

void Util::Sink::CSinkBase::UnRegister()
{
	if (m_pEvent)
	{
		delete m_pEvent;
		m_pEvent = NULL;
	}
}