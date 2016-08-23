﻿#pragma once

#include "stdint.h"
#include <tchar.h>
#include <wtypes.h>
#include <stdlib.h>

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

#define SAFEFREENEW(x)				do\
									{\
										if(x)\
										{\
											delete x;\
											x = NULL;\
										}\
									}while(0)

#define SAFEFREENEWARRAY(x)			do\
									{\
										if(x)\
										{\
											delete []x;\
											x = NULL;\
										}\
									}while(0)

#define SAFEFREEMALLOC(x)			do\
									{\
										if(x)\
										{\
											free(x);\
											x = NULL;\
										}\
									}while(0)

#define COM_SAFERELEASE(x)			if(x)\
									{\
										x->Release();\
										x = NULL;\
									}

#ifdef _UNICODE
#define FUNC		__FUNCTIONW__
#else
#define FUNC		__FUNCTION__
#endif

//base type define
/*
#ifndef INT64
typedef __int64 INT64;
#endif

#ifndef INT32
typedef __int32 INT32;
#endif

#ifndef INT16
typedef __int16 INT16;
#endif

#ifndef INT8
typedef __int8 INT8;
#endif

#ifndef UINT64
typedef unsigned __int64 UINT64;
#endif

#ifndef UINT32
typedef unsigned __int32 UINT32;
#endif

#ifndef UINT16
typedef unsigned __int16 UINT16;
#endif

#ifndef UINT8
typedef unsigned __int8 UINT8;
#endif
*/
//compile micro 
#ifndef BASE_API_OPTION		//外部使用者请定义，不可重入
#define BASE_API __declspec(dllexport)
#else
#define BASE_API __declspec(dllimport)
#endif

#ifndef COMMON_API_OPTION		//外部使用者请定义，不可重入
#define COMMON_API __declspec(dllexport)
#else
#define COMMON_API __declspec(dllimport)
#endif

#ifndef STORAGE_API_OPTION		//外部使用者请定义，不可重入
#define STORAGE_API __declspec(dllexport)
#else
#define STORAGE_API __declspec(dllimport)
#endif

#ifndef NETPROTO_API_OPTION		//外部使用者请定义，不可重入
#define NETPROTO_API __declspec(dllexport)
#else
#define NETPROTO_API __declspec(dllimport)
#endif

#ifndef LIPC_API_OPTION		//外部使用者请定义，不可重入
#define LIPC_API __declspec(dllexport)
#else
#define LIPC_API __declspec(dllimport)
#endif

#ifndef SERVICE_API_OPTION		//外部使用者请定义，不可重入
#define SERVICE_API __declspec(dllexport)
#else
#define SERVICE_API __declspec(dllimport)
#endif

#ifndef TRAYMGR_API_OPTION	//外部使用者请定义，不可重入
#define TRAYMGR_API __declspec(dllexport)
#else
#define TRAYMGR_API __declspec(dllimport)
#endif

#ifndef CEF_API_OPTION	//外部使用者请定义，不可重入
#define CEF_API __declspec(dllexport)
#else
#define CEF_API __declspec(dllimport)
#endif

#ifndef AUDIO_API_OPTION	//外部使用者请定义，不可重入
#define AUDIO_API __declspec(dllexport)
#else
#define AUDIO_API __declspec(dllimport)
#endif

#ifndef FILE_TRANSFER_API_OPTION		//外部使用者请定义，不可重入
#define FILE_TRANSFER_API __declspec(dllexport)
#else
#define FILE_TRANSFER_API __declspec(dllimport)
#endif

//CmdLine Enable/Disable，是否整合进编译的二进制文件里面
//#define ENABLE_CMDLINE_DEBUG

//VLD内存泄漏检查
//#define ENABLE_VLD
//兼容XP
#define _WIN32_BACKWARD_COMPAT_XP   0x0602
//harcode伪造数据
#define ENABLE_FARBRICATE


#include <crtdbg.h>

#if defined(_WIN32) && defined(_DEBUG)
static bool CMAssertFailedLine(const char* szFileName, int nLine)
{
	BOOL bResult = _CrtDbgReport(_CRT_ASSERT, szFileName, nLine, NULL, NULL);
	return !!bResult;
}
#define EC_Assert (!CMAssertFailedLine(__FILE__, __LINE__) || (_CrtDbgBreak(), 0))
#else
static bool CMAssertFailedLine(const char* szFileName, int nLine)
{
	return true;
}
#define EC_Assert
#endif


#define EC_AssertEx(b) if (!(b)) {EC_Assert;}

