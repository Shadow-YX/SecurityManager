#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"

// MyWindowsDlg �Ի���

class MyWindowsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyWindowsDlg)

public:
	MyWindowsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyWindowsDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDOWS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Windows;
	void PrintWindowsList();
}; 
