#ifndef guiframk_digit_clock_h
#define guiframk_digit_clock_h

#include "qwindow.h"

#include <atlimage.h>

class DigitClock : public QWindow
{
public:
    DigitClock();
    virtual ~DigitClock();

public:
    virtual bool Create(DWORD dwStyle, CRect rc, HWND hWndParent, UINT uID);

public:
   virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
   virtual LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
   virtual LRESULT OnPaint(WPARAM wParam, LPARAM lParam);

private:
    CImage m_imgClock[12];

};


#endif // guiframk_digit_clock_h