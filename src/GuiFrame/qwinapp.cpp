#include "qwinapp.h"
#include "global.h"

QWinApp::QWinApp()
{
    m_pMainWnd = NULL;
    g_pWinApp = this;
}

QWinApp::~QWinApp()
{
}

BOOL QWinApp::InitInstance()
{
    return TRUE;
}

BOOL QWinApp::ExitInstance()
{
    return TRUE;
}

void QWinApp::run()
{
    MSG msg;
    while (::GetMessage(&msg, NULL, NULL, NULL)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
}
