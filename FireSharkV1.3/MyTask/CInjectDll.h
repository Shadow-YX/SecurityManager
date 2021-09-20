#pragma once


// CInjectDll 对话框

class CInjectDll : public CDialogEx
{
	DECLARE_DYNAMIC(CInjectDll)

public:
	CInjectDll(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInjectDll();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_INJECTDLL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSeldll();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	
	CEdit m_edtPut;
};
