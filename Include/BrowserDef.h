#pragma once

#include "BaseDefine.h"
#include "atlbase.h"
#include "UtilString.h"

using namespace ATL;
#define SAFEFREEMALLOC(x)			do\
{\
if (x)\
{\
	free(x); \
	x = NULL; \
}\
}while (0)

namespace Aux
{
	namespace Browser
	{
		const UINT WM_ESC_CLOSE = 8888;
		
		const UINT32	GLOBAL_SINGLEPROCESS = 0x01;
		
		enum WndShowType
		{
			WST_NORMAL = 0,		//正常显示  
			WST_EMBED  = 1,	    //内嵌
			WST_POPUP = 2,		//弹窗资料 资料卡 
			WST_MULTITAB = 3,   //多tab显示
			WST_JSCALLPV = 4,   //这个是通过JSCall调用弹出的窗口
		};
		
		//标识具体的页面对象，其中pszTitle + winid标识窗口
		struct SERVICE_API BrwsType
		{
			TCHAR*          pszTitle;
			UINT32			winid;

			BrwsType()
			{
				Reset();
			}
			BrwsType(const BrwsType& other)
			{
				Reset();
				Assign(other.pszTitle, other.winid);
			}
			BrwsType(LPCTSTR _pTitle, UINT32 _winid)
			{
				Reset();
				Assign(_pTitle, _winid);
			}
			~BrwsType()
			{
				winid = 0;
				SAFEFREEMALLOC(pszTitle);
			}
			void Reset()
			{
				winid = 0;
				pszTitle = NULL;
			}
			bool operator() (const BrwsType& l, const BrwsType& r) const
			{
				int iCmp = Util::String::StrCmp(l.pszTitle, r.pszTitle);
				if (iCmp != 0)
				{
					return iCmp < 0;
				}
				return l.winid < r.winid;
			}
			BrwsType&	Assign(LPCTSTR _pszTitle, UINT32 _winid)
			{
				SAFEFREEMALLOC(pszTitle);

				winid = _winid;
				pszTitle = _pszTitle ? _tcsdup(_pszTitle) : NULL;
//				ATLASSERT(pszTitle);

				return *this;
			}
			BrwsType&	operator=(const BrwsType& other)
			{
				return Assign(other.pszTitle, other.winid);
			}
			BOOL IsValid() const
			{
				return (pszTitle && *pszTitle != _T('\0'));
			}
		};

		struct SERVICE_API BrwsID
		{
			BrwsType		type;
			UINT64			id;

			BrwsID()
			{
				Reset();
			}
			BrwsID(const BrwsID& other)
			{
				Reset();
				Assign(other.type, other.id);
			}
			BrwsID(LPCTSTR _pTitle, UINT32 _winid, UINT64 _id)
			{
				Reset();
				Assign(_pTitle, _winid, _id);
			}
			~BrwsID()
			{
				Reset();				
			}
			void Reset()
			{
				type.Reset();
				id = 0;
			}
			bool operator() (const BrwsID& l, const BrwsID& r) const
			{
				int iCmp = Util::String::StrCmp(l.type.pszTitle, r.type.pszTitle);
				if (iCmp != 0)
				{
					return iCmp < 0;
				}

				if (l.type.winid != r.type.winid)
				{
					return l.type.winid < r.type.winid;
				}

				return l.id < r.id;
			}
			BrwsID&	Assign(LPCTSTR _pszTitle, UINT32 _winid, UINT64 _id)
			{
				type.Assign(_pszTitle, _winid);
				id = _id;

				return *this;
			}
			BrwsID&	Assign(BrwsType	_type, UINT64 _id)
			{
				type = _type;
				id = _id;

				return *this;
			}
			BrwsID&	operator=(const BrwsID& other)
			{
				return Assign(other.type, other.id);
			}

			BOOL operator == (BrwsID& other)
			{ 
				if (type.winid == other.type.winid && id == other.id && 0 == Util::String::StrCmp(type.pszTitle, other.type.pszTitle))
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}	
			}
			BOOL operator != (BrwsID& other)
			{
				if (type.winid == other.type.winid && id == other.id && 0 == Util::String::StrCmp(type.pszTitle, other.type.pszTitle))
				{
					return FALSE;
				}
				else
				{
					return TRUE;
				}
			}

			BOOL IsValid() const
			{
				return /*0 != id &&*/ type.IsValid();
			}
		};
	}
}


