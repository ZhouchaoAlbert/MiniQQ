#pragma  once

#include "UIlib.h"
#include "pluginapi.h"
#include <windows.h>
#include "MsgDef.h"
#include "BaseDefine.h"
using namespace DuiLib;


UISKIN_API void InitSkinEngine(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

UISKIN_API void ShowPageItem(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

UISKIN_API void FindControl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

UISKIN_API void ShowLicense(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void SetProgressValue(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void SetPercentValue(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void OnBindControl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

UISKIN_API void SetControlFocus(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void SetControlData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void SetButtonData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

UISKIN_API void GetControlData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void SetControlCheck(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void GetControlCheck(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void CreateDirectoryPath(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void GetDiskSpaceFreeSize(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void MakeSureCreateDirectoryPath(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void GetInstallFileName(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

/* ������ 1. TimerID(һ���ǻص�������ID)
  *           2. interval
  * ���ܣ� ������ʱ��
*/
UISKIN_API void  TBCIACreatTimer(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

/* ������ 1. TimerID(һ���ǻص�������ID)
  * ���ܣ� ɱ����ʱ��
*/
UISKIN_API void  TBCIAKillTimer(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);

/* ������ 1. ���⣨���磺 ��ѡ���ļ��У�
  * ���ܣ� ����Ϣ
*/
UISKIN_API void  SelectFolderDialog(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void  ShowPage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void  ExitSkinEngine(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void  InitMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void  OnBindBoxControl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void  ShowMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
UISKIN_API void  CloseMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);



