#pragma once

#include <vector>

using std::vector;

//显示进程
typedef struct _TASKLIST
{
	CString *szExeFile;
	DWORD *th32ProcessID;
	DWORD *cntThreads;
	DWORD *th32ParentProcessID;
	CString *szExeFilePath;
	CString *WorkingSetSize;
	CString *szPriority;

}TASKLIST, *PTASKLIST;

//显示模块
typedef struct _MODULELIST
{
	CString *szModule;
	CString *szExePath;
	DWORD *modBaseAddr;
	DWORD *dwSize;
}MODULELIST, *PMOUDLELIST;

//显示线程
typedef struct _THREADLIST
{
	DWORD *th32ThreadID;
	DWORD *th32OwnerProcessID;
	DWORD *tpBasePri;
	CString *szExePath;
	CString *szThreadState;

}THREADLIST, *PTHREADLIST;

//显示堆
typedef struct _HEAPLIST
{
	DWORD *th32HeapID;
	DWORD *th32ProcessID;
	CString *szExeFile;

}HEAPLIST, *PHEAPLIST;

typedef struct _HEAPENTYR
{
	DWORD *dwAddress;//堆块的线性起始地址
	DWORD *dwBlockSize;//堆块的大小

}HEAPENTYR, *PHEAPENTYR;

//计算病毒信息
typedef struct _VIRUSINFO
{
	CHAR FileName[MAX_PATH]{};
	CHAR FileMd5[MAX_PATH]{};
}VIRUSINFO, *PVIRUSINFO;


typedef union _NTOHS_UNION
{
	struct _uu
	{
		BYTE A;
		BYTE B;
	}uu;
	WORD nPort;

}NTOHS_UNION, *PNTOHS_UNION;

void OutputDebugStringEx(const TCHAR* format, ...);
void OutputDebugEdit(CEdit &m_Edit_Info, const TCHAR* format, ...);

void MyRtlAdjustPrivilege();
DWORD EnumTaskList(TASKLIST &nTaskList);
DWORD EnumModuleList(MODULELIST &nModuleList, DWORD nPid);
DWORD EnumThreadList(THREADLIST &nThreadList, DWORD nPid);
DWORD EnumHeapList(HEAPLIST &nHeapList);
DWORD EnumHeapEntyr(HEAPENTYR &nHeapEntry, DWORD nHeapId, DWORD nPid);
void ClearTaskList(TASKLIST &nTaskList);
void ClearModuleList(MODULELIST &nModuleList);
void ClearThreadList(THREADLIST &nThreadList);
void ClearHeapList(HEAPLIST &nHeapList);
void ClearHeapEntyr(HEAPENTYR &nHeapEntyr);
void EndProcess(DWORD nPid);
void EndThread(DWORD nThreadid);
void GetProcessFilePath(HANDLE hProcess, CString& sFilePath);
void SetProcessState(DWORD nPid, BOOL nSuspend);
void SetThreadState(DWORD nThreadid, BOOL nSuspend);
DWORD GetMemState();
void OpenFile(CString &nFileNamePath);
void GetCpuState(CString& nStr);
bool GetKey(int vk_code);
void GetProcessName(CString &nProcessName, DWORD nPid);
void GetThreadState(CString &nState, DWORD nThreadid);
void GetProcessPriority(CString &nState, HANDLE nHandle);
void OpenFileInfo(HWND nHwnd, CString &nFileRoute);
DWORD GetProcessPid(CString nProcessName);

CString GetProgramDir();
CString GetPathEx(CString nPath);
void ClearMemory();
CString GetPathFrom();
vector<CString> GetDriverList();
CString GetPathShortFileName(CString nPath);
DWORD SegmentText(vector<CString>&nText, CString &nSrc, CString nSeg);
WORD NtohsEx(WORD ntemp);
VOID SetTransparentWin(HWND m_hWnd, BYTE nVal);
VOID SetCircularWin(HWND m_hWnd, BYTE nVal);
CString IntToIp(in_addr &nAddr);
DWORD IpToInt(CString nIp);
DWORD GetProcessThreadId(DWORD nPid);
