#ifndef guiframe_graph_ctrl_h
#define guiframe_graph_ctrl_h

#include <vector>

#include "qwindow.h"
#include "globalhead.h"

using std::vector;

using BIHUA = vector<CPoint>;


#define TEST_DRAW_LIEN 1

class CGraph
{
public:
    CGraph();
    ~CGraph();
    CGraph(const CGraph &grash);

public:
    CGraph &operator=(const CGraph &grash);

public:
    void SetGraphTitle(CString strGraphTitle);
    void SetGraphColor(COLORREF clrGraphColor);
    void SetGraphVisible(BOOL bGraphVisibale);
    void AddGraphData(int x, int y);
    void AddBiHua(BIHUA biHua);

public:
    CString GetGraphTitle() const;
    COLORREF GetGraphColor() const;
    BOOL GetGraphVisible() const;
    vector<CPoint> GetGraphData() const;

public:
    void Draw(HDC hdc);

private:
    CString m_strGraphTitle;
    COLORREF m_clrGraphColor;
    BOOL m_bGraphVisible;
    vector<CPoint> m_vecGraphData;
// ²âÊÔ»­×Ö°æ
    vector<BIHUA> mBiHuaData;

};

class CGraphCtrl : public QWindow
{
public: 
    CGraphCtrl();
    virtual ~CGraphCtrl();

public:
    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, QWindow *pParentWnd, UINT nID);

public:
    void OnPrepareDC(HDC hdc);
    void OnDraw(HDC hdc);
    void DrawGraph(HDC hdc);
    void DrawTip(HDC hdc);

public:
    void DrawCoordin(HDC hdc);

public:
    void AddGraph(CGraph &graph);

public:
    vector<CGraph *> m_vecGraph;
    vector<CRect> m_vecTip;
    CRect m_rcGraph;

public:
    virtual LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);

};

#endif // guiframe_graph_ctrl_h