#pragma once


// CAddVirus 

class CAddVirus : public CDialogEx
{
	DECLARE_DYNAMIC(CAddVirus)

public:
	CAddVirus(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAddVirus();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_ADDVIRUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
