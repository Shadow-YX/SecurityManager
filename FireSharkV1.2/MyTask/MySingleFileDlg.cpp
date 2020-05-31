// MySingleFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "MySingleFileDlg.h"
#include "afxdialogex.h"
#include "MD5_FILE.h"


// MySingleFileDlg 对话框

IMPLEMENT_DYNAMIC(MySingleFileDlg, CDialogEx)

MySingleFileDlg::MySingleFileDlg(CWnd* pParent,CString nPath)
	: CDialogEx(IDD_SINGLEFILE_DIALOG, pParent),
	m_Path(nPath)
{
	Create(IDD_SINGLEFILE_DIALOG);
}

MySingleFileDlg::~MySingleFileDlg()
{
}

void MySingleFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_FileName);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_Size);
	DDX_Control(pDX, IDC_EDIT3, m_Edit_Create);
	DDX_Control(pDX, IDC_EDIT4, m_Edit_Change);
	DDX_Control(pDX, IDC_EDIT5, m_Edit_Access);
	DDX_Control(pDX, IDC_EDIT34, m_Edit_MD5);
}


BEGIN_MESSAGE_MAP(MySingleFileDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// MySingleFileDlg 消息处理程序
void MySingleFileDlg::OnClose()
{
	CDialogEx::OnClose();
	DestroyWindow();
}


BOOL MySingleFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CStringA nFileMd5;
	WIN32_FIND_DATAW nFileInfo{};
	SYSTEMTIME nSystemTime{};
	nFileMd5 = GetFileMd5(m_Path);

	FindFirstFile(m_Path, &nFileInfo);

	CString nSize;
	nSize.Format(TEXT("%dKB"), nFileInfo.nFileSizeLow / 1024);

	FileTimeToLocalFileTime(&nFileInfo.ftCreationTime, &nFileInfo.ftCreationTime);
	FileTimeToSystemTime(&nFileInfo.ftCreationTime, &nSystemTime);
	CString nCreate;
	nCreate.Format(TEXT("%d年%d月%d日 %d时%d分%d秒"), nSystemTime.wYear, nSystemTime.wMonth, nSystemTime.wDay, nSystemTime.wHour, nSystemTime.wMinute, nSystemTime.wSecond);

	FileTimeToLocalFileTime(&nFileInfo.ftLastWriteTime, &nFileInfo.ftLastWriteTime);
	FileTimeToSystemTime(&nFileInfo.ftLastWriteTime, &nSystemTime);
	CString nChange;
	nChange.Format(TEXT("%d年%d月%d日 %d时%d分%d秒"), nSystemTime.wYear, nSystemTime.wMonth, nSystemTime.wDay, nSystemTime.wHour, nSystemTime.wMinute, nSystemTime.wSecond);

	FileTimeToLocalFileTime(&nFileInfo.ftLastAccessTime, &nFileInfo.ftLastAccessTime);
	FileTimeToSystemTime(&nFileInfo.ftLastAccessTime, &nSystemTime);
	CString nAccess;
	nAccess.Format(TEXT("%d年%d月%d日 %d时%d分%d秒"), nSystemTime.wYear, nSystemTime.wMonth, nSystemTime.wDay, nSystemTime.wHour, nSystemTime.wMinute, nSystemTime.wSecond);

	m_Edit_FileName.SetWindowText(nFileInfo.cFileName);
	m_Edit_Size.SetWindowText(nSize);
	m_Edit_Create.SetWindowText(nCreate);
	m_Edit_Change.SetWindowText(nChange);
	m_Edit_Access.SetWindowText(nAccess);
	m_Edit_MD5.SetWindowText(CString(nFileMd5));


	return TRUE;
}


