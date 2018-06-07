// load_method_dll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "load_method_dll.h"


// 这是导出变量的一个示例
// LOAD_METHOD_DLL_API int nload_method_dll=0;

// 这是导出函数的一个示例。
LOAD_METHOD_DLL_API int fnload_method_dll(void)
{
	MessageBox(NULL, TEXT("fnload_method_dll"), TEXT("title"), MB_OK);
	return 42;
}

LOAD_METHOD_DLL_API DWORD Demo(void)
{
	MessageBox(NULL, TEXT("Demo"), TEXT("title"), MB_OK);
	return 0;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 load_method_dll.h
//Cload_method_dll::Cload_method_dll()
//{
//	return;
//}
