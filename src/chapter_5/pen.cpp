// chapter_5.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "chapter_5.h"

#define MAX_LOADSTRING 100
#define NUM_LINES      7

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CHAPTER_5, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHAPTER_5));

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHAPTER_5));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CHAPTER_5);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

    TCHAR szBuffer[1024] = {0};
    static int cxClient, cyClient;
    static HPEN hPen1, hPen2, hPen3, hOldPen;
    int iLineDis;

	switch (message)
	{
    case WM_CREATE:
        hPen1 = CreatePen(PS_SOLID, 1, 0);
        hPen2 = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        hPen3 = CreatePen(PS_DOT, 0, RGB(0, 0, 255));
        break;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);

        break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
    {
		hdc = BeginPaint(hWnd, &ps);

        iLineDis = cyClient/(NUM_LINES+1);

        hOldPen  = (HPEN)SelectObject(hdc, hPen1);
        MoveToEx(hdc, cxClient/10, iLineDis, NULL);
        LineTo(hdc, cxClient/10*9, iLineDis);

        SelectObject(hdc, hPen2);
        MoveToEx(hdc, cxClient/10, iLineDis*2, NULL);
        LineTo(hdc, cxClient/10*9, iLineDis*2);

        SelectObject(hdc, hPen3);
        MoveToEx(hdc, cxClient/10, iLineDis*3, NULL);
        LineTo(hdc, cxClient/10*9, iLineDis*3);

        SelectObject(hdc, CreatePen(PS_DASH, 0, RGB(0, 255, 0)));
        MoveToEx(hdc, cxClient/10, iLineDis*4, NULL);
        LineTo(hdc, cxClient/10*9, iLineDis*4);

        DeleteObject(SelectObject(hdc, 
                                  CreatePen(PS_DASHDOT, 0, RGB(0, 255, 0))));
        MoveToEx(hdc, cxClient/10, iLineDis*5, NULL);
        LineTo(hdc, cxClient/10*9, iLineDis*5);

        DeleteObject(SelectObject(hdc, 
                                  CreatePen(PS_DASHDOTDOT, 0, 
                                  RGB(0, 255, 255))
                                  ));
        MoveToEx(hdc, cxClient/10, iLineDis*6, NULL);
        LineTo(hdc, cxClient/10*9, iLineDis*6);

        DeleteObject(SelectObject(hdc,GetStockObject(BLACK_PEN)));
        MoveToEx(hdc, cxClient/10, iLineDis*7, NULL);
        LineTo(hdc, cxClient/10*9, iLineDis*7);

        SelectObject(hdc, hOldPen);

		EndPaint(hWnd, &ps);
    }
		break;
	case WM_DESTROY:
        DeleteObject(hPen3);
        DeleteObject(hPen2);
        DeleteObject(hPen1);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
