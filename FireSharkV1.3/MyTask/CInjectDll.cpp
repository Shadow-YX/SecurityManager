// CInjectDll.cpp: 实现文件
//
#include "stdafx.h"
#include "MyTask.h"
#include "CInjectDll.h"
#include "afxdialogex.h"

// CInjectDll 对话框
extern DWORD g_Pid; //获取线程ID

IMPLEMENT_DYNAMIC(CInjectDll, CDialogEx)

CInjectDll::CInjectDll(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_INJECTDLL, pParent)
{

}

CInjectDll::~CInjectDll()
{
}

void CInjectDll::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, EDT_PUT, m_edtPut);
}


BEGIN_MESSAGE_MAP(CInjectDll, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInjectDll::OnBnClickedOk)
	ON_BN_CLICKED(BTN_SELDLL, &CInjectDll::OnBnClickedSeldll)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CInjectDll 消息处理程序


void CInjectDll::OnBnClickedOk()
{
	//获取calc进程句柄
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, g_Pid);

	//获取LoadLibrary地址
	HMODULE hMoudleKernel = GetModuleHandle(TEXT("Kernel32.dll"));
	LPVOID pfnLoadLibrary = GetProcAddress(hMoudleKernel, "LoadLibraryA");

	//在注入的进程中为dll里申请一块内存
	CString csDllPath;
	m_edtPut.GetWindowText(csDllPath);

	const char* szDllPath =(const char*)csDllPath.GetBuffer();
	LPVOID pAddr = ::VirtualAllocEx(hProcess,
		NULL,
		strlen(szDllPath),
		MEM_COMMIT | MEM_RESERVE, //将此内存映射到物理内存
		PAGE_READWRITE);

	//向进程写入数据
	DWORD dwBytesWrited = 0;
	::WriteProcessMemory(hProcess, pAddr, szDllPath, strlen(szDllPath) + 1, &dwBytesWrited);

	HANDLE hTread = ::CreateRemoteThread(
		hProcess,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)pfnLoadLibrary,
		(LPVOID)pAddr,
		0,
		NULL
	);

	if (hTread == NULL)
	{
		AfxMessageBox(TEXT("Inject Failed"));
	}
	else
	{
		AfxMessageBox(TEXT("Inject Sucess"));
	}

	CloseHandle(hTread);
	CDialogEx::OnOK();
}


void CInjectDll::OnBnClickedSeldll()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("exe Files (*.dll)|*.dll|All File (*.*)|*.*||"), this);
	CString  str_path;
	if (dlg.DoModal() == IDOK)
	{
		//获取文件绝对路径
		str_path = dlg.GetPathName();
	}

	m_edtPut.SetWindowText(str_path);
}

void CInjectDll::OnDropFiles(HDROP hDropInfo)
{
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);//取得被拖动文件的数目
	// 循环依次取得拖动文件的File Name信息

	for (int i = 0; i < DropCount; i++)
	{
		// 定义一个缓冲区来存放读取的文件名信息
		TCHAR filePath[MAX_PATH];
		DragQueryFile(hDropInfo, i, filePath, MAX_PATH);//获得拖曳的第i个文件的文件名
		//此时 filePath已经获取到了文件路径
		//do something...
		m_edtPut.SetWindowText(filePath);
	}

	//拖放结束后,释放内存
	DragFinish(hDropInfo);


	CDialogEx::OnDropFiles(hDropInfo);
}