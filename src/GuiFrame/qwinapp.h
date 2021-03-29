#ifndef guiframe_qwinapp_h
#define guiframe_qwinapp_h

#include <Windows.h>

class QWinApp
{
public:
    QWinApp();
    ~QWinApp();

public:
    virtual BOOL InitInstance();
    virtual BOOL ExitInstance();
    virtual void run();

public:
    LPVOID m_pMainWnd;

};

#endif // guiframe_qwinapp_h