// chapter_5.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "chapter_11.h"

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

void FillListBox(HWND hwndList)
{
	int iLength;
	TCHAR *pVarBlock, *pVarStartBlock;
	TCHAR *pVarBeg, *pVarEnd;
	TCHAR *pVarName;

	pVarStartBlock = pVarBlock = GetEnvironmentStrings();
	while (*pVarBlock) {
		if (*pVarBlock == '=' || ':' == *pVarBlock) {
			pVarBlock++;
			continue;
		}

		pVarBeg = pVarBlock;
		while (*pVarBlock++ != '=') ;
		pVarEnd = pVarBlock-1;
		iLength = pVarEnd-pVarBeg;

		pVarName = (TCHAR *)calloc(iLength+1, sizeof TCHAR);
		CopyMemory(pVarName, pVarBeg, iLength*sizeof TCHAR);
		SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)pVarName);
		free(pVarName);
		while (*pVarBlock++ != '\0');
	}

	FreeEnvironmentStrings(pVarStartBlock);
}

INT_PTR CALLBACK MyAbout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
	int cxChar, cyChar;

	static HWND hwndList, hwndText;
	const static int ID_LIST = 1;
	const static int ID_TEXT = 1;

	int iLength, iIndex;
	TCHAR *pVarName, *pVarValue;

	switch (message)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());

		hwndList = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), NULL,
								  WS_CHILD|WS_VISIBLE|LBS_NOTIFY|LBS_SORT|
								  WS_VSCROLL, cxChar, cyChar*3, 
								  cxChar*26+GetSystemMetrics(SM_CXVSCROLL), 
								  cyChar*15, hWnd, (HMENU)ID_LIST, 
								  (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
								  NULL);

		hwndText =  CreateWindow(TEXT("static"), NULL, WS_CHILD|WS_VISIBLE|
								 SS_LEFT, cxChar, cyChar, 
								 GetSystemMetrics(SM_CXSCREEN), cyChar,
								 hWnd, (HMENU)ID_TEXT, 
								 (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
								 NULL);

		FillListBox(hwndList);
		break;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		if (ID_LIST == wmId && LBN_SELCHANGE == wmEvent) {
			iIndex		= SendMessage(hwndList, LB_GETCURSEL, 0, 0);
			iLength		= SendMessage(hwndList, LB_GETTEXTLEN, iIndex, 0)+1;
			pVarName	= (TCHAR *)calloc(iLength, sizeof TCHAR);
			SendMessage(hwndList, LB_GETTEXT, iIndex, (LPARAM)pVarName);

			iLength = GetEnvironmentVariable(pVarName, NULL, 0);
			pVarValue = (TCHAR *)calloc(iLength, sizeof TCHAR);
			GetEnvironmentVariable(pVarName, pVarValue, iLength);

			SetWindowText(hwndText, pVarValue);
			free(pVarName);
			free(pVarValue);
			return 0;
		}

		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_COLOR_DIALOG), hWnd, MyAbout);
			break;
		case IDM_NEW_DIALOG:
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
