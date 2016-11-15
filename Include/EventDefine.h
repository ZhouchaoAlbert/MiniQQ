#pragma once
#include "BaseDefine.h"

class IEvent
{
public:
	virtual ~IEvent(){}
	virtual void OnEvent(void *pParam1, void *pParam2, void *pParam3) = 0;
};

template<class T>
class CEvent : public IEvent
{
public:
	typedef void (T::*Func)(void*, void*, void*);
public:
	CEvent(T *pObj, void (T::*pFunc)(void*, void*, void*)) :
		m_pObj(pObj),
		m_pFunc(pFunc)
	{
	}
	virtual ~CEvent(){}
public:
	void OnEvent(void *pParam1, void *pParam2, void *pParam3)
	{
		if (m_pObj && m_pFunc)
		{
			(m_pObj->*m_pFunc)(pParam1, pParam2, pParam3);
		}
	}
private:
	T *  m_pObj;
	Func m_pFunc;
};

