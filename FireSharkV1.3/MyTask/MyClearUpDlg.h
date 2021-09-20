#pragma once
#include "ClistCtrlEx.h"
#include "MyHead.h"
#include "afxcmn.h"
#include "afxwin.h"


// MyClearUpDlg 对话框

class MyClearUpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyClearUpDlg)

public:
	MyClearUpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyClearUpDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLEARUP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	ClistCtrlEx m_List_File;
	CButton m_Button_Scan;
	CButton m_Button_Clear;

	BOOL IsScanFile;
	CString m_szTemp;
	DOUBLE m_AllFileSize;
	CStatic m_Static_Path;
	CString m_ScaningPath;

	DWORD TIMER_SCANPATH = 0x8888;
	DWORD TIMER_CLEARATH = 0x999;

	CString GetShowSize(DOUBLE dwSize);
	BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni = FALSE, BOOL bWipeIndexDat = FALSE);
	BOOL WipeFile(LPCTSTR szDir, LPCTSTR szFile);
	BOOL DeleteUrlCache();
	
	VOID CleanSystemTemp(); //系统垃圾
	VOID CleanRubbishStation();  //回收站垃圾
	VOID CleanInternetCookieTemp();  //浏览器垃圾
	VOID CleanBrowseAddrHistory();  //浏览器历史记录
	VOID CleanPasswordHistory();   //清理历史密码记录



	afx_msg void OnBnClickedButton7();
	CButton m_Check_SystemTemp;
	CButton m_Check_Rubbish;
	CButton m_Check_InterCookies;
	CButton m_Check_PassHistory;
	CButton m_Check_BrowseHistroy;
	afx_msg void OnBnClickedButton9();
};
