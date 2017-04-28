#include "stdafx.h"
#include "PackageMgr.h"
#include "NamePipeHelper.h"
#include "Singleton.h"
#include "NamePipeSeriveMgr.h"
#include "PipeBuffer.h"
#include <time.h>
#include "NamePipeHelper.h"
const UINT8 MAX_RETRY_COUNT = 3;
const UINT32 RETRY_BREAK = 8 * 1000;
const UINT32 TIMEOUT_BREAK = 3 * 1000;



CPackageMgr::CPackageMgr()
{
	m_tmTimeout.Register(_T("Procotol_Timeout"), this, &CPackageMgr::OnTimer);

}


CPackageMgr::~CPackageMgr()
{
	m_tmTimeout.Stop();
	m_tmTimeout.UnRegister();
}


BOOL CPackageMgr::Send(UINT16 cmd, UINT8 *buf, UINT16 len, Util::Sink::ISink *pSink)
{
	C2SNode node;
	node.cmd = cmd;
	node.pSink = pSink;
	node.seq = Singleton<CSeqMgr>::Instance().GetSeq();
	node.buf = shared_ptr<Util::Buf::CByteIOS>(new Util::Buf::CByteIOS);
	UINT16 total = 0;
	(*node.buf) << PRO_FLAG << total << node.seq << cmd;
	total = node.buf->GetLen();
	node.buf->Write(buf, len);
	node.buf->Set(4, (UINT8*)&total, sizeof(UINT16));

	UINT32 bResult = Singleton<CNamePipeSeriveMgr>::Instance().Send(cmd,node.buf->GetBuf(), node.buf->GetLen());
	if (!bResult) //0 正确
	{
		m_mapRequest[node.seq] = node;
		return TRUE;
	}
	return FALSE;
}

void CPackageMgr::Cancel(Util::Sink::ISink *pSink)
{
	map<UINT64, C2SNode> mapTmp = m_mapRequest;
	for (map<UINT64, C2SNode>::iterator iter = mapTmp.begin(); iter != mapTmp.end(); ++iter)
	{
		if (iter->second.pSink == pSink)
		{
			m_mapRequest.erase(iter->first);
		}
	}
}

void CPackageMgr::OnDataCome()
{
	while (true)
	{
		CmdMeta meta;
		shared_ptr<Util::Buf::CByteIOS> package(new Util::Buf::CByteIOS);	//包体
		if (FALSE == Singleton<CPipeBuffer>::Instance().GetPackage(package, meta))
		{
			break;
		}
		map<UINT64, C2SNode>::iterator iterSub = m_mapRequest.find(meta.uSeq);
		if (iterSub != m_mapRequest.end())//c2s response
		{
			if (iterSub->second.pSink)
			{
				iterSub->second.pSink->OnSink((void*)0, (void*)package->GetBuf(), (void*)package->GetLen());
			}
			continue;
		}
	
	}
}

void CPackageMgr::OnTimer(void *pParam1, void *pParam2, void *pParam3)
{
	return;
	vector<UINT64> vRetry, vTimeout;
	for (map<UINT64, C2SNode>::iterator iter = m_mapRequest.begin(); iter != m_mapRequest.end(); iter++)
	{
		if ((iter->second.sendtime + RETRY_BREAK) <= (UINT64)time(0))
		{
			if (iter->second.retrycount >= MAX_RETRY_COUNT)
			{
				vTimeout.push_back(iter->first);
			}
			else
			{
				vRetry.push_back(iter->first);
			}
		}
	}
	for (vector<UINT64>::iterator it1 = vRetry.begin(); it1 != vRetry.end(); it1++)
	{
		map<UINT64, C2SNode>::iterator iterSub = m_mapRequest.find(*it1);
		if (iterSub != m_mapRequest.end())
		{
			//重发
			UINT32 bResult = Singleton<CNamePipeSeriveMgr>::Instance().Send(iterSub->second.cmd, iterSub->second.buf->GetBuf(), iterSub->second.buf->GetLen());
			if (bResult) //0 正确
			{
				
			}
		}
	}

	for (vector<UINT64>::iterator it2 = vTimeout.begin(); it2 != vTimeout.end(); it2++)
	{
		map<UINT64, C2SNode>::iterator iterSub = m_mapRequest.find(*it2);
		if (iterSub != m_mapRequest.end() && iterSub->second.pSink)
		{
			iterSub->second.buf->SetRPos(16);
			
			UINT16 len = iterSub->second.buf->GetLen();
			UINT8 *buf = new UINT8[len];

			iterSub->second.buf->Read(buf, len);
			iterSub->second.pSink->OnSink((void*)1, (void*)buf, (void*)len);//超时
			delete buf;
		}
		m_mapRequest.erase(*it2);
	}

}