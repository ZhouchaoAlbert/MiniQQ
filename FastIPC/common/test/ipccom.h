//////////////////////////////////////////////////////////////////////////  
/// �������fatipc��������ͻ���ͨ��ʱ��Ҫ�Ĵ��ڡ��������Լ�������
///  
/// Դ�룺http://git.oschina.net/washheart/fastipc
/// Դ�룺https://github.com/washheart/fastipc
/// ˵����https://github.com/washheart/fastipc/wiki
/// @version	1.0 
/// @author		washheart@163.com
/// @date       2015-10-22  
////////////////////////////////////////////////////////////////////////// 
#pragma once
#include <Windows.h>
#include <string>
#include <sstream>

// ���·���ʵ���ڡ�ipccom.cpp���ļ�------------------------------------------------
// ������Ϣ
DWORD sendMsg(std::wstring msg);
// ���ڴ��ڴ�����ִ��������������������������
void windowCreated(HWND hWnd);