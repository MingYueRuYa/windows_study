#include "digitclock.h"
#include "resource.h"

#define IDT_TIMER 1

DigitClock::DigitClock()
{
}

DigitClock::~DigitClock()
{
}

bool DigitClock::Create(DWORD dwStyle, CRect rc, HWND hWndParent, UINT uID)
{
    return QWindow::CreateEx(0, L"DIGITCLOCK", L"", dwStyle, rc.left, rc.top, rc.Width(), rc.Height(), hWndParent, (HMENU)uID);
}

LRESULT DigitClock::OnCreate(WPARAM wParam, LPARAM lParam)
{
    for (int i = 0; i < 12; ++i) {
        m_imgClock[i].LoadFromResource((HINSTANCE)::GetModuleHandle(NULL), IDB_BITMAP2+i);
    }

    SetTimer(m_hWnd, IDT_TIMER, 500, NULL);

    return TRUE;
}

LRESULT DigitClock::OnTimer(WPARAM wParam, LPARAM lParam)
{
    InvalidateRect(m_hWnd, NULL, TRUE);

    return QWindow::OnTimer(wParam, lParam);
}

LRESULT DigitClock::OnPaint(WPARAM wParam, LPARAM lParam)
{
    	PAINTSTRUCT ps;
	HDC hDC = ::BeginPaint(m_hWnd, &ps);
	assert(hDC);
	{
		//12:24:36
		SYSTEMTIME st;
		GetLocalTime(&st);
		int nHiHour = st.wHour/10;
		int nLoHour = st.wHour%10;

		int nHiMinute = st.wMinute/10;
		int nLoMinute = st.wMinute%10;

		int nHiSecond = st.wSecond/10;
		int nLoSecond = st.wSecond%10;

		CRect rcClient;
		GetClientRect(&rcClient);

		if(!m_imgClock[nHiHour].IsNull())
		{
			m_imgClock[nHiHour].Draw(hDC, CRect(rcClient.Width()*0/8, 0, rcClient.Width()*1/8, rcClient.Height()));
		}

		if(!m_imgClock[nLoHour].IsNull())
		{
			m_imgClock[nLoHour].Draw(hDC, CRect(rcClient.Width()*1/8, 0, rcClient.Width()*2/8, rcClient.Height()));
		}

		//
		static BOOL bRet = FALSE;
		if(bRet)
		{
			if(!m_imgClock[11].IsNull())
			{
				m_imgClock[11].Draw(hDC, CRect(rcClient.Width()*2/8, 0, rcClient.Width()*3/8, rcClient.Height()));
				m_imgClock[11].Draw(hDC, CRect(rcClient.Width()*5/8, 0, rcClient.Width()*6/8, rcClient.Height()));
			}
			bRet = FALSE;
		}
		else
		{
			if(!m_imgClock[10].IsNull())
			{
				m_imgClock[10].Draw(hDC, CRect(rcClient.Width()*2/8, 0, rcClient.Width()*3/8, rcClient.Height()));
				m_imgClock[10].Draw(hDC, CRect(rcClient.Width()*5/8, 0, rcClient.Width()*6/8, rcClient.Height()));
			}
			bRet = TRUE;
		}

		if(!m_imgClock[nHiMinute].IsNull())
		{
			m_imgClock[nHiMinute].Draw(hDC, CRect(rcClient.Width()*3/8, 0, rcClient.Width()*4/8, rcClient.Height()));
		}

		if(!m_imgClock[nLoMinute].IsNull())
		{
			m_imgClock[nLoMinute].Draw(hDC, CRect(rcClient.Width()*4/8, 0, rcClient.Width()*5/8, rcClient.Height()));
		}

		if(!m_imgClock[nHiSecond].IsNull())
		{
			m_imgClock[nHiSecond].Draw(hDC, CRect(rcClient.Width()*6/8, 0, rcClient.Width()*7/8, rcClient.Height()));
		}

		if(!m_imgClock[nLoSecond].IsNull())
		{
			m_imgClock[nLoSecond].Draw(hDC, CRect(rcClient.Width()*7/8, 0, rcClient.Width()*8/8, rcClient.Height()));
		}
	}
	::EndPaint(m_hWnd, &ps);
	return TRUE;
}
