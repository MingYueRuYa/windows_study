#ifndef guiframe_qbutton_h
#define guiframe_qbutton_h

#include "qwindow.h"
#include "commctrl.h"

class QButton : public QWindow
{
public:
    QButton();
    ~QButton();

public:
    virtual BOOL CreateEx(LPCTSTR lpszWindowName,
                        DWORD dwStyle,
                        int x,
                        int y,
                        int nWidth,
                        int nHeight,
                        HWND hWndParent,
                        HMENU nIDorHMenu,
                        LPVOID lpParam = NULL);
    virtual BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle,
		const RECT& rect, QWindow* pParentWnd, UINT nID);

    	// Attributes
	UINT GetState() const;
	void SetState(BOOL bHighlight);
	int GetCheck() const;
	void SetCheck(int nCheck);
	UINT GetButtonStyle() const;
	void SetButtonStyle(UINT nStyle, BOOL bRedraw = TRUE);

	HICON SetIcon(HICON hIcon);
	HICON GetIcon() const;
	HBITMAP SetBitmap(HBITMAP hBitmap);
	HBITMAP GetBitmap() const;
	HCURSOR SetCursor(HCURSOR hCursor);
	HCURSOR GetCursor();

#if (_WIN32_WINNT >= 0x501)
	BOOL GetIdealSize(SIZE* psize);
	BOOL SetImageList(PBUTTON_IMAGELIST pbuttonImagelist);
	BOOL GetImageList(PBUTTON_IMAGELIST pbuttonImagelist);
	BOOL SetTextMargin(RECT* pmargin);
	BOOL GetTextMargin(RECT* pmargin);
#endif  // (_WIN32_WINNT >= 0x501)

	// Overridables (for owner draw only)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

};

#endif // guiframe_qbutton_h