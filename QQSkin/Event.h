#pragma once


class IEvent
{
	virtual void DoEvent(void *pParam) = 0;
	virtual void* GetObj() = 0;
	virtual void* GetFunc() = 0;
};

template <class T>
class CEvent : public IEvent
{
	typedef void(T::*Func)(void* pParam);
public:
	CEvent(T* pObj, Func pFunc) : m_pObj(pObj), m_pFn(pFunc)
	{
	}
	~CEvent()
	{
	}
private:
	void DoEvent(void *pParam)
	{
		if (m_pObj && m_pFn)
		{
			(m_pObj->*m_pFn)(pParam);
		}
	}
	virtual void* GetObj()
	{
		return m_pObj;
	}
	virtual void* GetFunc()
	{
		return m_pFn;
	}

private:
	T*    m_pObj;
	Func  m_pFn;
};






