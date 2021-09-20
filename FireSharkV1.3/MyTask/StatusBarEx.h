#pragma once


// CStatusBarEx

class CStatusBarEx : public CStatusBar
{
	DECLARE_DYNAMIC(CStatusBarEx)

public:
	CStatusBarEx();
	virtual ~CStatusBarEx();


	BOOL SetPaneText(int nIndex, LPCTSTR lpszNewText)//��д����ֹ�̲߳�������
	{
		SendMessage(SB_SETTEXT, nIndex, (LPARAM)lpszNewText);
		return TRUE;
	}

	BOOL SetPaneText(int nIndex, DWORD lpszNewText)//��д����ֹ�̲߳�������
	{
		CString nStr;
		nStr.Format(TEXT("%d"), lpszNewText);
		SendMessage(SB_SETTEXT, nIndex, (LPARAM)&(*nStr));
		return TRUE;
	}

	BOOL SetPaneText(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate)
	{
		return CStatusBar::SetPaneText(nIndex,lpszNewText,bUpdate);
	}

	BOOL SetPaneText(int nIndex, DWORD lpszNewText, BOOL bUpdate)
	{
		CString nStr;
		nStr.Format(TEXT("%d"), lpszNewText);
		return CStatusBar::SetPaneText(nIndex, nStr, bUpdate);
	}
protected:
	DECLARE_MESSAGE_MAP()
};


