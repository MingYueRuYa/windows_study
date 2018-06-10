// nettime.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "nettime.h"

#include <winsock.h>

#define MAX_LOADSTRING		100
#define ID_TIMER			1
#define WM_SOCKET_NOTIFY	(WM_USER+1)

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

TCHAR szAppName[] = TEXT("NetTime");

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	MainDlg(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	ServerDlg(HWND, UINT, WPARAM, LPARAM);
void EditPrintf(HWND hwndEdit, TCHAR *szFormat, ...);
void ChangeSystemTime(HWND hwndEdit, ULONG ulTime);
void FormatUpdateTime(HWND hwndEdit, SYSTEMTIME *pstOld, SYSTEMTIME *pstNew);

HWND hwndModeless;

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NETTIME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NETTIME));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NETTIME));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCE(IDC_NETTIME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	RECT rect;

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	hwndModeless = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINDLG), hWnd, MainDlg);
	GetWindowRect(hwndModeless, &rect);
	AdjustWindowRect(&rect, WS_CAPTION | WS_BORDER, FALSE);

	SetWindowPos(hWnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);

	ShowWindow(hwndModeless, SW_SHOW);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
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

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
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
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
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

INT_PTR CALLBACK MainDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	static char szIPAddr[32] = { "96.47.67.105" };
	static HWND hwndEdit, hwndButton;
	static SOCKET sock;
	static TCHAR szOKLabel[32];
	static struct sockaddr_in sa;
	unsigned long ulTime;
	WORD wEvent, wError;

	WSADATA WSAData;
	int iError, iSize;

	switch (message)
	{
	case WM_INITDIALOG:
		hwndEdit	= GetDlgItem(hDlg, IDC_TEXTOUT);
		hwndButton	= GetDlgItem(hDlg, IDC_OK);
		return (INT_PTR)TRUE;
	case WM_SOCKET_NOTIFY:
		wEvent = WSAGETSELECTEVENT(lParam); // ie. LOWORD
		wError = WSAGETSELECTERROR(lParam); // ie. HIWORD

		switch (wEvent) {
		case FD_CONNECT:
			EditPrintf(hwndEdit, TEXT("\r\n"));
			if (wError) {
				KillTimer(hDlg, ID_TIMER);
				EditPrintf(hwndEdit, TEXT("Connect error #%i.\r\n"), wError);
				SendMessage(hDlg, WM_COMMAND, IDC_CANCEL, 0);
				return TRUE;
			}
			EditPrintf(hwndEdit, TEXT("Connected to %hs.\r\n"), szIPAddr);
			recv(sock, (char *)&ulTime, 4, MSG_PEEK);
			EditPrintf(hwndEdit, TEXT("Waiting to receive..."));
			break;
		case FD_READ:
			KillTimer(hDlg, ID_TIMER);
			EditPrintf(hwndEdit, TEXT("\r\n"));
	
			if (wError) {
				EditPrintf(hwndEdit, TEXT("FD_READ error #%i"), wError);
				SendMessage(hDlg, WM_COMMAND, IDC_CANCEL, 0);
				return FALSE;
			}

			iSize	= recv(sock, (char *)&ulTime, 4, 0);
			ulTime	= ntohl(ulTime);
			EditPrintf(hwndEdit, TEXT("Received current time of %u seconds ")
								 TEXT("since Jan. 1 1900.\r\n"), ulTime);
			
			ChangeSystemTime(hwndEdit, ulTime);
			SendMessage(hDlg, WM_COMMAND, IDC_CANCEL, 0);

			break;
		}

		break;
	case WM_TIMER:
		EditPrintf(hwndEdit, TEXT("."));
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_SERVER:
			DialogBoxParam(hInst, TEXT("Servers"), hDlg, ServerDlg, 
							(LPARAM)szIPAddr);
			return TRUE;
		case IDC_OK:
			if (iError = WSAStartup(MAKEWORD(2, 0), &WSAData)) {
				EditPrintf(hwndEdit, TEXT("Startup error #%i.\r\n"), iError);
				return FALSE;
			}
			
			EditPrintf(hwndEdit, TEXT("Startup %hs\r\n"),
						WSAData.szDescription);

			sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sock == INVALID_SOCKET) {
				EditPrintf(hwndEdit, TEXT("Socket creation error #%i.\r\n"),
							WSAGetLastError());
				WSACleanup();
				return FALSE;
			}
			EditPrintf(hwndEdit, TEXT("Socket %i created.\r\n"), sock);

			if (SOCKET_ERROR == WSAAsyncSelect(sock, hDlg, WM_SOCKET_NOTIFY,
												FD_CONNECT|FD_READ)) {
				EditPrintf(hwndEdit, TEXT("WSAAsyncSelect error #%i.\r\n"),
							WSAGetLastError());
				SendMessage(hDlg, WM_COMMAND, IDC_CANCEL, 0);
				return FALSE;
			}

			sa.sin_family	= AF_INET;
			sa.sin_port		= htons(IPPORT_TIMESERVER);
			sa.sin_addr.S_un.S_addr = inet_addr(szIPAddr);
			connect(sock, (SOCKADDR *)&sa, sizeof(sa));
			SetTimer(hDlg, ID_TIMER, 1000, NULL);

			GetWindowText(hwndButton, szOKLabel, 
							sizeof(szOKLabel)/sizeof(TCHAR));

			SetWindowText(hwndButton, TEXT("Cancel"));	
			SetWindowLong(hwndButton, GWL_ID, IDC_CANCEL);	

			return TRUE;
		case IDC_CANCEL:
			closesocket(sock);
			sock = 0;
			WSACleanup();
			KillTimer(hDlg, ID_TIMER);
			EditPrintf(hwndEdit, TEXT("\r\nSocket close.\r\n"));

			SetWindowText(hwndButton, szOKLabel);	
			SetWindowLong(hwndButton, GWL_ID, IDC_OK);	

			break;
		case IDC_CLOSE:
			if (0 != sock) {
				SendMessage(hDlg, WM_COMMAND, IDC_CANCEL, 0);
			}
			DestroyWindow(GetParent(hDlg));
			return TRUE;
		}

		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ServerDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	static char *szServer;
	static WORD wServer = IDC_SERVER1;
	char szLabel[128];

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		szServer = (char *)lParam;
		CheckRadioButton(hDlg, IDC_SERVER1, IDC_SERVER6, IDC_SERVER1);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_SERVER1:
		case IDC_SERVER2:
		case IDC_SERVER3:
		case IDC_SERVER4:
		case IDC_SERVER5:
		case IDC_SERVER6:
			wServer = LOWORD(wParam);
			return TRUE;
		case IDC_OK:
		{
			GetDlgItemTextA(hDlg, wServer, szLabel, sizeof(szLabel));
			char *next_token1 = NULL;
			strtok_s(szLabel, "(", &next_token1);
			strcpy_s(szServer, 32, strtok_s(NULL, ")", &next_token1));
			EndDialog(hDlg, TRUE);
		}
			break;
		case IDC_CANCEL:
			EndDialog(hDlg, FALSE);
			return TRUE;
		}

		break;
	}
	return (INT_PTR)FALSE;
}

void EditPrintf(HWND hwndEdit, TCHAR *szFormat, ...)
{
	TCHAR szBuffer[1024];
	va_list pArgList;
	va_start(pArgList, szFormat);
	wvsprintf(szBuffer, szFormat, pArgList);
	va_end(pArgList);

	SendMessage(hwndEdit, EM_SETSEL, (WPARAM)-1, (LPARAM)-1);
	SendMessage(hwndEdit, EM_REPLACESEL, FALSE, (LPARAM)szBuffer);
	SendMessage(hwndEdit, EM_SCROLLCARET, 0, 0);
}

void ChangeSystemTime(HWND hwndEdit, ULONG ulTime)
{
	FILETIME ftNew;
	SYSTEMTIME stOld, stNew;
	LARGE_INTEGER li;

	GetLocalTime(&stOld);
	stNew.wYear		= 1900;
	stNew.wMonth	= 1;
	stNew.wDay		= 1;
	stNew.wHour		= 0;
	stNew.wMinute	= 0;
	stNew.wSecond	= 0;
	stNew.wMilliseconds	= 0;

	SystemTimeToFileTime(&stNew, &ftNew);
	li			= *(LARGE_INTEGER *)&ftNew;
	li.QuadPart += (LONGLONG)10000000 * ulTime;
	ftNew		= *(FILETIME *)&li;
	FileTimeToSystemTime(&ftNew, &stNew);

	if (SetSystemTime(&stNew)) {
		GetLocalTime(&stNew);
		FormatUpdateTime(hwndEdit, &stOld, &stNew);
	} else {
		EditPrintf(hwndEdit, TEXT("Could NOT set new data and time,  \
			error code %i."), GetLastError());
	}
}

void FormatUpdateTime(HWND hwndEdit, SYSTEMTIME *pstOld, SYSTEMTIME *pstNew)
{
	TCHAR szDateOld[64], szTimeOld[64], szDateNew[64], szTimeNew[64];
	GetDateFormat(LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE|DATE_SHORTDATE,
					pstOld, NULL, szDateOld, sizeof(szDateOld));
	GetDateFormat(LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE|DATE_SHORTDATE,
					pstOld, NULL, szDateNew, sizeof(szDateNew));
	
	GetTimeFormat(LOCALE_USER_DEFAULT, 
					LOCALE_NOUSEROVERRIDE|TIME_FORCE24HOURFORMAT, pstOld,
					NULL, szTimeOld, sizeof(szTimeOld));

	GetTimeFormat(LOCALE_USER_DEFAULT, 
					LOCALE_NOUSEROVERRIDE|TIME_FORCE24HOURFORMAT, pstNew,
					NULL, szTimeNew, sizeof(szTimeNew));

	EditPrintf(hwndEdit,
				TEXT("System data and time successfully changed \
				from\r\n\t%s, %s.%03i to\r\n\t %s, %s.%03.i.\r\n\r\n"),
				szDateOld, szTimeOld, pstOld->wMilliseconds,
				szDateNew, szTimeNew, pstNew->wMilliseconds);

}
