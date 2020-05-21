#include "qwindemoapp.h"
#include "qmainframe.h"

QWinDemoApp::QWinDemoApp()
{
}

QWinDemoApp::~QWinDemoApp()
{
}

BOOL QWinDemoApp::InitInstance()
{
    QMainFrame *frame = new QMainFrame();
    assert(frame);

    m_pMainWnd = frame;

    BOOL bRet = frame->CreateEx(0, _T("GuiFrame"), _T("window GUI"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL);

    if (! bRet) { 
        MessageBox(NULL, _T("创建窗口失败"), _T("创建窗口"), 0);
        return false;
    }

    frame->ShowWindow(SW_SHOW);
    frame->UpdateWindow();

    return TRUE;
}

BOOL QWinDemoApp::ExitInstance()
{
    if (m_pMainWnd) {
        delete (QWindow *)m_pMainWnd;
        m_pMainWnd = NULL;
    }
    return TRUE;
}

QWinDemoApp theApp;
