#pragma once

#include <EventDefine.h>


namespace Util
{
	namespace Sink
	{
		class ISink
		{
		public:
			virtual void OnSink(void *pParam1, void *pParam2, void *pParam3) = 0;
		};

		class BASELIB_API CSinkBase : public ISink
		{
		public:
			CSinkBase();
			~CSinkBase();
		public:
			template<class T>
			void Register(LPCTSTR lpSinkName, T *obj, void (T::*func)(void *, void *, void *))
			{
				m_pEvent = new CEvent<T>(obj, func);
				_tcscpy_s(m_strSinkName, MAX_PATH, lpSinkName);
			}
			virtual void UnRegister();
			virtual void OnSink(void *pParam1, void *pParam2, void *pParam3);
		private:
			IEvent *m_pEvent;
			TCHAR m_strSinkName[MAX_PATH];
		};
	}
}
