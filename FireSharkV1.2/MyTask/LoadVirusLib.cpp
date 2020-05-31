#include "stdafx.h"
#include "LoadVirusLib.h"


LoadVirusLib::LoadVirusLib()
{

}


LoadVirusLib::~LoadVirusLib()
{
	ClearVirusLibData();
}

//读取本地病毒库
DWORD LoadVirusLib::LoadVirusLibData()
{
	HANDLE  pFile = nullptr;
	//打开本地这个.dat的病毒库文件
	pFile = CreateFile(GetProgramDir()+ TEXT("VirusLib.dat"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == (HANDLE)-1)return 0;

	DWORD nSize = GetFileSize((PHANDLE)pFile, 0);
	DWORD nReadSize = 0;

	m_VirusCount = nSize / sizeof(VIRUSINFO);
	m_VirusLib = new VIRUSINFO[m_VirusCount];
	ReadFile(pFile, (LPVOID)m_VirusLib, nSize, &nReadSize,NULL);
	CloseHandle(pFile);
	return m_VirusCount;
}

//取出病毒库信息
VOID LoadVirusLib::GetVirusLib(PVIRUSINFO &nVIRUSINFO)
{
	memcpy_s(nVIRUSINFO, sizeof(VIRUSINFO)*m_VirusCount, m_VirusLib, sizeof(VIRUSINFO)*m_VirusCount);
}

//打开病毒库
VOID LoadVirusLib::SetVirusLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount)
{
	FILE *pFile = nullptr;
	fopen_s(&pFile, GetProgramDir() + "VirusLib.dat", "wb");
	m_VirusCount = fwrite(nVIRUSINFO, sizeof(VIRUSINFO), nCount, pFile);
	fclose(pFile);
}

//关闭病毒库
VOID LoadVirusLib::ClearVirusLibData()
{
	if (m_VirusLib !=nullptr)
	{
		delete[] m_VirusLib;
	}
	m_VirusLib = nullptr;
	m_VirusCount = 0;
}


//读取白名单
DWORD LoadVirusLib::LoadProcessLibData()
{

	HANDLE  pFile = nullptr;

	pFile = CreateFile(GetProgramDir() + TEXT("ProcessLib.dat"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (pFile == (HANDLE)-1)return 0;

	DWORD nSize = GetFileSize((PHANDLE)pFile, 0);
	DWORD nReadSize = 0;

	m_ProcessCount = nSize / sizeof(VIRUSINFO);
	m_ProcessLib = new VIRUSINFO[m_ProcessCount];
	ReadFile(pFile, (LPVOID)m_ProcessLib, nSize, &nReadSize, NULL);
	CloseHandle(pFile);
	return m_ProcessCount;
}

//取出白名单信息
VOID LoadVirusLib::GetProcessLib(PVIRUSINFO &nVIRUSINFO)
{
	memcpy_s(nVIRUSINFO, sizeof(VIRUSINFO)*m_ProcessCount, m_ProcessLib, sizeof(VIRUSINFO)*m_ProcessCount);
}

//添加进程至白名单
VOID LoadVirusLib::SetProcessLib(PVIRUSINFO &nVIRUSINFO, DWORD nCount)
{
	//打开并写入ProcessLib序列化文件
	FILE *pFile = nullptr;
	fopen_s(&pFile, GetProgramDir() + "ProcessLib.dat", "wb");
	m_ProcessCount = fwrite(nVIRUSINFO, sizeof(VIRUSINFO), nCount, pFile);
	fclose(pFile);
}

//关闭白名单
VOID LoadVirusLib::ClearProcessLibData()
{
	if (m_ProcessLib != nullptr)
	{
		delete[] m_ProcessLib;
	}
	m_ProcessLib = nullptr;
	m_ProcessCount = 0;
}

