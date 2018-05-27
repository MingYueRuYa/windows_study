// chapter_5.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "chapter_8.h"

#define MAX_LOADSTRING 100

#define ID_USERNAME_EDIT	1
#define ID_PASSWORD_EDIT	2
#define ID_USERNAME_LABEL	3
#define ID_PASSWORD_LABEL	4
#define ID_LOGIN_BUTTON		5
#define ID_CANCEL_BUTTON	6

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
      CW_USEDEFAULT, 0, 420, 220, NULL, NULL, hInstance, NULL);

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
    static POINT apt[4];

	static HWND hwndUserNameEdit;
	static HWND hwndPasswdEdit;
	static HWND hwndUserNameLabel;
	static HWND hwndPasswdLabel;
	static HWND hwndLoginButton;
	static HWND hwndCancelButton;
	static int cxChar, cyChar;


	switch (message)
	{
	case WM_CREATE:
	hwndUserNameLabel = CreateWindow(TEXT("static"), TEXT("�������û�����"),
									WS_CHILD|WS_VISIBLE|SS_LEFT, 0, 0, 0, 0,
									hWnd, (HMENU)ID_USERNAME_LABEL, 
									((LPCREATESTRUCT)lParam)->hInstance, NULL);
	hwndUserNameEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL,
									WS_CHILD|WS_VISIBLE|SS_LEFT, 0, 0, 0, 0,
									hWnd, (HMENU)ID_USERNAME_EDIT, 
									((LPCREATESTRUCT)lParam)->hInstance, NULL);
	hwndPasswdLabel = CreateWindow(TEXT("static"), TEXT("���������룺"),
									WS_CHILD|WS_VISIBLE|SS_LEFT, 0, 0, 0, 0,
									hWnd, (HMENU)ID_PASSWORD_LABEL, 
									((LPCREATESTRUCT)lParam)->hInstance, NULL);
	hwndPasswdEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL,
									WS_CHILD | WS_VISIBLE | 
									SS_LEFT | ES_PASSWORD, 
									0, 0, 0, 0,
									hWnd, (HMENU)ID_PASSWORD_EDIT, 
									((LPCREATESTRUCT)lParam)->hInstance, NULL);

	hwndLoginButton = CreateWindow(TEXT("button"), TEXT("��½"),
								WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 0, 0, 0, 0,
								hWnd, (HMENU)ID_LOGIN_BUTTON, 
								((LPCREATESTRUCT)lParam)->hInstance, NULL);

	hwndCancelButton = CreateWindow(TEXT("button"), TEXT("ȡ��"),
								WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 0, 0, 0, 0,
								hWnd, (HMENU)ID_CANCEL_BUTTON, 
								((LPCREATESTRUCT)lParam)->hInstance, NULL);

	cxChar = HIWORD(GetDialogBaseUnits());
	cyChar = LOWORD(GetDialogBaseUnits());
		break;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
	
		MoveWindow(hwndUserNameLabel, 3*cxChar, 3*cyChar, 
				   20*cxChar, 3*cyChar, TRUE);

		MoveWindow(hwndUserNameEdit, 3*cxChar, 6*cyChar, 
				   20*cxChar, 3*cyChar, TRUE);

		MoveWindow(hwndPasswdLabel, 3*cxChar, 9*cyChar, 
				   20*cxChar, 3*cyChar, TRUE);
		MoveWindow(hwndPasswdEdit, 3*cxChar, 12*cyChar, 
				   20*cxChar, 3*cyChar, TRUE);

		MoveWindow(hwndLoginButton, 3*cxChar, 16*cyChar, 
				   6*cxChar, 3*cyChar, TRUE);
		MoveWindow(hwndCancelButton, 10*cxChar, 16*cyChar, 
				   6*cxChar, 3*cyChar, TRUE);


		return 0;
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

		EndPaint(hWnd, &ps);
    }
		break;
	case WM_DESTROY:
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
