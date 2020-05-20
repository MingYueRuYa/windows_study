#include <windows.h>
#include <tchar.h>
#include <cstdbool>


bool IsFullScreen()
{
    HWND fore_wnd = GetForegroundWindow();

    if (fore_wnd == GetDesktopWindow() || fore_wnd == GetShellWindow()) {
        return false;
    } 

    RECT desktop_rect;
    GetWindowRect(GetDesktopWindow(), &desktop_rect);

    RECT wnd_rect;
    GetWindowRect(fore_wnd, &wnd_rect);

    if (desktop_rect.left == wnd_rect.left &&
        desktop_rect.right == wnd_rect.right &&
        desktop_rect.top == wnd_rect.top &&
        desktop_rect.bottom == wnd_rect.bottom  ) {
        return true;
    }

    return false;
}


void EnumAllWindow()
{
    EnumWindows();
}


int main(int argc, TCHAR *argv[])
{
//    bool isfull = IsFullScreen();
//
//    printf(isfull ? "fulled" : "not fulled");

    system("pause");

    return 0;
}