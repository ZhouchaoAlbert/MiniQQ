//////////////////////////////////////////////////////////////////////////  
/// 定义测试fatipc服务器与客户端通信时需要的窗口、变量、以及公共类
///  
/// 源码：http://git.oschina.net/washheart/fastipc
/// 源码：https://github.com/washheart/fastipc
/// 说明：https://github.com/washheart/fastipc/wiki
/// @version	1.0 
/// @author		washheart@163.com
/// @date       2015-10-22  
////////////////////////////////////////////////////////////////////////// 
#pragma once
#include <Windows.h>
#include <string>
#include <sstream>

// 以下方法实现在【ipccom.cpp】文件------------------------------------------------
// 发送消息
DWORD sendMsg(std::wstring msg);
// 用于窗口创建后执行其他操作。比如启动服务器
void windowCreated(HWND hWnd);