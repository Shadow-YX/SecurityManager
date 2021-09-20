// MyWindowsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyWindowsDlg.h"
#include "afxdialogex.h"
#include "MyHead.h"


// MyWindowsDlg �Ի���

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


// MyWindowsDlg ��Ϣ�������

BOOL WINAPI WindowsProc(HWND nHwnd, LPARAM nLparam)
{
	ClistCtrlEx *m_List_Windows = (ClistCtrlEx*)nLparam;

	TCHAR buff[200];
	GetWindowText(nHwnd, buff,_countof(buff));

	if (IsWindowVisible(nHwnd) && _tcslen(buff) && _tcscmp(buff,TEXT("��ʼ")))
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

	static HIMAGELIST nHiml = { 0 };//ͼ���б�
	static SHFILEINFO nPsfi = { 0 };//�ļ���Ϣ
									//ImageList_Destroy(nHiml);//���ͼ���б�
	static BOOL nOn = FALSE;
	if (!nOn)
	{
		nOn = !nOn;
		nHiml = ImageList_Create(24, 24, ILC_COLOR32, 0, 0);//����ͼ����
		ImageList_SetBkColor(nHiml, m_List_Windows.GetBkColor());//����ͼ���б��ɫ
		m_List_Windows.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//���ó����б���ʾͼ��
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

		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//��ȡ�ļ���Ϣ
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
	m_List_Windows.InsertColumn(0, TEXT("��ǰ����"), 0, 880);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	PrintWindowsList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
