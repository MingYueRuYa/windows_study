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
	static int cxChar, cyChar;
	static HWND hButton;
	static HWND hCheckBox;
	static HWND hRadioButton;
	static int iCheck;

	switch (message)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());

		hButton = CreateWindow(TEXT("button"), TEXT("PushButton"), 
							   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
							   cxChar*10, cyChar*10,
							   cxChar*20, cyChar*2,
							   hWnd, (HMENU)1, hInst, NULL);

		hCheckBox = CreateWindow(TEXT("button"), TEXT("CheckBox"),
								 WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
								 cxChar*35, cyChar*10,
								 cxChar*20, cyChar*2,
								 hWnd, (HMENU)2, hInst, NULL);

		hRadioButton = CreateWindow(TEXT("button"), TEXT("RadioButton"),
								    WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
									cxChar*60, cyChar*10,
									cxChar*20, cyChar*2,
									hWnd, (HMENU)3, hInst, NULL);

		CreateWindow(TEXT("button"), TEXT("Set Checked"),
								    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
									cxChar*10, cyChar*20,
									cxChar*20, cyChar*2,
									hWnd, (HMENU)10, hInst, NULL);


		CreateWindow(TEXT("button"), TEXT("Set UnChecked"), 
				     WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					 cxChar*10, cyChar*23,
					 cxChar*20, cyChar*2,
					 hWnd, (HMENU)11, hInst, NULL);

		CreateWindow(TEXT("button"), TEXT("Get Checked"), 
				     WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					 cxChar*10, cyChar*26,
					 cxChar*20, cyChar*2,
					 hWnd, (HMENU)12, hInst, NULL);

		CreateWindow(TEXT("button"), TEXT("SetWindowText"), 
				     WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					 cxChar*35, cyChar*20,
					 cxChar*20, cyChar*2,
					 hWnd, (HMENU)20, hInst, NULL);

		CreateWindow(TEXT("button"), TEXT("GetWindowText"), 
				     WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					 cxChar*35, cyChar*23,
					 cxChar*20, cyChar*2,
					 hWnd, (HMENU)21, hInst, NULL);

		CreateWindow(TEXT("button"), TEXT("show/hide"), 
				     WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					 cxChar*60, cyChar*20,
					 cxChar*20, cyChar*2,
					 hWnd, (HMENU)30, hInst, NULL);

		CreateWindow(TEXT("button"), TEXT("enable/disable"), 
				     WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					 cxChar*60, cyChar*23,
					 cxChar*20, cyChar*2,
					 hWnd, (HMENU)31, hInst, NULL);
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
		case 1:
			MessageBox(NULL, TEXT("hello 1"), TEXT("title"), MB_OK);
			break;
		case 10:
			SendMessage(hCheckBox, BM_SETCHECK, 1, 0);
			SendMessage(hRadioButton, BM_SETCHECK, 1, 0);
			break;
		case 11:
			SendMessage(hCheckBox, BM_SETCHECK, 0, 0);
			SendMessage(hRadioButton, BM_SETCHECK, 0, 0);
			break;
		case 12:
			hdc = GetDC(hWnd);
			SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));	
			SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
		
			iCheck = (int)SendMessage(hCheckBox, BM_GETCHECK, 0, 0);
			if (iCheck) {
				TextOut(hdc, cxChar*10, cyChar*4, TEXT("ѡ����"), 3);
			} else {
				TextOut(hdc, cxChar*10, cyChar*4, TEXT("δѡ��"), 3);
			}

			ReleaseDC(hWnd, hdc);
			break;
		case 20:
			SetWindowText(hButton, TEXT("hello"));
			SetWindowText(hCheckBox, TEXT("hello"));
			SetWindowText(hRadioButton, TEXT("hello"));
			break;
		case 21:
			hdc = GetDC(hWnd);
			SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
			SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
			GetWindowText(hButton, szBuffer, 1024);
			TextOut(hdc, cxChar*35, cyChar*4, szBuffer, lstrlen(szBuffer));
			ReleaseDC(hWnd, hdc);
			break;
		case 30:
			ShowWindow(hButton, !IsWindowVisible(hButton));
			ShowWindow(hCheckBox, !IsWindowVisible(hCheckBox));
			ShowWindow(hRadioButton, !IsWindowVisible(hRadioButton));
			break;
		case 31:
			EnableWindow(hButton, !IsWindowEnabled(hButton));
			EnableWindow(hCheckBox, !IsWindowEnabled(hCheckBox));
			EnableWindow(hRadioButton, !IsWindowEnabled(hRadioButton));
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
