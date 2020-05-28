#ifndef guiframe_qmainframe_h
#define guiframe_qmainframe_h

#include "qwindow.h"
#include "qbutton.h"
#include "graphctrl.h"

#define TEST_TIMER 1

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
    virtual LRESULT OnSize(WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnTimer(WPARAM wParam, LPARAM lParam);

    virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);

public:
    // ²âÊÔÆ½ÆÌºÍÆÌÂú
    void TestBltPaint(HDC hdc);
    // Ë«»º³å»æÍ¼
    void DoubleBufferPaint(HDC hdc);
    // ×ø±ê×ª»»
    void CoordinateCovert(HDC hdc);
    void DrawCurve();
    void DrawTime(HDC hdc);

public:
    QButton m_wndButton;
    CGraphCtrl mGraphCtrl;

    // ²âÊÔ»­×Ö°æ
    BIHUA curBihua;
    BOOL m_bStart;
    CPoint m_ptOrg;
    CGraph mGraph;

#ifdef TEST_TIMER
    CString mTimeStr;
#endif // TEST_TIMER

};

#endif // guiframe_qmainframe_h