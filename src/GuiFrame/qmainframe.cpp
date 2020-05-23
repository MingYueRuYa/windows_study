#include "qmainframe.h"
#include "resource.h"

#define IDC_BUTTON 10001

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
    if (NULL == m_wndButton.m_hWnd) { 
        m_wndButton.CreateEx(_T("QButton Demo"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 0, 0, 200, 120, m_hWnd, (HMENU)IDC_BUTTON);
    }

    return TRUE;
}

LRESULT QMainFrame::OnPaint(WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = ::BeginPaint(m_hWnd, &ps);

    // TestBltPaint(hdc);
    DoubleBufferPaint(hdc);

    ::EndPaint(m_hWnd, &ps);
    return 0;
}

void QMainFrame::TestBltPaint(HDC hdc)
{
    RECT rcClient;
    GetClientRect(&rcClient);

    HDC hMemDC = ::CreateCompatibleDC(hdc);
    HBITMAP bitmap = (HBITMAP)::LoadBitmap((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));

    BITMAP bm;
    GetObject((HGDIOBJ)bitmap, sizeof(BITMAP), &bm);

    HBITMAP oldbitmap = (HBITMAP)::SelectObject(hMemDC, bitmap);

    /*
    // 平铺效果
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            ::BitBlt(hdc, rcClient.left+bm.bmWidth*i, rcClient.top+bm.bmHeight*j, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top,
                hMemDC, 0, 0, SRCCOPY);
        }
    }
    */

    // 拉伸
    ::StretchBlt(hdc, rcClient.left, rcClient.top, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    // ::SelectObject(hMemDC, oldbitmap);

    ::DeleteObject((HGDIOBJ)bitmap);
    ::DeleteDC(hMemDC);
}

void QMainFrame::DoubleBufferPaint(HDC hdc)
{
    RECT rcClient;
    GetClientRect(&rcClient);

    // 双缓冲绘制
    /*
    HDC hMemDC = ::CreateCompatibleDC(hdc);

    HBITMAP compatible_bitmap = ::CreateCompatibleBitmap(hdc, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top);
    HBITMAP oldbitmap = (HBITMAP)::SelectObject(hMemDC, compatible_bitmap);
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            SetPixel(hMemDC, i, j, RGB(0, 0, 255));
        }
    }

    // ::BitBlt(hdc, rcClient.left, rcClient.top, rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, hMemDC, 0, 0, SRCCOPY);
    ::BitBlt(hdc, rcClient.left+300, rcClient.top, 100, 100, hMemDC, 0, 0, SRCCOPY);

    ::DeleteObject((HGDIOBJ)compatible_bitmap);
    ::DeleteObject(hMemDC);
    */


    // 画长方形和原型
    /*
    HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
    HPEN holdPen = (HPEN)::SelectObject(hdc, hPen);

    HBRUSH hnewBrush = (HBRUSH)::CreateSolidBrush(RGB(0, 255, 0));
    HBRUSH holdBrush = (HBRUSH)::SelectObject(hdc, hnewBrush);

    // ::Rectangle(hdc, 300, 0, 400, 100);
    ::Ellipse(hdc, 300, 0, 400, 100);

    ::SelectObject(hdc, hnewBrush);
    ::DeleteObject(hnewBrush);

    ::SelectObject(hdc, holdPen);
    ::DeleteObject(hPen);
    */

    // 画直线
    /*
    HPEN hPen = CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));
    HPEN holdPen = (HPEN)::SelectObject(hdc, (HGDIOBJ)hPen);
    MoveToEx(hdc, 0, 0, NULL);
    LineTo(hdc, rcClient.right, rcClient.bottom);
    ::SelectObject(hdc, holdPen);
    ::DeleteObject((HGDIOBJ)hPen);
    */

    // 画像素点
    /*
    for (int i = 300; i < 400; ++i) {
        for (int j = 0; j < 100; ++j) {
            SetPixel(hdc, i, j, RGB(0, 0, 255));
        }
    }
    */

    // 设置文字的背景色和颜色
    /*
    COLORREF oldTextColor = SetTextColor(hdc, RGB(255, 0, 0));
    COLORREF oldbkColor = SetBkColor(hdc, RGB(0, 255, 0));
    // 画文字
    DrawText(hdc, L"我是中国人", _tcslen(L"我是中国人"), &rcClient, DT_CENTER | DT_VCENTER | DT_WORD_ELLIPSIS | DT_SINGLELINE);
    SetBkColor(hdc, oldbkColor);
    SetTextColor(hdc, oldTextColor);
    */

    // 创建字体绘制文字
    /*
    HFONT hFont = CreateFont(
    200						// nHeight
    , 0						// nWidth
    , 0						// nEscapement
    , 0						// nOrientation
    , 500						// nWeight
    , FALSE					// nItalic
    , FALSE					// nUnderline
    , 0						// cStrikeOut
    , ANSI_CHARSET			// nTCHARSet
    , OUT_DEFAULT_PRECIS	// nOutPrecision
    , CLIP_DEFAULT_PRECIS	// nClipPrecision
    , DEFAULT_QUALITY		// nQuality
    , DEFAULT_PITCH|FF_SWISS// nPitchAndFamily
    , _T("arial") );        // lpszFacename


    HFONT hOldFont = (HFONT)::SelectObject(hdc, (HGDIOBJ)hFont);
    DrawText(hdc, L"我是中国人", _tcslen(L"我是中国人"), &rcClient, DT_CENTER | DT_VCENTER | DT_WORD_ELLIPSIS | DT_SINGLELINE);
    // TextOut(hdc, 0, 0,_T("www.itxueba.org"), _tcslen(_T("www.itxueba.org"))) ;
    ::SelectObject(hdc, hOldFont);
    ::DeleteObject((HFONT)hFont);
    */
}
