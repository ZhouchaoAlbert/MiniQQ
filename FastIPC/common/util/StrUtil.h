//////////////////////////////////////////////////////////////////////////  
/// ����fastipc�õ������ݽṹ�ͱ���  
/// 
/// Դ�룺http://git.oschina.net/washheart/fastipc
/// Դ�룺https://github.com/washheart/fastipc
/// ˵����https://github.com/washheart/fastipc/wiki 
/// @version	1.0 
/// @author		washheart@163.com
/// @date       2015-05-16  
////////////////////////////////////////////////////////////////////////// 
#ifndef __MYUTIL_H__
#define __MYUTIL_H__
#include <xstring>
#include <windows.h>
using namespace std;

namespace jw{
	_declspec(dllexport) string w2s(const wstring &ws);
	_declspec(dllexport) wstring s2w(const string &s);

	// ����Ψһ���ַ���
	_declspec(dllexport) char* GenerateGuid();
	_declspec(dllexport) std::wstring GenerateGuidW();

	_declspec(dllexport) long parseLong(const wstring &ws);
}
#endif