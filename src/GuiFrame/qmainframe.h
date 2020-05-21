#ifndef guiframe_qmainframe_h
#define guiframe_qmainframe_h

#include "qwindow.h"

class QMainFrame : public QWindow
{
public:
    QMainFrame();
    virtual ~QMainFrame();

public:
    virtual LRESULT OnClose(WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);

};

#endif // guiframe_qmainframe_h