// chapter_5.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "chapter_8.h"

#define MAX_LOADSTRING 100

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
    static POINT apt[4];
	static HWND hwndEdit1;
	static HWND hwndEdit2;

	switch (message)
	{
	case WM_CREATE:
		hwndEdit1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL,
								   WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|
								   ES_LEFT|ES_MULTILINE|ES_AUTOHSCROLL|
								   ES_AUTOVSCROLL, 0, 0, 0, 0, hWnd, (HMENU)1,
								   ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		hwndEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL,
								   WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|
								   ES_LEFT|ES_MULTILINE|ES_AUTOHSCROLL|
								   ES_AUTOVSCROLL, 0, 0, 0, 0, hWnd, (HMENU)2,
								   ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		CreateWindow(TEXT("button"), TEXT("���"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 60, 260, 80, 36,
			hWnd, (HMENU)11, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CreateWindow(TEXT("button"), TEXT("Copy"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 60, 300, 80, 36,
			hWnd, (HMENU)12, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CreateWindow(TEXT("button"), TEXT("Paste"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 300, 80, 36,
			hWnd, (HMENU)13, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		CreateWindow(TEXT("button"), TEXT("Set Text"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 60, 340, 80, 36,
			hWnd, (HMENU)14, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CreateWindow(TEXT("button"), TEXT("Get Text"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 340, 80, 36,
			hWnd, (HMENU)15, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		break;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
		MoveWindow(hwndEdit1, 60, 60, 200, 160, TRUE);
		MoveWindow(hwndEdit2, 300, 60, 200, 160, TRUE);
        break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
//		switch (wmId)
//		{
//		case IDM_ABOUT:
//			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//			break;
//		case IDM_EXIT:
//			DestroyWindow(hWnd);
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//
		if (wmId == 1) {
			switch (wmEvent) {
			case EN_MAXTEXT:
				break;
			case EN_ERRSPACE:
				break;
			case EN_UPDATE:
				GetWindowText(hwndEdit1, szBuffer, 1024);
				SetWindowText(hwndEdit2, szBuffer);
			}
		}

		if (wmId == 11) { // ���
			switch (wmEvent) {
			case BN_CLICKED:
				// ���ѡ�е�����
				SendMessage(hwndEdit1, WM_CLEAR, 0, 0);
				break;
			case BN_SETFOCUS:
				break;
			}
		}

		if (wmId == 12) { // Copy
			switch (wmEvent) {
			case BN_CLICKED:
				// copyѡ�е�����
				SendMessage(hwndEdit1, WM_COPY, 0, 0);
				break;
			case BN_SETFOCUS:
				break;
			}
		}

		if (wmId == 13) { // Paste
			switch (wmEvent) {
			case BN_CLICKED:
				// pasteѡ�е�����
				SendMessage(hwndEdit1, WM_PASTE, 0, 0);
				break;
			case BN_SETFOCUS:
				break;
			}
		}

		if (wmId == 14) { // set text
			switch (wmEvent) {
			case BN_CLICKED:
				SetWindowText(hwndEdit1, TEXT("This is a line"));
				break;
			case BN_SETFOCUS:
				break;
			}
		}

		if (wmId == 15) { // set text
			switch (wmEvent) {
			case BN_CLICKED:
				GetWindowText(hwndEdit1, szBuffer, 1024);
				SetWindowText(hwndEdit2, szBuffer);
				break;
			case BN_SETFOCUS:
				break;
			}
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
