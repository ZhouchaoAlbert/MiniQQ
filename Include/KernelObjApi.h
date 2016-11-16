#pragma once
#include "BaseDefine.h"
#include <atlsync.h>
namespace Util
{
	namespace KObj
	{
		class BASELIB_API CLock
		{
		public:
			CLock();
			~CLock();
			CLock(CCriticalSection* pCS, BOOL bAttach = FALSE, BOOL bAutoLock = FALSE);
			void Lock();
			void UnLock();
			void Attach(CCriticalSection* pCS, BOOL bAttach = FALSE, BOOL bAutoLock = FALSE);
			void Detach();
		private:
			CCriticalSection* m_pCS;
			BOOL m_bAttach;
			BOOL m_bAutoLock; 
		};
	}
}
