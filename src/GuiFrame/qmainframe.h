#ifndef guiframe_qmainframe_h
#define guiframe_qmainframe_h

#include "qwindow.h"
#include "qbutton.h"
#include "graphctrl.h"

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

public:
    // ����ƽ�̺�����
    void TestBltPaint(HDC hdc);
    // ˫�����ͼ
    void DoubleBufferPaint(HDC hdc);
    // ����ת��
    void CoordinateCovert(HDC hdc);
    void DrawCurve();

public:
    QButton m_wndButton;
    CGraphCtrl mGraphCtrl;

};

#endif // guiframe_qmainframe_h