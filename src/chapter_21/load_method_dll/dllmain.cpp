// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBox(NULL, TEXT("title"), TEXT("process attach"), MB_OK);
		break;
	case DLL_THREAD_ATTACH:
		MessageBox(NULL, TEXT("title"), TEXT("thread attach"), MB_OK);
		break;
	case DLL_THREAD_DETACH:
		MessageBox(NULL, TEXT("title"), TEXT("thread deattach"), MB_OK);
		break;
	case DLL_PROCESS_DETACH:
		MessageBox(NULL, TEXT("title"), TEXT("process deattach"), MB_OK);
		break;
	}
	return TRUE;
}

