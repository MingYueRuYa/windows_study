#include "qmainframe.h"
#include "resource.h"
#include "registerfile.h"

#include <string>

using std::wstring;

#define IDC_BUTTON      10001
#define IDC_GRAPHCTRL   10002
#define IDC_CLOCK       10003
#define IDT_TIMER       1

QMainFrame::QMainFrame()
{
    wchar_t full_name[MAX_PATH] = {0};
    GetModuleFileNameW(NULL, full_name, MAX_PATH);
    wstring exe_path = wstring(full_name) + wstring(L" \"%%1\"");
    RegisterFile file(_T(".cusext"), _T("cusext"), _T(""), exe_path);
    bool is_register = file.IsRegister();

    file.RegistFile();
}

QMainFrame::~QMainFrame()
{
}

LRESULT QMainFrame::OnClose(WPARAM wParam, LPARAM lParam)
{
    mGraph.SetGraghToFile(L"a.graph");
    return DestroyWindow();
}

LRESULT QMainFrame::OnDestroy(WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
    return TRUE;
}

void CALLBACK TimerProc(
                    HWND hWnd,
                    UINT uMsg,
                    UINT idEvent,
                    DWORD dwTime)
{
    QMainFrame *main_frame = (QMainFrame *)QWindow::FromHandle(hWnd);
    if (IDT_TIMER == idEvent) {
#ifdef TEST_TIMER
        CString &str_time = main_frame->mTimeStr;
        SYSTEMTIME st;
        GetLocalTime(&st);
        str_time.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
        InvalidateRect(hWnd, NULL, TRUE);
#endif // TEST_TIMER
    }
}

LRESULT QMainFrame::OnCreate(WPARAM wParam, LPARAM lParam)
{
    // 创建button
    /*
    if (NULL == m_wndButton.m_hWnd) { 
        m_wndButton.CreateEx(_T("QButton Demo"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 0, 0, 200, 120, m_hWnd, (HMENU)IDC_BUTTON);
    }
    */

    // DrawCurve();

    // 加载数据
    // mGraph.GetGraghFromFile(L"a.graph");

    // timer有两种方式
    // 1.第一种指定TimerProc函数
    // SetTimer(m_hWnd, IDT_TIMER, 200, TimerProc);
    // 2.第一种没有指定TimerProc函数
#ifdef TEST_TIMER
    SetTimer(m_hWnd, IDT_TIMER, 200, NULL);
#endif // TEST_TIMER

    mDigitClock.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 160, 60), m_hWnd, IDC_CLOCK);

    return TRUE;
}

LRESULT QMainFrame::OnPaint(WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = ::BeginPaint(m_hWnd, &ps);

    // TestBltPaint(hdc);
    // DoubleBufferPaint(hdc);
    // CoordinateCovert(hdc);
#ifdef TEST_DRAW_LIEN
    mGraph.Draw(hdc);
#endif // TEST_DRAW_LIEN

#ifdef TEST_TIMER
    DrawTime(hdc);
#endif

    ::EndPaint(m_hWnd, &ps);
    return 0;
}

LRESULT QMainFrame::OnSize(WPARAM wParam, LPARAM lParam)
{
    if(mGraphCtrl.m_hWnd)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		::MoveWindow(mGraphCtrl.m_hWnd, 0, 0, rcClient.Width(), rcClient.Height(), TRUE);
	}
	return QWindow::Default(WM_SIZE, wParam, lParam);
}

LRESULT QMainFrame::OnTimer(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
#ifdef TEST_TIMER 
    case IDT_TIMER:
    {
        SYSTEMTIME st;
		GetLocalTime(&st);
		mTimeStr.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wMonth, st.wMinute, st.wSecond);
		InvalidateRect(m_hWnd, NULL, TRUE);
    }
        break;
#endif // IDT_TIMER
    default:
        break;
    }

    return QWindow::OnTimer(wParam, lParam);
}

LRESULT QMainFrame::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
    SetCapture(m_hWnd);
    m_bStart = TRUE;

    curBihua.clear();

    m_ptOrg.x = LOWORD(lParam);
    m_ptOrg.y = HIWORD(lParam);
    curBihua.push_back(m_ptOrg);

    return QWindow::OnLButtonDown(wParam, lParam);
}

LRESULT QMainFrame::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
    CPoint ptMove;
    ptMove.x = LOWORD(lParam);
    ptMove.y = HIWORD(lParam);

    if (m_bStart) {
        HDC hdc = ::GetDC(m_hWnd);

        MoveToEx(hdc, m_ptOrg.x, m_ptOrg.y, NULL);
        LineTo(hdc, ptMove.x, ptMove.y);
        m_ptOrg = ptMove;

        ::ReleaseDC(m_hWnd, hdc);

        curBihua.push_back(ptMove);
    }

    return QWindow::OnMouseMove(wParam, lParam);
}

LRESULT QMainFrame::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
    ReleaseCapture(); 
    m_bStart = FALSE;
    mGraph.AddBiHua(curBihua);

    return QWindow::OnLButtonUp(wParam,lParam);
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

void QMainFrame::CoordinateCovert(HDC hdc)
{
    POINT pt;
    pt.x = 100;
    pt.y = 300;
    int nWidth = 0;

    // 返回屏幕的像素单位 1920
    int hHortWidth_PX = GetDeviceCaps(hdc, HORZRES);
    // 返回屏幕MM毫米单位
    int nHortWidth_MM = GetDeviceCaps(hdc, HORZSIZE);

    // 返回屏幕的像素单位 1920
    int hVortWidth_PX = GetDeviceCaps(hdc, VERTRES);
    // 返回屏幕MM毫米单位
    int nVortWidth_MM = GetDeviceCaps(hdc, VERTSIZE);

    wstring text = wstring(L"屏幕X轴像素单位") + std::to_wstring(hHortWidth_PX);
    TextOut(hdc, pt.x, pt.y, text.c_str(), text.length());

    pt.y += 20;
    text = wstring(L"屏幕X轴毫米单位") + std::to_wstring(nHortWidth_MM);
    TextOut(hdc, pt.x, pt.y, text.c_str(), text.length());
    pt.y += 20;
    text = wstring(L"屏幕Y轴毫米单位") + std::to_wstring(hVortWidth_PX);
    TextOut(hdc, pt.x, pt.y, text.c_str(), text.length());

    pt.y += 20;
    text = wstring(L"屏幕Y轴毫米单位") + std::to_wstring(nVortWidth_MM);
    TextOut(hdc, pt.x, pt.y, text.c_str(), text.length());

    nWidth = 1000;
    pt.y += 20;
    MoveToEx(hdc, pt.x, pt.y, NULL);
    LineTo(hdc, pt.x+nWidth, pt.y);
    text = wstring(L"默认模式：MM_TEXT");
    TextOut(hdc, pt.x+nWidth, pt.y, text.c_str(), text.length());

    int y_pos = pt.y;
    {
        /*
			MM_LOMETRIC   1个单位 0.1mm
			?(1000)	  100mm
        */
        y_pos += 20;
        pt.y = y_pos;
        int nOldMap = SetMapMode(hdc, MM_LOMETRIC);
        DPtoLP(hdc, &pt, 1);
        MoveToEx(hdc, pt.x, pt.y, NULL);
        LineTo(hdc, pt.x+nWidth, pt.y);
        text = wstring(L"MM_LOMETRIC");
        TextOut(hdc, pt.x+nWidth, pt.y, text.c_str(), text.length());
        SetMapMode(hdc, nOldMap);
    }

    {
        y_pos += 20;
        /*
        MM_LOMETRIC   1个单位 0.01mm
        ?(10000)	  100mm
        */
        pt.x = 100;
        pt.y = y_pos;
        int nOldMap = SetMapMode(hdc, MM_HIMETRIC);
        DPtoLP(hdc, &pt, 1);
        MoveToEx(hdc, pt.x, pt.y, NULL);
        LineTo(hdc, pt.x+nWidth, pt.y);
        TextOut(hdc, pt.x+nWidth, pt.y, _T("MM_HIMETRIC"), _tcslen(_T("MM_HIMETRIC")));
        SetMapMode(hdc, nOldMap);
    }

    {
        /*
        MM_LOMETRIC   1个单位 0.001in (0.01*25.4)
        ?(10000)	  100mm
        */
        y_pos += 20;
        pt.x = 100;
        pt.y = y_pos;
        int nOldMap = SetMapMode(hdc, MM_HIENGLISH);
        DPtoLP(hdc, &pt, 1);
        MoveToEx(hdc, pt.x, pt.y, NULL);
        LineTo(hdc, pt.x+nWidth, pt.y);
        TextOut(hdc, pt.x+nWidth, pt.y, _T("MM_HIENGLISH"), _tcslen(_T("MM_HIENGLISH")));
        SetMapMode(hdc, nOldMap);
    }

    {
        /*
        MM_LOMETRIC   1个单位 1/1440in (1/1440*25.4)
        ?(10000)	  100mm
        */
        y_pos += 20;
        pt.x = 100;
        pt.y = y_pos;
        int nOldMap = SetMapMode(hdc, MM_TWIPS);
        nWidth = 1440*100/25.4;
        DPtoLP(hdc, &pt, 1);
        MoveToEx(hdc, pt.x, pt.y, NULL);
        LineTo(hdc, pt.x+nWidth, pt.y);
        TextOut(hdc, pt.x+nWidth, pt.y, _T("MM_TWIPS"), _tcslen(_T("MM_TWIPS")));
        SetMapMode(hdc, nOldMap);
    }

    {
        /*					1280  320mm
                            400	  100
        MM_ANISOTROPIC   1个单位 0.25MM
    
                    ?(400)	  100mm
        */
        y_pos += 20;
        pt.x = 100;
        pt.y = y_pos;
        int nOldMap = SetMapMode(hdc, MM_ANISOTROPIC);
        // SetViewportExtEx(hdc, 4, 4, NULL);//1280*800  1.25  = 1/8
        // SetWindowExtEx(hdc, 1, 1, NULL);//320*200
        nWidth = 100;
        DPtoLP(hdc, &pt, 1);
        MoveToEx(hdc, pt.x, pt.y, NULL);
        LineTo(hdc, pt.x+nWidth, pt.y);
        TextOut(hdc, pt.x+nWidth, pt.y, _T("MM_ANISOTROPIC"), _tcslen(_T("MM_ANISOTROPIC")));
        SetMapMode(hdc, nOldMap);
    }

    RECT rcClient;
    GetClientRect(&rcClient);

    // 设置设备坐标中心为逻辑坐标的(0, 0)
    // ::SetViewportOrgEx(hdc, (rcClient.right-rcClient.left)/2,(rcClient.bottom-rcClient.top)/2, NULL);
    // TextOut(hdc, 0, 0, _T("MM_ANISOTROPIC"), _tcslen(_T("MM_ANISOTROPIC")));


    /*
    int oldMapMode = SetMapMode(hdc, MM_ANISOTROPIC);
    SetWindowExtEx(hdc, 300, 300, NULL);
    // SetViewportExtEx(hdc, rcClient.right, -rcClient.bottom, NULL);
    SetViewportExtEx(hdc, 600, 300, NULL);
    // 设置设备坐标中心为逻辑坐标的(0, 0)
    ::SetViewportOrgEx(hdc, (rcClient.right-rcClient.left)/2,(rcClient.bottom-rcClient.top)/2, NULL);

    ::Ellipse(hdc, -100, -100, 100, 100);
    */

    int nRate = 10;
    SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, rcClient.right*nRate, -rcClient.bottom*nRate, NULL);
	SetViewportExtEx(hdc, rcClient.right, rcClient.bottom, NULL);  
	SetViewportOrgEx(hdc, rcClient.right/2, rcClient.bottom/2, NULL);
    ::Ellipse(hdc, -100, -100, 100, 100);
}

void QMainFrame::DrawCurve()
{
    if (NULL == mGraphCtrl.m_hWnd) {
        CRect rcClient;
        GetClientRect(&rcClient);

        mGraphCtrl.Create(_T(""), WS_CHILD|WS_VISIBLE, rcClient, this, IDC_GRAPHCTRL);

        {
            CGraph graph;
            graph.SetGraphTitle(_T("正旋曲线"));
            graph.SetGraphColor(0x0000FF);
            for (int i = -10000; i < 10000; ++i) {
                float fX = i;
                float fY = 1000*sin(2*3.141592654*i/1000.0f);
                graph.AddGraphData(fX, fY);
            }

            mGraphCtrl.AddGraph(graph);
        }

        {
            CGraph gragh;
			gragh.SetGraphTitle(_T("混合曲线"));
			gragh.SetGraphColor(0x00FF00);
			for(int i=-10000; i<10000; i++)
			{
				float fX = i;
				float fY = 1000*sin(2*3.1415926*i/1000.0f)+500*cos(3.1415926*i/80.0f);
				gragh.AddGraphData(fX, fY);
			}
			mGraphCtrl.AddGraph(gragh);
        }

        {
            CGraph gragh;
			gragh.SetGraphTitle(_T("圆曲线"));
			gragh.SetGraphColor(0xFF0000);
			for(int i=-10000; i<10000; i++)
			{
				float fX = 1000*cos(2*3.1415926*i/1000.0f);
				float fY = 1000*sin(2*3.1415926*i/1000.0f);
				gragh.AddGraphData(fX, fY);
			}
			mGraphCtrl.AddGraph(gragh);
        }

		{
			CGraph gragh;
			gragh.SetGraphTitle(_T("混合曲线"));
			gragh.SetGraphColor(0x00FFFF);
			for(int i=-10000; i<10000; i++)
			{
				float fX = i;
				float fY = 1000*sin(2*3.1415926*i/1000.0f)*cos(3.1415926*i/99.0f)+1000;
				gragh.AddGraphData(fX, fY);
			}
			mGraphCtrl.AddGraph(gragh);
		}

        {
			CGraph gragh;
			gragh.SetGraphTitle(_T("混合曲线"));
			gragh.SetGraphColor(0xFFFF00);
			for(int i=-10000; i<10000; i++)
			{
				float fX = i;
				float fY = 100000*1.0f/i;
				gragh.AddGraphData(fX, fY);
			}
			mGraphCtrl.AddGraph(gragh);
		}

        {
			CGraph gragh;
			gragh.SetGraphTitle(_T("混合曲线"));
			gragh.SetGraphColor(0xFF00FF);
			for(int i=-10000; i<10000; i++)
			{
				float fX = i;
				float fY = 0.0001*i*i;
				gragh.AddGraphData(fX, fY);
			}
			mGraphCtrl.AddGraph(gragh);
        }

        {
			CGraph gragh;
			gragh.SetGraphTitle(_T("混合曲线"));
			gragh.SetGraphColor(0x000000);
			for(int i=-10000; i<10000; i++)
			{
				float fX = i;
				float fY = -0.0001*i*i;
				gragh.AddGraphData(fX, fY);
			}
			mGraphCtrl.AddGraph(gragh);
        }
    }
}

#ifdef TEST_TIMER
void QMainFrame::DrawTime(HDC hdc)
{
    RECT rcClient;
    GetClientRect(&rcClient);
    DrawText(hdc, mTimeStr.GetBuffer(0), mTimeStr.GetLength(), &rcClient, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}
#endif // TEST_TIMER
