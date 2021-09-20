// MyClearUpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyClearUpDlg.h"
#include "afxdialogex.h"
#include <wininet.h>
#include <UrlHist.h>


// MyClearUpDlg �Ի���

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


// MyClearUpDlg ��Ϣ�������


BOOL MyClearUpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_List_File.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_File.InsertColumn(0, TEXT("�ļ�·��"), 0, 500);
	m_List_File.InsertColumn(1, TEXT("�ļ���С"), 0, 100);

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

		//�ж��Ƿ���ɨ���ļ�������� �������List
		if (IsScanFile)
		{
			m_ScaningPath = sFullPath;
			//���뵽List
			if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				//�����Ŀ¼����ݹ���ã��о��¼�Ŀ¼
				EmptyDirectory(sFullPath);
				continue;
			}
			//�����ļ�·��
			m_List_File.InsertItem(0, sFullPath);
			//�����ļ���С
			m_szTemp = GetShowSize(wfd.nFileSizeLow);
			m_List_File.SetItemText(0, 1, m_szTemp);
			//���������ļ��Ĵ�С ������ʾ�ж���
			m_AllFileSize += wfd.nFileSizeLow;
			continue;
		}

		//ȥ��ֻ������
		dwAttributes = GetFileAttributes(sFullPath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(sFullPath, dwAttributes);
		}
		//�ݹ��ļ����е��ļ�
		if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			//����ļ���С����0˵�����ļ��� �ݹ��ѯ
			EmptyDirectory(sFullPath);
			//ɾ�����ļ����е��ļ�����ɾ���ļ���
			RemoveDirectory(sFullPath);
		}
		else
		{
			m_ScaningPath = sFullPath;
			
			if (bWipeIndexDat && _tcsicmp(wfd.cFileName, TEXT("index.dat")) == 0)
			{
				WipeFile(szPath, wfd.cFileName);
			}
			//���������ļ��Ĵ�С ������ʾ�ж���
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

//�������������cookies
VOID MyClearUpDlg::CleanInternetCookieTemp()
{
	CString strPath;
	if (!IsScanFile)
	{
		DeleteUrlCache();
	}
	//SHGetSpecialFolderPath ��ȡϵͳָ��ϵͳ·��
	BOOL bSuccess = SHGetSpecialFolderPath(NULL,strPath.GetBuffer(MAX_PATH), CSIDL_INTERNET_CACHE, FALSE);
	strPath.ReleaseBuffer();
	if (bSuccess)
	{  //�õ���ʱĿ¼���������.
		EmptyDirectory(strPath);
	}

	bSuccess = SHGetSpecialFolderPath(NULL,strPath.GetBuffer(MAX_PATH), CSIDL_COOKIES, FALSE);
	strPath.ReleaseBuffer();
	if (bSuccess)
	{//�õ�Ŀ¼�������
		EmptyDirectory(strPath);
	}
}
BOOL MyClearUpDlg::DeleteUrlCache()
{
	BOOL bRet = FALSE;   //����ֵ
	HANDLE hEntry;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;
	DWORD dwEntrySize;

	//delete the files
	dwEntrySize = 0;
	//���INTERNET_CACHE_ENTRY_INFO�Ĵ�С
	hEntry = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize);
	//������Ӧ��С�Ļ�����
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new TCHAR[dwEntrySize];
	//��ý���ڵľ��
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

//���������¼
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

	// ���������벻����
	// ��������ģ������������ܹ���.
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

//���������¼
VOID MyClearUpDlg::CleanPasswordHistory()
{
	if (IsScanFile)
		return;
	SHDeleteKey(HKEY_CURRENT_USER,TEXT("Software\\Microsoft\\Internet Explorer\\IntelliForms"));
}
/*
//�����������ַ��¼
VOID MyClearUpDlg::CleanAddrBarHistory()
{
	if (IsScanFile)
		return;
	SHDeleteKey(HKEY_CURRENT_USER,TEXT("Software\\Microsoft\\Internet Explorer\\TypedURLs"));
}
*/

//����ϵͳ����
VOID MyClearUpDlg::CleanSystemTemp()
{
	CString strPath;
	BOOL bSuccess = GetTempPath(MAX_PATH, strPath.GetBuffer(MAX_PATH));

	strPath.ReleaseBuffer();
	if (bSuccess)//�õ�ϵͳ��ʱĿ¼
	{
		EmptyDirectory(strPath, TRUE);
	}
}

//�������վ
VOID MyClearUpDlg::CleanRubbishStation()
{
	if (IsScanFile)
		return;
	SHEmptyRecycleBin(NULL, NULL,SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}

//����һ���߳̽���ɨ��
DWORD WINAPI ScanClearUpThread(LPVOID lpThreadParameter)
{
	MyClearUpDlg *nDlg = (MyClearUpDlg*)lpThreadParameter;

	if (nDlg->m_Check_SystemTemp.GetCheck())nDlg->CleanSystemTemp();//����ϵͳ����
	if (nDlg->m_Check_Rubbish.GetCheck())nDlg->CleanRubbishStation();//�������վ


	if (nDlg->m_Check_InterCookies.GetCheck())nDlg->CleanInternetCookieTemp();//�������������cookies
	if (nDlg->m_Check_BrowseHistroy.GetCheck())nDlg->CleanBrowseAddrHistory();//���������¼
	if (nDlg->m_Check_PassHistory.GetCheck())nDlg->CleanPasswordHistory();//���������¼

	nDlg->m_Button_Scan.EnableWindow(TRUE);
	nDlg->m_Button_Clear.EnableWindow(TRUE);
	nDlg->SetTimer(nDlg->TIMER_SCANPATH, -1, NULL);
	nDlg->SetTimer(nDlg->TIMER_CLEARATH, -1, NULL);

	CString nTemp;
	if (nDlg->IsScanFile)nTemp.Format(TEXT("ɨ����� ����������%s"), nDlg->GetShowSize(nDlg->m_AllFileSize));
	else nTemp.Format(TEXT("������� ���ͷ�������%s"), nDlg->GetShowSize(nDlg->m_AllFileSize));

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
	SetTimer(TIMER_SCANPATH, 100, NULL);//���ö�ʱˢ��
	CreateThread(NULL, NULL, ScanClearUpThread, (LPVOID)this, NULL, NULL);

}

void MyClearUpDlg::OnBnClickedButton7()
{
	m_List_File.DeleteAllItems();
	
	IsScanFile = FALSE;
	m_AllFileSize = 0;
	m_Button_Scan.EnableWindow(FALSE);
	m_Button_Clear.EnableWindow(FALSE);
	SetTimer(TIMER_CLEARATH, 100, NULL);//���ö�ʱˢ��
	CreateThread(NULL, NULL, ScanClearUpThread, (LPVOID)this, NULL, NULL);

}

void MyClearUpDlg::OnBnClickedButton9()
{
	//��ȡ����ǰ���ڴ���Ϣ
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE preUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//��ʼ�����ڴ�
	ClearMemory();
	//�����������ڴ���Ϣ
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE afterUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//���������Ϣ
	CString m_szStatus;
	m_szStatus.Format(_T("�ڴ�������ϣ��������� : %.2f MB"), preUsedMem - afterUsedMem);

	MessageBox(m_szStatus, TEXT("��ʾ"), MB_ICONINFORMATION);
}

void MyClearUpDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	if (nIDEvent == TIMER_SCANPATH)
	{
		CString nTemp;
		nTemp.Format(TEXT("����������%s ��ǰ·����%s"), GetShowSize(m_AllFileSize), m_ScaningPath);
		m_Static_Path.SetWindowText(nTemp);
	}
	if (nIDEvent == TIMER_CLEARATH)
	{
		CString nTemp;
		nTemp.Format(TEXT("������%s ��ǰ·����%s"), GetShowSize(m_AllFileSize), m_ScaningPath);
		m_Static_Path.SetWindowText(nTemp);
	}
}






