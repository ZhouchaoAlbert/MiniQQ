#include<windows.h>
#include "WebkitApi.h"
#include "NamePipeApi.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int nCmdShow)
{

	//MessageBox(NULL, _T(""), _T(""), MB_OK);
//	Aux::WebkitApi::Init(hInstance);

	//���������ܵ�
	Aux::NameClient::Create(STR_NAMEPIPE);

	MSG msg;

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return 0;
}
