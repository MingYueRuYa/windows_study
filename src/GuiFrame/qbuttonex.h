#pragma once
#include "qbutton.h"
#include <atlimage.h>

class QButtonEx :
	public QButton
{
public:
	QButtonEx(void);
	virtual ~QButtonEx(void);

public:
	CImage m_imgBtn;
	BOOL m_bIsHover;
	BOOL m_bIsDown;

public:
	void SetImage(UINT  uID);

public:
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnPaint(WPARAM wParam, LPARAM lParam);

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
