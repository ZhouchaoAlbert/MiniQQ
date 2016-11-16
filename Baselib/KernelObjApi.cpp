#include "KernelObjApi.h"



Util::KObj::CLock::CLock():
m_bAttach(FALSE),
m_bAutoLock(FALSE),
m_pCS(NULL)
{
	m_pCS = new CCriticalSection;
}

Util::KObj::CLock::~CLock()
{
	if (m_pCS)
	{
		if (m_bAutoLock)
		{
			m_pCS->Leave();
		}
	    
		if (m_bAttach)
		{
			delete m_pCS;
			m_pCS = NULL;
		}
	}
	if (m_pCS)
	{
		delete m_pCS;
		m_pCS = NULL;
	}
}


Util::KObj::CLock::CLock(CCriticalSection* pCS, BOOL bAttach/* = FALSE*/, BOOL bAutoLock/*= FALSE*/):
m_bAttach(bAttach),
m_bAutoLock(bAutoLock),
m_pCS(pCS)
{
	if (m_pCS && m_bAutoLock)
	{
		Lock();
	}
}

void Util::KObj::CLock::Attach(CCriticalSection* pCS, BOOL bAttach/* = FALSE*/, BOOL bAutoLock /*= FALSE*/)
{
	if (m_pCS)
	{
		if (m_bAutoLock)
		{
			m_pCS->Leave();
		}
		if (m_bAttach)
		{
			delete m_pCS;
			m_pCS = NULL;
		}
	}
	if (m_pCS)
	{
		delete m_pCS;
		m_pCS = NULL;
	}

	m_pCS       = pCS;
	m_bAttach   = bAttach;
	m_bAutoLock = bAutoLock;
	if (m_bAutoLock && m_pCS)
	{
		Lock();
	}
}

void Util::KObj::CLock::Detach()
{
	m_bAttach = FALSE;
	m_bAutoLock = FALSE;
	m_pCS = NULL;
}

void Util::KObj::CLock::Lock()
{
	if (m_pCS)
	{
		m_pCS->Enter();
	}
}

void Util::KObj::CLock::UnLock()
{
	if (m_pCS)
	{
		m_pCS->Leave();
	}
}