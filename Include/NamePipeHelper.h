#pragma once
#include <memory>
#include"ByteIOS.h"

class CSeqMgr
{
public:
	CSeqMgr(){ m_uSeq = 0; }
	~CSeqMgr(){}
public:
	UINT64 GetSeq()
	{
		return ++m_uSeq;
	}
protected:
private:
	UINT64 m_uSeq;
};


struct Node
{
	Node()
	{
		memset(this, 0, sizeof(Node));
	}
	UINT16 cmd;
	shared_ptr<Util::Buf::CByteIOS> buf;
};

const UINT32 PRO_FLAG = 0xffffffff;