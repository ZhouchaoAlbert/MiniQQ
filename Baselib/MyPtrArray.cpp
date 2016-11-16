#include "MyPtrArray.h"


CMyPtrArray::CMyPtrArray():
m_ppVoid(NULL),
m_count(0),
m_alloc(0)
{
}


CMyPtrArray::~CMyPtrArray()
{
	Empty();
}


CMyPtrArray::CMyPtrArray(UINT32 uInitSize):
m_ppVoid(NULL),
m_count(0),
m_alloc(uInitSize)
{
	if (uInitSize)
	{
		m_ppVoid = new LPVOID[m_alloc];
	}
}

CMyPtrArray::CMyPtrArray(CMyPtrArray& obj):
m_ppVoid(NULL),
m_count(0),
m_alloc(0)
{
	UINT32 count = obj.GetCount();
	if (count > 0)
	{
		for (UINT32 uIndex = 0; uIndex < count;uIndex++)
		{
			LPVOID pVoid = GetData(uIndex);
			if (pVoid)
			{	
				AddData(pVoid);
			}
		}
	}
}


void CMyPtrArray::AddData(LPVOID pData)
{
	if (++m_count >= m_alloc)  //重新调正大小
	{
		UINT32 alloc = m_alloc * 2;
		alloc = (0 == alloc) ? INIT_SIZE_COUNT : alloc;

		LPVOID* temp = new LPVOID[alloc];
		if (temp && m_ppVoid)
		{
			memcpy(temp, m_ppVoid, m_count * sizeof(LPVOID));		
			if (m_ppVoid)
			{
				delete m_ppVoid;
			}
			m_ppVoid = temp;
			m_alloc  = alloc;
		}
		else
		{
			m_count--;
			return;
		}
	}
	m_ppVoid[m_count -1] = pData;
}


LPVOID CMyPtrArray::GetData(UINT32 uIndex)
{
	if (uIndex >= 0 && uIndex < m_count)
	{
		return m_ppVoid[uIndex];
	}
	return NULL;
}

LPVOID* CMyPtrArray::GetData()
{
	return m_ppVoid;
}

UINT32 CMyPtrArray::GetCount()
{
	return m_count;
}


void   CMyPtrArray::Empty()
{
	if (m_ppVoid)
	{
		delete m_ppVoid;
	}
	m_ppVoid = NULL;
	m_alloc = 0;
	m_count = 0;
}

BOOL  CMyPtrArray::IsEmpty()
{
	return m_count == 0;
}


UINT32 CMyPtrArray::FindData(LPVOID pData)
{
	for (UINT32 uIndex = 0; uIndex < m_count;uIndex++)
	{
		if (m_ppVoid[uIndex] == pData)
		{
			return uIndex;
		}
	}	
	return (UINT32)-1;
}

BOOL  CMyPtrArray::SetData(UINT32 uIndex, LPVOID pData)
{
	if (uIndex <0 || uIndex >= m_count)
	{
		return FALSE;
	}
	m_ppVoid[uIndex] = pData;
	return TRUE;
}

BOOL   CMyPtrArray::Remove(UINT32 uIndex)
{
	if (uIndex < 0 || uIndex >= m_count)
	{
		return FALSE;
	}
	if (uIndex < m_count--)
	{
		memmove(m_ppVoid + uIndex, m_ppVoid + uIndex + 1, (m_count - uIndex) * sizeof(LPVOID));
	}
	return TRUE;
}