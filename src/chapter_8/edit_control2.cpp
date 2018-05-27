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
	static HWND hwndList1, hwndList2, hwndList3, hwndText1, hwndText2;
	int iIndex, iLength, iCount, iSelect;
	TCHAR *pVarText;
	static int cxChar, cyChar;

	switch (message)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());

		hwndList1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), NULL,
									WS_CHILD|WS_VISIBLE|LBS_NOTIFY|LBS_SORT|
									WS_VSCROLL, 
									cxChar, cyChar, 
									cxChar*16+GetSystemMetrics(SM_CXVSCROLL), 
									cyChar*9, hWnd, (HMENU)1,
									((LPCREATESTRUCT)lParam)->hInstance, NULL);

		hwndList2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), NULL,
									WS_CHILD|WS_VISIBLE|LBS_NOTIFY|
									LBS_MULTIPLESEL|WS_VSCROLL, 
									cxChar*30, cyChar, 
									cxChar*16+GetSystemMetrics(SM_CXVSCROLL), 
									cyChar*9, hWnd, (HMENU)2,
									((LPCREATESTRUCT)lParam)->hInstance, NULL);

		hwndList3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), NULL,
									WS_CHILD|WS_VISIBLE|LBS_NOTIFY|WS_VSCROLL, 
									cxChar*60, cyChar, 
									cxChar*16+GetSystemMetrics(SM_CXVSCROLL), 
									cyChar*9, hWnd, (HMENU)3,
									((LPCREATESTRUCT)lParam)->hInstance, NULL);


		CreateWindow(TEXT("button"), TEXT(">"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cxChar * 22, cyChar * 4,
			cxChar * 6, cyChar * 2,
			hWnd, (HMENU)4,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindow(TEXT("button"), TEXT(">"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cxChar * 52, cyChar * 4,
			cxChar * 6, cyChar * 2,
			hWnd, (HMENU)5,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindow(TEXT("button"), TEXT("Add String"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cxChar, cyChar * 10,
			cxChar * 16, cyChar * 2,
			hWnd, (HMENU)11,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindow(TEXT("button"), TEXT("Delete String"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cxChar, cyChar * 13,
			cxChar * 16, cyChar * 2,
			hWnd, (HMENU)12,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindow(TEXT("button"), TEXT("Reset"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cxChar, cyChar * 16,
			cxChar * 16, cyChar * 2,
			hWnd, (HMENU)13,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindow(TEXT("button"), TEXT("Set Cur Sel"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cxChar, cyChar * 19,
			cxChar * 16, cyChar * 2,
			hWnd, (HMENU)14,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindow(TEXT("button"), TEXT("ȡ��ѡ��"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cxChar, cyChar * 22,
			cxChar * 16, cyChar * 2,
			hWnd, (HMENU)15,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindow(TEXT("button"), TEXT("Insert String"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cxChar * 30, cyChar * 10,
			cxChar * 16, cyChar * 2,
			hWnd, (HMENU)21,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindow(TEXT("button"), TEXT("Insert String"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cxChar * 30, cyChar * 13,
			cxChar * 16, cyChar * 2,
			hWnd, (HMENU)22,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindow(TEXT("button"), TEXT("Set Sel"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cxChar * 30, cyChar * 16,
			cxChar * 16, cyChar * 2,
			hWnd, (HMENU)23,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindow(TEXT("button"), TEXT("ȡ��ѡ��"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cxChar * 30, cyChar * 19,
			cxChar * 16, cyChar * 2,
			hWnd, (HMENU)24,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindow(TEXT("button"), TEXT("ȫ��ѡ��"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cxChar * 30, cyChar * 22,
			cxChar * 16, cyChar * 2,
			hWnd, (HMENU)25,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindow(TEXT("button"), TEXT("ȫ��ȡ��"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cxChar * 30, cyChar * 25,
			cxChar * 16, cyChar * 2,
			hWnd, (HMENU)26,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		break;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case 1:
			if (wmEvent == LBN_SELCHANGE) {
				iIndex = SendMessage(hwndList1, LB_GETCURSEL, 0, 0);
				iLength = SendMessage(hwndList1, LB_GETTEXTLEN, iIndex, 0)+1;
				pVarText = (TCHAR *)calloc(iLength, sizeof(TCHAR));
				SendMessage(hwndList1, LB_GETTEXT, iIndex, (LPARAM)pVarText);
				MessageBox(NULL, pVarText, TEXT("����"), MB_OK);
				free(pVarText);
				pVarText = NULL;
			}
			break;
		case 2:
			if (LBN_DBLCLK == wmEvent) {
				MessageBox(NULL, TEXT("˫��"), TEXT("����"), NULL);
			}
			break;
		case 4:
			iIndex	= SendMessage(hwndList1, LB_GETCURSEL, 0, 0);
			if (iIndex < 0) {
				return 0;
			}
			iLength = SendMessage(hwndList1, LB_GETTEXTLEN, iIndex, 0)+1;
			pVarText = (TCHAR *)calloc(iLength, sizeof(TCHAR));
			SendMessage(hwndList1, LB_GETTEXT, iIndex, (LPARAM)pVarText);
			SendMessage(hwndList2, LB_INSERTSTRING, -1, (LPARAM)pVarText);
			SendMessage(hwndList1, LB_DELETESTRING, iIndex, 0);
			free(pVarText);
			break;
		case 5:
			iCount = SendMessage(hwndList2, LB_GETCOUNT, 0 ,0);
			for (iIndex = iCount-1; iIndex>=0; iIndex--) {
				iSelect = SendMessage(hwndList2, LB_GETSEL, iIndex, 0);
				if (iSelect != 0) {
					iLength = SendMessage(hwndList2, LB_GETTEXTLEN, iIndex, 0)+1;
					pVarText = (TCHAR *)calloc(iLength, sizeof(TCHAR));
					SendMessage(hwndList2, LB_GETTEXT, iIndex, (LPARAM)pVarText);
					SendMessage(hwndList3, LB_INSERTSTRING, -1, (LPARAM)pVarText);
					SendMessage(hwndList2, LB_DELETESTRING, iIndex, 0);
				}
			}
			break;
		case 11:
			SendMessage(hwndList1, LB_ADDSTRING, 0, (LPARAM)TEXT("hello"));
			SendMessage(hwndList1, LB_ADDSTRING, 0, (LPARAM)TEXT("Windows"));
			SendMessage(hwndList1, LB_ADDSTRING, 0, (LPARAM)TEXT("Programming"));
			break;
		case 12:
			SendMessage(hwndList1, LB_DELETESTRING, 0, 0);
			break;
		case 13:
			SendMessage(hwndList1, LB_RESETCONTENT, 0, 0);
			break;
		case 14:
			SendMessage(hwndList1, LB_SETCURSEL, 0, 0);
			break;
		case 15:	// ȡ��ѡ��
			SendMessage(hwndList1, LB_SETCURSEL, -1, 0);
			break;
		case 21:
			SendMessage(hwndList2, LB_INSERTSTRING, -1, (LPARAM)TEXT("one"));
			SendMessage(hwndList2, LB_INSERTSTRING, -1, (LPARAM)TEXT("two"));
			SendMessage(hwndList2, LB_INSERTSTRING, -1, (LPARAM)TEXT("three"));
			break;
		case 23:
			SendMessage(hwndList2, LB_SETSEL, 1, 2);
			break;
		case 24:
			SendMessage(hwndList2, LB_SETSEL, 0, 2);
			break;
		case 25:
			SendMessage(hwndList2, LB_SETSEL, 1, -1);
			break;
		case 26:
			SendMessage(hwndList2, LB_SETSEL, 0, -1);
			break;
		default:
			break;
		}

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
