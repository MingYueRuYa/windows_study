// chapter_5.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "clipborad.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

TCHAR szDefaultText[] = TEXT("Default Text");

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#ifdef UNICODE
#define CF_TCHAR CF_UNICODETEXT
#else
#define CF_TCHAR CF_TEXT
#endif //UNICODE

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
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MENU);// MAKEINTRESOURCE(IDC_CHAPTER_5);
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
	RECT rect;
	LPWSTR pGlobal;

    TCHAR szBuffer[1024] = {0};
    static int cxClient, cyClient;

	static LPWSTR pText;
	BOOL bEnable;
	HGLOBAL hGlobal;

	switch (message)
	{
	case WM_CREATE:
		SendMessage(hWnd, WM_COMMAND, ID_EDIT_RESET, 0);
		break;
	case WM_INITMENUPOPUP:
		EnableMenuItem((HMENU)wParam, ID_EDIT_PASTE, 
						IsClipboardFormatAvailable(CF_TCHAR) ? 
						MF_ENABLED : MF_GRAYED);
		bEnable = pText ? MF_ENABLED : MF_GRAYED;

		EnableMenuItem((HMENU)wParam, ID_EDIT_CUT, bEnable);
		EnableMenuItem((HMENU)wParam, ID_EDIT_COPY, bEnable);
		EnableMenuItem((HMENU)wParam, ID_EDIT_DELETE, bEnable);

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
		case ID_EDIT_PASTE:
			OpenClipboard(hWnd);
			if ((hGlobal = GetClipboardData(CF_TCHAR))) {
				pGlobal = (LPWSTR)GlobalLock(hGlobal);
				if (pText) {
					free(pText);
					pText = NULL;
				}
				pText = (LPWSTR)malloc(GlobalSize(hGlobal));
				lstrcpy(pText, pGlobal);
				GlobalUnlock(hGlobal);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			CloseClipboard();
			break;
		case ID_EDIT_CUT:
		case ID_EDIT_COPY:
			if (!pText) {
				return 0;
			}
			hGlobal = GlobalAlloc(GHND | GMEM_SHARE, 
									(lstrlen(pText)+1)*sizeof(TCHAR));
			
			pGlobal = (LPWSTR)GlobalLock(hGlobal);
			lstrcpy(pGlobal, pText);

			OpenClipboard(hWnd);
			EmptyClipboard();
			SetClipboardData(CF_TCHAR, hGlobal);
			CloseClipboard();
			if (LOWORD(wParam) == ID_EDIT_COPY) {
				return 0;
			}
			break;
		case ID_EDIT_DELETE:
			if (pText) {
				free(pText);
				pText = NULL;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_EDIT_RESET:
			if (pText) {
				free(pText);
				pText = NULL;
			}
			pText = (LPWSTR)malloc((lstrlen(szDefaultText)+1)*sizeof TCHAR);
			lstrcpy(pText, szDefaultText);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
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
		GetClientRect(hWnd, &rect);
		if (NULL != pText) {
			DrawText(hdc, pText, -1, &rect, DT_EXPANDTABS | DT_WORDBREAK);
		}
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
