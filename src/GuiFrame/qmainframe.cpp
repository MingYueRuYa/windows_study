#include "qmainframe.h"
#include "resource.h"
#include "registerfile.h"

#include <string>
#include <CommCtrl.h>
#include <Winuser.h>

using std::wstring;

#define IDC_BUTTON      10001
#define IDC_GRAPHCTRL   10002
#define IDC_CLOCK       10003
#define IDT_TIMER       1
#define IDT_TIMER_ICON  2

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

BOOL CALLBACK DialogProc2(
						 HWND hwndDlg, 
						 UINT uMsg, 
						 WPARAM wParam, 
						 LPARAM lParam
						 )
{
    int i = 0;
	return ::DefWindowProc(hwndDlg, uMsg, wParam, lParam);
}

LRESULT QMainFrame::OnCreate(WPARAM wParam, LPARAM lParam)
{
    // 创建button
    if (NULL == m_wndButton.m_hWnd) { 
        m_wndButton.CreateEx(_T("QButton Demo"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_CHECKBOX, 300, 0, 200, 120, m_hWnd, (HMENU)IDC_BUTTON);
    }

    m_wndButtonEx.Create(_T("QButtonEx"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(600, 0, 700, 50), this, 10001);
    m_wndButtonEx.SetImage(IDB_BITMAP14);
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

    m_wndButton2.CreateEx(0, _T("BUTTON"), _T(""), WS_CHILD|WS_VISIBLE|WS_BORDER|BS_DEFPUSHBUTTON, 0, 0, 100, 50, m_hWnd, (HMENU)10001);
	m_wndEdit.CreateEx(0, _T("EDIT"), _T(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE|WS_VSCROLL|ES_AUTOVSCROLL, 0, 100, 100, 50, m_hWnd, (HMENU)10002);
    SendMessage(m_wndEdit.m_hWnd, WM_SETTEXT, 0, (LPARAM)(_T("测试性文字")));

	m_wndListBox.CreateEx(0, _T("LISTBOX"), _T(""), WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_HASSTRINGS, 0, 200, 100, 50, m_hWnd, (HMENU)10003);
    SendMessage(m_wndListBox.m_hWnd, LB_ADDSTRING, 0, (LPARAM)(_T("上海")));
	SendMessage(m_wndListBox.m_hWnd, LB_ADDSTRING, 0, (LPARAM)(_T("安徽")));

	m_wndScrollBar.CreateEx(0, _T("SCROLLBAR"), _T(""), WS_CHILD|WS_VISIBLE|WS_BORDER|SBS_HORZ , 0, 300, 200, 20, m_hWnd, (HMENU)10004);
	m_wndStatic.CreateEx(0, _T("STATIC"), _T(""), WS_CHILD|WS_VISIBLE, 0, 400, 100, 50, m_hWnd, (HMENU)10005);

	m_wndTreeCtrl.CreateEx(0, WC_TREEVIEW, _T(""), WS_CHILD|WS_VISIBLE|WS_BORDER|TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT, 0, 500, 200, 200, m_hWnd, (HMENU)10006);
    TV_INSERTSTRUCT tvis;
	tvis.hParent = TVI_ROOT;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask = TVIF_TEXT | TVIF_STATE;
	tvis.item.pszText = _T("Windows 程序设计");
	tvis.item.cchTextMax = sizeof(_T("Windows 程序设计"));
	tvis.item.state = TVIS_EXPANDED;
	tvis.item.stateMask = TVIS_EXPANDED;

	HTREEITEM hTreeRoot = (HTREEITEM)::SendMessage(m_wndTreeCtrl.m_hWnd, TVM_INSERTITEM, 0, (LPARAM)(&tvis));

	for(int i=1; i<=20; i++)
	{
		tvis.hParent = hTreeRoot;
		CString str;
		str.Format(_T("第%d讲"),  i);
		tvis.item.pszText = str.GetBuffer(0);
		tvis.item.cchTextMax = str.GetLength();
		SendMessage(m_wndTreeCtrl.m_hWnd, TVM_INSERTITEM, 0, (LPARAM)(&tvis));
	}

    SetTimer(m_hWnd, IDT_TIMER_ICON, 500, NULL);

    // 修改应用程序锁关联的图标
	HICON hIcon = (HICON)::LoadIcon((HINSTANCE)::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_UP));
	SetClassLong(m_hWnd, GCL_HICON, (DWORD)hIcon);
	SetClassLong(m_hWnd, GCL_HICONSM, (DWORD)hIcon);

    AddMenu();

    /*
    HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);

    HWND hDlg = ::CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), m_hWnd, DialogProc2);
    */

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
    case IDT_TIMER_ICON:
        {
            static int nCount = 0;
            HICON hIcon = NULL;
            // 修改应用程序锁关联的图标
            if(nCount%3==0)
            {
                hIcon = (HICON)::LoadIcon((HINSTANCE)::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_UP));
            }

            if(nCount%3==1)
            {
                hIcon = (HICON)::LoadIcon((HINSTANCE)::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_RIGHT));
            }

            if(nCount%3==2)
            {
                hIcon = (HICON)::LoadIcon((HINSTANCE)::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_DOWN));
            }

            SetClassLong(m_hWnd, GCL_HICON, (DWORD)hIcon);
            SetClassLong(m_hWnd, GCL_HICONSM, (DWORD)hIcon);
            SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

            nCount++;
        }
        break;
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

BOOL CALLBACK DialogProc(
						 HWND hwndDlg, 
						 UINT uMsg, 
						 WPARAM wParam, 
						 LPARAM lParam
						 )
{
	switch(uMsg)
	{
	case WM_CLOSE:
		{
			::EndDialog(hwndDlg, IDCANCEL);
		}
		return TRUE;
	case WM_COMMAND:
		{
			WORD wNotifyCode = HIWORD(wParam); 
			WORD wID = LOWORD(wParam); 
			HWND hwndCtl = (HWND) lParam;
			if(wID==IDOK)
			{
				HWND hEditUserName = GetDlgItem(hwndDlg, IDC_EDIT_USERNAME);
				assert(hEditUserName);
				TCHAR lpszUserName[1024];
				::GetWindowText(hEditUserName, lpszUserName, 1024);

				HWND hEditPassWord = GetDlgItem(hwndDlg, IDC_EDIT_PASSWORD);
				assert(hEditPassWord);
				TCHAR lpszPassWord[1024];
				::GetWindowText(hEditPassWord, lpszPassWord, 1024);
	
				if(_tcscmp(lpszUserName, _T("linux"))==0 && _tcscmp(lpszPassWord, _T("007"))==0)
				{
					MessageBox(NULL, _T("用户名密码正确"), _T("提示"), 0);
					EndDialog(hwndDlg, IDOK);
				}
				else
				{
					MessageBox(NULL, _T("用户名或密码错误"), _T("提示"), 0);
					::SetWindowText(hEditPassWord, _T(""));
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

LRESULT QMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
    UINT idButton = (int)LOWORD(wParam);
    WORD wNotifyCode = HIWORD(wParam);
    HWND hWnd = (HWND)lParam;

    switch (wNotifyCode) {
        case BN_CLICKED:
        {
            if (idButton == GetDlgCtrlID(m_wndButton2.m_hWnd)) {
                DWORD dwState = (DWORD)::SendMessage(m_wndButton.m_hWnd, BM_GETCHECK, 0, 0);
				if(dwState==BST_CHECKED)
				{
					// SendMessage(m_wndButton.m_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
                    m_wndButton.SetCheck(BST_CHECKED);
				}
				else
				{
					// SendMessage(m_wndButton.m_hWnd, BM_SETCHECK, BST_CHECKED, 0);
                    m_wndButton.SetCheck(BST_UNCHECKED);
				}
            } 
            if (idButton == ID_OPEN_FILE) {
                // MessageBox(NULL, _T("ID_OPEN_FILE"), _T("ID_OPEN_FILE"), 0);
                TCHAR lpszFilePath[MAX_PATH]={};
				TCHAR lpszFileName[MAX_PATH]={};
				OPENFILENAME ofn;
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = m_hWnd;
				ofn.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
				ofn.lpstrFilter = _T("TEXT Files(*.txt)\0\0");
				ofn.lpstrCustomFilter = NULL;
				ofn.nMaxCustFilter = 0;
				ofn.nFilterIndex = 0;
				ofn.lpstrFile = lpszFilePath;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrFileTitle = lpszFileName;
				ofn.nMaxFileTitle = MAX_PATH;
				ofn.lpstrInitialDir = NULL;
				ofn.lpstrTitle = NULL;
				ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
				ofn.nFileOffset = 0;
				ofn.nFileExtension = 0;
				ofn.lpstrDefExt = _T("txt");
				ofn.lCustData = 0;
				ofn.lpfnHook = NULL;
				ofn.lpTemplateName = NULL;
				int nRet = GetOpenFileName(&ofn);
				if(nRet!=0)
				{
					//  gGragh.GetGraghFromFile(lpszFilePath);
					InvalidateRect(m_hWnd, NULL, TRUE);
				}
            } else if (idButton == ID_SAVE_FILE) {
                // MessageBox(NULL, _T("ID_SAVE_FILE"), _T("ID_SAVE_FILE"), 0);
                TCHAR lpszFilePath[MAX_PATH]={};
				TCHAR lpszFileName[MAX_PATH]={};
				OPENFILENAME ofn;
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = m_hWnd;
				ofn.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
				ofn.lpstrFilter = _T("TEXT Files(*.txt)\0\0");
				ofn.lpstrCustomFilter = NULL;
				ofn.nMaxCustFilter = 0;
				ofn.nFilterIndex = 0;
				ofn.lpstrFile = lpszFilePath;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrFileTitle = lpszFileName;
				ofn.nMaxFileTitle = MAX_PATH;
				ofn.lpstrInitialDir = NULL;
				ofn.lpstrTitle = NULL;
				ofn.Flags = OFN_OVERWRITEPROMPT;
				ofn.nFileOffset = 0;
				ofn.nFileExtension = 0;
				ofn.lpstrDefExt = _T("txt");
				ofn.lCustData = 0;
				ofn.lpfnHook = NULL;
				ofn.lpTemplateName = NULL;
				int nRet = GetSaveFileName(&ofn);
				if(nRet!=0)
				{
					// gGragh.SetGraghToFile(lpszFilePath);
				}
            } else if (idButton == ID_ABOUT) {
                /*
                HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);
				assert(hInstance);

				int nRes = ::DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUT_DLG), m_hWnd, DialogProc);
				if(nRes==IDCANCEL)
				{

				}
                */
				return TRUE;
            }
        }
            break;
        default:
            break;
    }

    return QWindow::OnCommand(wParam, lParam);
}

LRESULT QMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
    UINT idCtrl = (int)wParam;
    NM_TREEVIEW *pNmtv = (NM_TREEVIEW *)lParam;
    int nCode = pNmtv->hdr.code;

    switch(nCode) {
        case TVN_SELCHANGED:
        {
            if (pNmtv->hdr.hwndFrom == m_wndTreeCtrl.m_hWnd) {
                HTREEITEM hItemNew = pNmtv->itemNew.hItem;

                TCHAR pszText[1024] = {0};
                TVITEM tvi;
                tvi.mask = TVIF_TEXT | TVIF_HANDLE;
                tvi.hItem = hItemNew;
				tvi.pszText = pszText;
				tvi.cchTextMax = 1024;
				::SendMessage(m_wndTreeCtrl.m_hWnd, TVM_GETITEM, 0, (LPARAM)&tvi);

				MessageBox(NULL, pszText, _T(""), 0);
            }
            break;
        }
        default:
            break;
    }

    return QWindow::OnNotify(wParam, lParam);
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


void QMainFrame::AddMenu()
{
    HMENU hMenu = (HMENU)::LoadMenu((HINSTANCE)::GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
    SetMenu(m_hWnd, hMenu);

    /*
    // 第二种方式创建menu
    HMENU hMenu = CreateMenu();
	HMENU hSubMenu1 = CreatePopupMenu();
	AppendMenu(hSubMenu1, MF_BYCOMMAND | MF_STRING, ID_OPEN_FILE, _T("打开文件"));
	AppendMenu(hSubMenu1, MF_SEPARATOR, 0, _T(""));
	AppendMenu(hSubMenu1, MF_BYCOMMAND | MF_STRING, ID_SAVE_FILE, _T("保存文件"));
	AppendMenu(hMenu, MF_POPUP | MF_STRING, (UINT)hSubMenu1, _T("文件"));
	// HMENU hSubMenu1 = GetSystemMenu(m_hWnd, FALSE)
    SetMenu(m_hWnd, hMenu);
    */
}

LRESULT QMainFrame::OnContextMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint pt;
	GetCursorPos(&pt);
    /*
	HMENU hSubMenu1 = CreatePopupMenu();
	AppendMenu(hSubMenu1, MF_BYCOMMAND | MF_STRING, ID_OPEN_FILE, _T("打开文件"));
	AppendMenu(hSubMenu1, MF_SEPARATOR, 0, _T(""));
	AppendMenu(hSubMenu1, MF_BYCOMMAND | MF_STRING, ID_SAVE_FILE, _T("保存文件"));
	TrackPopupMenu(hSubMenu1, TPM_LEFTALIGN|TPM_TOPALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
    */

    // HMENU subMenu = GetSystemMenu(m_hWnd, false); 
	return QWindow::OnContextMenu(wParam, lParam);
}
