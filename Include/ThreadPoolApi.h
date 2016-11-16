#pragma once
#include "BaseDefine.h"

namespace Util
{
	namespace ThreadPool
	{
		//�����߳������߳�Ͷ����Ϣ�ǻ��ڴ���

		//�����߳�
		class IThreadWork
		{
		public:
			virtual void DoWorkTask(LPVOID plParam) = 0;
			void SendMsg(UINT32 cmd, LPVOID msg);
	
		};

		//���߳�
		class IThreadSink
		{
		public:
			virtual void  OnTaskFinish(UINT32 uTaskID) = 0;
			virtual void  OnRevMsg(UINT32 cmd, UINT32 uTaskID,LPVOID msg){};
		};

		BASELIB_API UINT32 StartThread(IThreadWork* pIThreadWork, IThreadSink* pIThreadSink, LPVOID lParam);
		BASELIB_API void   StopThread(UINT32 uTaskID);

	}
}


