#pragma once
#include "MyHead.h"


class LoadVirusLib
{
public:

	LoadVirusLib();
	~LoadVirusLib();

	PVIRUSINFO m_VirusLib = nullptr;
	DWORD m_VirusCount = 0;
	PVIRUSINFO m_ProcessLib = nullptr;
	DWORD m_ProcessCount = 0;

	CString SEG = TEXT("|#|");
	BOOL m_IsLine = FALSE;

	DWORD LoadVirusLibData();
	VOID GetVirusLib(PVIRUSINFO &nVIRUSINFO);
	VOID ClearVirusLibData();
	VOID SetVirusLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount);

	DWORD LoadProcessLibData();
	VOID GetProcessLib(PVIRUSINFO &nVIRUSINFO);
	VOID ClearProcessLibData();
	VOID SetProcessLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount);


};

