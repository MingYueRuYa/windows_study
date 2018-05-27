// chapter_5.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "chapter_8.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
int idFocus;
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	ScrollProc(HWND, UINT, WPARAM, LPARAM);

WNDPROC OldScroll[3];

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
	wcex.hbrBackground	= CreateSolidBrush(0);//(HBRUSH)(COLOR_WINDOW+1);
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
    static int cxClient, cyClient, i, color[3];
    static POINT apt[4];

	static HWND hwndScroll[3], hwndLabel[3], hwndValue[3], hwndRect;
	static TCHAR *szColorLabel[] = { TEXT("Red"), TEXT("Green"), TEXT("Blue")};
	static int cyChar;
	static RECT rcColor;
	static HBRUSH hBrush[3];
	static HBRUSH hBrushStatic;

	static COLORREF crPrim[3] = {
		RGB(255, 0, 0),
		RGB(0, 255, 0),
		RGB(0, 0, 255),
	};

	HINSTANCE hInstance;

	switch (message)
	{
	case WM_CREATE:
		hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		hwndRect  = CreateWindow(TEXT("static"), NULL, 
								WS_CHILD | WS_VISIBLE | SS_WHITERECT,
								0, 0, 0, 0, hWnd, (HMENU)9, hInstance, NULL);
			
		
		for(i=0; i<3; ++i) {
			hwndScroll[i] = CreateWindow(TEXT("scrollbar"), NULL, 
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | SBS_VERT,
								 0, 0, 0, 0, hWnd, (HMENU)i, hInstance, NULL);

			OldScroll[i] = (WNDPROC)SetWindowLong(hwndScroll[i], 
												GWL_WNDPROC, (LONG)ScrollProc);

			SetScrollRange(hwndScroll[i], SB_CTL, 0, 255, FALSE);
			SetScrollPos(hwndScroll[i], SB_CTL, 0, FALSE);

			hwndLabel[i] = CreateWindow(TEXT("static"), szColorLabel[i], 
										WS_CHILD | WS_VISIBLE | SS_CENTER,
										0, 0, 0, 0, hWnd, (HMENU)(i+3), 
										hInstance, NULL);

			hwndValue[i] = CreateWindow(TEXT("static"), TEXT("0"), 
										WS_CHILD | WS_VISIBLE | SS_CENTER,
										0, 0, 0, 0, hWnd, (HMENU)(i+6),
										hInstance, NULL);

			hBrush[i] = CreateSolidBrush(crPrim[i]);

		}

		hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));

		cyChar = HIWORD(GetDialogBaseUnits());
		break;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);

		SetRect(&rcColor, cxClient/2, 0, cxClient, cyClient);

		MoveWindow(hwndRect, 0, 0, cxClient/2 ,cyClient, TRUE);

		for (i=0; i<3; ++i) {
			MoveWindow(hwndScroll[i], (2*i+1)*cxClient/14, 2*cyChar, 
					   cxClient/14, cyClient-4*cyChar, TRUE);

			MoveWindow(hwndLabel[i], (4*i+1)*cxClient/28, cyChar/2,
					   cxClient/7, cyChar, TRUE);
			MoveWindow(hwndValue[i], (4*i+1)*cxClient/28, cyClient-3*cyChar/2,
					   cxClient/7, cyChar, TRUE);
		}

		SetFocus(hWnd);
        break;
	case WM_VSCROLL:
		i = GetWindowLong((HWND)lParam, GWL_ID);
		
		switch (LOWORD(wParam)) {
		case SB_PAGEDOWN:
			color[i] = min(255, color[i]+15);
			break;
		case SB_PAGEUP:
			color[i] = max(0, color[i]-15);
		case SB_LINEDOWN:
			color[i] = min(255, color[i]+1);
			break;
		case SB_LINEUP:
			color[i] = max(0, color[i]-1);
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			color[i] = HIWORD(wParam);
		default:
			break;
		}

		SetScrollPos(hwndScroll[i], SB_CTL, color[i], TRUE);
		wsprintf(szBuffer, TEXT("%d"), color[i]);
		SetWindowText(hwndValue[i], szBuffer);

		DeleteObject((HBRUSH)SetClassLong(hWnd, GCL_HBRBACKGROUND, 
					(LONG)CreateSolidBrush(RGB(color[0], color[1], color[2]))));

		InvalidateRect(hWnd, &rcColor, TRUE);
		break;
	case WM_CTLCOLORSCROLLBAR:
		i = GetWindowLong((HWND)lParam, GWL_ID);
		return (LRESULT)hBrush[i];
	case WM_CTLCOLORSTATIC:
		i = GetWindowLong((HWND)lParam, GWL_ID);
		if (i>=3 && i<=8) {
			SetTextColor((HDC)wParam, crPrim[i%3]);
			return (LRESULT)hBrushStatic;
		}
		break;
	case WM_SYSCOLORCHANGE:
		DeleteObject(hBrushStatic);
		hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
		return 0;
	case WM_SETFOCUS:
		SetFocus(hwndScroll[idFocus]);
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

LRESULT CALLBACK	ScrollProc(HWND hWnd, UINT message, 
							   WPARAM wParam, LPARAM lParam)
{
	int id = GetWindowLong(hWnd, GWL_ID);

	switch (message) {
	case WM_KEYDOWN:
		if (wParam == VK_TAB) {
			int index = (id+(GetKeyState(VK_SHIFT)<0?2:1))%3;
			SetFocus(GetDlgItem(GetParent(hWnd), index));
		} else if (wParam == VK_F4) {
        }

		break;
	case WM_SETFOCUS:
		idFocus = id;
		break;
	}

	return CallWindowProc(OldScroll[id], hWnd, message, wParam, lParam);
}
