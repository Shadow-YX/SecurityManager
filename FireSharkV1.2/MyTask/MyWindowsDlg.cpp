// MyWindowsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyWindowsDlg.h"
#include "afxdialogex.h"
#include "MyHead.h"


// MyWindowsDlg 对话框

IMPLEMENT_DYNAMIC(MyWindowsDlg, CDialogEx)

MyWindowsDlg::MyWindowsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WINDOWS_DIALOG, pParent)
{
	Create(IDD_WINDOWS_DIALOG);
}

MyWindowsDlg::~MyWindowsDlg()
{
}

void MyWindowsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WINDOWS, m_List_Windows);
}


BEGIN_MESSAGE_MAP(MyWindowsDlg, CDialogEx)
END_MESSAGE_MAP()


// MyWindowsDlg 消息处理程序

BOOL WINAPI WindowsProc(HWND nHwnd, LPARAM nLparam)
{
	ClistCtrlEx *m_List_Windows = (ClistCtrlEx*)nLparam;

	TCHAR buff[200];
	GetWindowText(nHwnd, buff,_countof(buff));

	if (IsWindowVisible(nHwnd) && _tcslen(buff) && _tcscmp(buff,TEXT("开始")))
	{
		DWORD nIndex = m_List_Windows->GetItemCount();
		DWORD nPid = 0;
		m_List_Windows->InsertItem(nIndex, buff);
		GetWindowThreadProcessId(nHwnd, &nPid);
		m_List_Windows->SetItemData(nIndex, nPid);
	}
	return TRUE;
}

void MyWindowsDlg::PrintWindowsList()
{
	m_List_Windows.DeleteAllItems();

	EnumWindows(WindowsProc, (LPARAM)&m_List_Windows);

	static HIMAGELIST nHiml = { 0 };//图标列表
	static SHFILEINFO nPsfi = { 0 };//文件信息
									//ImageList_Destroy(nHiml);//清除图标列表
	static BOOL nOn = FALSE;
	if (!nOn)
	{
		nOn = !nOn;
		nHiml = ImageList_Create(24, 24, ILC_COLOR32, 0, 0);//订制图标框架
		ImageList_SetBkColor(nHiml, m_List_Windows.GetBkColor());//设置图标列表底色
		m_List_Windows.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//设置超级列表显示图标
	}

	DWORD nListNum = m_List_Windows.GetItemCount();
	ImageList_SetImageCount(nHiml, nListNum);

	for (DWORD i = 0; i < nListNum; i++)
	{
		CString nFileRoute;
		DWORD_PTR nPid;
		HANDLE nHandle;
		nFileRoute = m_List_Windows.GetItemText(i, 0);
		nPid = m_List_Windows.GetItemData(i);
		nHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)nPid);
		GetProcessFilePath(nHandle, nFileRoute);
		CloseHandle(nHandle);

		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//获取文件信息
		if (!SHGetFileInfo(nFileRoute, nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON))
			SHGetFileInfo(nFileRoute, nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON);
		ImageList_ReplaceIcon(nHiml, i, nPsfi.hIcon);
		DestroyIcon(nPsfi.hIcon);

		m_List_Windows.SetItem(i, 0, 2, TEXT(""), i, 0, 0, 0);
	}
}

BOOL MyWindowsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_List_Windows.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Windows.InsertColumn(0, TEXT("当前窗口"), 0, 880);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	PrintWindowsList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
