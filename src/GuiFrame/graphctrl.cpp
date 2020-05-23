#include "graphctrl.h"

CGraph::CGraph()
{
    m_strGraphTitle = _T("");
    m_clrGraphColor = RGB(0x00, 0x00, 0x00);
    m_bGraphVisible = TRUE;
    m_vecGraphData.clear();
}

CGraph::~CGraph()
{
    m_vecGraphData.clear();
}

CGraph::CGraph(const CGraph &graph)
{
    if(this == &graph) return;

	m_strGraphTitle = graph.m_strGraphTitle;
	m_clrGraphColor = graph.m_clrGraphColor;
	m_bGraphVisible = graph.m_bGraphVisible;
	m_vecGraphData = graph.m_vecGraphData;
}

CGraph &CGraph::operator=(const CGraph &graph)
{
    if(this == &graph) return *this;

	m_strGraphTitle = graph.m_strGraphTitle;
	m_clrGraphColor = graph.m_clrGraphColor;
	m_bGraphVisible = graph.m_bGraphVisible;
	m_vecGraphData = graph.m_vecGraphData;

    return *this;
}

void CGraph::SetGraphTitle(CString strGraphTitle)
{
    m_strGraphTitle = strGraphTitle;
}

void CGraph::SetGraphColor(COLORREF clrGraphColor)
{
    m_clrGraphColor = clrGraphColor;
}

void CGraph::SetGraphVisible(BOOL bGraphVisibale)
{
    m_bGraphVisible = bGraphVisibale;
}

void CGraph::AddGraphData(int x, int y)
{
    CPoint point;
    point.x = x;
    point.y = y;

    m_vecGraphData.push_back(point);
}

CString CGraph::GetGraphTitle() const
{
    return m_strGraphTitle;
}

COLORREF CGraph::GetGraphColor() const
{
    return m_clrGraphColor;
}

BOOL CGraph::GetGraphVisible() const
{
    return m_bGraphVisible;
}

vector<CPoint> CGraph::GetGraphData() const
{
    return m_vecGraphData;
}

void CGraph::Draw(HDC hdc)
{
    int nDataCount = m_vecGraphData.size();
    if (0 == nDataCount) { return; }

    HPEN hPenGraphColor = CreatePen(PS_SOLID, 20, m_clrGraphColor);
    HPEN holdPen = (HPEN)::SelectObject(hdc, hPenGraphColor);

    if (m_bGraphVisible) {
        CPoint pt = m_vecGraphData.at(0);        
        MoveToEx(hdc, pt.x, pt.y, NULL);
        
        for (int i = 0; i < nDataCount; ++i) {
            pt = m_vecGraphData[i];
            LineTo(hdc, pt.x, pt.y); 
            MoveToEx(hdc, pt.x, pt.y, NULL);
        }
    }

    ::SelectObject(hdc, holdPen);
    ::DeleteObject(hPenGraphColor);

}

CGraphCtrl::CGraphCtrl()
{

}

CGraphCtrl::~CGraphCtrl()
{
    int nCount = m_vecGraph.size();

    for (auto item : m_vecGraph) {
        delete item;
        item = NULL;
    }

    m_vecGraph.clear();
}

BOOL CGraphCtrl::Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, QWindow *pParentWnd, UINT nID)
{

    return QWindow::CreateEx(0, _T("GraghCtrl"), lpszWindowName, dwStyle, 
                            rect.left, rect.top, rect.right-rect.left, 
                            rect.bottom-rect.top, pParentWnd->m_hWnd, (HMENU)nID);

}

void CGraphCtrl::OnPrepareDC(HDC hdc)
{
    int nRate = 10;
    SetMapMode(hdc, MM_ANISOTROPIC);
    SetWindowExtEx(hdc, m_rcGraph.Width()*nRate, m_rcGraph.Height()*nRate, NULL);
    SetViewportExtEx(hdc, m_rcGraph.Width(), -m_rcGraph.Height(), NULL);
    SetViewportOrgEx(hdc, m_rcGraph.Width()/2, m_rcGraph.Height()/2, NULL);
}

void CGraphCtrl::OnDraw(HDC hdc)
{
    DrawCoordin(hdc);
    DrawGraph(hdc);
}

void CGraphCtrl::DrawGraph(HDC hdc)
{
    // for_each(m_vecGraph.begin(), m_vecGraph.end(), std::mem_ptr(&CGraph::Draw));

    int nSize = m_vecGraph.size();
	for(int i=0; i<nSize; i++)
	{
		CGraph* pGraph = m_vecGraph.at(i);
		pGraph->Draw(hdc);
	}
}

void CGraphCtrl::DrawTip(HDC hdc)
{
    m_vecTip.clear();

    int nCount = (int)m_vecGraph.size();

    for (int i = 0; i < nCount; ++i) {
        CRect rcTip;
        rcTip.left = i*50;
        rcTip.right = rcTip.left+50;
        rcTip.top = m_rcGraph.bottom;
        rcTip.bottom = rcTip.top+50;
        m_vecTip.push_back(rcTip);

        CGraph *graph = m_vecGraph.at(i);
        HBRUSH brush = ::CreateSolidBrush(graph->GetGraphColor());
        HBRUSH oldBursh = (HBRUSH)::SelectObject(hdc, brush); 
        ::FillRect(hdc, rcTip, brush);
        ::SelectObject(hdc, oldBursh);
        ::DeleteObject(brush);

        BOOL is_visible = graph->GetGraphVisible();
        if (! is_visible) {
            MoveToEx(hdc, rcTip.left, rcTip.top, NULL);
			LineTo(hdc, rcTip.right, rcTip.bottom);

			MoveToEx(hdc, rcTip.right, rcTip.top, NULL);
			LineTo(hdc, rcTip.left, rcTip.bottom);
        }
    }
}


// 绘制坐标系，采用逻辑坐标系
void CGraphCtrl::DrawCoordin(HDC hdc)
{
    POINT ptLeftTop;
    ptLeftTop.x = m_rcGraph.left;
    ptLeftTop.y = m_rcGraph.top;
    DPtoLP(hdc, &ptLeftTop, 1);

    POINT ptRightBottom;
    ptRightBottom.x = m_rcGraph.right;
    ptRightBottom.y = m_rcGraph.bottom;
    DPtoLP(hdc, &ptRightBottom, 1);
    
    HPEN hPen = ::CreatePen(PS_SOLID, 0, RGB(135, 135, 135));
    HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);
    
    // 画坐标系-横坐标
    MoveToEx(hdc, ptLeftTop.x, 0, NULL);
    LineTo(hdc, ptRightBottom.x, 0);
    // 画出逻辑坐标系中，X轴最大值
    CString str;
    str.Format(_T("%d"), ptRightBottom.x);
    SIZE szStr;
    GetTextExtentPoint32(hdc, str.GetBuffer(0), str.GetLength(), &szStr);
    TextOut(hdc, ptRightBottom.x-szStr.cx, 0, str.GetBuffer(0), str.GetLength());

    MoveToEx(hdc, 0, ptLeftTop.y, NULL);
    LineTo(hdc, 0, ptRightBottom.y);

    ::DeleteObject(hPen);
    ::SelectObject(hdc, hOldPen);
    str.Format(_T("%d"), ptLeftTop.y);
    GetTextExtentPoint32(hdc, str.GetBuffer(0), str.GetLength(), &szStr);
    TextOut(hdc, 0, ptLeftTop.y, str.GetBuffer(0), str.GetLength());
}

void CGraphCtrl::AddGraph(CGraph &graph)
{
    CGraph *new_graph = new CGraph(graph);
    m_vecGraph.push_back(new_graph);

    InvalidateRect(m_hWnd, NULL, TRUE);
}

LRESULT CGraphCtrl::OnPaint(WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = ::BeginPaint(m_hWnd, &ps);

    SetBkMode(hdc, TRANSPARENT);

    CRect rcClient;
    GetClientRect(rcClient);
    {
        HBRUSH hBrush = ::CreateSolidBrush(RGB(245, 247, 232));
        ::FillRect(hdc, &rcClient, hBrush);
        DeleteObject((HGDIOBJ)hBrush);
    }

    m_rcGraph = rcClient;
    m_rcGraph.DeflateRect(0, 0, 0, 50);

    DrawTip(hdc);
    OnPrepareDC(hdc);
    OnDraw(hdc);

    ::EndPaint(m_hWnd, &ps);
    return TRUE;
}

LRESULT CGraphCtrl::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
    CPoint pt;
    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);

    int nCount = (int)m_vecTip.size();

    for (int i = 0; i < nCount; ++i) {
        CRect rcTip = m_vecTip.at(i);
		BOOL bPtInRect = rcTip.PtInRect(pt);
		if(bPtInRect)
		{
			CGraph* pGragh = (CGraph*)m_vecGraph.at(i);
			BOOL bGraghVisible = pGragh->GetGraphVisible();
			pGragh->SetGraphVisible(!bGraghVisible);
			InvalidateRect(m_hWnd, NULL, TRUE);
			break;
        }	
    }

    return QWindow::OnLButtonUp(wParam, lParam);
}
