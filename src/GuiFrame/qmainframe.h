#ifndef guiframe_qmainframe_h
#define guiframe_qmainframe_h

#include "qwindow.h"
#include "qbutton.h"

class QMainFrame : public QWindow
{
public:
    QMainFrame();
    virtual ~QMainFrame();

public:
    virtual LRESULT OnClose(WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnDestroy(WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnPaint(WPARAM wParam, LPARAM lParam);

public:
    // ²âÊÔÆ½ÆÌºÍÆÌÂú
    void TestBltPaint(HDC hdc);
    // Ë«»º³å»æÍ¼
    void DoubleBufferPaint(HDC hdc);

public:
    QButton m_wndButton;

};

#endif // guiframe_qmainframe_h