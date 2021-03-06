#ifndef guiframe_qmainframe_h
#define guiframe_qmainframe_h

#include "qwindow.h"
#include "qbutton.h"
#include "qbuttonex.h"
#include "graphctrl.h"
#include "digitclock.h"

// #define TEST_TIMER 1

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
    virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);

public:
    // ����ƽ�̺�����
    void TestBltPaint(HDC hdc);
    // ˫�����ͼ
    void DoubleBufferPaint(HDC hdc);
    // ����ת��
    void CoordinateCovert(HDC hdc);
    void DrawCurve();
    void DrawTime(HDC hdc);

    void AddMenu();

public:
    QButton m_wndButton;
    CGraphCtrl mGraphCtrl;

    // ���Ի��ְ�
    BIHUA curBihua;
    BOOL m_bStart;
    CPoint m_ptOrg;
    CGraph mGraph;

#ifdef TEST_TIMER
    CString mTimeStr;
#endif // TEST_TIMER

    DigitClock mDigitClock;

public:
	// ��׼�ؼ�
	QButtonEx m_wndButtonEx;
	QWindow m_wndButton2;
	QWindow m_wndEdit;
	QWindow m_wndListBox;
	QWindow m_wndScrollBar;
	QWindow m_wndStatic;

	// ͨ�ÿؼ�
	QWindow m_wndTreeCtrl;

};

#endif // guiframe_qmainframe_h