// load_method_dll.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "load_method_dll.h"


// ���ǵ���������һ��ʾ��
// LOAD_METHOD_DLL_API int nload_method_dll=0;

// ���ǵ���������һ��ʾ����
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

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� load_method_dll.h
//Cload_method_dll::Cload_method_dll()
//{
//	return;
//}
