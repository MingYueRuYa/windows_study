
// test_load_method_dllDlg.h : ͷ�ļ�
//

#pragma once


// Ctest_load_method_dllDlg �Ի���
class Ctest_load_method_dllDlg : public CDialogEx
{
// ����
public:
	Ctest_load_method_dllDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEST_LOAD_METHOD_DLL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedClick();
	afx_msg void OnBnClickedFree();
};
