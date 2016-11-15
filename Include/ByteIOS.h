#pragma once
#include "BaseDefine.h"
namespace Util
{
	namespace Buf
	{
		class BASELIB_API CByteIOS
		{
		public:
			CByteIOS();
			~CByteIOS();

			CByteIOS(const CByteIOS& obj);
			CByteIOS& operator = (const CByteIOS& obj);
			operator UINT8*(){ return m_buf; }
			operator UINT16(){ return m_len; }

			//写操作
			template < class T >
			CByteIOS& operator << (const T& inValue)
			{
				UINT16 len = sizeof(T);
				return Write((UINT8*)&inValue, len);
			}
			CByteIOS& operator << (LPCSTR inValue)
			{
				UINT16 len = (strlen(inValue) + 1) * sizeof(CHAR);
				return Write((UINT8*)inValue, len);
			}
			CByteIOS& operator << (LPCWSTR inValue)
			{
				UINT16 len = (wcslen(inValue) + 1)* sizeof(WCHAR);
				return Write((UINT8*)inValue, len);
			}
			CByteIOS& Write(UINT8* buf, UINT16 len);

			//读操作：
			template<class T>
			CByteIOS& operator >> (T& outValue)
			{
				UINT16 len = sizeof(T);
				return Read((UINT8*)&outValue, len);
			}
			CByteIOS& operator >> (CStringA& outValue);
			CByteIOS& operator >> (CStringW& outValue);

			CByteIOS& Read(UINT8* buf, UINT16& len);

			//设置操作：
			void Empty();
			void Reset();
			void SetRPos(UINT16 pos);
			BOOL Set(UINT16 pos, UINT8 *buf, UINT16 len);

			//获取操作：
			UINT8* GetBuf(){ return m_buf; }
			UINT16 GetLen(){ return m_len; }
			UINT16 GetRPos(){ return m_rpos; }
			UINT16 GetWPos(){ return m_wpos; }


		private:
			UINT8* m_buf;
			UINT16 m_alloc;
			UINT16 m_len;
			UINT16 m_wpos;
			UINT16 m_rpos;
		};

	}
}

