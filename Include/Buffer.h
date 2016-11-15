#pragma once
#include "BaseDefine.h"

namespace Util
{
	 namespace Buf
	{
		class BASELIB_API CBuffer
		{
		public:
			CBuffer();
			~CBuffer();
			CBuffer(const CBuffer& obj);
			CBuffer& operator = (const CBuffer& obj);
			operator UINT8*(){ return m_buf; }
			operator UINT16(){ return m_len; }

			BOOL Append(UINT8*buf, UINT16 len);

			void EraseLeft(UINT16 len);
			void EraseRight(UINT16 len);

			UINT8* GetBuf(){ return m_buf; }
			UINT16 GetLen(){ return m_len; }
			void Empty(){ m_len = 0; }
		private:
			UINT8* m_buf;
			UINT16 m_alloc;
			UINT16 m_len;
			BOOL   m_resize;
		};
	}
}


