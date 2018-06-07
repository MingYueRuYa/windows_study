
// test_load_method_dllDlg.h : 头文件
//

#pragma once


// Ctest_load_method_dllDlg 对话框
class Ctest_load_method_dllDlg : public CDialogEx
{
// 构造
public:
	Ctest_load_method_dllDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEST_LOAD_METHOD_DLL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedClick();
	afx_msg void OnBnClickedFree();
};
