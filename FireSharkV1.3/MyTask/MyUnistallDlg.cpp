// MyUnistallDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyHead.h"
#include "MyUnistallDlg.h"
#include "afxdialogex.h"


// MyUnistallDlg �Ի���

IMPLEMENT_DYNAMIC(MyUnistallDlg, CDialogEx)

MyUnistallDlg::MyUnistallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UNISTALL_DIALOG, pParent)
{
	Create(IDD_UNISTALL_DIALOG);
}

MyUnistallDlg::~MyUnistallDlg()
{
}

void MyUnistallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UNISTALL, m_List_Unistall);
}


BEGIN_MESSAGE_MAP(MyUnistallDlg, CDialogEx)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_UNISTALL, &MyUnistallDlg::OnKeydownListUnistall)
	ON_COMMAND(ID_UNINSTALL, &MyUnistallDlg::OnUninstall)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_UNISTALL, &MyUnistallDlg::OnRclickListUnistall)
	ON_COMMAND(ID_REFERSHSOFTLIST, &MyUnistallDlg::OnRefershsoftlist)
END_MESSAGE_MAP()


// MyUnistallDlg ��Ϣ�������


BOOL MyUnistallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Menu.LoadMenu(IDR_MENU4);
	SetMenu(&m_Menu);



	m_List_Unistall.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Unistall.InsertColumn(0, TEXT("�������"), 0, 280);
	m_List_Unistall.InsertColumn(1, TEXT("��Ʒ��˾"), 0, 300);
	m_List_Unistall.InsertColumn(2, TEXT("�汾"), 0, 100);
	m_List_Unistall.InsertColumn(3, TEXT("ж��Ŀ¼"), 0, 200);


	PrintUninstallList();

	return TRUE;
}

void MyUnistallDlg::PrintUninstallList()
{
	m_List_Unistall.DeleteAllItems();

	static HIMAGELIST nHiml = { 0 };//ͼ���б�
	static SHFILEINFO nPsfi = { 0 };//�ļ���Ϣ
									//ImageList_Destroy(nHiml);//���ͼ���б�
	static BOOL nOn = FALSE;
	if (!nOn)
	{
		nOn = !nOn;
		nHiml = ImageList_Create(16, 16, ILC_COLOR32, 0, 0);//����ͼ����
		ImageList_SetBkColor(nHiml, m_List_Unistall.GetBkColor());//����ͼ���б��ɫ
		m_List_Unistall.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//���ó����б���ʾͼ��
	}


	TCHAR szValueName[MAXBYTE] = { 0 };
	TCHAR szValueKey[MAXBYTE] = { 0 };

	DWORD dwBufferSize = MAXBYTE;

	CString REG_RUN = TEXT("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
	CString nPath;

	HKEY hKey = NULL;
	HKEY hKey2 = NULL;

	LONG nError;

	nError = RegOpenKey(HKEY_LOCAL_MACHINE, REG_RUN, &hKey);

	if (nError != ERROR_SUCCESS) {
		MessageBox(TEXT("��ע���ʧ��"), TEXT("��ʾ"), MB_ICONWARNING);
		return ;
	}

	DWORD i = 0;

	while (true)
	{
		nError = RegEnumKey(hKey, i++, szValueKey, MAXBYTE);
		if (nError == ERROR_NO_MORE_ITEMS || nError != 0) break;

		nError = RegOpenKey(HKEY_LOCAL_MACHINE, REG_RUN + TEXT("\\") + szValueKey + TEXT("\\"), &hKey2);
		nError = RegQueryValueEx(hKey2, L"DisplayName", 0, 0, (LPBYTE)szValueName, &dwBufferSize);

		if (nError == ERROR_FILE_NOT_FOUND || nError != 0) 
		{ 
			RegCloseKey(hKey2);
			continue;
		}


		m_List_Unistall.InsertItem(m_List_Unistall.GetItemCount(), szValueName);
		dwBufferSize = MAXBYTE;
		//��������
		nError = RegQueryValueEx(hKey2, L"Publisher", 0, 0, (LPBYTE)szValueName, &dwBufferSize);
		m_List_Unistall.SetItemText(m_List_Unistall.GetItemCount() - 1, 1, szValueName);
		dwBufferSize = MAXBYTE;
		//��װ����
		nError = RegQueryValueEx(hKey2, L"DisplayVersion", 0, 0, (LPBYTE)szValueName, &dwBufferSize);
		m_List_Unistall.SetItemText(m_List_Unistall.GetItemCount() - 1, 2, szValueName);
		dwBufferSize = MAXBYTE;
		//ж����Ϣ
		nError = RegQueryValueEx(hKey2, L"UninstallString", 0, 0, (LPBYTE)szValueName, &dwBufferSize);
		m_List_Unistall.SetItemText(m_List_Unistall.GetItemCount() - 1, 3, GetPathEx(szValueName));
		dwBufferSize = MAXBYTE;

		RegCloseKey(hKey2);
	}

	DWORD nListNum = m_List_Unistall.GetItemCount();

	ImageList_SetImageCount(nHiml, nListNum);

	for (DWORD i =0;i<nListNum;i++)
	{
		nPath = GetPathEx(m_List_Unistall.GetItemText(i, 3));

		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//��ȡ�ļ���Ϣ
		if (!SHGetFileInfo(nPath, nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON))
			SHGetFileInfo(nPath, nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON);
		//DWORD nIco = ImageList_AddIcon(nHiml, nPsfi.hIcon);//����ͼ�귵��ͼ���±�
		ImageList_ReplaceIcon(nHiml, i, nPsfi.hIcon);
		DestroyIcon(nPsfi.hIcon);

		m_List_Unistall.SetItemImageEx(i, 0, m_List_Unistall.GetItemText(i, 0), i);
	}

	RegCloseKey(hKey);
}


void MyUnistallDlg::OnKeydownListUnistall(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	*pResult = 0;
	if (pLVKeyDow->wVKey == 116)PrintUninstallList();

}


void MyUnistallDlg::OnUninstall()
{
	CString nPath = m_List_Unistall.GetItemText(m_List_Unistall.GetSelectionEx(), 3);
	ShellExecute(NULL,NULL,nPath,NULL,NULL,NULL);
}


void MyUnistallDlg::OnRclickListUnistall(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	CMenu *nMenu = m_Menu.GetSubMenu(0);
	POINT pos;
	GetCursorPos(&pos);

	if (pNMItemActivate->iItem == -1)nMenu->EnableMenuItem(ID_UNINSTALL,TRUE);
	else nMenu->EnableMenuItem(ID_UNINSTALL, FALSE);

	nMenu->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, this);

}


void MyUnistallDlg::OnRefershsoftlist()
{
	PrintUninstallList();
}
