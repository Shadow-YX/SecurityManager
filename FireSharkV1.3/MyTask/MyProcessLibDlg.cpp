// MyProcessLibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyProcessLibDlg.h"
#include "afxdialogex.h"
#include "MyHead.h"
#include "MD5_FILE.h"


// MyProcessLibDlg 对话框

IMPLEMENT_DYNAMIC(MyProcessLibDlg, CDialogEx)

MyProcessLibDlg::MyProcessLibDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROCESSLIB_DIALOG, pParent)
{

}

MyProcessLibDlg::~MyProcessLibDlg()
{
}

void MyProcessLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_List_Process);
	DDX_Control(pDX, IDC_LIST_LIB, m_List_Lib);
	DDX_Control(pDX, IDC_BUTTON1, m_Button_AddAll);
}


BEGIN_MESSAGE_MAP(MyProcessLibDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROCESS, &MyProcessLibDlg::OnDblclkListProcess)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LIB, &MyProcessLibDlg::OnDblclkListLib)
	ON_BN_CLICKED(IDC_BUTTON1, &MyProcessLibDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &MyProcessLibDlg::OnBnClickedButton7)
END_MESSAGE_MAP()


// MyProcessLibDlg 消息处理程序


BOOL MyProcessLibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_List_Process.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Lib.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_List_Process.InsertColumn(0, TEXT("进程名"), 0, 100);
	m_List_Process.InsertColumn(1, TEXT("进程路径"), 0, 100);
	//m_List_Process.InsertColumn(2, TEXT("文件MD5"), 0, 100);

	m_List_Lib.InsertColumn(0, TEXT("进程名"), 0, 100);
	m_List_Lib.InsertColumn(1, TEXT("文件MD5"), 0, 100);

	TASKLIST nTaskList = {};
	DWORD nNum = EnumTaskList(nTaskList);//枚举进程列表
	CStringA nFileMd5;
	DWORD nIndex;

	for (DWORD i = 0; i < nNum; i++)
	{
		if (nTaskList.szExeFilePath[i].IsEmpty())continue;

		nIndex = m_List_Process.GetItemCount();
		m_List_Process.InsertItem(nIndex, nTaskList.szExeFile[i], 0);
		m_List_Process.SetItemText(nIndex, 1, nTaskList.szExeFilePath[i]);
	}
	ClearTaskList(nTaskList);


	PrintProcessList();


	return TRUE;
}

void MyProcessLibDlg::PrintProcessList()
{
	m_List_Lib.DeleteAllItems();

	LoadVirusLib nLoadProcessLib;
	DWORD nCount = nLoadProcessLib.LoadProcessLibData();
	PVIRUSINFO m_VirusInfo = new VIRUSINFO[nCount];
	nLoadProcessLib.GetProcessLib(m_VirusInfo);
	nLoadProcessLib.ClearProcessLibData();


	for (DWORD i = 0; i < nCount; i++)
	{
		m_List_Lib.InsertItem(i, CString(m_VirusInfo[i].FileName), 0);
		m_List_Lib.SetItemText(i, 1, CString(m_VirusInfo[i].FileMd5));
	}

	delete[]m_VirusInfo;
}

void MyProcessLibDlg::OnDblclkListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	if (pNMItemActivate->iItem == -1)return;

	CString nFileName;
	CString nFilePath;
	CString nFileMd5;

	nFileName = m_List_Process.GetItemText(pNMItemActivate->iItem, 0);
	nFilePath = m_List_Process.GetItemText(pNMItemActivate->iItem, 1);

	nFileMd5 = GetFileMd5(nFilePath);
	if (nFileMd5.IsEmpty())return;

	CString nLibName;
	CString nLibMd5;

	for (INT i = 0 ; i < m_List_Lib.GetItemCount();i++)
	{
		nLibName = m_List_Lib.GetItemText(i, 0);
		nLibMd5 = m_List_Lib.GetItemText(i, 1);

		if (strcmp(CStringA(nFileName), CStringA(nLibName)) == 0)return;
	}

	m_List_Lib.InsertItem(m_List_Lib.GetItemCount(), nFileName);
	m_List_Lib.SetItemText(m_List_Lib.GetItemCount() - 1, 1, nFileMd5);

	UpDataProcessLib();
}


void MyProcessLibDlg::UpDataProcessLib()
{
	DWORD nCount = m_List_Lib.GetItemCount();
	PVIRUSINFO nVirusInfo = new VIRUSINFO[nCount];
	for (DWORD i = 0; i < nCount; i++)
	{
		strcpy_s(nVirusInfo[i].FileName, MAX_PATH, CStringA(m_List_Lib.GetItemText(i, 0)));
		strcpy_s(nVirusInfo[i].FileMd5, MAX_PATH, CStringA(m_List_Lib.GetItemText(i, 1)));
	}

	LoadVirusLib nLoadVirusLib;
	nLoadVirusLib.SetProcessLib(nVirusInfo, nCount);
}

void MyProcessLibDlg::OnDblclkListLib(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	if (pNMItemActivate->iItem == -1)return;

	m_List_Lib.DeleteItem(pNMItemActivate->iItem);

	UpDataProcessLib();


}


void MyProcessLibDlg::OnBnClickedButton1()
{
	m_Button_AddAll.EnableWindow(FALSE);
	CString nFileName;
	CString nFilePath;
	CString nFileMd5;
	DWORD nCount;
	BOOL nHave;

	nCount = m_List_Process.GetItemCount();
	for (DWORD i = 0; i < nCount; i++)
	{
		nFileName = m_List_Process.GetItemText(i, 0);
		nFilePath = m_List_Process.GetItemText(i, 1);

		nFileMd5 = GetFileMd5(nFilePath);
		if (nFileMd5.IsEmpty())continue;;

		CString nLibName;
		CString nLibMd5;
		nHave = FALSE;

		for (INT x = 0; x < m_List_Lib.GetItemCount(); x++)
		{
			nLibName = m_List_Lib.GetItemText(x, 0);
			nLibMd5 = m_List_Lib.GetItemText(x, 1);

			if (strcmp(CStringA(nFileName), CStringA(nLibName)) == 0)
			{
				nHave = TRUE;
				break;
			}
		}

		if (!nHave)
		{
			m_List_Lib.InsertItem(m_List_Lib.GetItemCount(), nFileName);
			m_List_Lib.SetItemText(m_List_Lib.GetItemCount() - 1, 1, nFileMd5);
		}
	}

	UpDataProcessLib();
	m_Button_AddAll.EnableWindow(TRUE);

}


void MyProcessLibDlg::OnBnClickedButton7()
{
	m_List_Lib.DeleteAllItems();
	UpDataProcessLib();
}
