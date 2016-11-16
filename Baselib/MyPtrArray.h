#pragma once
#include "BaseDefine.h"


const UINT32 INIT_SIZE_COUNT = 20;
class CMyPtrArray
{
public:

	CMyPtrArray();
	~CMyPtrArray();
	CMyPtrArray(CMyPtrArray& obj);
	CMyPtrArray(UINT32 uInitSize);
	
	void   AddData(LPVOID pData);
	LPVOID GetData(UINT32 uIndex);
	UINT32 FindData(LPVOID pData);
	BOOL   SetData(UINT32 uIndex, LPVOID pData);
	BOOL   Remove(UINT32 uIndex);
public:
	UINT32 GetCount();
	LPVOID* GetData();

	void   Empty();
	BOOL   IsEmpty();
private:
	LPVOID* m_ppVoid;
	UINT32  m_count;
	UINT32  m_alloc;
};

