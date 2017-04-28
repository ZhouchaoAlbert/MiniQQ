#pragma once
#include "BaseDefine.h"
#include "SinkApi.h"
#define   STR_NAMEPIPE    "\\\\.\\Pipe\\NamePipe"  
namespace Aux
{
	class INamePipeSink
	{
		virtual void OnDataCommon();
	};

	namespace NameService
	{
		class NAMEPIPE_API  CS2CSink : public Util::Sink::CSinkBase
		{
		public:
			CS2CSink(){};
			~CS2CSink(){};
		public:
			BOOL Send(UINT16 uCmd, UINT8 *buf, UINT16 len);
			void Cancel(){};
		};

		NAMEPIPE_API UINT32 Create(LPCSTR lpszName);
		NAMEPIPE_API UINT32 Send(UINT16 cmd,UINT8* buf,UINT16 len);
	}

	namespace NameClient
	{
		class CC2SSink : public Util::Sink::CSinkBase
		{
		public:
			CC2SSink();
			~CC2SSink();
		public:
			BOOL Send(UINT16 uCmd, UINT8 *buf, UINT16 len);
			void Cancel();
		};

		NAMEPIPE_API UINT32 Create(LPCSTR lpszName);
		NAMEPIPE_API UINT32 Send(UINT16 cmd, UINT8* buf, UINT16 len);
	}
}




