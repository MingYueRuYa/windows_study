
// test_load_method_dllDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "test_load_method_dll.h"
#include "test_load_method_dllDlg.h"
#include "afxdialogex.h"
#include "load_method_dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctest_load_method_dllDlg 对话框



Ctest_load_method_dllDlg::Ctest_load_method_dllDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ctest_load_method_dllDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ctest_load_method_dllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Ctest_load_method_dllDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_LOAD, &Ctest_load_method_dllDlg::OnBnClickedLoad)
	ON_BN_CLICKED(ID_CLICK, &Ctest_load_method_dllDlg::OnBnClickedClick)
	ON_BN_CLICKED(ID_FREE, &Ctest_load_method_dllDlg::OnBnClickedFree)
END_MESSAGE_MAP()


// Ctest_load_method_dllDlg 消息处理程序

BOOL Ctest_load_method_dllDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Ctest_load_method_dllDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Ctest_load_method_dllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HINSTANCE hLibrary = 0;

void Ctest_load_method_dllDlg::OnBnClickedLoad()
{
	// TODO:  在此添加控件通知处理程序代码
//	if (0 != hLibrary) { return; }
//	hLibrary = LoadLibrary(TEXT("load_method_dll.dll"));
}

typedef int(*Demo)(void);
typedef DWORD (*DemoW)(void);

void Ctest_load_method_dllDlg::OnBnClickedClick()
{
	// TODO:  在此添加控件通知处理程序代码
//	Demo demo = (Demo)GetProcAddress(hLibrary, "fnload_method_dll");
//	DWORD errlast = GetLastError();
//	demo();
	
	fnload_method_dll();
}

void Ctest_load_method_dllDlg::OnBnClickedFree()
{
	if (0 == hLibrary) { return; }

	FreeLibrary(hLibrary);
}
