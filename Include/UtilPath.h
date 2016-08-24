#pragma once
#include <atlstr.h>
using namespace  ATL;

#include "BaseDefine.h"
namespace Util
{
	namespace Path
	{
		COMMON_API  CString GetAppPath();    //获取应用程序执行路径
		COMMON_API  CString GetCurrentDir(); //获取应用程序当前目录
		COMMON_API  CString GetTempPath();   //获取系统临时路径
		COMMON_API  CString GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);	//获取随机文件名(全路径)
		COMMON_API  CString GetTempFileName(LPCTSTR lpszFileName);						//获取当前系统的临时文件夹的路径下的唯一命名的临时文件名(全路径)
		COMMON_API  CString GetInstallFolder();    //获取安装目录
		COMMON_API  CString GetResFolder();			 //获取主Res所在目录
		COMMON_API  CString GetResXmlFolder();	     //获取主Resource::XML所在目录
	
		COMMON_API  BOOL IsDirectory(LPCTSTR lpszPath);			// 检测指定路径是否目录
		COMMON_API  BOOL IsFileExist(LPCTSTR lpszFileName);		// 检测指定文件是否存在
		COMMON_API  BOOL IsDirectoryExist(LPCTSTR lpszPath);	// 检测指定目录是否存在
		COMMON_API  BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);  //创建目录


		COMMON_API  CString GetPathRoot(LPCTSTR lpszPath);				  //获取指定路径的根目录信息
		COMMON_API  CString GetExtension(LPCTSTR lpszPath);	              //返回指定的路径字符串的扩展名
		COMMON_API  CString GetFileNameWithoutExtension(LPCTSTR lpszPath); //返回不具有扩展名的路径字符串的文件名
	}
}


