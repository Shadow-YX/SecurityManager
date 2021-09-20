#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"
#include "LoadVirusLib.h"
#include "afxwin.h"


// MyProcessLibDlg �Ի���

class MyProcessLibDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyProcessLibDlg)

public:
	MyProcessLibDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyProcessLibDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSLIB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Process;
	ClistCtrlEx m_List_Lib;
	afx_msg void OnDblclkListProcess(NMHDR *pNMHDR, LRESULT *pResult);
	void UpDataProcessLib();
	void PrintProcessList();
	afx_msg void OnDblclkListLib(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton7();
	CButton m_Button_AddAll;
};
