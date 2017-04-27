#pragma once
#include "BaseDefine.h"
#include "EventDefine.h"

namespace Util
{
	namespace Event
	{
		class IHookEvent
		{
		public:
			virtual void OnHookEvent(void* pLaram1, void* pLaram2, void* pLaram3) = 0;
		};

		class BASELIB_API CHookEvent : public IHookEvent
		{
		public:
			CHookEvent();
			~CHookEvent();

			template<class T>
			void HookEvent(LPCTSTR lpszName, T* obj, void(T::*Func)(void*, void*, void*))
			{
				if (NULL != m_pEvent)
				{
					ATLASSERT(FALSE);
					return;
				}
				m_pEvent = new CEvent<T>(obj, Func);
				strcpy_s(m_Name, MAX_PATH, lpszName);

				//管理类做登记
				Register(lpszName, m_pEvent);
			}

			virtual void OnHookEvent(void* pLaram1, void* pLaram2, void* pLaram3);

		protected:
			void   Register(LPCTSTR lpszName, IEvent * pEvent);
			void   UnRegister(LPCTSTR lpszName, IEvent* pEvent);
		private:
			TCHAR   m_Name[MAX_PATH];
			IEvent* m_pEvent;
		};

		BASELIB_API void BrodcaseEvent(LPCTSTR lpazName, void*pLaram1, void*pLaram2, void*pLaram3);
		BASELIB_API void DoExit();

	}
}
