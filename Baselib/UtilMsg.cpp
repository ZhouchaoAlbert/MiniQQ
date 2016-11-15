#include "UtilMsg.h"
#include "Singleton.h"
#include "MsgObjectMgr.h"
Util::Msg::CMsgBase::CMsgBase()
{
	Singleton<CMsgObjectMgr>::Instance().Register(this);
}


Util::Msg::CMsgBase::~CMsgBase()
{
	Singleton<CMsgObjectMgr>::Instance().UnRegister(this);
}

void Util::Msg::CMsgBase::Start()
{
	Singleton<CMsgObjectMgr>::Instance().Start(this);
}

void Util::Msg::CMsgBase::Stop()
{
	Singleton<CMsgObjectMgr>::Instance().Stop(this);
}

void Util::Msg::CMsgBase::AddMsg(UINT32 uMsgID)
{
	Singleton<CMsgObjectMgr>::Instance().AddMsg(this,uMsgID);
}

void Util::Msg::CMsgBase::DeleteMsg(UINT32 uMsgID)
{
	Singleton<CMsgObjectMgr>::Instance().DeleteMsg(this, uMsgID);
}

HWND Util::Msg::CMsgBase::GetMsgWnd()
{
	return Singleton<CMsgObjectMgr>::Instance().GetMsgWnd(this);
}


