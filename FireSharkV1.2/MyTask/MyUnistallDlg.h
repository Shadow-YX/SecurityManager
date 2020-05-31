#pragma once
#include "ClistCtrlEx.h"
#include "afxcmn.h"


// MyUnistallDlg 对话框

class MyUnistallDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyUnistallDlg)

public:
	MyUnistallDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyUnistallDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNISTALL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
