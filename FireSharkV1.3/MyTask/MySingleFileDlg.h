#pragma once
#include "afxwin.h"


// MySingleFileDlg �Ի���

class MySingleFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MySingleFileDlg)

public:
	MySingleFileDlg(CWnd* pParent , CString nPath);   // ��׼���캯��
	virtual ~MySingleFileDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SINGLEFILE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_Path;
	CEdit m_Edit_FileName;
	CEdit m_Edit_Size;
	CEdit m_Edit_Create;
	CEdit m_Edit_Change;
	CEdit m_Edit_Access;
	CEdit m_Edit_MD5;
	afx_msg void OnClose();
};
