#include "stdafx.h"
#include "BrowserMgr.h"



CBrowserMgr::CBrowserMgr()
{
	m_pSink.Register(_T("NameService"), this, &CBrowserMgr::OnSink);

}


CBrowserMgr::~CBrowserMgr()
{
}


BOOL CBrowserMgr::OpenPV()
{
	Util::Buf::CByteIOS buf;

	CString strTestA = _T("sfvwsggegr34r34t34t435y54y5yerhhjjkjl");
	CString strTestB = _T("°¡¹þ¹þ");
	buf << (LPCWSTR)strTestA << (LPCWSTR)strTestB;

	m_pSink.Send(11, buf, buf);
	return TRUE;
}


void CBrowserMgr::OnSink(void* pLaram1, void* pLaram2, void* pLaram3)
{
	UINT32 i = 0;
}