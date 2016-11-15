#pragma once

#include <BaseDefine.h>
#include <atlwin.h>
using namespace ATL;
#include <map>
using namespace std;

class CMsgObject : public CWindowImpl<CMsgObject, CWindow, CWinTraits<WS_OVERLAPPEDWINDOW, 0>>
{
public:
	CMsgObject();
	~CMsgObject();
	DECLARE_WND_CLASS(_T("CMsgObjectWindow"))
	void Destroy();
public:
	void Start();
	void Stop();
	HWND GetMsgWnd();
	void AddMsg(UINT32 uMsgID);
	void DeleteMsg(UINT32 uMsgID);

private:
	virtual void OnFinalMessage(_In_ HWND /*hWnd*/)
	{
		delete this;
	}
	virtual BOOL ProcessWindowMessage(
		_In_ HWND hWnd,
		_In_ UINT uMsg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam,
		_Inout_ LRESULT& lResult,
		_In_ DWORD dwMsgMapID);

	BOOL m_bStop;
	HWND m_hWnd;
	map<UINT32, UINT32> m_mapMsgID;
};

