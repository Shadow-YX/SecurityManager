
// MyTaskDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"
#include "StatusBarEx.h"
#include "afxwin.h"
#include "MyMoudleDlg.h"
#include "MyThreadDlg.h"
#include "MyWindowsDlg.h"
#include "MyUnistallDlg.h"
#include "MyStartupDlg.h"
#include "MyAntiVirusDlg.h"
#include "MyClearUpDlg.h"
#include "MySingleFileDlg.h"


// CMyTaskDlg 对话框
class CMyTaskDlg : public CDialogEx
{
// 构造
public:
	CMyTaskDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYTASK_DIALOG };
#endif

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
	HMODULE hModule;
	ClistCtrlEx m_List_Task;
	CMenu m_Menu;
	CStatusBarEx m_Status; 
	BOOL m_CheckMenu = FALSE;

	MyWindowsDlg *m_PWindowsDlg=nullptr;
	MyUnistallDlg *m_PUnistallDlg = nullptr;
	MyStartupDlg *m_PStartupDlg = nullptr;
	MyAntiVirusDlg *m_PAntiVirusDlg = nullptr;
	MyClearUpDlg *m_PClearUpDlg = nullptr;

	DWORD m_HotKey = 5235;

	const DWORD TIMER_REFRESHTASKLIST = 0x10000;
	const DWORD STATUS_0 = 0x10001;
	const DWORD STATUS_1 = 0x10002;
	const DWORD STATUS_2 = 0x10003;
	const DWORD STATUS_3 = 0x10004;

	afx_msg void OnRclickListTask(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEndprocess();
	afx_msg void OnEnummoudle();
	afx_msg void OnOpenfile();
	afx_msg void OnSuspenprocess();
	afx_msg void OnResumeprocess();
	afx_msg void OnEnumthread();

	void Fun();
	void HookTaskmgr();
	void UnHookTaskmgr();
	void LoadSafeSkin();
	afx_msg void OnBnClickedRadio2();
	CButton m_Button_Process;
	CButton m_Button_AntiVirus;
	afx_msg void OnTop();
	afx_msg void OnUpdateTop(CCmdUI *pCmdUI);
	afx_msg void OnGuangji();
	afx_msg void OnChongqi();
	afx_msg void OnZhuxiao();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnFileinfo();
	afx_msg void OnEnd();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio11();
	afx_msg void OnSinglefileinfo();
	afx_msg void OnInjectDll();
	afx_msg void OnClose();
	afx_msg void OnLock();
};
