#include "qbutton.h"

QButton::QButton()
{
}

QButton::~QButton()
{

}

BOOL QButton::CreateEx(LPCTSTR lpszWindowName,
                        DWORD dwStyle,
                        int x,
                        int y,
                        int nWidth,
                        int nHeight,
                        HWND hWndParent,
                        HMENU nIDorHMenu,
                        LPVOID lpParam)
{
    return QWindow::CreateEx(0, _T("BUTTON"), lpszWindowName, 
                                dwStyle, x, y, nWidth, nHeight, 
                                hWndParent, nIDorHMenu, lpParam);
}
