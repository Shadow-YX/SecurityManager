#pragma once
#include "ClistCtrlEx.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "MyHead.h"
#include"CAddVirus.h"

// MyAntiVirusDlg �Ի���

class MyAntiVirusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyAntiVirusDlg)

public:
	MyAntiVirusDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyAntiVirusDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANTIVIRUS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	CMenu m_Menu;
	CImageList  m_VirusImageList;
	CImageList  m_ProcessImageList;

	PVIRUSINFO m_VirusInfo = nullptr;//��Ų�����Ϣ
	DWORD m_VirusInfoNum = 0;//����������

	BOOL m_IsScaning = FALSE;//ɨ����̵߳Ŀ���

	CString m_ScaningPath;//����ɨ���·��
	DWORD m_ScaningNum;//��ɨ���ļ�����
	DWORD m_VirusNum;//��ɨ�財������
	CString m_ThisPath;//ָ��λ��ɨ��

	DWORD REFERSHPATH_TIMER = 0x888;


	CButton m_Radio_All_Control;
	CButton m_Radio_This_Control;
	CButton m_Check_Cloud;
	CButton m_Check_End;

	BOOL m_Radio_All_Value;
	CButton m_Button_Start_File;
	CButton m_Button_Start_Process;
	ClistCtrlEx m_List_File;
	ClistCtrlEx m_List_Process;
	CStatic m_Static_Path;
	CStatic m_Static_Group;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnClearonece();
	afx_msg void OnClearall();
	afx_msg void OnRclickListFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton7();


	void ScanFile( CString dir);
	void ScanProcess();

	void LoadVirus();
	void AddEditData(CEdit &m_Edit_Contral, CString &nData);
	void LoadImageVirus();
	void LoadImageProcess();


	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedCheck1();

};
