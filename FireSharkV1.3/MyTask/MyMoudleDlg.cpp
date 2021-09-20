// MyMoudleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyMoudleDlg.h"
#include "afxdialogex.h"
#include "MyHead.h"

extern DWORD g_Pid;

// MyMoudleDlg �Ի���

IMPLEMENT_DYNAMIC(MyMoudleDlg, CDialogEx)

MyMoudleDlg::MyMoudleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYMOUDLEDIALOG, pParent)
{

}

MyMoudleDlg::~MyMoudleDlg()
{
}

void MyMoudleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODULE, m_List_Module);
}


BEGIN_MESSAGE_MAP(MyMoudleDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MODULE, &MyMoudleDlg::OnDblclkListModule)
END_MESSAGE_MAP()


// MyMoudleDlg ��Ϣ�������
void PrintModuleList(ClistCtrlEx &m_List_Module, MODULELIST &nModuleList, DWORD nTaskNum)
{

	for (DWORD i = 0; i < nTaskNum; i++)
	{
		DWORD nIndex = m_List_Module.GetItemCount();
		m_List_Module.InsertItem(nIndex, nModuleList.szModule[nIndex]);
		m_List_Module.SetItemText(nIndex, 1, nModuleList.szExePath[nIndex]);
		CString str;
		str.Format(L"%p", nModuleList.modBaseAddr[nIndex]);
		m_List_Module.SetItemText(nIndex, 2, str);
		m_List_Module.SetItemText(nIndex, 3, nModuleList.dwSize[nIndex]);
	}
}


//��ʾdllģ����Ϣ
BOOL MyMoudleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_List_Module.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List_Module.InsertColumn(0, TEXT("ģ����"), 0, 100);
	m_List_Module.InsertColumn(1, TEXT("ģ��·��"), 0, 350);
	m_List_Module.InsertColumn(2, TEXT("ģ���ַ"), 0, 80);
	m_List_Module.InsertColumn(3, TEXT("ģ���С"), 0, 60);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	MODULELIST nModuleList = {};
	DWORD nNum = EnumModuleList(nModuleList,g_Pid);//ö�ٽ����б�
	PrintModuleList(m_List_Module, nModuleList, nNum);//��ӡ���̵��б�
	ClearModuleList(nModuleList);

	return TRUE; 
}


void MyMoudleDlg::OnDblclkListModule(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	DWORD nNow = m_List_Module.GetSelectionMark();

	CString szExeName = m_List_Module.GetItemText(nNow, 1);
	OpenFile(szExeName);
}
