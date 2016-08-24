#pragma once
#include <atlstr.h>
using namespace  ATL;

#include "BaseDefine.h"
namespace Util
{
	namespace Path
	{
		COMMON_API  CString GetAppPath();    //��ȡӦ�ó���ִ��·��
		COMMON_API  CString GetCurrentDir(); //��ȡӦ�ó���ǰĿ¼
		COMMON_API  CString GetTempPath();   //��ȡϵͳ��ʱ·��
		COMMON_API  CString GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);	//��ȡ����ļ���(ȫ·��)
		COMMON_API  CString GetTempFileName(LPCTSTR lpszFileName);						//��ȡ��ǰϵͳ����ʱ�ļ��е�·���µ�Ψһ��������ʱ�ļ���(ȫ·��)
		COMMON_API  CString GetInstallFolder();    //��ȡ��װĿ¼
		COMMON_API  CString GetResFolder();			 //��ȡ��Res����Ŀ¼
		COMMON_API  CString GetResXmlFolder();	     //��ȡ��Resource::XML����Ŀ¼
	
		COMMON_API  BOOL IsDirectory(LPCTSTR lpszPath);			// ���ָ��·���Ƿ�Ŀ¼
		COMMON_API  BOOL IsFileExist(LPCTSTR lpszFileName);		// ���ָ���ļ��Ƿ����
		COMMON_API  BOOL IsDirectoryExist(LPCTSTR lpszPath);	// ���ָ��Ŀ¼�Ƿ����
		COMMON_API  BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);  //����Ŀ¼


		COMMON_API  CString GetPathRoot(LPCTSTR lpszPath);				  //��ȡָ��·���ĸ�Ŀ¼��Ϣ
		COMMON_API  CString GetExtension(LPCTSTR lpszPath);	              //����ָ����·���ַ�������չ��
		COMMON_API  CString GetFileNameWithoutExtension(LPCTSTR lpszPath); //���ز�������չ����·���ַ������ļ���
	}
}


