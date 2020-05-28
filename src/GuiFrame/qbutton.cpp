#include "qbutton.h"

QButton::QButton()
{
}

QButton::~QButton()
{

}

BOOL QButton::CreateEx(LPCTSTR lpszWindowName,
                        DWORD dwStyle,
                        int x,
                        int y,
                        int nWidth,
                        int nHeight,
                        HWND hWndParent,
                        HMENU nIDorHMenu,
                        LPVOID lpParam)
{
    return QWindow::CreateEx(0, _T("BUTTON"), lpszWindowName, 
                                dwStyle, x, y, nWidth, nHeight, 
                                hWndParent, nIDorHMenu, lpParam);
}

BOOL QButton::Create(LPCTSTR lpszCaption, DWORD dwStyle,
		const RECT& rect, QWindow* pParentWnd, UINT nID)
{
    QWindow* pWnd = this;
	return pWnd->CreateEx(0, _T("BUTTON"), lpszCaption, dwStyle, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, pParentWnd->m_hWnd, (HMENU)nID);
}


// Derived class is responsible for implementing all of these handlers
//   for owner/self draw controls
void QButton::DrawItem(LPDRAWITEMSTRUCT)
{
	assert(FALSE);
}

UINT QButton::GetState() const
{ assert(::IsWindow(m_hWnd)); return (UINT)::SendMessage(m_hWnd, BM_GETSTATE, 0, 0); }
void QButton::SetState(BOOL bHighlight)
{ ::SendMessage(m_hWnd, BM_SETSTATE, bHighlight, 0); }
int QButton::GetCheck() const
{ assert(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, BM_GETCHECK, 0, 0); }
void QButton::SetCheck(int nCheck)
{ assert(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, BM_SETCHECK, nCheck, 0); }
UINT QButton::GetButtonStyle() const
{ assert(::IsWindow(m_hWnd)); return (UINT)GetWindowLong(m_hWnd, GWL_STYLE) & 0xff; }
void QButton::SetButtonStyle(UINT nStyle, BOOL bRedraw)
{ assert(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, BM_SETSTYLE, nStyle, (LPARAM)bRedraw); }
// Win4
HICON QButton::SetIcon(HICON hIcon)
{ assert(::IsWindow(m_hWnd)); return (HICON)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon); }
HICON QButton::GetIcon() const
{ assert(::IsWindow(m_hWnd)); return (HICON)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_ICON, 0L); }
HBITMAP QButton::SetBitmap(HBITMAP hBitmap)
{ assert(::IsWindow(m_hWnd)); return (HBITMAP)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap); }
HBITMAP QButton::GetBitmap() const
{ assert(::IsWindow(m_hWnd)); return (HBITMAP)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_BITMAP, 0L); }
HCURSOR QButton::SetCursor(HCURSOR hCursor)
{ assert(::IsWindow(m_hWnd)); return (HCURSOR)::SendMessage(m_hWnd, BM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor); }
HCURSOR QButton::GetCursor()
{ assert(::IsWindow(m_hWnd)); return (HCURSOR)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_CURSOR, 0L); }
