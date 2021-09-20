#pragma once
#include "ClistCtrlEx.h"

// CMyThreadDlg �Ի���

class MyThreadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyThreadDlg)

public:
	MyThreadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyThreadDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYTHREAD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Thread;
	CMenu m_Menu;
	afx_msg void OnSuspendthread();
	afx_msg void OnResumethread();
	afx_msg void OnTerminatethread();
	afx_msg void OnRclickListThread(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListThread(NMHDR *pNMHDR, LRESULT *pResult);
};
