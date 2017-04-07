#pragma  once

#include "UIlib.h"
#include "pluginapi.h"
#include <windows.h>
#include "MsgDef.h"
#ifdef __cplusplus
#define DLLEXPORT extern "C"  __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllexport)
#endif 

using namespace DuiLib;


DLLEXPORT void InitSkinEngine(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

DLLEXPORT void ShowPageItem(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

DLLEXPORT void FindControl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

DLLEXPORT void ShowLicense(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void SetProgressValue(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void SetPercentValue(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void OnBindControl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

DLLEXPORT void SetControlFocus(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void SetControlData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void SetButtonData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

DLLEXPORT void GetControlData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void SetControlCheck(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void GetControlCheck(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void CreateDirectoryPath(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void GetDiskSpaceFreeSize(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void MakeSureCreateDirectoryPath(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void GetInstallFileName(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

/* 参数： 1. TimerID(一般是回调函数的ID)
  *           2. interval
  * 功能： 创建定时器
*/
DLLEXPORT void  TBCIACreatTimer(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

/* 参数： 1. TimerID(一般是回调函数的ID)
  * 功能： 杀死定时器
*/
DLLEXPORT void  TBCIAKillTimer(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

/* 参数： 1. 标题（例如： 请选择文件夹）
  * 功能： 发消息
*/
DLLEXPORT void  SelectFolderDialog(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void  ShowPage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void  ExitSkinEngine(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void  InitMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void  OnBindBoxControl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void  ShowMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
DLLEXPORT void  CloseMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);



