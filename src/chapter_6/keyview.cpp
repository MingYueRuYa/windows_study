// chapter_5.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "chapter_6.h"

#define MAX_LOADSTRING 100
#define MAX_MSG 100

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

    TEXTMETRIC tm;
    static int cxClient, cyClient;
    static int cxChar, cyChar;
    static int count;
    static PMSG pmsg;

    static TCHAR szTop[] = TEXT("Message        Key       Char     Repeat Scan Ext ALT Prev Tran");
    static TCHAR szUnd[] = TEXT("_______        ___       ___      ______ ____ ___ ___ ____ ____");

    static TCHAR *szMessage[] = {
        TEXT("WM_KEYDOWN"),
        TEXT("WM_KEYUP"),
        TEXT("WM_CHAR"),
        TEXT("WM_DEADCHAR"),
        TEXT("WM_SYSKEYDOWN"),
        TEXT("WM_SYSCHAR"),
        TEXT("WM_SYSDEADCHAR")
    };

	static TCHAR *szYes = TEXT("Yes");
	static TCHAR *szNo	= TEXT("No");
	static TCHAR *szDown	= TEXT("Down"); 
	static TCHAR *szUp		= TEXT("Up"); 

    TCHAR  szBuffer[128], szKeyName[32];
    int iType;

    static TCHAR *szFormat[2] = {
        TEXT("%-13s %3d %-15s%c%6u %4d %3s %3s %4s %4s"),
        TEXT("%-13s            0x%04X%1s%c %6u %4d %3s %3s %4s %4s")
    };

	switch (message)
	{
    case WM_CREATE:
        count = 0;
        hdc = GetDC(hWnd);
        SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
        GetTextMetrics(hdc, &tm);
        cxChar = tm.tmAveCharWidth;
        cyChar = tm.tmHeight;
        ReleaseDC(hWnd, hdc);
        if (pmsg) { free(pmsg); }

        pmsg = (PMSG)malloc(sizeof(MSG)*MAX_MSG);
        break;
    case WM_KEYDOWN:
    case WM_CHAR:
    case WM_KEYUP:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:
		for (int i = min(count, MAX_MSG); i > 0; i--) {
			pmsg[i] = pmsg[i-1];
		}

		pmsg[0].hwnd	= hWnd;
		pmsg[0].message = message;
		pmsg[0].wParam	= wParam;
		pmsg[0].lParam	= lParam;

		count++;
        InvalidateRect(hWnd, NULL, TRUE);
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
        SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
        SetBkMode(hdc, TRANSPARENT);
        TextOut(hdc, 0, 0, szTop, lstrlen(szTop));
        TextOut(hdc, 0, 0, szUnd, lstrlen(szUnd));

        for (int i=0; i<min(MAX_MSG,count); ++i) {
            iType = pmsg[i].message == WM_CHAR || 
                    pmsg[i].message == WM_SYSCHAR ||
                    pmsg[i].message == WM_DEADCHAR ||
                    pmsg[i].message == WM_SYSDEADCHAR;

            GetKeyNameText(pmsg[i].lParam, szKeyName, 
                           sizeof(szKeyName)/sizeof(TCHAR));

            TextOut(hdc, 0, (i+1)*cyChar, szBuffer,
                    wsprintf(szBuffer, szFormat[iType], 
                             szMessage[pmsg[i].message-WM_KEYFIRST],
                             pmsg[i].wParam, 
                             (PSTR)(iType ? TEXT("") : szKeyName),
                             (TCHAR)(iType ? pmsg[i].wParam : ' '),
							 LOWORD(pmsg[i].lParam),
							 HIWORD(pmsg[i].lParam) & 0x00ff,
							 0x01000000 & pmsg[i].lParam ? szYes : szNo,
							 0x20000000 & pmsg[i].lParam ? szYes : szNo,
							 0x40000000 & pmsg[i].lParam ? szYes : szNo,
							 0x80000000 & pmsg[i].lParam ? szYes : szNo
                             )
                    );

        }

		EndPaint(hWnd, &ps);
    }
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		// �ͷ��ڴ浼���쳣
        // free(pmsg);
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
