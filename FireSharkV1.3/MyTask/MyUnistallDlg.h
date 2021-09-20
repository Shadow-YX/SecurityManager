#pragma once
#include "ClistCtrlEx.h"
#include "afxcmn.h"


// MyUnistallDlg �Ի���

class MyUnistallDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyUnistallDlg)

public:
	MyUnistallDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyUnistallDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNISTALL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Unistall;
	CMenu m_Menu;


	void PrintUninstallList();
	afx_msg void OnKeydownListUnistall(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUninstall();
	afx_msg void OnRclickListUnistall(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRefershsoftlist();
};
