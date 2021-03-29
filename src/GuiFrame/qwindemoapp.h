#ifndef guiframe_qwindemoapp_h
#define guiframe_qwindemoapp_h

#include "qwinapp.h"
#include "globalhead.h"

class QWinDemoApp : public QWinApp
{
public:
    QWinDemoApp();
    ~QWinDemoApp();

public:
    virtual BOOL InitInstance();
    virtual BOOL ExitInstance();
};

extern QWinDemoApp theApp;

#endif // guiframe_qwindemoapp_h