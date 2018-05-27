// chapter_5.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "chapter_7.h"

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

const int DIVISIONS_WIDTH   = 5;
const int DIVISIONS_HEIGHT  = 6;

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
    static BOOL fState[DIVISIONS_WIDTH][DIVISIONS_HEIGHT];
    static int cxBlock, cyBlock;
    int x, y;
    RECT rect;

    TCHAR szBuffer[1024] = {0};
    static int cxClient, cyClient;
    static POINT apt[4];

	switch (message)
	{
    case WM_SIZE:
        cxBlock = LOWORD(lParam)/DIVISIONS_WIDTH;
        cyBlock = HIWORD(lParam)/DIVISIONS_HEIGHT;
        break;
    case WM_LBUTTONDOWN:
        x = LOWORD(lParam)/cxBlock;
        y = HIWORD(lParam)/cyBlock;

        if (x < DIVISIONS_WIDTH && y < DIVISIONS_HEIGHT) {
            fState[x][y] ^= 1;
            rect.left   = x * cxBlock;
            rect.top    = y * cyBlock;
            rect.right  = (x+1)*cxBlock;
            rect.bottom = (y+1)*cyBlock;
            InvalidateRect(hWnd, &rect, FALSE);
        }
        break;
    case WM_KEYDOWN:
    {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(hWnd, &point);

        x = max(0, min(DIVISIONS_WIDTH-1, point.x/cxBlock));
        y = max(0, min(DIVISIONS_HEIGHT-1, point.y/cyBlock));

        switch(wParam)
        {
        case VK_UP:
            y--;
            break;
        case VK_DOWN:
            y++;
            break;
        case VK_RIGHT:
            x++;
            break;
        case VK_LEFT:
            x--;
            break;
        case VK_HOME:
            x = y = 0;
            break;
        case VK_END:
            x = DIVISIONS_WIDTH-1;
            y = DIVISIONS_HEIGHT-1;
            break;
        case VK_SPACE:
        case VK_RETURN:
            SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 
                        MAKELONG(x*cxBlock, y*cyBlock));
            break;
        default:
            break;
        }

        x = (x+DIVISIONS_WIDTH)%DIVISIONS_WIDTH;
        y = (y+DIVISIONS_HEIGHT)%DIVISIONS_HEIGHT;

        point.x = x*cxBlock + cxBlock/2;
        point.y = y*cyBlock + cyBlock/2;

        ClientToScreen(hWnd, &point);
        SetCursorPos(point.x, point.y);
    }
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

        for (int x=0; x<DIVISIONS_WIDTH; x++) {
            for (int y=0; y<DIVISIONS_HEIGHT; y++) {
                Rectangle(hdc, x*cxBlock, y*cyBlock, 
                          (x+1)*cxBlock, (y+1)*cyBlock);

                if (fState[x][y]) {
                    MoveToEx(hdc, x*cxBlock, y*cyBlock, NULL);
                    LineTo(hdc, (x+1)*cxBlock, (y+1)*cyBlock);
                    MoveToEx(hdc, x*cxBlock, (y+1)*cyBlock, NULL);
                    LineTo(hdc, (x+1)*cxBlock, y*cyBlock);
                }
            }
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
