#pragma once
#include "ClistCtrlEx.h"
#include "afxcmn.h"

// MyVirusLibDlg 对话框

class MyVirusLibDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyVirusLibDlg)

public:
	MyVirusLibDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyVirusLibDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIRUSLIB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMenu m_Menu;
	CImageList  m_VirusImageList;

	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_VirusLib;
	afx_msg void OnDeleteallvirus(); //全部删除
	afx_msg void OnDeleteonece();  //单个删除
	afx_msg void OnRclickListFile(NMHDR *pNMHDR, LRESULT *pResult);  //右键弹出菜单
	void UpDataVirusLib();  //更新病毒菜单
	void LoadImageVirus();  //加载图标
	void PrintVirusList();  //打印病毒
	afx_msg void OnAddvirus();  //上传病毒至本地病毒库
};
