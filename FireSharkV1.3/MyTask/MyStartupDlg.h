#pragma once
#include "ClistCtrlEx.h"
#include "afxcmn.h"


// MyStartupDlg �Ի���

class MyStartupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyStartupDlg)

public:
	MyStartupDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyStartupDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STARTUP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CMenu m_Menu;
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Startup;
	void PrintStartup();
	afx_msg void OnDeletestattup();
	afx_msg void OnRefershstatrup();
	afx_msg void OnRclickListStartup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeydownListStartup(NMHDR *pNMHDR, LRESULT *pResult);
};
