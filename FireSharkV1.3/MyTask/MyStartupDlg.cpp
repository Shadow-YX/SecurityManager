// MyStartupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyHead.h"
#include "MyStartupDlg.h"
#include "afxdialogex.h"


// MyStartupDlg 对话框

IMPLEMENT_DYNAMIC(MyStartupDlg, CDialogEx)

MyStartupDlg::MyStartupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STARTUP_DIALOG, pParent)
{
	Create(IDD_STARTUP_DIALOG);
}

MyStartupDlg::~MyStartupDlg()
{
}

void MyStartupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STARTUP, m_List_Startup);
}


BEGIN_MESSAGE_MAP(MyStartupDlg, CDialogEx)
	ON_COMMAND(ID_DELETESTATTUP, &MyStartupDlg::OnDeletestattup)
	ON_COMMAND(ID_REFERSHSTATRUP, &MyStartupDlg::OnRefershstatrup)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_STARTUP, &MyStartupDlg::OnRclickListStartup)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_STARTUP, &MyStartupDlg::OnKeydownListStartup)
END_MESSAGE_MAP()


// MyStartupDlg 消息处理程序


BOOL MyStartupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Menu.LoadMenu(IDR_MENU8);
	SetMenu(&m_Menu);

	m_List_Startup.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Startup.InsertColumn(0, TEXT("软件名称"), 0, 200);
	m_List_Startup.InsertColumn(1, TEXT("命令行"), 0, 600);
	m_List_Startup.InsertColumn(2, TEXT("注册表位置"), 0, 0);

	PrintStartup();

	return TRUE;
}

void MyStartupDlg::PrintStartup()
{
	m_List_Startup.DeleteAllItems();

	static HIMAGELIST nHiml = { 0 };//图标列表
	static SHFILEINFO nPsfi = { 0 };//文件信息
									//ImageList_Destroy(nHiml);//清除图标列表
	static BOOL nOn = FALSE;
	if (!nOn)
	{
		nOn = !nOn;
		nHiml = ImageList_Create(32, 32, ILC_COLOR32, 0, 0);//订制图标框架
		ImageList_SetBkColor(nHiml, m_List_Startup.GetBkColor());//设置图标列表底色
		m_List_Startup.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//设置超级列表显示图标
	}


	TCHAR szValueName[MAXBYTE] = { 0 };
	TCHAR szValueKey[MAXBYTE] = { 0 };

	TCHAR *REG_RUN[2] = { L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run" };
	HKEY HKEY_RUN[2] = { HKEY_CURRENT_USER ,HKEY_LOCAL_MACHINE };

	HKEY hKey = NULL;
	LONG nError;
	CString nPath;

	for (DWORD i = 0; i < _countof(REG_RUN); i++)
	{
		nError = RegOpenKey(HKEY_RUN[i], REG_RUN[i], &hKey);

		if (nError != ERROR_SUCCESS) {
			MessageBox(TEXT("打开注册表失败"), TEXT("提示"), MB_ICONWARNING);
			return;
		}

		DWORD nIndex = 0;
		DWORD dwBufferSize = MAXBYTE;
		while (true)
		{
			nError = RegEnumValue(hKey, nIndex++, szValueKey, &dwBufferSize, 0, 0, 0, &dwBufferSize);

			if (nError == ERROR_NO_MORE_ITEMS || nError != 0) break;

			RegQueryValueEx(hKey, szValueKey, 0, 0, (LPBYTE)szValueName, &dwBufferSize);

			m_List_Startup.InsertItem(m_List_Startup.GetItemCount(), szValueKey);
			m_List_Startup.SetItemText(m_List_Startup.GetItemCount() - 1, 1, GetPathEx(szValueName));
			m_List_Startup.SetItemText(m_List_Startup.GetItemCount() - 1, 2, REG_RUN[i]);
			m_List_Startup.SetItemData(m_List_Startup.GetItemCount() - 1, i);


			dwBufferSize = MAXBYTE;

		}
		RegCloseKey(hKey);
	}



	DWORD nListNum = m_List_Startup.GetItemCount();

	ImageList_SetImageCount(nHiml, nListNum);

	for (DWORD i = 0; i < nListNum; i++)
	{

		nPath = GetPathEx(m_List_Startup.GetItemText(i, 1));

		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//获取文件信息
		if (!SHGetFileInfo(nPath, nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON))
			SHGetFileInfo(nPath, nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON);
		//DWORD nIco = ImageList_AddIcon(nHiml, nPsfi.hIcon);//置入图标返回图标下标
		ImageList_ReplaceIcon(nHiml, i, nPsfi.hIcon);
		DestroyIcon(nPsfi.hIcon);

		m_List_Startup.SetItemImageEx(i, 0, m_List_Startup.GetItemText(i, 0), i);
	}
}

//关闭开启启动项
void MyStartupDlg::OnDeletestattup()
{
	HKEY HKEY_RUN[2] = { HKEY_CURRENT_USER ,HKEY_LOCAL_MACHINE };
	HKEY hKey = NULL;
	LONG nError;
	DWORD nIndex;
	CString nHkeyPath;
	CString nValueKey;
		
	nIndex = m_List_Startup.GetItemData(m_List_Startup.GetSelectionEx());
	nValueKey = m_List_Startup.GetItemText(m_List_Startup.GetSelectionEx(), 0);
	nHkeyPath = m_List_Startup.GetItemText(m_List_Startup.GetSelectionEx(), 2);

	nError = RegOpenKey(HKEY_RUN[nIndex], nHkeyPath, &hKey);
	if (nError != ERROR_SUCCESS) {
		MessageBox(TEXT("打开注册表失败"), TEXT("提示"), MB_ICONWARNING);
		return;
	}
	//删掉选中的该项的键和值
	RegDeleteValue(hKey, nValueKey);
	RegCloseKey(hKey);
	PrintStartup();
}


void MyStartupDlg::OnRefershstatrup()
{
	PrintStartup();
}


void MyStartupDlg::OnRclickListStartup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	CMenu *nMenu = m_Menu.GetSubMenu(0);
	POINT pos;
	GetCursorPos(&pos);

	if (pNMItemActivate->iItem==-1)nMenu->EnableMenuItem(ID_DELETESTATTUP, TRUE);
	else nMenu->EnableMenuItem(ID_DELETESTATTUP, FALSE);

	nMenu->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, this);

}


void MyStartupDlg::OnKeydownListStartup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	*pResult = 0;
	if (pLVKeyDow->wVKey == 116)PrintStartup();

}
