#include "qbuttonex.h"
#include "resource.h"

QButtonEx::QButtonEx(void)
{
	m_bIsHover = FALSE;
	m_bIsDown = FALSE;
}

QButtonEx::~QButtonEx(void)
{
}

void QButtonEx::SetImage(UINT uID)
{
	if(!m_imgBtn.IsNull())
	{
		m_imgBtn.Destroy();
	}
	m_imgBtn.LoadFromResource((HINSTANCE)::GetModuleHandle(NULL), uID);
	InvalidateRect(m_hWnd, NULL, TRUE);
}

LRESULT QButtonEx::OnPaint(WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC = ::BeginPaint(m_hWnd, &ps);
	assert(hDC);
	{
		RECT rc;
		GetClientRect(&rc);
		DRAWITEMSTRUCT dis;
		dis.hDC = hDC;
		dis.CtlType = ODT_BUTTON;
		dis.hwndItem = m_hWnd;
		dis.rcItem = rc;
		DrawItem(&dis);
	}
	EndPaint(m_hWnd, &ps);
	return TRUE;
}

void QButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	HDC hDC = lpDrawItemStruct->hDC;
	CRect rc = lpDrawItemStruct->rcItem;

	if(!m_imgBtn.IsNull())
	{
		if(!m_bIsHover)
		{
			m_imgBtn.Draw(hDC, rc, CRect(0, 0, m_imgBtn.GetWidth()/4, m_imgBtn.GetHeight()));
		}
		else
		{
			if(m_bIsDown)
			{
				m_imgBtn.Draw(hDC, rc, CRect(m_imgBtn.GetWidth()/4*2, 0, m_imgBtn.GetWidth()/4*3, m_imgBtn.GetHeight()));
			}
			else
			{
				m_imgBtn.Draw(hDC, rc, CRect(m_imgBtn.GetWidth()/4, 0, m_imgBtn.GetWidth()/4*2, m_imgBtn.GetHeight()));
			}
		}
	}
}

LRESULT QButtonEx::OnCreate(WPARAM wParam, LPARAM lParam)
{
	return QButton::OnCreate(wParam, lParam);
}

LRESULT QButtonEx::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.hwndTrack = m_hWnd;
	tme.dwHoverTime = HOVER_DEFAULT;
	TrackMouseEvent(&tme);
	return QButton::OnMouseMove(wParam, lParam);
}

LRESULT QButtonEx::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg==WM_MOUSEHOVER)
	{
		return OnMouseHover(wParam, lParam);
	}

	if(uMsg==WM_MOUSELEAVE)
	{
		return OnMouseLeave(wParam, lParam);
	}
	return QButton::WindowProc(uMsg, wParam, lParam);
}

LRESULT QButtonEx::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if(!m_bIsHover)
	{
		m_bIsHover = TRUE;
		InvalidateRect(m_hWnd, NULL, TRUE);
	}
	return TRUE;
}

LRESULT QButtonEx::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if(m_bIsHover)
	{
		m_bIsHover = FALSE;
		InvalidateRect(m_hWnd, NULL, TRUE);
	}
	return TRUE;
}

LRESULT QButtonEx::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if(!m_bIsDown)
	{
		m_bIsDown = TRUE;
		InvalidateRect(m_hWnd, NULL, TRUE);
	}
	return QButton::OnLButtonDown(wParam, lParam);
}

LRESULT QButtonEx::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if(m_bIsDown)
	{
		m_bIsDown = FALSE;
		InvalidateRect(m_hWnd, NULL, TRUE);
	}
	return QButton::OnLButtonUp(wParam, lParam);
}
