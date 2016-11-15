#include "ByteIOS.h"


Util::Buf::CByteIOS::CByteIOS():
m_buf(NULL),
m_alloc(0),
m_len(0),
m_rpos(0),
m_wpos(0)
{

}


Util::Buf::CByteIOS::~CByteIOS()
{
	if (m_buf)
	{
		delete m_buf;
		m_alloc = 0;
		m_len   = 0;
		m_rpos  = 0;
		m_wpos  = 0;
	}
}


Util::Buf::CByteIOS::CByteIOS(const CByteIOS& obj) :
m_buf(NULL),
m_alloc(0),
m_len(0),
m_rpos(0),
m_wpos(0)
{
	if (obj.m_buf && obj.m_len)
	{
		m_buf = new UINT8[obj.m_len];
		memcpy(m_buf, obj.m_buf, obj.m_len);
		
		m_alloc = obj.m_len;
		m_len   = obj.m_len;
		m_rpos  = obj.m_rpos;
		m_wpos  = obj.m_wpos;
	}
}

Util::Buf::CByteIOS& Util::Buf::CByteIOS::operator=(const CByteIOS& obj)
{
	if (this != &obj)
	{
		if (m_buf)
		{
			delete m_buf;
			m_alloc = 0;
			m_len   = 0;
			m_rpos  = 0;
			m_wpos  = 0;
		}

		if (obj.m_buf && obj.m_len)
		{
			m_buf = new UINT8[obj.m_len];
			memcpy(m_buf, obj.m_buf, obj.m_len);

			m_alloc = obj.m_len;
			m_len   = obj.m_len;
			m_rpos  = obj.m_rpos;
			m_wpos  = obj.m_wpos;
		}
	}
	return *this;
}

//Ð´²Ù×÷£º

Util::Buf::CByteIOS& Util::Buf::CByteIOS::Write(UINT8* buf, UINT16 len)
{
	if (buf && len)
	{
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
		m_len  += len;
		m_wpos += len;
	}
	else
	{
		ATLASSERT(FALSE);
	}
	return *this;
}


Util::Buf::CByteIOS& Util::Buf::CByteIOS::Read(UINT8* buf, UINT16& len)
{
	if (m_len > m_rpos && m_buf &&& len)
	{
		if (m_rpos + len > m_len)
		{
			len = m_len - m_rpos;
		}
		memcpy(buf, m_buf + m_rpos, len);
		m_rpos += len;
	}
	return *this;
}

Util::Buf::CByteIOS& Util::Buf::CByteIOS::operator >> (CStringA& outValue)
{
	if (m_len > m_rpos)
	{
		UINT16 len = m_len - m_rpos + 1;
		CHAR* buf = new CHAR[len];
		memset(buf, 0, sizeof(CHAR)* len);
		strcpy_s(buf, len,(CHAR*)(m_buf + m_rpos));

		m_rpos += (strlen(buf) + 1)*sizeof(CHAR);
		outValue.Format("%s", buf);
		delete buf;
	}
	return *this;
}

Util::Buf::CByteIOS& Util::Buf::CByteIOS::operator >> (CStringW& outValue)
{
	if (m_len > m_rpos)
	{
		UINT16 len = m_len - m_rpos + 1;
		WCHAR* buf = new WCHAR[len];
		memset(buf, 0, sizeof(WCHAR)* len);
		wcscpy_s(buf, len, (const WCHAR*)(m_buf + m_rpos));
		m_rpos += (wcslen(buf) + 1) * sizeof(WCHAR);
		outValue.Format(L"%s", buf);
		delete buf;
	}
	return *this;
}



void Util::Buf::CByteIOS::Empty()
{
	m_len = 0;
	m_rpos = 0;
	m_wpos = 0;
}

void Util::Buf::CByteIOS::Reset()
{
	m_rpos = 0;
	m_wpos = 0;
}

void Util::Buf::CByteIOS::SetRPos(UINT16 pos)
{
	if (pos < m_len)
	{
		m_rpos = pos;
	}
}

BOOL Util::Buf::CByteIOS::Set(UINT16 pos, UINT8 *buf, UINT16 len)
{
	if ((pos + len) > m_len || NULL == buf || 0 == len)
	{
		return FALSE;
	}
	memcpy(m_buf + pos, buf, len);
	return TRUE;
}