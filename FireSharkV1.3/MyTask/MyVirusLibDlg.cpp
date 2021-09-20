// MyVirusLibDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyHead.h"
#include "MyVirusLibDlg.h"
#include "afxdialogex.h"
#include "MD5_FILE.h"
#include "LoadVirusLib.h"
#include"CAddVirus.h"

// MyVirusLibDlg �Ի���

IMPLEMENT_DYNAMIC(MyVirusLibDlg, CDialogEx)

MyVirusLibDlg::MyVirusLibDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIRUSLIB_DIALOG, pParent)
{

}

MyVirusLibDlg::~MyVirusLibDlg()
{
}

void MyVirusLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_List_VirusLib);
}


BEGIN_MESSAGE_MAP(MyVirusLibDlg, CDialogEx)
	ON_WM_DROPFILES()
	//ON_COMMAND(ID_UPDATAVIRUS, &MyVirusLibDlg::OnUpdatavirus)
	//ON_COMMAND(ID_DOWNVIRUS, &MyVirusLibDlg::OnDownvirus)
	ON_COMMAND(ID_DELETEALLVIRUS, &MyVirusLibDlg::OnDeleteallvirus)
	ON_COMMAND(ID_DELETEONECE, &MyVirusLibDlg::OnDeleteonece)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILE, &MyVirusLibDlg::OnRclickListFile)
	ON_COMMAND(ID_ADDVIRUS, &MyVirusLibDlg::OnAddvirus)
END_MESSAGE_MAP()


// MyVirusLibDlg ��Ϣ�������


BOOL MyVirusLibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Menu.LoadMenu(IDR_MENU5);
	SetMenu(&m_Menu);

	LoadImageVirus();

	m_List_VirusLib.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_VirusLib.InsertColumn(0, TEXT("�����ļ���"), 0, 260);
	m_List_VirusLib.InsertColumn(1, TEXT("�����ļ�MD5"), 0, 250);

	PrintVirusList(); //��ʾ����
	return TRUE;
}

//���ز���ͼ��
void MyVirusLibDlg::LoadImageVirus()
{
	m_VirusImageList.Create(20, 20, ILC_COLOR32 | ILC_MASK, 2, 2);

	HICON hSmallIco;
	CString nDir = GetProgramDir();

	hSmallIco = ExtractIcon(AfxGetInstanceHandle(), nDir + TEXT("res\\virus.ico"), 0);
	m_VirusImageList.Add(hSmallIco);
	DestroyIcon(hSmallIco);

	m_List_VirusLib.SetImageList(&m_VirusImageList, LVSIL_SMALL);
}

//�ڲ���������ʾ��������
void MyVirusLibDlg::PrintVirusList()
{
	m_List_VirusLib.DeleteAllItems();

	LoadVirusLib nLoadVirusLib;
	DWORD nCount = nLoadVirusLib.LoadVirusLibData();
	PVIRUSINFO m_VirusInfo = new VIRUSINFO[nCount];
	nLoadVirusLib.GetVirusLib(m_VirusInfo);
	nLoadVirusLib.ClearVirusLibData();


	for (DWORD i = 0; i < nCount; i++)
	{
		m_List_VirusLib.InsertItem(i, CString(m_VirusInfo[i].FileName), 0);
		m_List_VirusLib.SetItemText(i, 1, CString(m_VirusInfo[i].FileMd5));
	}

	delete[]m_VirusInfo;
}


//��ղ�����
void MyVirusLibDlg::OnDeleteallvirus()
{
	DWORD nRet = MessageBox(TEXT("�Ƿ�Ҫ��ղ�����������Ϣ��"), TEXT("��ʾ"), MB_ICONASTERISK | MB_YESNO);
	if (nRet == IDNO)return;
	m_List_VirusLib.DeleteAllItems();
	UpDataVirusLib();
}

//ɾ���������в���
void MyVirusLibDlg::OnDeleteonece()
{
	CString nFileName;
	nFileName.Format(TEXT("�Ƿ񽫣�%s\n�Ƴ������⣿"), m_List_VirusLib.GetItemText(m_List_VirusLib.GetSelectionEx(), 0));
	DWORD nRet = MessageBox(nFileName, TEXT("��ʾ"), MB_ICONASTERISK | MB_YESNO);
	if (nRet == IDNO)return;

	m_List_VirusLib.DeleteItem(m_List_VirusLib.GetSelectionEx());

	UpDataVirusLib();
}

//���²�����
void MyVirusLibDlg::UpDataVirusLib()
{
	DWORD nCount = m_List_VirusLib.GetItemCount();
	PVIRUSINFO nVirusInfo = new VIRUSINFO[nCount];
	for (DWORD i = 0; i < nCount; i++)
	{
		strcpy_s(nVirusInfo[i].FileName, MAX_PATH, CStringA(m_List_VirusLib.GetItemText(i, 0)));
		strcpy_s(nVirusInfo[i].FileMd5, MAX_PATH, CStringA(m_List_VirusLib.GetItemText(i, 1)));
	}

	LoadVirusLib nLoadVirusLib;
	nLoadVirusLib.SetVirusLib(nVirusInfo, nCount);
}

//��Ӳ�����������
void MyVirusLibDlg::OnAddvirus()
{
	CAddVirus AddVirus;
	AddVirus.DoModal();
}


//�Ҽ������˵�
void MyVirusLibDlg::OnRclickListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	CMenu *nMenu = m_Menu.GetSubMenu(0);
	POINT pos;
	GetCursorPos(&pos);

	if (pNMItemActivate->iItem == -1)
	{
		nMenu->EnableMenuItem(ID_UPDATAVIRUS, TRUE);
		nMenu->EnableMenuItem(ID_DELETEONECE, TRUE);
	}
	else
	{
		nMenu->EnableMenuItem(ID_UPDATAVIRUS, FALSE);
		nMenu->EnableMenuItem(ID_DELETEONECE, FALSE);
	}
	nMenu->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, this);

}


