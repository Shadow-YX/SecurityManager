// MyThreadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyThreadDlg.h"
#include "afxdialogex.h"
#include "MyHead.h"

extern DWORD g_Pid;

// CMyThreadDlg 对话框

IMPLEMENT_DYNAMIC(MyThreadDlg, CDialogEx)

MyThreadDlg::MyThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYTHREAD_DIALOG, pParent)
{

}

MyThreadDlg::~MyThreadDlg()
{
}

void MyThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_THREAD, m_List_Thread);
}


BEGIN_MESSAGE_MAP(MyThreadDlg, CDialogEx)
	ON_COMMAND(ID_SUSPENDTHREAD, &MyThreadDlg::OnSuspendthread)
	ON_COMMAND(ID_RESUMETHREAD, &MyThreadDlg::OnResumethread)
	ON_COMMAND(ID_TERMINATETHREAD, &MyThreadDlg::OnTerminatethread)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_THREAD, &MyThreadDlg::OnRclickListThread)
	ON_NOTIFY(NM_CLICK, IDC_LIST_THREAD, &MyThreadDlg::OnClickListThread)
END_MESSAGE_MAP()


// CMyThreadDlg 消息处理程序
void PrintThreadList(ClistCtrlEx &m_List_Thread, THREADLIST &nThreadList, DWORD nThreadNum)
{
	m_List_Thread.DeleteAllItems();

	for (DWORD i = 0; i < nThreadNum; i++)
	{
		DWORD nIndex = m_List_Thread.GetItemCount();

		m_List_Thread.InsertItem(nIndex, nThreadList.th32ThreadID[nIndex] );
		m_List_Thread.SetItemText(nIndex, 1, nThreadList.szExePath[nIndex]);
		m_List_Thread.SetItemText(nIndex, 2, nThreadList.th32OwnerProcessID[nIndex]);
		m_List_Thread.SetItemText(nIndex, 3, nThreadList.tpBasePri[nIndex]);
		m_List_Thread.SetItemText(nIndex, 4, nThreadList.szThreadState[nIndex]);
	}

}


BOOL MyThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Menu.LoadMenu(IDR_MENU2);
	SetMenu(&m_Menu);

	m_List_Thread.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List_Thread.InsertColumn(0, TEXT("线程ID"), 0, 60);
	m_List_Thread.InsertColumn(1, TEXT("进程名"), 0, 120);
	m_List_Thread.InsertColumn(2, TEXT("进程ID"), 0, 60);
	m_List_Thread.InsertColumn(3, TEXT("优先级"), 0, 60);
	m_List_Thread.InsertColumn(4, TEXT("线优状态"), 0, 100);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	THREADLIST nThreadList = {};
	DWORD nNum = EnumThreadList(nThreadList, g_Pid);//枚举进程列表
	PrintThreadList(m_List_Thread, nThreadList, nNum);//打印进程到列表
	ClearThreadList(nThreadList);

	return TRUE;
}


void MyThreadDlg::OnSuspendthread()
{
	DWORD nNow = m_List_Thread.GetSelectionMark();
	CString szPid = m_List_Thread.GetItemText(nNow, 0);

	DWORD nPid;
	_stscanf_s(szPid, TEXT("%d"), &nPid);
	SetThreadState(nPid, TRUE);
}


void MyThreadDlg::OnResumethread()
{
	DWORD nNow = m_List_Thread.GetSelectionMark();
	CString szPid = m_List_Thread.GetItemText(nNow, 0);

	DWORD nPid;
	_stscanf_s(szPid, TEXT("%d"), &nPid);
	SetThreadState(nPid,FALSE);
}


void MyThreadDlg::OnTerminatethread()
{
	DWORD nNow = m_List_Thread.GetSelectionMark();
	CString szPid = m_List_Thread.GetItemText(nNow, 0);

	DWORD nPid;
	_stscanf_s(szPid, TEXT("%d"), &nPid);
	EndThread(nPid);
}

void MyThreadDlg::OnClickListThread(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

}


void MyThreadDlg::OnRclickListThread(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if (m_List_Thread.GetSelectionMark() != -1)
	{
		CMenu *nMenu = m_Menu.GetSubMenu(0);
		POINT pos;
		GetCursorPos(&pos);
		nMenu->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, this);
	}


}



