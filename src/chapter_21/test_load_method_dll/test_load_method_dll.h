
// test_load_method_dll.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Ctest_load_method_dllApp: 
// �йش����ʵ�֣������ test_load_method_dll.cpp
//

class Ctest_load_method_dllApp : public CWinApp
{
public:
	Ctest_load_method_dllApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Ctest_load_method_dllApp theApp;