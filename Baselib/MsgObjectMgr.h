#pragma once
#include "MsgObject.h"
#include "UtilMsg.h"
using namespace Util::Msg;

class CMsgObjectMgr
{
public:
	CMsgObjectMgr();
	~CMsgObjectMgr();

	void Register(CMsgBase* pMsgBase);
	void UnRegister(CMsgBase* pMsgBase);
	void AddMsg(CMsgBase* pMsgBase, UINT32 uMsgID);
	void DeleteMsg(CMsgBase* pMsgBase, UINT32 uMsgID);
	void Start(CMsgBase* pMsgBase);
	void Stop(CMsgBase* pMsgBase);
	HWND GetMsgWnd(CMsgBase* pMsgBase);

	void OnMessage(CMsgObject* pMsgObject,UINT32 uMsgID, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	CMsgObject* FindMsgBase(CMsgBase* pMsgBase);
private:
	map<CMsgBase*, CMsgObject*> m_mapMsgObject;
};

