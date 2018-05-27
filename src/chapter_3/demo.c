#include <windows.h>
#include <tchar.h>

// ���ڹ��̻ص�����
LRESULT CALLBACK WndProc(HWND hHwnd, 
						 UINT message,
						 WPARAM wParam,
						 LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPreInstance,
                   LPTSTR szlpCmdLine,
                   int nShowCmd)
{
    HWND hwnd; //���ھ��
    MSG msg;    //��Ϣ
    WNDCLASS wc;    // ������

    //1.���һ��������
    wc.style = 0;
    wc.lpfnWndProc  = (WNDPROC)WndProc;
    wc.cbClsExtra   = 0;
    wc.cbWndExtra   = 0;
    wc.hInstance    = hInstance;
    wc.hIcon        = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor      = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = _T("MyWndClass");

    // 2.ע�ᴰ����
    RegisterClass(&wc);

    // 3.��������
    hwnd = CreateWindow(_T("MyWndClass"),
                        _T("Hello SDK Application"), 
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        NULL, 
                        NULL,
                        hInstance,
                        NULL);

    // 4.��ʾ�͸��´���
    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    // 5.������Ϣѭ��
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  //ת�������ڹ���
    }    

    return 0;
}

LRESULT CALLBACK WndProc(HWND hHwnd,
                         UINT message, 
                         WPARAM wParam, 
                         LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect;

    switch (message) {

    case WM_PAINT:
        hdc = BeginPaint(hHwnd, &ps);
        GetClientRect(hHwnd, &rect);
        // Ellipse(hdc, 0, 0, 200, 200);
        // DrawText(hdc, _T("Hello, Windows!"), -1, &rect,
        //         DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        TextOut(hdc, 100, 100, TEXT("hello"), 5);
        SetMapMode(hdc, MM_ISOTROPIC);
        // �Զ���ģʽ
        // 1.��Ҫ�����߼����귶Χ
        // 2.�������豸���귶Χ
        SetWindowExtEx(hdc, 4, 4, NULL);
        SetViewportExtEx(hdc, 8, 8, NULL);
        TextOut(hdc, 100, 100, TEXT("hello"), 5);
        EndPaint(hHwnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hHwnd, message, wParam, lParam);
}