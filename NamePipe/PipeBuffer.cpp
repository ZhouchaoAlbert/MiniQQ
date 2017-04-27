#include "stdafx.h"
#include "PipeBuffer.h"
#include "NamePipeHelper.h"

const UINT16 MIN_PROTO_LEN = 16;
CPipeBuffer::CPipeBuffer()
{

}

CPipeBuffer::~CPipeBuffer()
{

}

void CPipeBuffer::Append(UINT8 *buf, UINT16 len)
{
	m_buf.Append(buf, len);
}

BOOL CPipeBuffer::GetPackage(shared_ptr<Util::Buf::CByteIOS> package, CmdMeta &meta)
{
	while (m_buf.GetLen() > MIN_PROTO_LEN)
	{
		if (PRO_FLAG != *(UINT32*)m_buf.GetBuf())
		{
			m_buf.EraseLeft(1);
			continue;
		}
		UINT16 uSize = *(UINT16*)(m_buf.GetBuf() + sizeof(UINT32));
		if (m_buf.GetLen() >= uSize)
		{
			package->Write(m_buf.GetBuf(), uSize);
			(*package) >> meta.uFlag >> meta.uSize >> meta.uSeq >> meta.uCmd;
			UINT16 uHeadSize = package->GetRPos();
			package->Empty();
			package->Write(m_buf.GetBuf() + uHeadSize, uSize - uHeadSize);
			m_buf.EraseLeft(uSize);
			return TRUE;
		}
		else
		{
			break;
		}
	}
	return FALSE;
}

void CPipeBuffer::Empty()
{
	m_buf.Empty();
}
