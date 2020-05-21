#include "qmainframe.h"

QMainFrame::QMainFrame()
{
}

QMainFrame::~QMainFrame()
{
}

LRESULT QMainFrame::OnClose(WPARAM wParam, LPARAM lParam)
{
    return DestroyWindow();
}

LRESULT QMainFrame::OnDestroy(WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
    return TRUE;
}

LRESULT QMainFrame::OnCreate(WPARAM wParam, LPARAM lParam)
{
    return TRUE;
}
