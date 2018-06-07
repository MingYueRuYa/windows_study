
// test_load_method_dllDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "test_load_method_dll.h"
#include "test_load_method_dllDlg.h"
#include "afxdialogex.h"
#include "load_method_dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctest_load_method_dllDlg �Ի���



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


// Ctest_load_method_dllDlg ��Ϣ�������

BOOL Ctest_load_method_dllDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Ctest_load_method_dllDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Ctest_load_method_dllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HINSTANCE hLibrary = 0;

void Ctest_load_method_dllDlg::OnBnClickedLoad()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	if (0 != hLibrary) { return; }
//	hLibrary = LoadLibrary(TEXT("load_method_dll.dll"));
}

typedef int(*Demo)(void);
typedef DWORD (*DemoW)(void);

void Ctest_load_method_dllDlg::OnBnClickedClick()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
