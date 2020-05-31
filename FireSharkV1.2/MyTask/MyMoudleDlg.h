#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"


// MyMoudleDlg 对话框

class MyMoudleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyMoudleDlg)

public:
	MyMoudleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyMoudleDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYMOUDLEDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Module;
	afx_msg void OnDblclkListModule(NMHDR *pNMHDR, LRESULT *pResult);
};
