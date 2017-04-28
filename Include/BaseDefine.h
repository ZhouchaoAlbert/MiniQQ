#pragma once

#include "stdint.h"
#include <tchar.h>
#include <wtypes.h>
#include <stdlib.h>
#include <atlstr.h>
using namespace ATL;
#include <vector>
using namespace std;
#define EC_TRACE ATLTRACE

#define my_min(x, y)				((x) < (y) ? (x) : (y))
#define my_max(x, y)				((x) > (y) ? (x) : (y))
#define my_diff(x, y)				((x) > (y) ? ((x) - (y)) : ((y) - (x)))
#define GETBIT(x, y)				((x) & (y))
#define ADDBIT(x, y)				((x) |= (y))
#define SETBIT(x, y)				((x) = (y))
#define CLRBIT(x, y)				((x) &= ~(y))
#define CHGBIT(x, add, del)			((x) = ((x) & (~(del))) | (add))

#define MAKE_UINT64(hi, lo)			(UINT64((UINT64(hi)) << 32) | (UINT32(lo)))
#define HIDWORD_UINT64(x)			(UINT32((UINT64(x)) >> 32))
#define LODWORD_UINT64(x)			(UINT32((x) & 0xffffffff))
#define MAKE_UINT32(hi, lo)			(UINT32((UINT32(hi)) << 16) | (UINT16(lo)))
#define HIWORD_UINT32(x)			(UINT16((UINT32(x)) >> 16))
#define LOWORD_UINT32(x)			(UINT16((x) & 0xffff))
#define MAKE_UINT16(hi, lo)			(UINT16((UINT16(hi)) << 8) | (UINT8(lo)))
#define HIBYTE_UINT16(x)			(UINT8((UINT16(x)) >> 8))
#define LOBYTE_UINT16(x)			(UINT8((x) & 0xff))


//compile micro 
#ifndef BASELIB_API_OPTION		//外部使用者请定义，不可重入
#define BASELIB_API __declspec(dllexport)
#else
#define BASELIB_API __declspec(dllimport)
#endif

#ifndef SKIN_API_OPTION		//外部使用者请定义，不可重入
#define SKIN_API __declspec(dllexport)
#else
#define SKIN_API __declspec(dllimport)
#endif

 #ifndef UISKIN_API_OPTION		//外部使用者请定义，不可重入
 #define UISKIN_API __declspec(dllexport)
 #else
 #define UISKIN_API __declspec(dllimport)
 #endif

#ifndef NAMEPIPE_API_OPTION		//外部使用者请定义，不可重入
#define NAMEPIPE_API __declspec(dllexport)
#else
#define NAMEPIPE_API __declspec(dllimport)
#endif

#ifndef SERVICE_API_OPTION		//外部使用者请定义，不可重入
#define SERVICE_API __declspec(dllexport)
#else
#define SERVICE_API __declspec(dllimport)
#endif

#ifndef WEBKIT_API_OPTION		//外部使用者请定义，不可重入
#define WEBKIT_API __declspec(dllexport)
#else
#define WEBKIT_API __declspec(dllimport)
#endif

