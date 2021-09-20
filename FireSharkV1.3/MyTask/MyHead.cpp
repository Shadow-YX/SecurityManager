#include "stdafx.h"
#include <windows.h>
#include <TlHelp32.h>
#include "MyHead.h"
#include <Psapi.h>
#include <ws2tcpip.h>


void OutputDebugStringEx(const TCHAR* format, ...) 
{
	TCHAR buff[500];
	va_list pArgs = nullptr;
	va_start(pArgs, format);
	_vstprintf_s(buff, _countof(buff), format, pArgs);
	CString nTemp =(buff);
	nTemp += TEXT("\n");
	OutputDebugString(nTemp);
}
void OutputDebugEdit(CEdit &m_Edit_Info, const TCHAR* format, ...)
{
	TCHAR buff[500];
	va_list pArgs = nullptr;
	va_start(pArgs, format);
	_vstprintf_s(buff, _countof(buff), format, pArgs);

	CString nTemp = (buff);
	nTemp += TEXT("\n");
	m_Edit_Info.SetSel(-1);
	m_Edit_Info.ReplaceSel(nTemp);
}

CString GetPathEx(CString nPath)
{

	nPath = nPath.MakeLower();
	UINT nIndex = nPath.Find(TEXT(".exe"));
	nPath = nPath.Left(nIndex + 4);

	nIndex = nPath.Find('\"');
	if (nIndex == 0)nPath = nPath.Right(nPath.GetLength() - 1);

	return nPath;
}

void MyRtlAdjustPrivilege()
{
	const int SE_SHUTDOWN_PRIVILEGE = 20;
	int nAddress = 0;
	typedef int(WINAPI*RtlAdjustPrivilege)(int, bool, bool, int*);
	HMODULE module = LoadLibrary(TEXT("ntdll.dll"));
	RtlAdjustPrivilege pRt = (RtlAdjustPrivilege)GetProcAddress(module, "RtlAdjustPrivilege");
	pRt(SE_SHUTDOWN_PRIVILEGE, TRUE, FALSE, &nAddress);
	FreeLibrary(module);
}

DWORD EnumTaskList(TASKLIST &nTaskList)
{
	BOOL nRet;
	DWORD nIndex = 0;
	PROCESSENTRY32 nPT;
	nPT.dwSize = sizeof(PROCESSENTRY32);
	HANDLE nSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	nRet = Process32First(nSnapShot, &nPT);
	while (nRet)
	{
		nRet = Process32Next(nSnapShot, &nPT);
		nIndex++;
	}

	nTaskList.szExeFile = new CString[nIndex];
	nTaskList.th32ProcessID = new DWORD[nIndex];
	nTaskList.cntThreads = new DWORD[nIndex];
	nTaskList.th32ParentProcessID = new DWORD[nIndex];
	nTaskList.szExeFilePath = new CString[nIndex];
	nTaskList.WorkingSetSize = new CString[nIndex];
	nTaskList.szPriority = new CString[nIndex];

	nIndex = 0;
	nRet = Process32First(nSnapShot, &nPT);
	while (nRet)
	{
		HANDLE nHandle;
		CString nFilePath;
		CString nMenory;
		CString nPriority;
		PROCESS_MEMORY_COUNTERS nMen;
		nHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, nPT.th32ProcessID);
		GetProcessFilePath(nHandle, nFilePath);
		GetProcessMemoryInfo(nHandle,&nMen,sizeof(PROCESS_MEMORY_COUNTERS));
		nMenory.Format(TEXT("%d K"), nMen.WorkingSetSize / 1024);
		GetProcessPriority(nPriority, nHandle);
		CloseHandle(nHandle);

		nTaskList.szExeFile[nIndex] = nPT.szExeFile;
		nTaskList.th32ProcessID[nIndex] = nPT.th32ProcessID;
		nTaskList.cntThreads[nIndex] = nPT.cntThreads;
		nTaskList.th32ParentProcessID[nIndex] = nPT.th32ParentProcessID;
		nTaskList.szExeFilePath[nIndex] = nFilePath;
		nTaskList.WorkingSetSize[nIndex] = nMenory;
		nTaskList.szPriority[nIndex] = nPriority;

		nRet = Process32Next(nSnapShot, &nPT);
		nIndex++;
	}

	CloseHandle(nSnapShot);

	return nIndex;
}


DWORD EnumModuleList(MODULELIST &nModuleList, DWORD nPid)
{
	BOOL nRet;
	DWORD nIndex = 0;
	MODULEENTRY32 nModule;

	nModule.dwSize = sizeof(MODULEENTRY32);
	HANDLE nSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nPid);

	nRet = Module32First(nSnapShot, &nModule);
	while (nRet)
	{
		nRet = Module32Next(nSnapShot, &nModule);
		nIndex++;
	}

	nModuleList.szModule = new CString[nIndex];
	nModuleList.szExePath = new CString[nIndex];
	nModuleList.modBaseAddr = new DWORD[nIndex];
	nModuleList.dwSize = new DWORD[nIndex];

	nIndex = 0;
	nRet = Module32First(nSnapShot, &nModule);
	while (nRet)
	{
		nModuleList.szModule[nIndex] = nModule.szModule;
		nModuleList.szExePath[nIndex] = nModule.szExePath;
		nModuleList.modBaseAddr[nIndex] = (DWORD)nModule.modBaseAddr;
		nModuleList.dwSize[nIndex] = nModule.dwSize;

		nRet = Module32Next(nSnapShot, &nModule);
		nIndex++;
	}

	CloseHandle(nSnapShot);

	return nIndex;
}
DWORD EnumThreadList(THREADLIST &nThreadList, DWORD nPid)
{
	BOOL nRet;
	DWORD nIndex = 0;
	THREADENTRY32 nThread;

	nThread.dwSize = sizeof(THREADENTRY32);
	HANDLE nSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, nPid);

	nRet = Thread32First(nSnapShot, &nThread);
	while (nRet)
	{
		if ((nThread.th32OwnerProcessID == nPid || nPid==0xABCDEF)&&nThread.th32OwnerProcessID!=0)nIndex++;
		nRet = Thread32Next(nSnapShot, &nThread);
		
	}

	nThreadList.th32ThreadID = new DWORD[nIndex];
	nThreadList.th32OwnerProcessID = new DWORD[nIndex];
	nThreadList.tpBasePri = new DWORD[nIndex];
	nThreadList.szExePath = new CString[nIndex];
	nThreadList.szThreadState = new CString[nIndex];

	nIndex = 0;
	nRet = Thread32First(nSnapShot, &nThread);
	while (nRet)
	{
		if ((nThread.th32OwnerProcessID == nPid || nPid == 0xABCDEF)&&nThread.th32OwnerProcessID != 0) 
		{
			nThreadList.th32ThreadID[nIndex] = nThread.th32ThreadID;
			nThreadList.th32OwnerProcessID[nIndex] = nThread.th32OwnerProcessID;
			nThreadList.tpBasePri[nIndex] = nThread.tpBasePri;

			CString nProcessName;
			GetProcessName(nProcessName, nThread.th32OwnerProcessID);
			nThreadList.szExePath[nIndex] = nProcessName;

			CString nThreadState;
			GetThreadState(nThreadState, nThread.th32ThreadID);
			nThreadList.szThreadState[nIndex] = nThreadState;

			nIndex++;
		}
		nRet = Thread32Next(nSnapShot, &nThread);
	}

	CloseHandle(nSnapShot);

	return nIndex;
}
DWORD EnumHeapList(HEAPLIST &nHeapList)
{
	BOOL nRet;
	DWORD nIndex = 0;
	HEAPLIST32 nHeap{};

	PROCESSENTRY32 nPT;
	nPT.dwSize = sizeof(PROCESSENTRY32);
	HANDLE nSnapShotTask = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	nRet = Process32First(nSnapShotTask, &nPT);
	while (nRet)
	{
		nHeap.dwSize = sizeof(HEAPLIST32);
		HANDLE nSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, nPT.th32ProcessID);
		nRet = Heap32ListFirst(nSnapShot, &nHeap);
		while (nRet)
		{
			nIndex++;
			nRet = Heap32ListNext(nSnapShot, &nHeap);
		}
		CloseHandle(nSnapShot);
		nRet = Process32Next(nSnapShotTask, &nPT);
	}

	nHeapList.th32HeapID = new DWORD[nIndex];
	nHeapList.th32ProcessID = new DWORD[nIndex];
	nHeapList.szExeFile = new CString[nIndex];


	nPT = { sizeof(PROCESSENTRY32) };
	nHeap = { sizeof(HEAPLIST32) };
	nIndex = 0;

	nRet = Process32First(nSnapShotTask, &nPT);
	while (nRet)
	{
		
		HANDLE nSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, nPT.th32ProcessID);
		nRet = Heap32ListFirst(nSnapShot, &nHeap);
		while (nRet)
		{
			nHeapList.th32HeapID[nIndex] = (DWORD)nHeap.th32HeapID;
			nHeapList.th32ProcessID[nIndex] = nPT.th32ProcessID;
			CString nProcessName;
			GetProcessName(nProcessName, nPT.th32ProcessID);
			nHeapList.szExeFile[nIndex] = nProcessName;

			nIndex++;
			nRet = Heap32ListNext(nSnapShot, &nHeap);
		}
		CloseHandle(nSnapShot);
		nRet = Process32Next(nSnapShotTask, &nPT);
	}

	CloseHandle(nSnapShotTask);

	return nIndex;
}
DWORD EnumHeapEntyr(HEAPENTYR &HeapEntry,DWORD nHeapId,DWORD nPid)
{

	BOOL nRet;
	DWORD nIndex = 0;
	HEAPENTRY32 nHeapEntry{};
	nHeapEntry.dwSize = sizeof(HEAPENTRY32);

	HANDLE nSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	nRet = Heap32First( &nHeapEntry, nPid, nHeapId);
	while (nRet)
	{
	nIndex++;
	nRet = Heap32Next (&nHeapEntry);
	}


	HeapEntry.dwAddress = new DWORD[nIndex];
	HeapEntry.dwBlockSize = new DWORD[nIndex];


	nIndex = 0;
	nRet = Heap32First(&nHeapEntry, nPid, nHeapId);
	while (nRet)
	{
		HeapEntry.dwAddress[nIndex] = (DWORD)nHeapEntry.dwAddress;
		HeapEntry.dwBlockSize[nIndex] = (DWORD)nHeapEntry.dwBlockSize;
		nIndex++;
		nRet = Heap32Next(&nHeapEntry);
	}

	return nIndex;
}



void ClearTaskList(TASKLIST &nTaskList)
{
	delete[]nTaskList.szExeFile;
	delete[]nTaskList.th32ProcessID;
	delete[]nTaskList.cntThreads;
	delete[]nTaskList.th32ParentProcessID;
	delete[]nTaskList.szExeFilePath;
	delete[]nTaskList.WorkingSetSize;
	delete[]nTaskList.szPriority;

}
void ClearModuleList(MODULELIST &nModuleList)
{
	delete[]nModuleList.szModule;
	delete[]nModuleList.szExePath;
	delete[]nModuleList.modBaseAddr;
	delete[]nModuleList.dwSize;

}
void ClearThreadList(THREADLIST &nThreadList)
{
	delete[]nThreadList.th32ThreadID;
	delete[]nThreadList.th32OwnerProcessID;
	delete[]nThreadList.tpBasePri;
	delete[]nThreadList.szExePath;
	delete[]nThreadList.szThreadState;
}
void ClearHeapList(HEAPLIST &nHeapList)
{
	delete[]nHeapList.th32HeapID;
	delete[]nHeapList.th32ProcessID;
	delete[]nHeapList.szExeFile;
}
void ClearHeapEntyr(HEAPENTYR &nHeapEntyr)
{
	delete[]nHeapEntyr.dwAddress;
	delete[]nHeapEntyr.dwBlockSize;

}


void EndProcess(DWORD nPid)
{
	HANDLE nHandle = OpenProcess(PROCESS_TERMINATE, FALSE, nPid);

	TerminateProcess(nHandle, 0);

	CloseHandle(nHandle);
}
void EndThread(DWORD nThreadid)
{
	HANDLE nHandle = OpenThread(PROCESS_TERMINATE, FALSE, nThreadid);

	TerminateThread(nHandle, 0);

	CloseHandle(nHandle);
}


void GetProcessFilePath(HANDLE hProcess, CString& sFilePath)
{
	sFilePath = _T("");
	TCHAR tsFileDosPath[MAX_PATH + 1];
	ZeroMemory(tsFileDosPath, sizeof(TCHAR)*(MAX_PATH + 1));
	if (0 == GetProcessImageFileName(hProcess, tsFileDosPath, MAX_PATH + 1))
	{
		return;
	}

	// 获取Logic Drive String长度
	
	SIZE_T uiLen = GetLogicalDriveStrings(0, NULL);
	if (0 == uiLen)
	{
		return;
	}

	PTSTR pLogicDriveString = new TCHAR[uiLen + 1];
	ZeroMemory(pLogicDriveString, uiLen + 1);
	uiLen = GetLogicalDriveStrings((DWORD)uiLen, pLogicDriveString);
	if (0 == uiLen)
	{
		delete[]pLogicDriveString;
		return;
	}

	TCHAR szDrive[3] = TEXT(" :");
	PTSTR pDosDriveName = new TCHAR[MAX_PATH];
	PTSTR pLogicIndex = pLogicDriveString;

	do
	{
		szDrive[0] = *pLogicIndex;
		uiLen = QueryDosDevice(szDrive, pDosDriveName, MAX_PATH);
		if (0 == uiLen)
		{
			if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
			{
				break;
			}

			delete[]pDosDriveName;
			pDosDriveName = new TCHAR[uiLen + 1];
			uiLen = QueryDosDevice(szDrive, pDosDriveName, (DWORD)uiLen + 1);
			if (0 == uiLen)
			{
				break;
			}
		}

		uiLen = _tcslen(pDosDriveName);
		if (0 == _tcsnicmp(tsFileDosPath, pDosDriveName, uiLen))
		{
			sFilePath.Format(_T("%s%s"), szDrive, tsFileDosPath + uiLen);
			break;
		}

		while (*pLogicIndex++);
	} while (*pLogicIndex);

	delete[]pLogicDriveString;
	delete[]pDosDriveName;
}

void SetProcessState(DWORD nPid, BOOL nSuspend)
{
	HANDLE nHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nPid);

	if (nSuspend)
	{
		typedef int(WINAPI*ZwSuspendProcess)(HANDLE);
		HMODULE module = LoadLibrary(TEXT("ntdll.dll"));
		ZwSuspendProcess ZwSuspend = (ZwSuspendProcess)GetProcAddress(module, "ZwSuspendProcess");
		ZwSuspend(nHandle);
		FreeLibrary(module);
	}
	else
	{
		typedef int(WINAPI*ZwResumeProcess)(HANDLE);
		HMODULE module = LoadLibrary(TEXT("ntdll.dll"));
		ZwResumeProcess ZwResume = (ZwResumeProcess)GetProcAddress(module, "ZwResumeProcess");
		ZwResume(nHandle);
		FreeLibrary(module);
	}
	CloseHandle(nHandle);

}
void SetThreadState(DWORD nThreadid, BOOL nSuspend)
{
	HANDLE nHandle = OpenThread(PROCESS_ALL_ACCESS, FALSE, nThreadid);

	if (nSuspend)
	{
		SuspendThread(nHandle);
	}
	else
	{
		ResumeThread(nHandle);
	}
	CloseHandle(nHandle);
}

//内存
DWORD GetMemState()
{
	MEMORYSTATUSEX nStatex;

	nStatex.dwLength = sizeof(nStatex);

	GlobalMemoryStatusEx(&nStatex);

	DOUBLE nAll = (DOUBLE)nStatex.ullTotalPhys / 1048576;
	DOUBLE nHave = (DOUBLE)nStatex.ullAvailPhys / 1048576;
	return (DWORD)((nAll - nHave) / nAll * 100);
}


void OpenFile(CString &nFileNamePath)
{
	LPITEMIDLIST nItem;

	nItem = ILCreateFromPath(nFileNamePath);

	SHOpenFolderAndSelectItems(nItem, 0, 0, 0);

	ILFree(nItem);

}


//cpu
void GetCpuState(CString& nStr)
{
	static LONGLONG nFree{}, nNt{}, nUser{};
	static LONGLONG nFreet{}, nNtt{}, nUsert{};
	static DOUBLE nHave{};
	GetSystemTimes((PFILETIME)&nFree, (PFILETIME)&nNt, (PFILETIME)&nUser);
	nHave = (DOUBLE)(nFree - nFreet) / (DOUBLE)(nNt - nNtt + nUser - nUsert);
	nStr.Format(L"%d", DWORD(100.0 - nHave * 100));
	nFreet = nFree;
	nNtt = nNt;
	nUsert = nUser;
}

//获取注册表键值
bool GetKey(int vk_code)
{
	return (GetAsyncKeyState(vk_code) & 0x8000) ? true : false;
}

//获取进程名
void GetProcessName(CString &nProcessName,DWORD nPid)
{
	PROCESSENTRY32 nPT;
	nPT.dwSize = sizeof(nPT);
	HANDLE nSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	BOOL nRet = Process32First(nSnapShot, &nPT);
	while (nRet)
	{
		if (nPid==nPT.th32ProcessID)
		{
			nProcessName = nPT.szExeFile;
			return;
		}
		nRet = Process32Next(nSnapShot, &nPT);
	}
}

//获取进程Id
DWORD GetProcessPid(CString nProcessName)
{
	PROCESSENTRY32 nPT;
	nPT.dwSize = sizeof(nPT);
	HANDLE nSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	BOOL nRet = Process32First(nSnapShot, &nPT);
	while (nRet)
	{
		if (nProcessName == CString(nPT.szExeFile))
		{
			return nPT.th32ProcessID;
		}
		nRet = Process32Next(nSnapShot, &nPT);
	}
	return 0;
}

void GetThreadState(CString &nState, DWORD nThreadid)
{
	HANDLE nHandle = OpenThread(PROCESS_ALL_ACCESS, FALSE, nThreadid);

	DWORD WaitCode;
	WaitCode = WaitForSingleObject(nHandle, 0);

	CloseHandle(nHandle);

	switch (WaitCode)
	{
	case 258:
		nState = "正在运行";
		return;
	case -1:
		nState = "已销毁";
		return;
	}
	nState = "已结束";


}

void GetProcessPriority(CString &nState, HANDLE nHandle)
{
	if ((DWORD)nHandle == -1)
	{
		nState = "无权限";
		return;
	}

	DWORD nClass;
	nClass = GetPriorityClass(nHandle);

	switch (nClass)
	{
	case 256:
		nState = "实时";
		return;
	case 128:
		nState = "最高";
		return;
	case 32768:
		nState = "高于标准";
		return;
	case 32:
		nState = "标准";
		return;
	case 16384:
		nState = "低于标准";
		return;
	case 64:
		nState = "低";
		return;
	case 0:
		nState = "最高";
		return;
	}
	nState = "无权限";
}

//显示文件信息
void OpenFileInfo(HWND nHwnd,CString &nFileRoute)
{
	SHELLEXECUTEINFO   sei;
	sei.hwnd = nHwnd;
	sei.lpVerb = TEXT("properties");
	sei.lpFile = nFileRoute;
	sei.lpDirectory = NULL;
	sei.lpParameters = NULL;
	sei.nShow = SW_SHOWNORMAL;
	sei.fMask = SEE_MASK_INVOKEIDLIST;
	sei.lpIDList = NULL;
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	ShellExecuteEx(&sei);

}

//获得文件目录
CString GetProgramDir()
{
	TCHAR exeFullPath[MAX_PATH]; // Full path
	TCHAR *nWeak;

	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	nWeak = _tcsrchr(exeFullPath, '\\');

	nWeak[1] = 0;
	return CString(exeFullPath);
	system("pause");

}


void ClearMemory()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}

	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize = sizeof(ProcessInfo);
	int count = 0;
	//获取系统中第一个进程的信息  
	BOOL Status = Process32First(hProcessSnap, &ProcessInfo);
	while (Status)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE,
			ProcessInfo.th32ProcessID);
		if (hProcess)
		{
			//设置进程工作区大小
			SetProcessWorkingSetSize(hProcess, -1, -1);
			//尽可能多的将指定进程的页面从工作区移除
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		//获取下一个进程的信息  
		Status = Process32Next(hProcessSnap, &ProcessInfo);
	}
}

CString GetPathFrom()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("从下面选文件夹目录:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return CString("");
	}
	SHGetPathFromIDList(idl, szBuffer);
	return szBuffer;
}

vector<CString> GetDriverList()
{
	vector<CString>nDiverList;
	TCHAR nDrive[MAX_PATH];
	GetLogicalDriveStrings(100, (LPWSTR)nDrive);
	TCHAR* pName = nDrive;

	while (*pName != 0) {
		nDiverList.push_back(pName);
		pName += _tcslen(pName) + 1;
	}
	return nDiverList;
}

CString GetPathShortFileName(CString nPath)
{
	DWORD nIndex = nPath.ReverseFind('\\');
	nPath = nPath.Right(nPath.GetLength() - nIndex - 1);
	return nPath;
}

CStringArray* DivString(CString test, CString nSeg, DWORD &nNum)
{
	CStringArray* m_result = new CStringArray;
	while (TRUE)
	{
		int index = test.Find(nSeg);
		if (index == -1)
		{
			nNum++;
			m_result->Add(test);
			return m_result;
		}
		CString test1 = test.Left(index);
		m_result->Add(test1);
		test = test.Right(test.GetLength() - index - nSeg.GetLength());
		nNum++;
	}
}
DWORD SegmentText(vector<CString>&nText, CString &nSrc, CString nSeg)
{
	nText.swap(vector<CString>());

	DWORD nNum = 0;
	CStringArray* result = DivString(nSrc, nSeg, nNum);
	for (DWORD i = 0; i < nNum; i++)
	{
		nText.push_back(result->GetAt(i));
	}
	delete result;
	return nText.size();
}


WORD NtohsEx(WORD ntemp)
{
	NTOHS_UNION *nUnion = nullptr;
	nUnion = (NTOHS_UNION*)&ntemp;

	NTOHS_UNION nUnion2{};
	nUnion2.uu.A = nUnion->uu.B;
	nUnion2.uu.B = nUnion->uu.A;

	return nUnion2.nPort;
}

VOID SetTransparentWin(HWND m_hWnd,BYTE nVal)
{
	LONG nRet = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	nRet |= 524288;
	SetWindowLong(m_hWnd, GWL_EXSTYLE, nRet);
	SetLayeredWindowAttributes(m_hWnd,NULL, nVal, LWA_ALPHA);
}
VOID SetCircularWin(HWND m_hWnd, BYTE nVal)
{
	CRect nRect{};
	GetWindowRect(m_hWnd,&nRect);
	HRGN nHrgn = CreateRoundRectRgn(0, 0, nRect.right - nRect.left + 1, nRect.bottom - nRect.top + 1, nVal, nVal);
	SetWindowRgn(m_hWnd,nHrgn, TRUE);
	DeleteObject(nHrgn);
}

CString IntToIp(in_addr &nAddr)
{
	char nTempIp[16]{};
	inet_ntop(AF_INET, &nAddr, nTempIp, sizeof(nTempIp));
	return CString(nTempIp);
}

DWORD IpToInt(CString nIp)
{
	DWORD nTempIp = 0;
	inet_pton(AF_INET, CStringA(nIp), &nTempIp);
	return nTempIp;
}

DWORD GetProcessThreadId(DWORD nPid)
{
	BOOL nRet;
	DWORD nIndex = 0;
	THREADENTRY32 nThread;

	nThread.dwSize = sizeof(THREADENTRY32);
	HANDLE nSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, nPid);

	nRet = Thread32First(nSnapShot, &nThread);
	while (nRet)
	{
		if (nThread.th32OwnerProcessID == nPid)
		{
			CloseHandle(nSnapShot);
			return nThread.th32ThreadID;
		}
		nRet = Thread32Next(nSnapShot, &nThread);
	}
	CloseHandle(nSnapShot);
	return NULL;
}