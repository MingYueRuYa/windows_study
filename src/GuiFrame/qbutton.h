#ifndef guiframe_qbutton_h
#define guiframe_qbutton_h

#include "qwindow.h"

class QButton : public QWindow
{
public:
    QButton();
    ~QButton();

public:
    virtual BOOL CreateEx(LPCTSTR lpszWindowName,
                        DWORD dwStyle,
                        int x,
                        int y,
                        int nWidth,
                        int nHeight,
                        HWND hWndParent,
                        HMENU nIDorHMenu,
                        LPVOID lpParam = NULL);
};

#endif // guiframe_qbutton_h