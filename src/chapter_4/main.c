// demo.cpp : 定义应用程序的入口点。
//

#include <windows.h>
#include <tchar.h>
#include "data.h"

#define MAX_LOADSTRING  100
#define LINEHEIGHT      15
#define MAX_LINE        100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING] = _T("myClass");			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
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
	wcex.hIcon			= NULL; //LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_DEMO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL; //LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, _T("title"),
      WS_OVERLAPPEDWINDOW | WM_VSCROLL,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
	   DWORD retval = GetLastError();
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

#include <tlhelp32.h>

#pragma comment(lib, "Kernel32.lib")

int KillProcess(LPCTSTR lpszProcessName)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	BOOL bRun = Process32First(hSnapshot, &pe32);
	while (bRun)
	{
		if (!_tcscmp(pe32.szExeFile, lpszProcessName))
		{
			DWORD proid = pe32.th32ProcessID;
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proid);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 0);
				WaitForSingleObject(hProcess, INFINITE);
				CloseHandle(hProcess);
				CloseHandle(hSnapshot);
				return 1;
			}
			DWORD dwError = GetLastError();
			CloseHandle(hProcess);
		}
		bRun = Process32Next(hSnapshot, &pe32);
	}
	CloseHandle(hSnapshot);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, 
                           UINT message, 
                           WPARAM wParam, 
                           LPARAM lParam)
{
    TEXTMETRIC tm;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szBuffer[1024];
    static int iVscrollPos;
    static int cyClient;
    static int cxChar, cxCaps, cyChar;
	SCROLLINFO si;

	int iVertPos = 0;
	int iPaintBeg, iPaintEnd;

    ULONG ulScrollLines;    // 行数
    static int iDeltaPerLine;    // 每一行 滚轮滚动的标准距离
    static int iAccumDelta; // 累加



	switch (message)
	{
    case WM_SIZE:
        cyClient = HIWORD(lParam);

		si.cbSize = sizeof(si);
		si.fMask  = SIF_RANGE | SIF_PAGE;
		si.nPage  = cyClient/cyChar;
		si.nMin	  = 0;
		si.nMax	  = NUMLINES-1;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
        break;
    case WM_CREATE:
        hdc = GetDC(hWnd);
        GetTextMetrics(hdc, &tm);
        cyChar = tm.tmHeight + tm.tmExternalLeading;
        cxCaps = ((tm.tmPitchAndFamily & 1) ? 3 : 2) * cxChar / 2;
        cxChar = tm.tmAveCharWidth;
        ReleaseDC(hWnd, hdc);

		iVertPos = 0;

        SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0);
        if (ulScrollLines) {
            iDeltaPerLine = WHEEL_DELTA/ulScrollLines;
        } else {
            iDeltaPerLine = 0;
        }
        break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
        TCHAR szBuffer[1024] = {0};
    
		/*
		int y = 0;
        for (int i=0; i<NUMLINES; ++i) {
			y = cyChar * (i-iVscrollPos);
            TextOut(hdc, 0, y, 
                    sysmetrics[i].szLabel,
                    lstrlen(sysmetrics[i].szLabel));
            
            TextOut(hdc, 22*cxChar+10*cxChar, y, 
                    sysmetrics[i].szDesc,
                    lstrlen(sysmetrics[i].szDesc));

            SetTextAlign(hdc, TA_RIGHT | TA_TOP);
            TextOut(hdc, 22*cxCaps+70*cxChar, y, 
                    szBuffer, 
                    wsprintf(szBuffer, 
                            TEXT("%5d"),
                            GetSystemMetrics(sysmetrics[i].iIndex))
                    );
            SetTextAlign(hdc, TA_LEFT | TA_TOP);
        }
		*/

		// 更加优化的做法，就是只需要画出一个客户区的大小，不需要全部绘制
		// 这样效率会更高
		si.cbSize	= sizeof(si);
		si.fMask	= SIF_POS;	
		GetScrollInfo(hWnd, SB_VERT, &si);
		iVertPos	= si.nPos;

		iPaintBeg = max(0, iVertPos+ps.rcPaint.top / cyChar);
		iPaintEnd = min(NUMLINES-1, iVertPos+ps.rcPaint.bottom / cyChar);

		int y = 0;
		for (int i= iPaintBeg; i<= iPaintEnd; ++i) {
			y = cyChar*(i-iVertPos);	
            TextOut(hdc, 0, y, 
                    sysmetrics[i].szLabel,
                    lstrlen(sysmetrics[i].szLabel));
            
            TextOut(hdc, 22*cxChar+10*cxChar, y, 
                    sysmetrics[i].szDesc,
                    lstrlen(sysmetrics[i].szDesc));

            SetTextAlign(hdc, TA_RIGHT | TA_TOP);
            TextOut(hdc, 22*cxCaps+70*cxChar, y, 
                    szBuffer, 
                    wsprintf(szBuffer, 
                            TEXT("%5d"),
                            GetSystemMetrics(sysmetrics[i].iIndex))
                    );
            SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}

		EndPaint(hWnd, &ps);
		break;
    case WM_MOUSEWHEEL:
        if (0 == iDeltaPerLine) {
            return 0;
        }

        iAccumDelta += (short)HIWORD(wParam);

        while (iAccumDelta >= iDeltaPerLine) {
            SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0);
            iAccumDelta -= iDeltaPerLine;
        }

        while (iAccumDelta <= iDeltaPerLine) {
            SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
            iAccumDelta += iDeltaPerLine;
        }

        break;
    case WM_VSCROLL:
		si.cbSize	= sizeof(si);
		si.fMask	= SIF_ALL;
		GetScrollInfo(hWnd, SB_VERT, &si);

        switch (LOWORD(wParam))
        {
        case SB_LINEUP:
            // iVscrollPos -= 1;
			si.nPos -= 1;
            break;
        case SB_LINEDOWN:
            // iVscrollPos += 1;
			si.nPos += 1;
            break;
        case SB_PAGEDOWN:
            // iVscrollPos += cyClient/cyChar;
			si.nPos += si.nPage;
            break;
        case SB_PAGEUP:
            // iVscrollPos -= cyClient/cyChar;
			si.nPos -= si.nPage;
            break;
        case SB_THUMBPOSITION:
            // iVscrollPos = HIWORD(wParam);
			si.nPos = si.nTrackPos;
            break;
        case SB_TOP:
            si.nPos = 0;
            break;
        case SB_BOTTOM:
            si.nPos = si.nMax;
            break;
        default:
            break;
        }

		si.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hWnd, SB_VERT, &si);

		if (si.nPos != iVertPos) {
			ScrollWindow(hWnd, 0, cyChar*(iVertPos-si.nPos), NULL, NULL);
			UpdateWindow(hWnd);
		}

//        iVscrollPos = max(0, min(iVscrollPos, MAX_LINE-1)); 
//        if (iVscrollPos != GetScrollPos(hWnd, SB_VERT)) {
//            SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
//            InvalidateRect(hWnd, NULL, TRUE);
//			UpdateWindow(hWnd);
//        }
        break;
    case WM_KEYDOWN:
        switch (wParam){
            case VK_DOWN:
                SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
                break;
            case VK_UP:
                KillProcess(L"FlashHelperService.exe");
                SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0);
                break;
            case VK_NEXT:
                SendMessage(hWnd, WM_VSCROLL, SB_PAGEDOWN, 0);
                break;
            case VK_PRIOR:
                SendMessage(hWnd, WM_VSCROLL, SB_PAGEUP, 0);
                break;
            case VK_HOME:
                SendMessage(hWnd, WM_VSCROLL, SB_TOP, 0);
                break;
            case VK_END:
                SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM, 0);
                break;
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

LRESULT CALLBACK WndProc03(HWND hWnd, 
                           UINT message, 
                           WPARAM wParam, 
                           LPARAM lParam)
{

    TEXTMETRIC tm;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szBuffer[1024];

    static int iVscrollPos;
    static int cyChar;  // 字符高度，行高，行距
    static int cyClient;

	switch (message)
	{
    case WM_SIZE:
        cyClient = HIWORD(lParam);
        break;
    case WM_CREATE:
        hdc = GetDC(hWnd);
        GetTextMetrics(hdc, &tm);
        cyChar = tm.tmHeight + tm.tmExternalLeading;
        ReleaseDC(hWnd, hdc);

        iVscrollPos = 0;
        SetScrollRange(hWnd, SB_VERT, 0, MAX_LINE-1, FALSE);
        SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
        break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
        TCHAR szBuffer[1024] = {0};
    
        for (int i=0; i<MAX_LINE; ++i) {
            int x = 0;
            int y = cyChar *(i-iVscrollPos);
            _sntprintf_s(szBuffer, 1024, 1024, TEXT("hello %d"), i);
            TextOut(hdc, x, y, szBuffer, lstrlen(szBuffer));
        }

		EndPaint(hWnd, &ps);
		break;
    case WM_VSCROLL:
        switch (LOWORD(wParam))
        {
        case SB_LINEUP:
            iVscrollPos -= 1;
            break;
        case SB_LINEDOWN:
            iVscrollPos += 1;
            break;
        case SB_PAGEDOWN:
            iVscrollPos += cyClient/cyChar;
            break;
        case SB_PAGEUP:
            iVscrollPos -= cyClient/cyChar;
            break;
        case SB_THUMBPOSITION:
            iVscrollPos = HIWORD(wParam);
            break;
        default:
            break;
        }

        iVscrollPos = max(0, min(iVscrollPos, MAX_LINE-1)); 
        if (iVscrollPos != GetScrollPos(hWnd, SB_VERT)) {
            SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
            InvalidateRect(hWnd, NULL, TRUE);
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


LRESULT CALLBACK WndProc02(HWND hWnd, 
                           UINT message, 
                           WPARAM wParam, 
                           LPARAM lParam)
{
    static int cyChar;  // 字符高度，行高，行距
    static int cxChar;
    static int cxCaps;

    static int cxScreen;
    static int cyScreen;

    TEXTMETRIC tm;

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szBuffer[1024];

	switch (message)
	{
    case WM_CREATE:
        hdc = GetDC(hWnd);
        GetTextMetrics(hdc, &tm);
        cxChar = tm.tmAveCharWidth;
        cyChar = tm.tmHeight + tm.tmExternalLeading;
        cxCaps = ((tm.tmPitchAndFamily & 1) ? 3 : 2) * cxChar/2;
        ReleaseDC(hWnd, hdc);

        cxScreen = GetSystemMetrics(SM_CXSCREEN);
        cyScreen = GetSystemMetrics(SM_CYSCREEN);
        break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 0, 0, TEXT("SM_CXSCREEN"), lstrlen(TEXT("SM_CXSCREEN")));
        TextOut(hdc, 22*cxCaps, 0, TEXT("Screen width in pixels"),
                lstrlen(TEXT("Screen width in pixels")));

        SetTextAlign(hdc, TA_RIGHT | TA_TOP);
        int ilength = wsprintf(szBuffer, TEXT("%d"), cxScreen);
        TextOut(hdc, 22*cxCaps+40*cxChar, 0, szBuffer, ilength);

        SetTextAlign(hdc, TA_LEFT | TA_TOP);
        TextOut(hdc, 0, cyChar, TEXT("SM_CYSCREEN"), lstrlen(TEXT("SM_CYSCREEN")));
        TextOut(hdc, 22*cxCaps, cyChar, TEXT("Screen higth in pixels"),
                lstrlen(TEXT("Screen hight in pixels")));

        SetTextAlign(hdc, TA_RIGHT | TA_TOP);
        ilength = wsprintf(szBuffer, TEXT("%d"), cyScreen);
        TextOut(hdc, 22*cxCaps+40*cxChar, cyChar, szBuffer, ilength);

        SetTextAlign(hdc, TA_LEFT | TA_TOP);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc01(HWND hWnd, 
                           UINT message, 
                           WPARAM wParam, 
                           LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szBuffer[1024];

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
//		case IDM_ABOUT:
//			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//			break;
//		case IDM_EXIT:
//			DestroyWindow(hWnd);
//			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...

		for(int i = 0; i < 100; ++i)
		{
			_sntprintf_s(szBuffer,1024, 1024, TEXT("Hello %d"),i+1);
			TextOut(hdc,0,i*LINEHEIGHT,szBuffer,lstrlen(szBuffer));
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
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
