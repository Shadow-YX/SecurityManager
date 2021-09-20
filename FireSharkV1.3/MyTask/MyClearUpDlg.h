#pragma once
#include "ClistCtrlEx.h"
#include "MyHead.h"
#include "afxcmn.h"
#include "afxwin.h"


// MyClearUpDlg �Ի���

class MyClearUpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyClearUpDlg)

public:
	MyClearUpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyClearUpDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLEARUP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	
	VOID CleanSystemTemp(); //ϵͳ����
	VOID CleanRubbishStation();  //����վ����
	VOID CleanInternetCookieTemp();  //���������
	VOID CleanBrowseAddrHistory();  //�������ʷ��¼
	VOID CleanPasswordHistory();   //������ʷ�����¼



	afx_msg void OnBnClickedButton7();
	CButton m_Check_SystemTemp;
	CButton m_Check_Rubbish;
	CButton m_Check_InterCookies;
	CButton m_Check_PassHistory;
	CButton m_Check_BrowseHistroy;
	afx_msg void OnBnClickedButton9();
};
