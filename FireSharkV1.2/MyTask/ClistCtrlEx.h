#pragma once


//重写 ClistCtrlEx

class ClistCtrlEx : public CListCtrl
{
	DECLARE_DYNAMIC(ClistCtrlEx)

public:
	ClistCtrlEx();
	virtual ~ClistCtrlEx();


	VOID SetSelectionEx(DWORD nItem)
	{
		SendMessage(LVM_SETSELECTIONMARK, 0, nItem);
		SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		//选中时第二个参数值为1，取消选中时第二个参数为0
	}
	DWORD GetSelectionEx()
	{
		return SendMessage(LVM_GETSELECTIONMARK, 0, 0);
	}

	BOOL IsSelectionItem(DWORD nItem)
	{
		return SendMessage(LVM_GETITEMSTATE, nItem, 2) == 2;
	}

	BOOL SetItemImageEx(int nItem, int nSubItem, CString lpszItem, CString nImage)
	{
		int nTempImage;
		_stscanf_s(nImage, TEXT("%d"), &nTempImage);
		return CListCtrl::SetItem(nItem, nSubItem, 2, lpszItem, nTempImage, 0, 0, 0);
	}
	BOOL SetItemImageEx(int nItem, int nSubItem, CString lpszItem, INT nImage)
	{
		//							行		列		  显示文本  图片索引
		return CListCtrl::SetItem(nItem, nSubItem, 2, lpszItem, nImage, 0, 0, 0);
	}


	CString GetItemDataStr(int nItem)
	{
		CString nTemp;
		nTemp.Format(TEXT("%d"), CListCtrl::GetItemData(nItem));
		return nTemp;
	}


	BOOL SetItemData(int nItem, CString szData)
	{
		DWORD dwData = 0;
		_stscanf_s(szData, TEXT("%d"), &dwData);
		return CListCtrl::SetItemData(nItem, dwData);
	}

	BOOL SetItemData(int nItem, DWORD_PTR dwData)
	{
		return CListCtrl::SetItemData(nItem, dwData);
	}

	BOOL SetItemText(_In_ int nItem, _In_ int nSubItem, _In_z_ LPCTSTR lpszText)
	{
		return CListCtrl::SetItemText(nItem, nSubItem, lpszText);
	}
	BOOL SetItemText(_In_ int nItem, _In_ int nSubItem, _In_z_ LPCSTR lpszText)
	{
		return CListCtrl::SetItemText(nItem, nSubItem, CString(lpszText));
	}
	BOOL SetItemText(_In_ int nItem, _In_ int nSubItem, DWORD Dword)
	{
		CString buf;
		buf.Format(L"%d", Dword);
		return CListCtrl::SetItemText(nItem, nSubItem, buf);
	}

	BOOL SetItemText(_In_ int nItem, _In_ int nSubItem, FILETIME time)
	{
		// 将文件时间转换成本地的文件时间
		FileTimeToLocalFileTime(&time, &time);
		SYSTEMTIME st = { 0 };
		// 将文件时间转换系统时间
		CString str;
		FileTimeToSystemTime(&time, &st);
		str.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		return CListCtrl::SetItemText(nItem, nSubItem, str);
	}

	int InsertItem(int nItem, DWORD lpszItem)
	{
		CString buf;
		buf.Format(L"%d", lpszItem);
		return 	CListCtrl::InsertItem(nItem, buf);
	}

	int InsertItem(_In_ int nItem, _In_z_ LPCTSTR lpszItem)
	{
		return 	CListCtrl::InsertItem(nItem, lpszItem);
	}

	int InsertItem(_In_ int nItem, _In_z_ LPCTSTR lpszItem, _In_ int nImage)
	{
		return 	CListCtrl::InsertItem(nItem, lpszItem, nImage);
	}
	int InsertItem(_In_ int nItem, _In_z_ LPCTSTR lpszItem, CString nImage)
	{
		int nTempImage;
		_stscanf_s(nImage, TEXT("%d"), &nTempImage);

		return 	CListCtrl::InsertItem(nItem, lpszItem, nTempImage);
	}

protected:
	DECLARE_MESSAGE_MAP()
};


