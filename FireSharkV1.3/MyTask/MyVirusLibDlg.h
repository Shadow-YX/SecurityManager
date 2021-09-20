#pragma once
#include "ClistCtrlEx.h"
#include "afxcmn.h"

// MyVirusLibDlg �Ի���

class MyVirusLibDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyVirusLibDlg)

public:
	MyVirusLibDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyVirusLibDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIRUSLIB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CMenu m_Menu;
	CImageList  m_VirusImageList;

	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_VirusLib;
	afx_msg void OnDeleteallvirus(); //ȫ��ɾ��
	afx_msg void OnDeleteonece();  //����ɾ��
	afx_msg void OnRclickListFile(NMHDR *pNMHDR, LRESULT *pResult);  //�Ҽ������˵�
	void UpDataVirusLib();  //���²����˵�
	void LoadImageVirus();  //����ͼ��
	void PrintVirusList();  //��ӡ����
	afx_msg void OnAddvirus();  //�ϴ����������ز�����
};
