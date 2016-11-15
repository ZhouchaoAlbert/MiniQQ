#pragma once
#include "BaseDefine.h"

namespace Util
{
	namespace Msg
	{
		class BASELIB_API CMsgBase
		{
		public:
			CMsgBase();
			~CMsgBase();
		public:
			void Start();
			void Stop();
			void AddMsg(UINT32 uMsgID);
			void DeleteMsg(UINT32 uMsgID);
			HWND GetMsgWnd();

			virtual void OnMessage(UINT32 uMsgID, WPARAM wParam, LPARAM lParam, BOOL& bHandled) = 0;
		};
	}
}

