#include "qwindow.h"

QWindow::QWindow()
{
    m_hWnd = NULL;
    m_lpfnOldWndProc = NULL;
}

QWindow::~QWindow()
{
}

BOOL QWindow::SubClassWindow(HWND hWnd)
{
    assert(hWnd);
    if (m_hWnd == hWnd) { return TRUE; }

    m_lpfnOldWndProc = (WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC);

    if (m_lpfnOldWndProc != QWindow::WndProc) {
        m_hWnd = hWnd;
        SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
        SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)QWindow::WndProc);
        return TRUE;
    }

    return false;
}

QWindow *QWindow::FromHandle(HWND hWnd)
{
    assert(hWnd);

    QWindow *win = (QWindow *)GetWindowLong(hWnd, GWL_USERDATA);

    return win;
}

BOOL QWindow::PreCreateWindow(CREATESTRUCT &cs)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    BOOL bRet = ::GetClassInfoEx(cs.hInstance, cs.lpszClass, &wcex);
    if (bRet) { return TRUE; }

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra  = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = cs.hInstance;
    wcex.hIcon = (HICON)::LoadIcon(NULL, IDI_QUESTION);
    wcex.hIconSm = (HICON)::LoadIcon(NULL, IDI_QUESTION); 
    wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
    wcex.hCursor = (HCURSOR)::LoadCursor(NULL, IDC_ARROW);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = cs.lpszClass;

    bRet = ::RegisterClassEx(&wcex);
    if (! bRet) { return false; } 

    return true;
}

BOOL QWindow::CreateEx(DWORD dwExStyle,
                            LPCTSTR lpszeClassName,
                            LPCTSTR lpszWindowName,
                            DWORD dwStyle,
                            int x,
                            int y,
                            int nWidth,
                            int nHeight,
                            HWND hWndParent,
                            HMENU nIDorHMenu,
                            LPVOID lpParam)
{
    HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);
    assert(hInstance);

    CREATESTRUCT cs;
    cs.cx = nWidth;
    cs.cy = nHeight;
    cs.dwExStyle = dwExStyle;
    cs.hInstance = hInstance;
    cs.hMenu = nIDorHMenu;
    cs.lpszClass = lpszeClassName;
    cs.lpszName = lpszWindowName;
    cs.style = dwStyle;
    cs.x = x;
    cs.y = y;
    cs.hwndParent = hWndParent;
    cs.lpCreateParams = lpParam;

    BOOL bRet = PreCreateWindow(cs);
    if (! bRet) { 
        MessageBox(NULL, _T("注册窗口失败"), _T("注册窗口类"), 0);
        return false;
    }

    // 将自身指针通过最有参数lParameter传递进去
    HWND hWnd = CreateWindowEx(cs.dwExStyle, cs.lpszClass, cs.lpszName, 
                                cs.style, cs.x, cs.y, cs.cx, cs.cy, 
                                cs.hwndParent, cs.hMenu, cs.hInstance, 
                                this);

    if (NULL == hWnd) { return false; }

    m_hWnd = hWnd;

    SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);

    m_lpfnOldWndProc = (WNDPROC)GetWindowLong(m_hWnd, GWL_WNDPROC);

    // 子类化消息
    if (m_lpfnOldWndProc != QWindow::WndProc) {
        SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)QWindow::WndProc);

        WindowProc(WM_CREATE, 0, 0);
        WindowProc(WM_NCCREATE, 0, 0);
    }

    return true;
}

BOOL QWindow::ShowWindow(int nCmdShow)
{
    assert(m_hWnd);

    return ::ShowWindow(m_hWnd, nCmdShow);
}

BOOL QWindow::UpdateWindow()
{
    assert(m_hWnd);

    return ::UpdateWindow(m_hWnd);
}

BOOL QWindow::DestroyWindow()
{
    assert(m_hWnd);

    return ::DestroyWindow(m_hWnd);
}

BOOL QWindow::GetClientRect(LPRECT lpRect)
{
    assert(lpRect);

    return ::GetClientRect(m_hWnd, lpRect);
}

LRESULT QWindow::Default(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (m_lpfnOldWndProc == QWindow::WndProc) {
        return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
    }

    return CallWindowProc(m_lpfnOldWndProc, m_hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK QWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE || uMsg == WM_NCCREATE) {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        if (pcs) {
            QWindow *window = (QWindow *)pcs->lpCreateParams;

            if (window) {
                window->m_hWnd = hWnd;
                window->m_lpfnOldWndProc = (WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC);
                return window->WindowProc(uMsg, wParam, lParam);
            }
        }
    }

    QWindow *window = (QWindow *)GetWindowLong(hWnd, GWL_USERDATA);
    if (window) { return window->WindowProc(uMsg, wParam, lParam); }

    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT QWindow::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_ACTIVATE:
		return OnActivate(wParam, lParam);
	case WM_ACTIVATEAPP:
		return OnActivateApp(wParam, lParam);
	case WM_ASKCBFORMATNAME:
		return OnAskCbFormatName(wParam, lParam);
	case WM_CANCELMODE:
		return OnCancelMode(wParam, lParam);
	case WM_CAPTURECHANGED:
		return OnCaptureChanged(wParam, lParam);
	case WM_CHANGECBCHAIN:
		return OnChangeCbChain(wParam, lParam);
	case WM_CHAR:
		return OnChar(wParam, lParam);
	case WM_CHARTOITEM:
		return OnCharToItem(wParam, lParam);
	case WM_CHILDACTIVATE:
		return OnChildActivate(wParam, lParam);
	case WM_CLOSE:
		return OnClose(wParam, lParam);
	case WM_COMPACTING:
		return OnCompacting(wParam, lParam);
	case WM_COMPAREITEM:
		return OnCompareItem(wParam, lParam);
	case WM_CONTEXTMENU:
		return OnContextMenu(wParam, lParam);
	case WM_COPYDATA:
		return OnCopyData(wParam, lParam);
	case WM_CREATE:
		return OnCreate(wParam, lParam);
    case WM_COMMAND:
        return OnCommand(wParam, lParam);
	case WM_DEADCHAR:
		return OnDeadChar(wParam, lParam);
	case WM_DELETEITEM:
		return OnDeleteItem(wParam, lParam);
	case WM_DESTROY:
		return OnDestroy(wParam, lParam);
	case WM_DESTROYCLIPBOARD:
		return OnDestroyClipboard(wParam, lParam);
	case WM_DEVICECHANGE:
		return OnDeviceChange(wParam, lParam);
	case WM_DEVMODECHANGE:
		return OnDevModeChange(wParam, lParam);
	case WM_DRAWCLIPBOARD:
		return OnDrawClipboard(wParam, lParam);
	case WM_DRAWITEM:
		return OnDrawItem(wParam, lParam);
	case WM_DROPFILES:
		return OnDropFiles(wParam, lParam);
	case WM_ENABLE:
		return OnEnable(wParam, lParam);
	case WM_ENDSESSION:
		return OnEndSession(wParam, lParam);
	case WM_ENTERIDLE:
		return OnEnterIdle(wParam, lParam);
	case WM_ERASEBKGND:
		return OnEraseBkgnd(wParam, lParam);

	case WM_FONTCHANGE:
		return OnFontChange(wParam, lParam);		
	case WM_GETDLGCODE:
		return OnGetDlgCode(wParam, lParam);		
	case WM_GETMINMAXINFO:
		return OnGetMinMaxInfo(wParam, lParam);	
	case WM_HELP:
		return OnHelpInfo(wParam, lParam);		
	case WM_HSCROLL:
		return OnHScroll(wParam, lParam);		
	case WM_HSCROLLCLIPBOARD:
		return OnHScrollClipboard(wParam, lParam);		
	case WM_ICONERASEBKGND:
		return OnIconEraseBkgnd(wParam, lParam);		
	case WM_INITMENU:
		return OnInitMenu(wParam, lParam);
	case WM_INITMENUPOPUP:
		return OnInitMenuPopup(wParam, lParam);		
	case WM_KEYDOWN:
		return OnKeyDown(wParam, lParam);
	case WM_KEYUP:
		return OnKeyUp(wParam, lParam);
	case WM_KILLFOCUS:
		return OnKillFocus(wParam, lParam);

	case WM_LBUTTONDBLCLK:
		return OnLButtonDblClk(wParam, lParam);		
	case WM_LBUTTONDOWN:
		return OnLButtonDown(wParam, lParam);		
	case WM_LBUTTONUP:
		return OnLButtonUp(wParam, lParam);		
	case WM_MBUTTONDBLCLK:
		return OnMButtonDblClk(wParam, lParam);	
	case WM_MBUTTONDOWN:
		return OnMButtonDown(wParam, lParam);	
	case WM_MBUTTONUP:
		return OnMButtonUp(wParam, lParam);		
	case WM_MDIACTIVATE:
		return OnMDIActivate(wParam, lParam);		
	case WM_MEASUREITEM:
		return OnMeasureItem(wParam, lParam);		
	case WM_MENUCHAR:
		return OnMenuChar(wParam, lParam);		
	case WM_MENUSELECT:
		return OnMenuSelect(wParam, lParam);		
	case WM_MOUSEACTIVATE:
		return OnMouseActivate(wParam, lParam);
	case WM_MOUSEMOVE:
		return OnMouseMove(wParam, lParam);
	case WM_MOUSEWHEEL:
		return OnMouseWheel(wParam, lParam);	
	case WM_MOVE:
		return OnMove(wParam, lParam);		
	case WM_MOVING:
		return OnMoving(wParam, lParam);

	case WM_NCACTIVATE:
		return OnNcActivate(wParam, lParam);		
	case WM_NCCALCSIZE:
		return OnNcCalcSize(wParam, lParam);		
	case WM_NCCREATE:
		return OnNcCreate(wParam, lParam);		
	case WM_NCDESTROY:
		return OnNcDestroy(wParam, lParam);		
	case WM_NCHITTEST:
		return OnNcHitTest(wParam, lParam);		
	case WM_NCLBUTTONDBLCLK:
		return OnNcLButtonDblClk(wParam, lParam);		
	case WM_NCLBUTTONDOWN:
		return OnNcLButtonDown(wParam, lParam);		
	case WM_NCLBUTTONUP:
		return OnNcLButtonUp(wParam, lParam);		
	case WM_NCMBUTTONDBLCLK:
		return OnNcMButtonDblClk(wParam, lParam);		
	case WM_NCMBUTTONDOWN:
		return OnNcMButtonDown(wParam, lParam);		
	case WM_NCMBUTTONUP:
		return OnNcMButtonUp(wParam, lParam);
	case WM_NCMOUSEMOVE:
		return OnNcMouseMove(wParam, lParam);		
	case WM_NCPAINT:
		return OnNcPaint(wParam, lParam);	
	case WM_NCRBUTTONDBLCLK:
		return OnNcRButtonDblClk(wParam, lParam);		
	case WM_NCRBUTTONDOWN:
		return OnNcRButtonDown(wParam, lParam);		
	case WM_NCRBUTTONUP:
		return OnNcRButtonUp(wParam, lParam);		
    case WM_NOTIFY:
        return OnNotify(wParam, lParam);
	case WM_PAINT:
		return OnPaint(wParam, lParam);			
	case WM_PAINTCLIPBOARD:
		return OnPaintClipboard(wParam, lParam);			
	case WM_PALETTECHANGED:
		return OnPaletteChanged(wParam, lParam);			
	case WM_PALETTEISCHANGING:
		return OnPaletteIsChanging(wParam, lParam);			
	case WM_PARENTNOTIFY:
		return OnParentNotify(wParam, lParam);			
	case WM_QUERYDRAGICON:
		return OnQueryDragIcon(wParam, lParam);			
	case WM_QUERYENDSESSION:
		return OnQueryEndSession(wParam, lParam);			
	case WM_QUERYNEWPALETTE:
		return OnQueryNewPalette(wParam, lParam);			
	case WM_QUERYOPEN:
		return OnQueryOpen(wParam, lParam);	
	case WM_RBUTTONDBLCLK:
		return OnNcRButtonDblClk(wParam, lParam);	
	case WM_RBUTTONDOWN:
		return OnRButtonDown(wParam, lParam);			
	case WM_RBUTTONUP:
		return OnRButtonUp(wParam, lParam);			
	case WM_RENDERALLFORMATS:
		return OnRenderAllFormats(wParam, lParam);			
	case WM_RENDERFORMAT:
		return OnRenderFormat(wParam, lParam);			

	case WM_SETCURSOR:
		return OnSetCursor(wParam, lParam);			
	case WM_SETFOCUS:
		return OnSetFocus(wParam, lParam);					
	case WM_SHOWWINDOW:
		return OnShowWindow(wParam, lParam);			
	case WM_SIZE:
		return OnSize(wParam, lParam);			
	case WM_SIZECLIPBOARD:
		return OnSizeClipboard(wParam, lParam);			
	case WM_SIZING:
		return OnSizing(wParam, lParam);			
	case WM_SPOOLERSTATUS:
		return OnSpoolerStatus(wParam, lParam);			
	case WM_STYLECHANGED:
		return OnStyleChanged(wParam, lParam);			
	case WM_STYLECHANGING:
		return OnStyleChanging(wParam, lParam);			
	case WM_SYSCHAR:
		return OnSysChar(wParam, lParam);	
	case WM_SYSCOLORCHANGE:
		return OnSysColorChange(wParam, lParam);			
	case WM_SYSCOMMAND:
		return OnSysCommand(wParam, lParam);			
	case WM_SYSDEADCHAR:
		return OnSysDeadChar(wParam, lParam);			
	case WM_SYSKEYDOWN:
		return OnSysKeyDown(wParam, lParam);			
	case WM_SYSKEYUP:
		return OnSysKeyUp(wParam, lParam);			

	case WM_TCARD:
		return OnTCard(wParam, lParam);		
	case WM_TIMECHANGE:
		return OnTimeChange(wParam, lParam);		
	case WM_TIMER:
		return OnTimer(wParam, lParam);		
	case WM_VKEYTOITEM:
		return OnVKeyToItem(wParam, lParam);	
	case WM_VSCROLL:
		return OnVScroll(wParam, lParam);	
	case WM_VSCROLLCLIPBOARD:
		return OnVScrollClipboard(wParam, lParam);
	case WM_WINDOWPOSCHANGED:
		return OnWindowPosChanged(wParam, lParam);	
	case WM_WINDOWPOSCHANGING:
		return OnWindowPosChanging(wParam, lParam);		
	case WM_WININICHANGE:
		return OnWinIniChange(wParam, lParam);			

	default:
		break;
	}
	return Default(uMsg, wParam, lParam);
}

// WM_ Message Handlers: A - C 
LRESULT QWindow::OnActivate(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_ACTIVATE, wParam, lParam);
}

LRESULT QWindow::OnActivateApp(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_ACTIVATEAPP, wParam, lParam);
}

LRESULT QWindow::OnAskCbFormatName(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_ASKCBFORMATNAME, wParam, lParam);
}

LRESULT QWindow::OnCancelMode(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_CANCELMODE, wParam, lParam);
}

LRESULT QWindow::OnCaptureChanged(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_CAPTURECHANGED, wParam, lParam);
}

LRESULT QWindow::OnChangeCbChain(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_CHANGECBCHAIN, wParam, lParam);
}

LRESULT QWindow::OnChar(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_CHAR, wParam, lParam);
}

LRESULT QWindow::OnCharToItem(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_CHARTOITEM, wParam, lParam);
}

LRESULT QWindow::OnChildActivate(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_CHILDACTIVATE, wParam, lParam);
}

LRESULT QWindow::OnClose(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_CLOSE, wParam, lParam);
}

LRESULT QWindow::OnCompacting(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_COMPACTING, wParam, lParam);
}

LRESULT QWindow::OnCompareItem(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_COMPAREITEM, wParam, lParam);
}

LRESULT QWindow::OnContextMenu(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_CONTEXTMENU, wParam, lParam);
}

LRESULT QWindow::OnCopyData(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_COPYDATA, wParam, lParam);
}

LRESULT QWindow::OnCreate(WPARAM wParam, LPARAM lParam)
{
    return true;
	// return Default(WM_CREATE, wParam, lParam);
}

LRESULT QWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
    return Default(WM_COMMAND, wParam, lParam);
}

// WM_ Message Handlers: D - E 
LRESULT QWindow::OnDeadChar(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_DEADCHAR, wParam, lParam);
}

LRESULT QWindow::OnDeleteItem(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_DELETEITEM, wParam, lParam);
}

LRESULT QWindow::OnDestroy(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_DESTROY, wParam, lParam);
}

LRESULT QWindow::OnDestroyClipboard(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_DESTROYCLIPBOARD, wParam, lParam);
}

LRESULT QWindow::OnDeviceChange(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_DEVICECHANGE, wParam, lParam);
}

LRESULT QWindow::OnDevModeChange(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_DEVMODECHANGE, wParam, lParam);
}

LRESULT QWindow::OnDrawClipboard(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_DRAWCLIPBOARD, wParam, lParam);
}

LRESULT QWindow::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_DRAWITEM, wParam, lParam);
}

LRESULT QWindow::OnDropFiles(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_DROPFILES, wParam, lParam);
}

LRESULT QWindow::OnEnable(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_ENABLE, wParam, lParam);
}

LRESULT QWindow::OnEndSession(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_ENDSESSION, wParam, lParam);
}

LRESULT QWindow::OnEnterIdle(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_ENTERIDLE, wParam, lParam);
}

LRESULT QWindow::OnEraseBkgnd(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_ERASEBKGND, wParam, lParam);
}

// WM_ Message Handlers: F - K
LRESULT QWindow::OnFontChange(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_FONTCHANGE, wParam, lParam);
}

LRESULT QWindow::OnGetDlgCode(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_GETDLGCODE, wParam, lParam);
}

LRESULT QWindow::OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_GETMINMAXINFO, wParam, lParam);
}

LRESULT QWindow::OnHelpInfo(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_HELP, wParam, lParam);
}

LRESULT QWindow::OnHScroll(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_HSCROLL, wParam, lParam);
}

LRESULT QWindow::OnHScrollClipboard(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_HSCROLLCLIPBOARD, wParam, lParam);
}

LRESULT QWindow::OnIconEraseBkgnd(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_ICONERASEBKGND, wParam, lParam);
}

LRESULT QWindow::OnInitMenu(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_INITMENU, wParam, lParam);
}

LRESULT QWindow::OnInitMenuPopup(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_INITMENUPOPUP, wParam, lParam);
}

LRESULT QWindow::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_KEYDOWN, wParam, lParam);
}

LRESULT QWindow::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_KEYUP, wParam, lParam);
}

LRESULT QWindow::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_KILLFOCUS, wParam, lParam);
}

//WM_ Message Handlers: L - M 
LRESULT QWindow::OnLButtonDblClk(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_LBUTTONDBLCLK, wParam, lParam);
}

LRESULT QWindow::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_LBUTTONDOWN, wParam, lParam);
}

LRESULT QWindow::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_LBUTTONUP, wParam, lParam);
}

LRESULT QWindow::OnMButtonDblClk(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_MBUTTONDBLCLK, wParam, lParam);
}

LRESULT QWindow::OnMButtonDown(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_MBUTTONDOWN, wParam, lParam);
}

LRESULT QWindow::OnMButtonUp(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_MBUTTONUP, wParam, lParam);
}

LRESULT QWindow::OnMDIActivate(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_MDIACTIVATE, wParam, lParam);
}

LRESULT QWindow::OnMeasureItem(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_MEASUREITEM, wParam, lParam);
}

LRESULT QWindow::OnMenuChar(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_MENUCHAR, wParam, lParam);
}

LRESULT QWindow::OnMenuSelect(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_MENUSELECT, wParam, lParam);
}

LRESULT QWindow::OnMouseActivate(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_MOUSEACTIVATE, wParam, lParam);
}

LRESULT QWindow::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_MOUSEMOVE, wParam, lParam);
}

LRESULT QWindow::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_MOUSEWHEEL, wParam, lParam);
}

LRESULT QWindow::OnMove(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_MOVE, wParam, lParam);
}

LRESULT QWindow::OnMoving(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_MOVING, wParam, lParam);
}

//WM_ Message Handlers: N - O 
LRESULT QWindow::OnNcActivate(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCACTIVATE, wParam, lParam);
}

LRESULT QWindow::OnNcCalcSize(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCCALCSIZE, wParam, lParam);
}

LRESULT QWindow::OnNcCreate(WPARAM wParam, LPARAM lParam)
{
	// return Default(WM_NCCREATE, wParam, lParam);
    return true;
}

LRESULT QWindow::OnNcDestroy(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCDESTROY, wParam, lParam);
}

LRESULT QWindow::OnNcHitTest(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCHITTEST, wParam, lParam);
}

LRESULT QWindow::OnNcLButtonDblClk(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCLBUTTONDBLCLK, wParam, lParam);
}

LRESULT QWindow::OnNcLButtonDown(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCLBUTTONDOWN, wParam, lParam);
}

LRESULT QWindow::OnNcLButtonUp(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCLBUTTONUP, wParam, lParam);
}

LRESULT QWindow::OnNcMButtonDblClk(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCRBUTTONDBLCLK, wParam, lParam);
}

LRESULT QWindow::OnNcMButtonDown(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCMBUTTONDOWN, wParam, lParam);
}

LRESULT QWindow::OnNcMButtonUp(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCMBUTTONUP, wParam, lParam);
}

LRESULT QWindow::OnNcMouseMove(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCMOUSEMOVE, wParam, lParam);
}

LRESULT QWindow::OnNcPaint(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCPAINT, wParam, lParam);
}

LRESULT QWindow::OnNcRButtonDblClk(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCRBUTTONDBLCLK, wParam, lParam);
}

LRESULT QWindow::OnNcRButtonDown(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCRBUTTONDOWN, wParam, lParam);
}

LRESULT QWindow::OnNcRButtonUp(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NCRBUTTONUP, wParam, lParam);
}

LRESULT QWindow::OnNotify(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_NOTIFY, wParam, lParam);
}

// WM_ Messages: P - R 
LRESULT QWindow::OnPaint(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_PAINT, wParam, lParam);
}

LRESULT QWindow::OnPaintClipboard(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_PAINTCLIPBOARD, wParam, lParam);
}

LRESULT QWindow::OnPaletteChanged(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_PALETTECHANGED, wParam, lParam);
}

LRESULT QWindow::OnPaletteIsChanging(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_PALETTEISCHANGING, wParam, lParam);
}

LRESULT QWindow::OnParentNotify(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_PARENTNOTIFY, wParam, lParam);
}

LRESULT QWindow::OnQueryDragIcon(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_QUERYDRAGICON, wParam, lParam);
}

LRESULT QWindow::OnQueryEndSession(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_QUERYENDSESSION, wParam, lParam);
}

LRESULT QWindow::OnQueryNewPalette(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_QUERYNEWPALETTE, wParam, lParam);
}

LRESULT QWindow::OnQueryOpen(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_QUERYOPEN, wParam, lParam);
}

LRESULT QWindow::OnRButtonDblClk(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_RBUTTONDBLCLK, wParam, lParam);
}

LRESULT QWindow::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_RBUTTONDOWN, wParam, lParam);
}

LRESULT QWindow::OnRButtonUp(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_RBUTTONUP, wParam, lParam);
}

LRESULT QWindow::OnRenderAllFormats(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_RENDERALLFORMATS, wParam, lParam);
}

LRESULT QWindow::OnRenderFormat(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_RENDERFORMAT, wParam, lParam);
}


// WM_ Messages: S 
LRESULT QWindow::OnSetCursor(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_SETCURSOR, wParam, lParam);
}

LRESULT QWindow::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_SETFOCUS, wParam, lParam);
}

LRESULT QWindow::OnShowWindow(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_SHOWWINDOW, wParam, lParam);
}

LRESULT QWindow::OnSize(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_SIZE, wParam, lParam);
}

LRESULT QWindow::OnSizeClipboard(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_SIZECLIPBOARD, wParam, lParam);
}

LRESULT QWindow::OnSizing(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_SIZING, wParam, lParam);
}

LRESULT QWindow::OnSpoolerStatus(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_SPOOLERSTATUS, wParam, lParam);
}

LRESULT QWindow::OnStyleChanged(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_STYLECHANGED, wParam, lParam);
}

LRESULT QWindow::OnStyleChanging(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_STYLECHANGING, wParam, lParam);
}

LRESULT QWindow::OnSysChar(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_SYSCHAR, wParam, lParam);
}

LRESULT QWindow::OnSysColorChange(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_SYSCOLORCHANGE, wParam, lParam);
}

LRESULT QWindow::OnSysCommand(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_SYSCOMMAND, wParam, lParam);
}

LRESULT QWindow::OnSysDeadChar(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_DEADCHAR, wParam, lParam);
}

LRESULT QWindow::OnSysKeyDown(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_SYSKEYDOWN, wParam, lParam);
}

LRESULT QWindow::OnSysKeyUp(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_SYSKEYUP, wParam, lParam);
}

//WM_ Messages: T - Z 
LRESULT QWindow::OnTCard(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_TCARD, wParam, lParam);
}

LRESULT QWindow::OnTimeChange(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_TIMECHANGE, wParam, lParam);
}

LRESULT QWindow::OnTimer(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_TIMECHANGE, wParam, lParam);
}

LRESULT QWindow::OnVKeyToItem(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_VKEYTOITEM, wParam, lParam);
}

LRESULT QWindow::OnVScroll(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_VSCROLL, wParam, lParam);
}

LRESULT QWindow::OnVScrollClipboard(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_VSCROLLCLIPBOARD, wParam, lParam);
}

LRESULT QWindow::OnWindowPosChanged(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_WINDOWPOSCHANGED, wParam, lParam);
}

LRESULT QWindow::OnWindowPosChanging(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_WINDOWPOSCHANGING, wParam, lParam);
}

LRESULT QWindow::OnWinIniChange(WPARAM wParam, LPARAM lParam)
{
	return Default(WM_WININICHANGE, wParam, lParam);
}
