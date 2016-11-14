#include "Buffer.h"


Util::Buf::CBuffer::CBuffer():
m_alloc(0),
m_buf(NULL),
m_len(0),
m_resize(FALSE)
{
}



Util::Buf::CBuffer::~CBuffer()
{
	if (m_buf)
	{
		delete m_buf;
		m_buf    = NULL;
		m_alloc  = 0;
		m_len    = 0;
		m_resize = FALSE;
	}
}


Util::Buf::CBuffer::CBuffer(const CBuffer& obj) :
m_alloc(0),
m_buf(NULL),
m_len(0),
m_resize(FALSE)
{
	if (obj.m_buf && obj.m_len)
	{
		m_buf = new UINT8[obj.m_len];
		memcpy(m_buf, obj.m_buf, obj.m_len);
		m_alloc  = obj.m_len;
		m_len    = obj.m_len;
		m_resize = obj.m_resize;
	}
}


Util::Buf::CBuffer& Util::Buf::CBuffer::operator = (const CBuffer& obj)
{
	if (this != &obj)
	{
		if (m_buf)
		{
			delete m_buf;
			m_len = 0;
			m_alloc = 0;
			m_resize = FALSE;
		}
		if (obj.m_buf && obj.m_len)
		{
			m_buf = new UINT8[obj.m_len];
			memcpy(m_buf, obj.m_buf, obj.m_len);
			m_alloc  = obj.m_len;
			m_len    = obj.m_len;
			m_resize = obj.m_resize;
		}
	}
	return *this;
}


BOOL Util::Buf::CBuffer::Append(UINT8* buf, UINT16 len)
{
	if (NULL == buf && 0 == len)
	{
		return FALSE;
	}
	if (m_len + len > m_alloc)
	{
		m_alloc = (m_len + len) * 3 / 2;
		UINT8* temp = new UINT8[m_alloc];
		if (temp && m_buf && m_len)
		{
			memcpy(temp, m_buf, m_len);
		}
		if (m_buf)
		{
			delete m_buf;
		}
		m_buf = temp;
	}
	memcpy(m_buf + m_len, buf, len);
	m_len += len;
	return TRUE;
}

void Util::Buf::CBuffer::EraseLeft(UINT16 len)
{
	if (len > m_len)
	{
		m_len = 0;
	}
	else
	{
		memmove(m_buf, m_buf + len, len);
	}
}

void Util::Buf::CBuffer::EraseRight(UINT16 len)
{
	if (len > m_len)
	{
		len = 0;
	}
	else
	{
		m_len -= len;
	}
}