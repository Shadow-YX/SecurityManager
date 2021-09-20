#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"

// MyWindowsDlg 对话框

class MyWindowsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyWindowsDlg)

public:
	MyWindowsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyWindowsDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDOWS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Windows;
	void PrintWindowsList();
}; 
