// MyClearUpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyClearUpDlg.h"
#include "afxdialogex.h"
#include <wininet.h>
#include <UrlHist.h>


// MyClearUpDlg 对话框

IMPLEMENT_DYNAMIC(MyClearUpDlg, CDialogEx)

MyClearUpDlg::MyClearUpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLEARUP_DIALOG, pParent)
{
	Create(IDD_CLEARUP_DIALOG);
}

MyClearUpDlg::~MyClearUpDlg()
{
}

void MyClearUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_List_File);
	DDX_Control(pDX, IDC_BUTTON1, m_Button_Scan);
	DDX_Control(pDX, IDC_STATIC_PATH, m_Static_Path);
	DDX_Control(pDX, IDC_BUTTON7, m_Button_Clear);
	DDX_Control(pDX, IDC_CHECK1, m_Check_SystemTemp);
	DDX_Control(pDX, IDC_CHECK3, m_Check_Rubbish);
	//DDX_Control(pDX, IDC_CHECK4, m_Check_FindHistory);
	DDX_Control(pDX, IDC_CHECK6, m_Check_InterCookies);
	//DDX_Control(pDX, IDC_CHECK7, m_Check_AddrHistory);
	DDX_Control(pDX, IDC_CHECK8, m_Check_PassHistory);
	DDX_Control(pDX, IDC_CHECK5, m_Check_BrowseHistroy);
}


BEGIN_MESSAGE_MAP(MyClearUpDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &MyClearUpDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON7, &MyClearUpDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &MyClearUpDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// MyClearUpDlg 消息处理程序


BOOL MyClearUpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_List_File.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_File.InsertColumn(0, TEXT("文件路径"), 0, 500);
	m_List_File.InsertColumn(1, TEXT("文件大小"), 0, 100);

	IsScanFile = TRUE;

	m_Check_SystemTemp.SetCheck(TRUE);
	m_Check_Rubbish.SetCheck(TRUE);
	//m_Check_FindHistory.SetCheck(TRUE);

	m_Check_InterCookies.SetCheck(TRUE);
	//m_Check_AddrHistory.SetCheck(TRUE);
	m_Check_BrowseHistroy.SetCheck(TRUE);
	m_Check_PassHistory.SetCheck(TRUE);

	return TRUE;
}

CString MyClearUpDlg::GetShowSize(DOUBLE dwSize)
{
	CString szSize;
	if (dwSize < 1024)
	{
		//B
		szSize.Format(_T("%d B"), (DWORD)dwSize);
		return szSize;
	}
	else if (dwSize < 1024 * 1024)
	{
		//KB
		szSize.Format(_T("%.2f KB"), ((DOUBLE)dwSize) / 1024);
		return szSize;
	}
	else if (dwSize < 1024 * 1024 * 1024)
	{
		//MB
		szSize.Format(_T("%.2f MB"), (DOUBLE)dwSize / 1024 / 1024);
		return szSize;
	}if (dwSize > 1024 * 1024 * 1024)
	{
		//GB
		szSize.Format(_T("%.2f GB"), (DOUBLE)dwSize / 1024 / 1024 / 1024);
		return szSize;
	}
	return _T("");
}
BOOL MyClearUpDlg::EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni, BOOL bWipeIndexDat)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes = 0;

	sFindFilter = szPath;
	sFindFilter += TEXT("\\*.*");
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
		return FALSE;

	do
	{
		if (_tcscmp(wfd.cFileName, TEXT(".")) == 0 ||
			_tcscmp(wfd.cFileName, TEXT("..")) == 0 ||
			(bDeleteDesktopIni == FALSE && _tcsicmp(wfd.cFileName, TEXT("desktop.ini")) == 0))
		{
			continue;
		}

		sFullPath = szPath;
		sFullPath += TEXT("\\");
		sFullPath += wfd.cFileName;

		//判断是否是扫描文件，如果是 就输出到List
		if (IsScanFile)
		{
			m_ScaningPath = sFullPath;
			//插入到List
			if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				//如果是目录，则递归调用，列举下级目录
				EmptyDirectory(sFullPath);
				continue;
			}
			//插入文件路径
			m_List_File.InsertItem(0, sFullPath);
			//插入文件大小
			m_szTemp = GetShowSize(wfd.nFileSizeLow);
			m_List_File.SetItemText(0, 1, m_szTemp);
			//保存所有文件的大小 用于显示有多少
			m_AllFileSize += wfd.nFileSizeLow;
			continue;
		}

		//去掉只读属性
		dwAttributes = GetFileAttributes(sFullPath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(sFullPath, dwAttributes);
		}
		//递归文件夹中的文件
		if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			//如果文件大小等于0说明是文件夹 递归查询
			EmptyDirectory(sFullPath);
			//删除完文件夹中的文件，再删除文件夹
			RemoveDirectory(sFullPath);
		}
		else
		{
			m_ScaningPath = sFullPath;
			
			if (bWipeIndexDat && _tcsicmp(wfd.cFileName, TEXT("index.dat")) == 0)
			{
				WipeFile(szPath, wfd.cFileName);
			}
			//保存所有文件的大小 用于显示有多少
			m_AllFileSize += wfd.nFileSizeLow;
			if (!DeleteFile(sFullPath))
			{
				m_AllFileSize -= wfd.nFileSizeLow;
			}
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind);

	return TRUE;
}
BOOL MyClearUpDlg::WipeFile(LPCTSTR szDir, LPCTSTR szFile)
{
	CString sPath;
	HANDLE	hFile;
	DWORD	dwSize;
	DWORD	dwWrite;
	TCHAR	sZero[10000];
	memset(sZero, 0, 10000);

	sPath = szDir;
	sPath += TEXT("\\");
	sPath += szFile;

	hFile = CreateFile(sPath, GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	dwSize = GetFileSize(hFile, NULL);

	//skip file header (actually, I don't know the file format of index.dat)
	dwSize -= 64;
	SetFilePointer(hFile, 64, NULL, FILE_BEGIN);

	while (dwSize > 0)
	{
		if (dwSize > 10000)
		{
			WriteFile(hFile, sZero, 10000, &dwWrite, NULL);
			dwSize -= 10000;
		}
		else
		{
			WriteFile(hFile, sZero, dwSize, &dwWrite, NULL);
			break;
		}
	}

	CloseHandle(hFile);
	return TRUE;
}

//清理浏览器缓存cookies
VOID MyClearUpDlg::CleanInternetCookieTemp()
{
	CString strPath;
	if (!IsScanFile)
	{
		DeleteUrlCache();
	}
	//SHGetSpecialFolderPath 获取系统指定系统路径
	BOOL bSuccess = SHGetSpecialFolderPath(NULL,strPath.GetBuffer(MAX_PATH), CSIDL_INTERNET_CACHE, FALSE);
	strPath.ReleaseBuffer();
	if (bSuccess)
	{  //得到临时目录，并清空它.
		EmptyDirectory(strPath);
	}

	bSuccess = SHGetSpecialFolderPath(NULL,strPath.GetBuffer(MAX_PATH), CSIDL_COOKIES, FALSE);
	strPath.ReleaseBuffer();
	if (bSuccess)
	{//得到目录，并清空
		EmptyDirectory(strPath);
	}
}
BOOL MyClearUpDlg::DeleteUrlCache()
{
	BOOL bRet = FALSE;   //返回值
	HANDLE hEntry;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;
	DWORD dwEntrySize;

	//delete the files
	dwEntrySize = 0;
	//获得INTERNET_CACHE_ENTRY_INFO的大小
	hEntry = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize);
	//分配响应大小的缓冲区
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new TCHAR[dwEntrySize];
	//获得进入口的句柄
	hEntry = FindFirstUrlCacheEntry(NULL, lpCacheEntry, &dwEntrySize);

	if (hEntry)
	{
		do
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);

			dwEntrySize = 0;
			FindNextUrlCacheEntry(hEntry, NULL, &dwEntrySize);
			delete[] lpCacheEntry;
			lpCacheEntry = NULL;
			lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new TCHAR[dwEntrySize];
		} while (FindNextUrlCacheEntry(hEntry, lpCacheEntry, &dwEntrySize));

		bRet = TRUE;
	}

	if (lpCacheEntry)
	{
		delete[] lpCacheEntry;
		lpCacheEntry = NULL;
	}
	return bRet;
}

//清理浏览记录
VOID MyClearUpDlg::CleanBrowseAddrHistory()
{
	if (IsScanFile)
		return;
	HRESULT hr;
	CString strPath;
	IUrlHistoryStg2* pUrlHistoryStg2 = NULL;
	hr = CoCreateInstance(CLSID_CUrlHistory, NULL,
		CLSCTX_INPROC_SERVER, IID_IUrlHistoryStg2,
		(VOID**)&pUrlHistoryStg2);
	if (SUCCEEDED(hr))
	{
		hr = pUrlHistoryStg2->ClearHistory();
		pUrlHistoryStg2->Release();
	}

	delete pUrlHistoryStg2;

	// 如果上面代码不能清
	// 则有下面的，不完美，但能工作.
	GetWindowsDirectory(strPath.GetBuffer(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	strPath += TEXT("\\History");
	EmptyDirectory(strPath, FALSE, TRUE);

	BOOL bSuccess = SHGetSpecialFolderPath(NULL, strPath.GetBuffer(MAX_PATH), CSIDL_HISTORY, FALSE);
	strPath.ReleaseBuffer();
	if (bSuccess)
	{
		EmptyDirectory(strPath, FALSE, TRUE);
	}
}

//清理密码记录
VOID MyClearUpDlg::CleanPasswordHistory()
{
	if (IsScanFile)
		return;
	SHDeleteKey(HKEY_CURRENT_USER,TEXT("Software\\Microsoft\\Internet Explorer\\IntelliForms"));
}
/*
//清理浏览器地址记录
VOID MyClearUpDlg::CleanAddrBarHistory()
{
	if (IsScanFile)
		return;
	SHDeleteKey(HKEY_CURRENT_USER,TEXT("Software\\Microsoft\\Internet Explorer\\TypedURLs"));
}
*/

//清理系统缓存
VOID MyClearUpDlg::CleanSystemTemp()
{
	CString strPath;
	BOOL bSuccess = GetTempPath(MAX_PATH, strPath.GetBuffer(MAX_PATH));

	strPath.ReleaseBuffer();
	if (bSuccess)//得到系统临时目录
	{
		EmptyDirectory(strPath, TRUE);
	}
}

//清理回收站
VOID MyClearUpDlg::CleanRubbishStation()
{
	if (IsScanFile)
		return;
	SHEmptyRecycleBin(NULL, NULL,SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}

//创建一个线程进行扫描
DWORD WINAPI ScanClearUpThread(LPVOID lpThreadParameter)
{
	MyClearUpDlg *nDlg = (MyClearUpDlg*)lpThreadParameter;

	if (nDlg->m_Check_SystemTemp.GetCheck())nDlg->CleanSystemTemp();//清理系统缓存
	if (nDlg->m_Check_Rubbish.GetCheck())nDlg->CleanRubbishStation();//清理回收站


	if (nDlg->m_Check_InterCookies.GetCheck())nDlg->CleanInternetCookieTemp();//清理浏览器缓存cookies
	if (nDlg->m_Check_BrowseHistroy.GetCheck())nDlg->CleanBrowseAddrHistory();//清理浏览记录
	if (nDlg->m_Check_PassHistory.GetCheck())nDlg->CleanPasswordHistory();//清理密码记录

	nDlg->m_Button_Scan.EnableWindow(TRUE);
	nDlg->m_Button_Clear.EnableWindow(TRUE);
	nDlg->SetTimer(nDlg->TIMER_SCANPATH, -1, NULL);
	nDlg->SetTimer(nDlg->TIMER_CLEARATH, -1, NULL);

	CString nTemp;
	if (nDlg->IsScanFile)nTemp.Format(TEXT("扫描完成 垃圾容量：%s"), nDlg->GetShowSize(nDlg->m_AllFileSize));
	else nTemp.Format(TEXT("清理完成 共释放容量：%s"), nDlg->GetShowSize(nDlg->m_AllFileSize));

	nDlg->m_Static_Path.SetWindowText(nTemp);

	return 0;
}

void MyClearUpDlg::OnBnClickedButton1()
{

	m_List_File.DeleteAllItems();

	IsScanFile = TRUE;
	m_AllFileSize = 0;
	m_Button_Scan.EnableWindow(FALSE);
	m_Button_Clear.EnableWindow(FALSE);
	SetTimer(TIMER_SCANPATH, 100, NULL);//设置定时刷新
	CreateThread(NULL, NULL, ScanClearUpThread, (LPVOID)this, NULL, NULL);

}

void MyClearUpDlg::OnBnClickedButton7()
{
	m_List_File.DeleteAllItems();
	
	IsScanFile = FALSE;
	m_AllFileSize = 0;
	m_Button_Scan.EnableWindow(FALSE);
	m_Button_Clear.EnableWindow(FALSE);
	SetTimer(TIMER_CLEARATH, 100, NULL);//设置定时刷新
	CreateThread(NULL, NULL, ScanClearUpThread, (LPVOID)this, NULL, NULL);

}

void MyClearUpDlg::OnBnClickedButton9()
{
	//获取清理前的内存信息
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE preUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//开始清理内存
	ClearMemory();
	//计算清理后的内存信息
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE afterUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//输出清理信息
	CString m_szStatus;
	m_szStatus.Format(_T("内存清理完毕！本次清理 : %.2f MB"), preUsedMem - afterUsedMem);

	MessageBox(m_szStatus, TEXT("提示"), MB_ICONINFORMATION);
}

void MyClearUpDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	if (nIDEvent == TIMER_SCANPATH)
	{
		CString nTemp;
		nTemp.Format(TEXT("垃圾容量：%s 当前路径：%s"), GetShowSize(m_AllFileSize), m_ScaningPath);
		m_Static_Path.SetWindowText(nTemp);
	}
	if (nIDEvent == TIMER_CLEARATH)
	{
		CString nTemp;
		nTemp.Format(TEXT("已清理：%s 当前路径：%s"), GetShowSize(m_AllFileSize), m_ScaningPath);
		m_Static_Path.SetWindowText(nTemp);
	}
}






