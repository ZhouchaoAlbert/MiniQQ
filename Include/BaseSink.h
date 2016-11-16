#pragma once

#include "EventDefine.h"

class IBaseSink
{
public:
	virtual void OnSink(void* pLaram1, void* pLaram2, void* pLaram3) = 0;
};


class BASELIB_API CBaseSink : public IBaseSink
{
public:
	CBaseSink();
	~CBaseSink();
	template<class T>
	void Register(LPCTSTR lpszName, T* obj, void(T::*Func)(void* pLaram1, void* pLaram2, void* pLaram3))
	{
		m_pEvent = new CEvent<T>(obj, Func);
		strcpy_s(m_Name, MAX_PATH, lpszName);
	}
	
	void UnRegister();

	virtual void OnSink(void* pLaram1, void* pLaram2, void* pLaram3);
private:
	TCHAR   m_Name[MAX_PATH];
	IEvent* m_pEvent;
};

