#pragma once
#include <atlstr.h>
using namespace  ATL;

#include "BaseDefine.h"
namespace Util
{
	namespace Path
	{
		BASELIB_API  CString GetAppPath();    //��ȡӦ�ó���ִ��·��
		BASELIB_API  CString GetCurrentDir(); //��ȡӦ�ó���ǰĿ¼
		BASELIB_API  CString GetTempPath();   //��ȡϵͳ��ʱ·��
		BASELIB_API  CString GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);	//��ȡ����ļ���(ȫ·��)
		BASELIB_API  CString GetTempFileName(LPCTSTR lpszFileName);						//��ȡ��ǰϵͳ����ʱ�ļ��е�·���µ�Ψһ��������ʱ�ļ���(ȫ·��)
		BASELIB_API  CString GetInstallFolder();    //��ȡ��װĿ¼
		BASELIB_API  CString GetResFolder();			 //��ȡ��Res����Ŀ¼
		BASELIB_API  CString GetResMaterialFolder();			 //��ȡ��Res����Ŀ¼
		BASELIB_API  CString GetResXmlFolder();	     //��ȡ��Resource::XML����Ŀ¼
	
		BASELIB_API  BOOL IsDirectory(LPCTSTR lpszPath);			// ���ָ��·���Ƿ�Ŀ¼
		BASELIB_API  BOOL IsFileExist(LPCTSTR lpszFileName);		// ���ָ���ļ��Ƿ����
		BASELIB_API  BOOL IsDirectoryExist(LPCTSTR lpszPath);	// ���ָ��Ŀ¼�Ƿ����
		BASELIB_API  BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);  //����Ŀ¼


		BASELIB_API  CString GetPathRoot(LPCTSTR lpszPath);				  //��ȡָ��·���ĸ�Ŀ¼��Ϣ
		BASELIB_API  CString GetExtension(LPCTSTR lpszPath);	              //����ָ����·���ַ�������չ��
		BASELIB_API  CString GetFileNameWithoutExtension(LPCTSTR lpszPath); //���ز�������չ����·���ַ������ļ���
	}
}


